// Okay, let's break down these failing test cases and pinpoint the areas in your minishell code that likely need adjustment.

// General Observations & Common Problem Areas:

// Quoting and Expansion: Tests 8 and 16 clearly show that your shell is expanding variables ($PWD, $?, $USER, $HOME) even when they are inside single quotes ('...'). Single quotes should prevent all expansions. This points to an issue either in your tokenizer (not correctly identifying single-quoted segments) or your expander (expanding variables regardless of the token type/context).

// Builtin Argument Handling: Tests 23 (pwd oi) and 40 (cd $PWD hi) show your builtins (pwd, cd) handle extra arguments differently than Bash. Bash's pwd ignores them, while Bash's cd reports "too many arguments". Your versions seem to have inconsistent or incorrect checks.

// Pipe and Redirection Interaction: Tests 53, 68, 69, 70, 71, 72 all involve pipelines (|) often combined with redirections (<). Your shell consistently produces different output than Bash in these scenarios. This is often tricky and suggests problems in:

// How file descriptors (stdin/stdout) are managed (dup2, close) for pipeline stages.

// The order in which redirections are applied relative to pipe connections. Redirections on a command should generally override the pipe's input/output for that specific command.

// Error handling or resource cleanup within the pipeline execution logic. Test 53 (unsorted output) strongly suggests sort isn't receiving input correctly via the pipe. Tests 68/71 show < file redirection isn't overriding pipe input for cat. Tests 69/72 show cat file_arg isn't working correctly when stdin is piped.

// Error Reporting and Exit Status: Tests 23, 40, 56, 65, 66 show discrepancies in exit codes and error messages (especially for redirection errors and too many arguments). Your shell needs to more accurately mimic Bash's exit status and stderr messages for failures.

// Comment Handling: Test 74 (#cat ...) fails because your shell tries to execute #cat instead of treating the line as a comment.

// Subshell Environment (Pipes): Test 56 (export GHOST=123 | env ...) highlights how environment changes in one part of a pipeline affect subsequent parts. In Bash (and typically Unix shells), each command in a pipeline runs in a separate subshell. Changes made by export in one subshell are inherited by later commands in that same pipeline execution but do not affect the parent shell. Your shell's lack of output suggests the export modification is lost before env runs, likely due to correct subshell isolation, but the tester's expected Bash exit code (1) seems incorrect for this specific command if env and grep succeed (Bash yields 0). The primary issue here is likely the environment not propagating as expected within the pipeline context.

// Specific Fixes Based on Test Cases:

// Test 8: echo '$PWD' & Test 16: echo '...'

// Problem: Variable expansion inside single quotes.

// Location: expander.c: expand_variables, lexer*.c (tokenizer logic).

// Cause: Your tokenizer likely merges quoted content into a general T_WORD token, losing the single-quote context. Alternatively, expand_variables might be incorrectly processing tokens. The function currently expands T_WORD and T_DOUBLE_QUOTED, which seems right if the tokenizer produced a T_SINGLE_QUOTED token correctly.

// Fix:

// Tokenizer: Ensure your tokenizer (tokenize_input and helpers like handle_quoted) creates distinct tokens with type T_SINGLE_QUOTED for content within single quotes, rather than just appending the content to a current_word that becomes T_WORD. The handle_quoted function in lexer_utils3.c seems problematic as it appends content instead of creating a typed token. It should likely call add_token with T_SINGLE_QUOTED or T_DOUBLE_QUOTED.

// Expander: Verify that expand_variables explicitly skips expansion for tokens of type T_SINGLE_QUOTED. The current code if (tokens->type == T_DOUBLE_QUOTED || tokens->type == T_WORD) already does this implicitly, so the issue is almost certainly in the tokenizer creating the wrong token type.

// Test 23: pwd oi

// Problem: pwd incorrectly fails with "too many arguments". Bash's pwd ignores extra arguments.

// Location: builtins/bi_pwd.c: bi_pwd

// Cause: The check if (argv[1]) causes the failure.

// Fix: Remove the argument check from bi_pwd. It should simply get and print the directory, ignoring argv[1], argv[2], etc., and always return 0 on success.

// // In bi_pwd function:
// int bi_pwd(char *argv[])
// {
//     char cwd[PATH_MAX];

//     // REMOVE or comment out this check:
//     /*
//     if (argv[1])
//     {
//         ft_putstr_fd("pwd: too many arguments\n", STDERR_FILENO);
//         return (1);
//     }
//     */
//     (void)argv; // Indicate argv is intentionally unused

//     if (getcwd(cwd, sizeof(cwd)) != NULL)
//     {
//         ft_putstr_fd(cwd, STDOUT_FILENO);
//         ft_putstr_fd("\n", STDOUT_FILENO);
//         return (0); // Success
//     }
//     perror("minishell: pwd");
//     return (1); // Error (getcwd failed)
// }


// Test 40: cd $PWD hi

// Problem: cd doesn't detect too many arguments and exits with 0. Bash exits with 1 and an error.

// Location: builtins/bi_cd.c: bi_cd

// Cause: Missing check for args[2].

// Fix: Add a check at the beginning of bi_cd.

// // At the beginning of bi_cd function:
// int bi_cd(char **args, t_env **env)
// {
//     char *old_pwd;
//     int special_case_result;

//     // ADD this check
//     if (args[1] && args[2])
//     {
//          ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
//          return (ERROR); // Return 1
//     }

//     old_pwd = getcwd(NULL, 0);
//     // ... rest of the function ...
// }
// IGNORE_WHEN_COPYING_START
// content_copy
// download
// Use code with caution.
// C
// IGNORE_WHEN_COPYING_END

// Test 53: env | sort | grep -v SHLVL | grep -v ^_

// Problem: Output is incorrect (not sorted, contains filtered items). Suggests pipes aren't working correctly, especially for sort.

// Location: execution.c: handle_child_process, pipes.c: execute_pipeline, pipes.c: update_pipe_status / cleanup_pipes.

// Cause: Likely incorrect file descriptor management (dup2, close) for pipes. sort probably isn't receiving the full output of env on its standard input.

// Fix: Carefully review the pipe setup in handle_child_process and the closing of pipe ends in both the child and the parent loop (execute_pipeline).

// Child (handle_child_process):

// dup2(prev_pipe_read, STDIN_FILENO): Correctly sets input from previous command.

// dup2(pipe_fd[1], STDOUT_FILENO): Correctly sets output to next command (if cmd->next).

// Crucially, all pipe ends related to the current pipe (pipe_fd[0], pipe_fd[1]) and the previous pipe (prev_pipe_read) must be closed in the child after dup2 calls, as the child doesn't need them directly anymore. Your code seems to close pipe_fd[0], pipe_fd[1], and prev_pipe_read which looks right.

// Parent (execute_pipeline loop):

// The parent must close the ends of the pipes it's not using to avoid hangs and ensure EOF is sent correctly.

// After forking, the parent needs to close pipe_fd[1] (the write end of the current pipe).

// It also needs to close prev_pipe_read (the read end from the previous command's pipe) after the current child has been forked (as the child inherits it, dups it, and closes it).

// The update_pipe_status / cleanup_pipes logic seems to handle this, but double-check its correctness. Ensure prev_pipe_read is correctly updated to pipe_fd[0] for the next iteration.

// Test 56: export GHOST=123 | env | grep GHOST

// Problem: export change doesn't persist for env in the pipe. Mini exit 0, Bash exit 1 (Tester seems wrong here, Bash usually exits 0 if grep succeeds).

// Location: execution.c: handle_child_process, builtin_handler.c: run_builtin.

// Cause: Standard subshell behavior in pipelines. export runs in a child process. Its environment modifications are local to that child and are lost. The next child (env) inherits the original environment from the main minishell process.

// Fix: This behavior is technically correct regarding subshell isolation. Matching Bash's apparent propagation within the pipeline (where env does see GHOST) is complex and might require non-standard environment passing between pipeline stages. For this project, it might be acceptable to differ from Bash here, but you should ensure your exit code logic is correct (likely 0 if env | grep GHOST succeeds). The tester's claim of Bash exit 1 needs verification for this specific command.

// Test 65 & 66: echo < ... <missing < ...

// Problem: Failure on non-existent input redirection file (missing) is not reported, exit code is 0 instead of 1.

// Location: redirs_execute.c: setup_redirections, redirs_execute.c: process_single_redirection, execution.c: handle_child_process, builtin_handler.c: handle_parent_builtin.

// Cause: The return status (-1) from handle_input_redir (when open fails) isn't properly handled higher up. The error message is also missing.

// Fix:

// Error Message: In process_single_redirection, when status < 0, print the specific error using perror or ft_putstr_fd mimicking bash: missing: No such file or directory. Your code already prints this, ensure it goes to STDERR_FILENO.

// Propagation:

// Ensure process_single_redirection returns -1 on failure.

// Ensure setup_redirections returns -1 if any process_single_redirection call fails.

// In handle_child_process: Check the return value of setup_redirections. If it's < 0, exit(1).

// In handle_parent_builtin: Check the return value of setup_redirections. If it's < 0, set g_exit_status = 1 and return (after restoring fds).

// Test 68: echo <in_big | cat <in & Test 71: echo hi | cat <in

// Problem: Input redirection (<infile) on the cat command doesn't override the input coming from the pipe. cat reads pipe instead of the file.

// Location: execution.c: handle_child_process.

// Cause: The order of operations seems correct (pipe dup2 then redirection dup2), so the issue might be subtle. Possibilities:

// A file descriptor leak somewhere.

// An error during setup_redirections for cat that isn't properly handled (less likely, as cat <infile works alone).

// Incorrect closing of file descriptors related to the pipe or the redirected file fd, confusing cat.

// Fix: This requires careful debugging.

// Add fprintf(stderr, ...) statements in handle_child_process before and after the pipe dup2, and before and after the setup_redirections call, printing STDIN_FILENO and relevant pipe/file fds to trace how stdin is changing.

// Verify that handle_input_redir closes the original file fd after the dup2 call. (It does).

// Ensure pipe file descriptors are meticulously closed.

// Test 69: echo <in_big | cat infile & Test 72: echo hi | cat infile

// Problem: cat command with a file argument doesn't read the argument file when its stdin is connected to a pipe. It seems to be reading from the (empty or 'hi') pipe instead.

// Location: execution.c: execute_external, execution.c: handle_child_process.

// Cause: This is strange. cat (the external program) should ignore stdin if given a file argument. Why would it read from the pipe?

// Are cmd->args correct when execute_external is called for cat? (i.e., {"cat", "./test_files/infile", NULL})

// Is execve actually succeeding? Check the return value and errno.

// Is the PATH environment variable correctly passed in env_arr?

// Could setup_redirections for the first command (echo) somehow mess up the environment or FDs for the second command? Unlikely, but possible.

// Fix: Debugging is needed.

// In execute_external, print the cmd->args[0], cmd->args[1], etc., and the path found just before calling execve.

// Check the exit status of the child process running cat using the status variable in wait_for_children. Did cat exit normally (0) or with an error?

// Test 70: echo <in_big | echo <in

// Problem: Second echo doesn't produce output. Bash prints a newline.

// Location: Pipe/Redirection interaction (handle_child_process).

// Cause: Similar to Test 68. The setup for the second echo (pipe stdin, overridden by <infile stdin redirection, standard builtin execution writing to pipe stdout) seems to be failing.

// Fix: Debugging needed, similar to Test 68. Focus on the file descriptors for the second echo command in the pipeline. Ensure its STDOUT_FILENO is correctly connected (it should be the default stdout if it's the last command, or the write end of the next pipe if there was one) after input redirections are handled.

// Test 74: #cat ...

// Problem: # not treated as a comment start.

// Location: main.c: process_input or lexer.c: tokenize_input.

// Cause: No logic to detect comments.

// Fix: Add comment handling before tokenization.

// // In process_input (main.c) before calling tokenize_input
// void process_input(char *input, t_env **env)
// {
//     t_token *tokens = NULL;
//     t_cmd *cmds = NULL;
//     t_resources res = {0};
//     char *comment_ptr;

//     // ADD COMMENT HANDLING HERE
//     comment_ptr = ft_strchr(input, '#');
//     if (comment_ptr) {
//          *comment_ptr = '\0'; // Truncate input at the comment character
//     }
//     // Now check if input is empty after removing comment / whitespace
//     // (Need a trim function or loop to check if only whitespace remains)
//     // If input becomes empty after comment removal, just free and return.

//     res.input = input; // Keep track even if modified
//     res.env = *env;

//     // Only tokenize if input is not empty/comment
//     if (input[0] == '\0' /* || is_only_whitespace(input) */ ) {
//          free(input);
//          return;
//     }

//     tokens = tokenize_input(input);
//     // ... rest of the function ...
// }
// IGNORE_WHEN_COPYING_START
// content_copy
// download
// Use code with caution.
// C
// IGNORE_WHEN_COPYING_END

// You'll need a way to handle potential leading whitespace before the # and to check if the line becomes effectively empty after stripping the comment.

// By addressing these points, especially the tokenizer/expander for quotes, the pipe/redirection FD management, and the builtin argument/error handling, you should be able to fix most of these failing tests. Good luck!