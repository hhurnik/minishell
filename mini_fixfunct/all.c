// #include "inc/minishell.h"

// int	is_builtin(char *cmd)
// {
// 	if ((!strcmp(cmd, "echo") || !strcmp(cmd, "cd") || !strcmp(cmd, "pwd")
// 			|| !strcmp(cmd, "export") || !strcmp(cmd, "unset") || !strcmp(cmd,
// 				"env") || !strcmp(cmd, "exit")))
// 		return (1);
// 	else
// 		return (0);
// }

// int run_builtin(t_cmd *cmd, t_resources *res)
// {
//     if (!strcmp(cmd->args[0], "echo"))
//         return (bi_echo(cmd->args));
//     if (!strcmp(cmd->args[0], "pwd"))
//         return (bi_pwd(cmd->args));
//     if (!strcmp(cmd->args[0], "exit"))
//         return (bi_exit(cmd->args, res));
//     if (!strcmp(cmd->args[0], "unset"))
//         return (bi_unset(cmd->args, &res->env));
//     if (!strcmp(cmd->args[0], "export"))
//         return (bi_export(cmd->args, &res->env));
//     if (!strcmp(cmd->args[0], "env"))
//         return (bi_env(cmd->args, &res->env));
//     if (!strcmp(cmd->args[0], "cd"))
//         return (bi_cd(cmd->args, &res->env));
//     return (0);
// }

// //2 leaki, ok!
// // int handle_parent_builtin(t_cmd *cmd, t_env **env, int stdin_copy, int stdout_copy)
// // {
// //     t_resources res;

// //     res.env = *env;
// //     res.cmds = cmd;
// //     res.tokens = NULL; // Tokens are not used here
// //     res.input = NULL;  // Input is not used here

// //     if (setup_redirections(cmd->redirections) < 0)
// //     {
// //         restore_std_fds(stdin_copy, stdout_copy);
// //         free_cmds(cmd);
// //         free_env(*env);
// //         return (1);
// //     }
// //     g_exit_status = run_builtin(cmd, &res); // Pass res instead of env
// //     restore_std_fds(stdin_copy, stdout_copy);
// //     return (1);
// // }

// int handle_parent_builtin(t_cmd *cmd, t_fds fds, t_resources *res)
// {
//     if (setup_redirections(cmd->redirections) < 0)
//     {
//         restore_std_fds(fds.stdin, fds.stdout);
//         g_exit_status = 1;
//         return (1);
//     }
    
//     g_exit_status = run_builtin(cmd, res);
    
//     restore_std_fds(fds.stdin, fds.stdout);
//     return (1);
// }


// int handle_single_builtin(t_cmd *cmds, t_resources *res)
// {
//     if (is_builtin(cmds->args[0]) && !cmds->next)
//     {
//         // Create a t_fds structure with duplicated file descriptors
//         t_fds fds;
//         fds.stdin = dup(STDIN_FILENO);
//         fds.stdout = dup(STDOUT_FILENO);
        
//         // Pass the t_fds structure to handle_parent_builtin
//         handle_parent_builtin(cmds, fds, res);
        
//         return (1);
//     }
//     return (0);
// }#include "inc/minishell.h"

// //stare dobre, ale ponizej naprawione z valgrindem
// // t_env	*env_new(const char *key, const char *value)
// // {
// // 	t_env	*new_var;

// // 	new_var = (t_env *)malloc(sizeof(t_env));
// // 	if (!new_var)
// // 		return (NULL);

// // 	new_var->key = ft_strdup(key);
// // 	if (!new_var->key)
// // 	{
// // 		free(new_var);
// // 		return (NULL);
// // 	}
// // 	if (value)
// // 		new_var->value = ft_strdup(value);
// // 	else
// // 		new_var->value = ft_strdup("");
// // 	if (!new_var->value)
// // 	{
// // 		free(new_var->key);
// // 		free(new_var);
// // 		return (NULL);
// // 	}
// // 	new_var->next = NULL;
// // 	return (new_var);
// // }

// //dodane valgrind
// void free_env_node(t_env *node)
// {
//     if (node)
//     {
//         free(node->key);
//         free(node->value);
//         free(node);
//     }
// }
// //leakami 28.04
// t_env *env_new(const char *key, const char *value)
// {
//     t_env *new_var;

//     new_var = (t_env *)malloc(sizeof(t_env));
//     if (!new_var)
//         return (NULL);

//     new_var->key = ft_strdup(key);
//     if (!new_var->key)
//     {
//         free_env_node(new_var); // Use helper function for cleanup
//         return (NULL);
//     }

//     if (value)
//         new_var->value = ft_strdup(value);
//     else
//         new_var->value = ft_strdup("");

//     if (!new_var->value)
//     {
//         free_env_node(new_var); // Use helper function for cleanup
//         return (NULL);
//     }

//     new_var->next = NULL;
//     return (new_var);
// }

// t_env	*get_env(const char *key, t_env *env)
// {
// 	while (env)
// 	{
// 		if (ft_strcmp(env->key, key) == 0)
// 			return (env);
// 		env = env->next;
// 	}
// 	return (NULL);
// }

// //leaki
// char	*get_env_value(const char *key, t_env *env)
// {
// 	t_env	*var;

// 	var = get_env(key, env);
// 	if (!var)
// 		return (NULL);
// 	return (var->value);
// }



// // dodaje new node to the end of a linked list of t_env nodes
// void	env_add_back(t_env **env, t_env *new_node)
// {
// 	t_env	*last;
// 	// if empty list, zrob nowy node
// 	if (!*env)
// 	{
// 		*env = new_node;
// 	}
// 	else
// 	{
// 		last = *env;
// 		// dopoki ten node ma nastepny node,
// 		// keep going(last idzie a do final node)
// 		while (last->next)
// 		{
// 			last = last->next;
// 		}
// 		last->next = new_node;
// 	}
// }
// #include "inc/minishell.h"

// // //2 leaki
// // void execute(t_cmd *cmds, t_env **env)
// // {
// //     int stdin_copy;
// //     int stdout_copy;
// //     int prev_pipe_read;

// //     if (!init_execution(cmds, &stdin_copy, &stdout_copy))
// //     {
// //         free_cmds(cmds);
// //         free_env(*env);
// //         return;
// //     }
// //     if (handle_single_builtin(cmds, env, stdin_copy, stdout_copy))
// //         return;
// //     prepare_pipeline_execution(cmds, &prev_pipe_read);
// //     execute_pipeline(cmds, env, &prev_pipe_read);
// //     cleanup_execution(stdin_copy, stdout_copy);
// // }
// void execute(t_cmd *cmds, t_env **env)
// {
//     int stdin_copy;
//     int stdout_copy;
//     int prev_pipe_read;
//     t_resources res = {0}; // Initialize resources structure
    
//     // Populate resources for proper cleanup in case of exit
//     res.env = *env;
//     res.cmds = cmds;
//     // Note: tokens and input would need to be provided from the caller
//     // For now, set them to NULL
//     res.tokens = NULL;
//     res.input = NULL;

//     if (!init_execution(cmds, &stdin_copy, &stdout_copy))
//     {
//         free_cmds(cmds);
//         free_env(*env);
//         return;
//     }
    
//     // Pass the resources struct to handle_single_builtin
//     if (handle_single_builtin(cmds, &res))
//         return;
        
//     prepare_pipeline_execution(cmds, &prev_pipe_read);
//     execute_pipeline(cmds, env, &prev_pipe_read);
//     cleanup_execution(stdin_copy, stdout_copy);
// }

// int	setup_pipe_and_fork(t_cmd *cmd, int *pipe_fd)
// {
// 	pid_t	pid;

// 	if (cmd->next && pipe(pipe_fd) == -1)
// 	{
// 		perror("minishell: pipe");
// 		return (0);
// 	}
// 	pid = fork();
// 	if (pid == -1)
// 	{
// 		perror("minishell: fork");
// 		if (pipe_fd[0] != -1)
// 			close(pipe_fd[0]);
// 		if (pipe_fd[1] != -1)
// 			close(pipe_fd[1]);
// 		return (0);
// 	}
// 	return (pid);
// }

// //2 leaki, ok
// void handle_child_process(t_cmd *cmd, t_env **env, int prev_pipe_read, int *pipe_fd)
// {
//     t_resources res;

//     res.env = *env;
//     res.cmds = cmd;
//     res.tokens = NULL; // Tokens are not used here
//     res.input = NULL;  // Input is not used here

//     signal(SIGINT, SIG_DFL);
//     signal(SIGQUIT, SIG_DFL);
//     if (prev_pipe_read != -1)
//     {
//         dup2(prev_pipe_read, STDIN_FILENO);
//         close(prev_pipe_read);
//     }
//     if (cmd->next)
//         dup2(pipe_fd[1], STDOUT_FILENO);
//     if (pipe_fd[0] != -1)
//         close(pipe_fd[0]);
//     if (pipe_fd[1] != -1)
//         close(pipe_fd[1]);
//     if (setup_redirections(cmd->redirections) < 0)
//     {
//         free_cmds(cmd);
//         free_env(*env);
//         exit(1);
//     }
//     if (is_builtin(cmd->args[0]))
//         exit(run_builtin(cmd, &res)); // Pass res instead of env
//     else
//         execute_external(cmd, *env);
// }

// //leaki
// void execute_external(t_cmd *cmd, t_env *env)
// {
//     char *path = NULL;
//     char **env_arr = NULL;

//     path = get_exec_path(cmd->args[0], env);
//     env_arr = env_to_arr(env);

//     if (path)
//     {
//         execve(path, cmd->args, env_arr);
//         // If execve fails, we'll continue to the error handling
//         free(path);
//     }

//     // If we reach here, exec failed (either path wasn't found or execve failed)
//     ft_free_split(env_arr);
//     if (path == NULL)
//     {
//         // Command not found in any PATH directory
//         ft_putstr_fd("minishell: ", STDERR_FILENO);
//         ft_putstr_fd(cmd->args[0], STDERR_FILENO);
//         ft_putstr_fd(": command not found\n", STDERR_FILENO);
//         exit(127);
//     }
//     else
//     {
//         // execve failed for some other reason
//         perror("minishell");
//         exit(126); // Typically 126 is used for "found but not executable"
//     }
// }
// #include "inc/minishell.h"

// void cleanup(t_resources *res)
// {
//     if (res->input)
//         free(res->input);
//     if (res->tokens)
//         free_tokens(res->tokens);
//     if (res->cmds)
//         free_cmds(res->cmds);
//     if (res->env)
//         free_env(res->env);
    
//     // Clear readline history
//     rl_clear_history();
// }
// //2 leaki
// void	process_command(t_cmd *cmd, t_env **env, int prev_pipe_read,
// 		int *pipe_fd)
// {
// 	pid_t pid;

// 	pid = setup_pipe_and_fork(cmd, pipe_fd);
// 	if (pid == 0)
// 		handle_child_process(cmd, env, prev_pipe_read, pipe_fd);
// }


// int	init_execution(t_cmd *cmds, int *stdin_copy, int *stdout_copy)
// {
// 	*stdin_copy = dup(STDIN_FILENO);
// 	*stdout_copy = dup(STDOUT_FILENO);
// 	if (!cmds || !cmds->args || !cmds->args[0])
// 	{
// 		ft_putstr_fd("minishell: null command\n", STDERR_FILENO);
// 		return (0);
// 	}
// 	return (1);
// }

// void	cleanup_execution(int stdin_copy, int stdout_copy)
// {
// 	setup_signals_for_prompt();
// 	restore_std_fds(stdin_copy, stdout_copy);
// }

// //moje
// void	wait_for_children(void)
// {
// 	int status;

// 	while (waitpid(-1, &status, 0) > 0)
// 	{
// 		if (WIFEXITED(status))
// 			g_exit_status = WEXITSTATUS(status);
// 	}
// }
// #include "inc/minishell.h"

// int		g_exit_status = 0; // Globalna zmienna stanu

// // Count the number of nodes in the environment list
// int	count_env_nodes(t_env *env)
// {
// 	int		count;
// 	t_env	*current;
// 	// var: env(param), count, current = 3 <= 5 OK

// 	count = 0;
// 	current = env;
// 	while (current)
// 	{
// 		count++;
// 		current = current->next;
// 	}
// 	return (count);
// }

// //Create a single "KEY=VALUE" string entry.
// //Returns the allocated string or NULL on error (malloc fail, NULL key/value).
// char	*create_env_entry(t_env *node)
// {
// 	char	*key_eq;
// 	char	*entry;
// 	// var: node(param), key_eq, entry = 3 <= 5 OK
// 	// Check for NULL key or value within the node first
// 	if (!node || !node->key || !node->value)
// 		return (NULL);
// 	key_eq = ft_strjoin(node->key, "=");
// 	if (!key_eq)
// 		return (NULL);
// 	entry = ft_strjoin(key_eq, node->value);
// 	free(key_eq); // Free intermediate string immediately
// 	if (!entry)
// 		return (NULL); // Check final string allocation
// 	return (entry);
// }

// //Free the partially allocated array on error
// //Frees arr[0] up to arr[count-1], then frees arr itself
// void	free_array_on_error(char **arr, int count)
// {
// 	int	i; // var: arr(param), count(param), i = 3 <= 5 OK
// 	i = 0;
// 	while (i < count)
// 	{
// 		free(arr[i]); // Free only the elements that were successfully allocated
// 		i++;
// 	}
// 	free(arr); // Free the array container
// }

// int	populate_env_array(t_env *env, char **arr)
// {
// 	int		i;
// 	t_env	*current;
// 	char	*entry;

// 	i = 0;
// 	current = env;
// 	while (current)
// 	{
// 		entry = create_env_entry(current);
// 		if (!entry)
// 			return (-1); // Signal error during entry creation
// 		arr[i] = entry;
// 		i++;
// 		current = current->next;
// 	}
// 	arr[i] = NULL; // Null-terminate the array
// 	return (i); // Return the count of successfully added entries
// }

// /*
//  * Main Function: Converts the environment linked list to a NULL-terminated
//  * array of strings ("KEY=VALUE"). Returns NULL on any error.
//  */
// char	**env_to_arr(t_env *env)
// {
// 	int		count;
// 	char	**arr;
// 	int		populate_result;

// 	if (!env)
// 		return (NULL);
// 	count = count_env_nodes(env);
// 	arr = malloc((count + 1) * sizeof(char *));
// 	if (!arr)
// 	{
// 		perror("minishell: malloc");
// 		return (NULL);
// 	}
// 	populate_result = populate_env_array(env, arr);
// 	if (populate_result == -1) // Check for failure
// 	{
// 		free_array_on_error(arr, count); // Free all allocated entries
// 		return (NULL);
// 	}
// 	return (arr);
// }
// #include "inc/minishell.h"

// /**
//  * Wyodrębnia nazwę zmiennej środowiskowej (np. z "$USER" → "USER").
//  * - str: wskaźnik do znaku '$' (np. "USER=john").
//  * Zwraca: nazwa zmiennej (malloc).
//  */
// char	*extract_var_name(char *str)
// {
// 	int	len;

// 	len = 0;
// 	while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
// 	{
// 		len++;
// 	}
// 	return (ft_substr(str, 0, len));
// }

// void expand_variables(t_token *tokens, t_env *env)
// {
//     char *expanded;

//     while (tokens)
//     {
//         if (tokens->type == T_DOUBLE_QUOTED || tokens->type == T_WORD)
//         {
//             expanded = expand_str(tokens->value, env);
//             if (expanded)
//             {
//                 free(tokens->value); // Free the old value
//                 tokens->value = expanded;
//             }
//         }
//         tokens = tokens->next;
//     }
// }


// int	handle_braced_var(char **result, char *str, int i, t_env *env)
// {
// 	int		start;
// 	char	*var_name;
// 	char	*var_value;

// 	start = i + 2;
// 	while (str[i] && str[i] != '}')
// 		i++;
// 	if (str[i] == '}')
// 	{
// 		var_name = ft_substr(str, start, i - start);
// 		var_value = get_env_value(var_name, env);
// 		*result = ft_strjoin_free(*result, var_value ? var_value : "");
// 		free(var_name);
// 		i++;
// 	}
// 	return (i);
// }
// int	handle_standard_var(char **result, char *str, int i, t_env *env)
// {
// 	char	*var_name;
// 	char	*var_value;

// 	var_name = extract_var_name(str + i + 1);
// 	if (var_name[0] == '\0')
// 	{
// 		*result = ft_strjoin_free(*result, "$");
// 		free(var_name);
// 		return (i + 1);
// 	}
// 	var_value = get_env_value(var_name, env);
// 	*result = ft_strjoin_free(*result, var_value ? var_value : "");
// 	i += ft_strlen(var_name) + 1;
// 	free(var_name);
// 	return (i);
// }


// //2 leaki
// // char *expand_str(char *str, t_env *env)
// // {
// //     char *result;
// //     int i;

// //     result = ft_strdup("");
// //     if (!result)
// //         return (NULL);
// //     i = 0;
// //     while (str[i])
// //     {
// //         if (is_special_var(str, i))
// //             i = handle_special_var(&result, i);
// //         else if (is_braced_var(str, i))
// //             i = handle_braced_var(&result, str, i, env);
// //         else if (is_standard_var(str, i))
// //             i = handle_standard_var(&result, str, i, env);
// //         else
// //         {
// //             result = ft_strjoin_char(result, str[i]);
// //             if (!result)
// //                 return (NULL);
// //             i++;
// //         }
// //     }
// //     return (result);
// // }
// char *expand_str(char *str, t_env *env)
// {
//     char *result;
//     int i;

//     result = ft_strdup("");
//     if (!result)
//         return (NULL); //bylo samo return null
//     i = 0;
//     while (str[i])
//     {
//         if (is_special_var(str, i))
//             i = handle_special_var(&result, i);
//         else if (is_braced_var(str, i))
//             i = handle_braced_var(&result, str, i, env);
//         else if (is_standard_var(str, i))
//             i = handle_standard_var(&result, str, i, env);
//         else
//         {
//             char *old_result = result;
//             result = ft_strjoin_char(old_result, str[i]);
//             if (!result)
//             {
//                 free(old_result);
//                 return (NULL);
//             }
//             i++;
//         }
//     }
//     return (result);
// }#include "inc/minishell.h"

// int	is_special_var(char *str, int i)
// {
// 	return (str[i] == '$' && str[i + 1] == '?');
// }

// int	is_braced_var(char *str, int i)
// {
// 	return (str[i] == '$' && str[i + 1] == '{');
// }

// int	is_standard_var(char *str, int i)
// {
// 	return (str[i] == '$');
// }

// void append_char(char **result, char c)
// {
//     char tmp[2];
//     char *new_result;

//     tmp[0] = c;
//     tmp[1] = '\0';
//     new_result = ft_strjoin(*result, tmp); // Don't free here
//     if (!new_result)
//     {
//         free(*result);
//         *result = NULL;
//         return;
//     }
//     *result = new_result;
// }
// //diala, ale with leaks on /bin/ls /nonexistent
// // int	handle_special_var(char **result, int i)
// // {
// // 	*result = ft_strjoin_free(*result, ft_itoa(g_exit_status));
// // 	return (i + 2);
// // }

// int handle_special_var(char **result, int i)
// {
//     char *exit_status_str = ft_itoa(g_exit_status);
//     if (!exit_status_str)
//         return (-1);  // Error handling
    
//     char *new_result = ft_strjoin_free(*result, exit_status_str);
//     free(exit_status_str);  // Free the string after using it
    
//     if (!new_result)
//         return (-1);
    
//     *result = new_result;
//     return (i + 2);
// }// Okay, let's break down these failing test cases and pinpoint the areas in your minishell code that likely need adjustment.

// // General Observations & Common Problem Areas:

// // Quoting and Expansion: Tests 8 and 16 clearly show that your shell is expanding variables ($PWD, $?, $USER, $HOME) even when they are inside single quotes ('...'). Single quotes should prevent all expansions. This points to an issue either in your tokenizer (not correctly identifying single-quoted segments) or your expander (expanding variables regardless of the token type/context).

// // Builtin Argument Handling: Tests 23 (pwd oi) and 40 (cd $PWD hi) show your builtins (pwd, cd) handle extra arguments differently than Bash. Bash's pwd ignores them, while Bash's cd reports "too many arguments". Your versions seem to have inconsistent or incorrect checks.

// // Pipe and Redirection Interaction: Tests 53, 68, 69, 70, 71, 72 all involve pipelines (|) often combined with redirections (<). Your shell consistently produces different output than Bash in these scenarios. This is often tricky and suggests problems in:

// // How file descriptors (stdin/stdout) are managed (dup2, close) for pipeline stages.

// // The order in which redirections are applied relative to pipe connections. Redirections on a command should generally override the pipe's input/output for that specific command.

// // Error handling or resource cleanup within the pipeline execution logic. Test 53 (unsorted output) strongly suggests sort isn't receiving input correctly via the pipe. Tests 68/71 show < file redirection isn't overriding pipe input for cat. Tests 69/72 show cat file_arg isn't working correctly when stdin is piped.

// // Error Reporting and Exit Status: Tests 23, 40, 56, 65, 66 show discrepancies in exit codes and error messages (especially for redirection errors and too many arguments). Your shell needs to more accurately mimic Bash's exit status and stderr messages for failures.

// // Comment Handling: Test 74 (#cat ...) fails because your shell tries to execute #cat instead of treating the line as a comment.

// // Subshell Environment (Pipes): Test 56 (export GHOST=123 | env ...) highlights how environment changes in one part of a pipeline affect subsequent parts. In Bash (and typically Unix shells), each command in a pipeline runs in a separate subshell. Changes made by export in one subshell are inherited by later commands in that same pipeline execution but do not affect the parent shell. Your shell's lack of output suggests the export modification is lost before env runs, likely due to correct subshell isolation, but the tester's expected Bash exit code (1) seems incorrect for this specific command if env and grep succeed (Bash yields 0). The primary issue here is likely the environment not propagating as expected within the pipeline context.

// // Specific Fixes Based on Test Cases:

// // Test 8: echo '$PWD' & Test 16: echo '...'

// // Problem: Variable expansion inside single quotes.

// // Location: expander.c: expand_variables, lexer*.c (tokenizer logic).

// // Cause: Your tokenizer likely merges quoted content into a general T_WORD token, losing the single-quote context. Alternatively, expand_variables might be incorrectly processing tokens. The function currently expands T_WORD and T_DOUBLE_QUOTED, which seems right if the tokenizer produced a T_SINGLE_QUOTED token correctly.

// // Fix:

// // Tokenizer: Ensure your tokenizer (tokenize_input and helpers like handle_quoted) creates distinct tokens with type T_SINGLE_QUOTED for content within single quotes, rather than just appending the content to a current_word that becomes T_WORD. The handle_quoted function in lexer_utils3.c seems problematic as it appends content instead of creating a typed token. It should likely call add_token with T_SINGLE_QUOTED or T_DOUBLE_QUOTED.

// // Expander: Verify that expand_variables explicitly skips expansion for tokens of type T_SINGLE_QUOTED. The current code if (tokens->type == T_DOUBLE_QUOTED || tokens->type == T_WORD) already does this implicitly, so the issue is almost certainly in the tokenizer creating the wrong token type.

// // Test 23: pwd oi

// // Problem: pwd incorrectly fails with "too many arguments". Bash's pwd ignores extra arguments.

// // Location: builtins/bi_pwd.c: bi_pwd

// // Cause: The check if (argv[1]) causes the failure.

// // Fix: Remove the argument check from bi_pwd. It should simply get and print the directory, ignoring argv[1], argv[2], etc., and always return 0 on success.

// // // In bi_pwd function:
// // int bi_pwd(char *argv[])
// // {
// //     char cwd[PATH_MAX];

// //     // REMOVE or comment out this check:
// //     /*
// //     if (argv[1])
// //     {
// //         ft_putstr_fd("pwd: too many arguments\n", STDERR_FILENO);
// //         return (1);
// //     }
// //     */
// //     (void)argv; // Indicate argv is intentionally unused

// //     if (getcwd(cwd, sizeof(cwd)) != NULL)
// //     {
// //         ft_putstr_fd(cwd, STDOUT_FILENO);
// //         ft_putstr_fd("\n", STDOUT_FILENO);
// //         return (0); // Success
// //     }
// //     perror("minishell: pwd");
// //     return (1); // Error (getcwd failed)
// // }


// // Test 40: cd $PWD hi

// // Problem: cd doesn't detect too many arguments and exits with 0. Bash exits with 1 and an error.

// // Location: builtins/bi_cd.c: bi_cd

// // Cause: Missing check for args[2].

// // Fix: Add a check at the beginning of bi_cd.

// // // At the beginning of bi_cd function:
// // int bi_cd(char **args, t_env **env)
// // {
// //     char *old_pwd;
// //     int special_case_result;

// //     // ADD this check
// //     if (args[1] && args[2])
// //     {
// //          ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
// //          return (ERROR); // Return 1
// //     }

// //     old_pwd = getcwd(NULL, 0);
// //     // ... rest of the function ...
// // }
// // IGNORE_WHEN_COPYING_START
// // content_copy
// // download
// // Use code with caution.
// // C
// // IGNORE_WHEN_COPYING_END

// // Test 53: env | sort | grep -v SHLVL | grep -v ^_

// // Problem: Output is incorrect (not sorted, contains filtered items). Suggests pipes aren't working correctly, especially for sort.

// // Location: execution.c: handle_child_process, pipes.c: execute_pipeline, pipes.c: update_pipe_status / cleanup_pipes.

// // Cause: Likely incorrect file descriptor management (dup2, close) for pipes. sort probably isn't receiving the full output of env on its standard input.

// // Fix: Carefully review the pipe setup in handle_child_process and the closing of pipe ends in both the child and the parent loop (execute_pipeline).

// // Child (handle_child_process):

// // dup2(prev_pipe_read, STDIN_FILENO): Correctly sets input from previous command.

// // dup2(pipe_fd[1], STDOUT_FILENO): Correctly sets output to next command (if cmd->next).

// // Crucially, all pipe ends related to the current pipe (pipe_fd[0], pipe_fd[1]) and the previous pipe (prev_pipe_read) must be closed in the child after dup2 calls, as the child doesn't need them directly anymore. Your code seems to close pipe_fd[0], pipe_fd[1], and prev_pipe_read which looks right.

// // Parent (execute_pipeline loop):

// // The parent must close the ends of the pipes it's not using to avoid hangs and ensure EOF is sent correctly.

// // After forking, the parent needs to close pipe_fd[1] (the write end of the current pipe).

// // It also needs to close prev_pipe_read (the read end from the previous command's pipe) after the current child has been forked (as the child inherits it, dups it, and closes it).

// // The update_pipe_status / cleanup_pipes logic seems to handle this, but double-check its correctness. Ensure prev_pipe_read is correctly updated to pipe_fd[0] for the next iteration.

// // Test 56: export GHOST=123 | env | grep GHOST

// // Problem: export change doesn't persist for env in the pipe. Mini exit 0, Bash exit 1 (Tester seems wrong here, Bash usually exits 0 if grep succeeds).

// // Location: execution.c: handle_child_process, builtin_handler.c: run_builtin.

// // Cause: Standard subshell behavior in pipelines. export runs in a child process. Its environment modifications are local to that child and are lost. The next child (env) inherits the original environment from the main minishell process.

// // Fix: This behavior is technically correct regarding subshell isolation. Matching Bash's apparent propagation within the pipeline (where env does see GHOST) is complex and might require non-standard environment passing between pipeline stages. For this project, it might be acceptable to differ from Bash here, but you should ensure your exit code logic is correct (likely 0 if env | grep GHOST succeeds). The tester's claim of Bash exit 1 needs verification for this specific command.

// // Test 65 & 66: echo < ... <missing < ...

// // Problem: Failure on non-existent input redirection file (missing) is not reported, exit code is 0 instead of 1.

// // Location: redirs_execute.c: setup_redirections, redirs_execute.c: process_single_redirection, execution.c: handle_child_process, builtin_handler.c: handle_parent_builtin.

// // Cause: The return status (-1) from handle_input_redir (when open fails) isn't properly handled higher up. The error message is also missing.

// // Fix:

// // Error Message: In process_single_redirection, when status < 0, print the specific error using perror or ft_putstr_fd mimicking bash: missing: No such file or directory. Your code already prints this, ensure it goes to STDERR_FILENO.

// // Propagation:

// // Ensure process_single_redirection returns -1 on failure.

// // Ensure setup_redirections returns -1 if any process_single_redirection call fails.

// // In handle_child_process: Check the return value of setup_redirections. If it's < 0, exit(1).

// // In handle_parent_builtin: Check the return value of setup_redirections. If it's < 0, set g_exit_status = 1 and return (after restoring fds).

// // Test 68: echo <in_big | cat <in & Test 71: echo hi | cat <in

// // Problem: Input redirection (<infile) on the cat command doesn't override the input coming from the pipe. cat reads pipe instead of the file.

// // Location: execution.c: handle_child_process.

// // Cause: The order of operations seems correct (pipe dup2 then redirection dup2), so the issue might be subtle. Possibilities:

// // A file descriptor leak somewhere.

// // An error during setup_redirections for cat that isn't properly handled (less likely, as cat <infile works alone).

// // Incorrect closing of file descriptors related to the pipe or the redirected file fd, confusing cat.

// // Fix: This requires careful debugging.

// // Add fprintf(stderr, ...) statements in handle_child_process before and after the pipe dup2, and before and after the setup_redirections call, printing STDIN_FILENO and relevant pipe/file fds to trace how stdin is changing.

// // Verify that handle_input_redir closes the original file fd after the dup2 call. (It does).

// // Ensure pipe file descriptors are meticulously closed.

// // Test 69: echo <in_big | cat infile & Test 72: echo hi | cat infile

// // Problem: cat command with a file argument doesn't read the argument file when its stdin is connected to a pipe. It seems to be reading from the (empty or 'hi') pipe instead.

// // Location: execution.c: execute_external, execution.c: handle_child_process.

// // Cause: This is strange. cat (the external program) should ignore stdin if given a file argument. Why would it read from the pipe?

// // Are cmd->args correct when execute_external is called for cat? (i.e., {"cat", "./test_files/infile", NULL})

// // Is execve actually succeeding? Check the return value and errno.

// // Is the PATH environment variable correctly passed in env_arr?

// // Could setup_redirections for the first command (echo) somehow mess up the environment or FDs for the second command? Unlikely, but possible.

// // Fix: Debugging is needed.

// // In execute_external, print the cmd->args[0], cmd->args[1], etc., and the path found just before calling execve.

// // Check the exit status of the child process running cat using the status variable in wait_for_children. Did cat exit normally (0) or with an error?

// // Test 70: echo <in_big | echo <in

// // Problem: Second echo doesn't produce output. Bash prints a newline.

// // Location: Pipe/Redirection interaction (handle_child_process).

// // Cause: Similar to Test 68. The setup for the second echo (pipe stdin, overridden by <infile stdin redirection, standard builtin execution writing to pipe stdout) seems to be failing.

// // Fix: Debugging needed, similar to Test 68. Focus on the file descriptors for the second echo command in the pipeline. Ensure its STDOUT_FILENO is correctly connected (it should be the default stdout if it's the last command, or the write end of the next pipe if there was one) after input redirections are handled.

// // Test 74: #cat ...

// // Problem: # not treated as a comment start.

// // Location: main.c: process_input or lexer.c: tokenize_input.

// // Cause: No logic to detect comments.

// // Fix: Add comment handling before tokenization.

// // // In process_input (main.c) before calling tokenize_input
// // void process_input(char *input, t_env **env)
// // {
// //     t_token *tokens = NULL;
// //     t_cmd *cmds = NULL;
// //     t_resources res = {0};
// //     char *comment_ptr;

// //     // ADD COMMENT HANDLING HERE
// //     comment_ptr = ft_strchr(input, '#');
// //     if (comment_ptr) {
// //          *comment_ptr = '\0'; // Truncate input at the comment character
// //     }
// //     // Now check if input is empty after removing comment / whitespace
// //     // (Need a trim function or loop to check if only whitespace remains)
// //     // If input becomes empty after comment removal, just free and return.

// //     res.input = input; // Keep track even if modified
// //     res.env = *env;

// //     // Only tokenize if input is not empty/comment
// //     if (input[0] == '\0' /* || is_only_whitespace(input) */ ) {
// //          free(input);
// //          return;
// //     }

// //     tokens = tokenize_input(input);
// //     // ... rest of the function ...
// // }
// // IGNORE_WHEN_COPYING_START
// // content_copy
// // download
// // Use code with caution.
// // C
// // IGNORE_WHEN_COPYING_END

// // You'll need a way to handle potential leading whitespace before the # and to check if the line becomes effectively empty after stripping the comment.

// // By addressing these points, especially the tokenizer/expander for quotes, the pipe/redirection FD management, and the builtin argument/error handling, you should be able to fix most of these failing tests. Good luck!#include "inc/minishell.h"

// //caly file - podzielony init_env
// char	*extract_key(char *env_var, int *key_len)
// {
// 	char	*eq;
// 	char	*key;

// 	eq = ft_strchr(env_var, '=');
// 	if (!eq)
// 		return (NULL);
// 	*key_len = eq - env_var;
// 	key = ft_substr(env_var, 0, *key_len);
// 	return (key);
// }

// int	handle_env_node_creation(char *key, char *value, t_env **env)
// {
// 	t_env	*new_node;

// 	new_node = env_new(key, value);
// 	if (!new_node)
// 	{
// 		free(key);
// 		free(value);
// 		perror("minishell: malloc");
// 		free_env(*env);
// 		exit(EXIT_FAILURE);
// 	}
// 	env_add_back(env, new_node);
// 	free(key);
// 	free(value);
// 	return (1);
// }

// int	process_single_env_var(char *env_var, t_env **env)
// {
// 	char	*key;
// 	char	*value;
// 	int		key_len;

// 	key = extract_key(env_var, &key_len);
// 	if (!key)
// 		return (0);
// 	value = ft_strdup(ft_strchr(env_var, '=') + 1);
// 	if (!value)
// 	{
// 		free(key);
// 		return (0);
// 	}
// 	return (handle_env_node_creation(key, value, env));
// }

// void	init_env(char **envp, t_env **env)
// {
// 	int	i;

// 	i = 0;
// 	while (envp[i])
// 	{
// 		process_single_env_var(envp[i], env);
// 		i++;
// 	}
// }
// #include "inc/minishell.h"

// // dodaje tokeny for a single command line, to the beginning of the linked list
// void	add_token(t_token **tokens, char *value, t_token_type type)
// {
// 	t_token	*new;

// 	new = malloc(sizeof(t_token));
// 	if (!new)
// 		return ;
// 	new->value = ft_strdup(value); // MUST duplicate the string
// 	if (!new->value)
// 	{
// 		free(new);
// 		return ;
// 	}
// 	new->type = type;
// 	new->next = *tokens;
// 	*tokens = new;
// }

// t_token	*reverse_tokens(t_token *tokens)
// {
// 	t_token	*prev;
// 	t_token	*current;
// 	t_token	*next;

// 	prev = NULL;
// 	current = tokens;
// 	next = NULL;
// 	while (current)
// 	{
// 		next = current->next;
// 		current->next = prev;
// 		prev = current;
// 		current = next;
// 	}
// 	return (prev);
// }
// //moje
// // void	parse_input_loop(char *input, t_parse_state *state)
// // {
// // 	while (input[state->i] && !(state->error))
// // 	{
// // 		if (state->current_quote != NO_QUOTE)
// // 		{
// // 			state->tokens = handle_unexpected_quote(state->current_word,
// // 					state->tokens);
// // 			return ;
// // 		}
// // 		else if (is_quote(input[state->i])) // Sprawdzanie, czy to cytat
// // 		{
// // 			// state->error = handle_quoted(input, &state->i, &state->tokens,
// // 			// 		&state->error);
// // 			state->error = handle_quoted(input, &state->i, &state->current_word,
// // 				 	&state->error);
// // 		}
// // 		else // Inne przypadki
// // 		{
// // 			handle_input_char(input, &state->i, &state->current_word,
// // 				&state->tokens);
// // 		}
// // 	}
// // }


// void	parse_input_loop(char *input, t_parse_state *state)
// {
//     while (input[state->i] && !(state->error))
//     {
//         // No need for the state->current_quote check here anymore

//         if (is_quote(input[state->i])) // Found a quote character
//         {
//             // If there's an existing word being built, finalize it first
//             if (state->current_word)
//                 finalize_current_word(&state->tokens, &state->current_word);

//             // Now handle the quote, passing the main token list
//             state->error = handle_quoted(input, &state->i, &state->tokens, &state->error);
//         }
//         else // Handle other characters (space, pipe, redirect, word chars)
//         {
//             // handle_input_char might finalize current_word if it encounters space/pipe/redirect
//             handle_input_char(input, &state->i, &state->current_word, &state->tokens);
//         }
//     }
// }

// t_token	*tokenize_input(char *input)
// {
// 	t_parse_state	state;

// 	state.tokens = NULL;
// 	state.current_word = NULL;
// 	state.current_quote = NO_QUOTE;
// 	state.i = 0;
// 	state.error = 0;
// 	parse_input_loop(input, &state);
// 	if (state.current_word)
// 		finalize_current_word(&state.tokens, &state.current_word);
// 	if (state.error)
// 	{
// 		free_tokens(state.tokens);
// 		return (NULL);
// 	}
// 	return (reverse_tokens(state.tokens));
// }

// void	handle_input_char(char *input, int *i, char **current_word,
// 		t_token **tokens)
// {
// 	if (ft_isspace(input[*i]))
// 		skip_space(i, current_word, tokens);
// 	else if (is_pipe(input[*i]))
// 		handle_pipe(i, current_word, tokens);
// 	else if (is_redirect(input[*i]))
// 		handle_redirect_token(input, i, current_word, tokens);
// 	else
// 		append_char_to_word(current_word, input[(*i)++]);
// }#include "inc/minishell.h"

// int	is_pipe(char c)
// {
// 	return (c == '|');
// }

// int	is_redirect(char c)
// {
// 	return (c == '>' || c == '<');
// }

// int	is_quote(char c)
// {
// 	return (c == '\'' || c == '"');
// }

// t_quote	get_quote_type(char c)
// {
// 	if (c == '\'')
// 		return (SINGLE_QUOTE);
// 	return (DOUBLE_QUOTE);
// }

// t_token_type	get_token_type_from_quote(t_quote quote)
// {
// 	if (quote == SINGLE_QUOTE)
// 		return (T_SINGLE_QUOTED);
// 	return (T_DOUBLE_QUOTED);
// }#include "inc/minishell.h"

// void	finalize_current_word(t_token **tokens, char **current_word)
// {
// 	add_token(tokens, *current_word, T_WORD);
// 	free(*current_word);
// 	*current_word = NULL;
// }

// int	handle_quote(char *input, int *i, t_token **tokens, int *error)
// {
// 	t_quote			current_quote;
// 	char			*quoted;
// 	t_token_type	quote_type;

// 	current_quote = get_quote_type(input[*i]);
// 	(*i)++;
// 	quoted = read_quoted_content(input, i, current_quote, error);
// 	if (*error)
// 		return (ft_putstr_fd("Minishell: syntax error: unclosed quote\n",
// 				STDERR_FILENO), 1);
// 	quote_type = get_token_type_from_quote(current_quote);
// 	add_token(tokens, quoted, quote_type);
// 	free(quoted);
// 	return (0);
// }

// char	*read_quoted_content(char *input, int *i, t_quote quote, int *error)
// {
// 	char	quote_char;
// 	int		start;
// 	char	*content;

// 	if (quote == SINGLE_QUOTE)
// 		quote_char = '\'';
// 	else
// 		quote_char = '"';
// 	start = *i;
// 	while (input[*i] && input[*i] != quote_char)
// 		(*i)++;
// 	if (input[*i] != quote_char)
// 	{
// 		*error = 1;
// 		return (NULL);
// 	}
// 	content = ft_substr(input, start, *i - start);
// 	(*i)++;
// 	return (content);
// }

// //stare - diala program, ale valgrind wywala bledy
// // void	free_tokens(t_token *tokens)
// // {
// // 	t_token	*tmp;

// // 	while (tokens)
// // 	{
// // 		tmp = tokens;
// // 		tokens = tokens->next;
// // 		free(tmp->value);
// // 		free(tmp);
// // 	}
// // }

// //2 leaks
// void free_tokens(t_token *tokens)
// {
//     t_token *tmp;

//     while (tokens)
//     {
//         tmp = tokens;
//         if (tokens->value)
//             free(tokens->value); // <<< Free the string!
//         tokens = tokens->next;
//         free(tmp);
//     }
// }
// #include "inc/minishell.h"

// void	skip_space(int *i, char **word, t_token **tokens)
// {
// 	if (*word)
// 		finalize_current_word(tokens, word);
// 	(*i)++;
// }

// void	handle_pipe(int *i, char **word, t_token **tokens)
// {
// 	if (*word)
// 		finalize_current_word(tokens, word);
// 	add_token(tokens, "|", T_PIPE);
// 	(*i)++;
// }

// // //moje
// // int	handle_quoted(char *input, int *i, char **current_word, int *error)
// // {
// // 	t_quote current_quote;
// // 	char *quoted;

// // 	current_quote = get_quote_type(input[*i]);
// // 	(*i)++;
// // 	quoted = read_quoted_content(input, i, current_quote, error);
// // 	if (*error)
// // 		return (ft_putstr_fd("Minishell: syntax error: unclosed quote\n", STDERR_FILENO), 1);
	
// // 	// Append quoted content to the current word
// // 	if (*current_word == NULL)
// // 		*current_word = ft_strdup(quoted);
// // 	else
// // 	{
// // 		char *tmp = ft_strjoin(*current_word, quoted);
// // 		free(*current_word);
// // 		*current_word = tmp;
// // 	}
// // 	free(quoted);
// // 	return (0);
// // }

// int handle_quoted(char *input, int *i, t_token **tokens, int *error)
// {
//     t_quote         current_quote;
//     char            *quoted_content;
//     t_token_type    quote_type;

//     current_quote = get_quote_type(input[*i]);
//     (*i)++; // Move past the opening quote
//     quoted_content = read_quoted_content(input, i, current_quote, error);
//     if (*error)
//     {
//         // read_quoted_content returns NULL on error, nothing to free here
//         ft_putstr_fd("Minishell: syntax error: unclosed quote\n", STDERR_FILENO);
//         return (1); // Indicate error
//     }

//     // Determine token type based on quote type
//     quote_type = get_token_type_from_quote(current_quote);

//     // Add the quoted content as a specific token type
//     add_token(tokens, quoted_content, quote_type);

//     free(quoted_content); // Free the content returned by read_quoted_content
//     return (0); // Success
// }

// void	append_char_to_word(char **word, char c)
// {
// 	*word = ft_strjoin_char(*word, c);
// }

// t_token	*handle_unexpected_quote(char *word, t_token *tokens)
// {
// 	free(word);
// 	free_tokens(tokens);
// 	return (NULL);
// }#include "inc/minishell.h"

// void	handle_redirect_token(char *input, int *i, char **word,
// 		t_token **tokens)
// {
// 	if (*word)
// 		finalize_current_word(tokens, word);
// 	handle_redirection(input, i, tokens);
// }

// void	validate_redirection_syntax(t_token **tokens)
// {
// 	if (*tokens && (*tokens)->next)
// 	{
// 		if ((*tokens)->next->type != T_WORD
// 			&& (*tokens)->next->type != T_SINGLE_QUOTED
// 			&& (*tokens)->next->type != T_DOUBLE_QUOTED)
// 		{
// 			ft_putstr_fd("minishell: syntax error:\n", STDERR_FILENO);
// 			ft_putstr_fd("Missing file name after redirection\n",
// 				STDERR_FILENO);
// 			*tokens = NULL; // To indicate an error and avoid further processing
// 		}
// 	}
// }

// void	handle_redirection(char *input, int *i, t_token **tokens)
// {
// 	char			first_char;
// 	t_token_type	type;

// 	first_char = input[*i];
// 	(*i)++;
// 	// Move to the next character to check for redirection
// 	if (input[*i] == first_char) // Handle double operator (>> or <<)
// 	{
// 		(*i)++;
// 		if (first_char == '>')
// 			type = T_APPEND;
// 		else
// 			type = T_HEREDOC;
// 		add_token(tokens, (char[]){first_char, first_char, '\0'}, type);
// 	}
// 	else // Handle single operator (> or <)
// 	{
// 		if (first_char == '>')
// 			type = T_REDIR_OUT;
// 		else
// 			type = T_REDIR_IN;
// 		add_token(tokens, (char[]){first_char, '\0'}, type);
// 	}
// 	validate_redirection_syntax(tokens); // Call the validation function
// }#include "inc/minishell.h"
// #include <mcheck.h>

// int main(int argc, char **argv, char **envp)
// {
//     t_env *env;
//     char *input;
//     t_resources res;

//     (void)argc;
//     (void)argv;
//     init_shell(envp, &env);

//     while (1)
//     {
//         input = readline("minishell$ ");
//         if (!input)
//         {
//             res.env = env;
//             res.cmds = NULL;
//             res.tokens = NULL;
//             res.input = NULL;
//             cleanup(&res); // Pass the t_resources structure
//             //handle_eof(env, g_exit_status);
//             handle_eof(g_exit_status);
//         }
//         if (*input)
//         {
//             add_history(input);
//             process_input(input, &env);
//         }
//         else
//             free(input);
//     }
//     return (0);
// }

// //clear history at the end
// /* 4,016 bytes in 1 blocks are still reachable in loss record 41 of 63
// ==2074==    at 0x484880F: malloc (vg_replace_malloc.c:446)
// ==2074==    by 0x48A0BAC: xmalloc (in /usr/lib/x86_64-linux-gnu/libreadline.so.8.1)
// ==2074==    by 0x4899FD6: add_history (in /usr/lib/x86_64-linux-gnu/libreadline.so.8.1)
// ==2074==    by 0x10B197: main (main.c:19)*/#include "inc/minishell.h"

// // leakami, pred valgrindem
// // void	free_env(t_env *env)
// // {
// // 	t_env	*tmp;

// // 	while (env)
// // 	{
// // 		tmp = env;
// // 		env = env->next;
// // 		free(tmp->key);
// // 		free(tmp->value);
// // 		free(tmp);
// // 	}
// // }

// //po valgrindie
// void free_env(t_env *env)
// {
//     t_env *tmp;
    
//     while (env)
//     {
//         tmp = env->next;  // Save next pointer before freeing
//         free(env->key);
//         free(env->value);
//         free(env);
//         env = tmp;
//     }
// }

// void	init_shell(char **envp, t_env **env)
// {
// 	setup_signals_for_prompt();
// 	*env = NULL;
// 	init_env(envp, env);
// }

// // void	handle_eof(t_env *env, int exit_status)
// // {
// // 	ft_putstr_fd("exit\n", STDOUT_FILENO);
// // 	cleanup(env, NULL, NULL, NULL);
// // 	exit(exit_status);
// // }

// //dobre tera
// // void handle_eof(t_env *env, int exit_status)
// // {
// //     t_resources res;

// //     res.env = env;
// //     res.cmds = NULL;
// //     res.tokens = NULL;
// //     res.input = NULL;

// //     ft_putstr_fd("exit\n", STDOUT_FILENO);
// // 	rl_clear_history(); // added 28.04
// //     cleanup(&res); // Pass the t_resources structure
// //     exit(exit_status);
// // }



// //diala  exit no leaks!! ale nie /bin/;s
// // void handle_eof(t_env *env, int exit_status)
// // {
// //     ft_putendl_fd("exit", STDERR_FILENO);
// //     rl_clear_history(); // Clear Readline history
// //     free_env(env);
// //     exit(exit_status);
// // }

// void handle_eof(int exit_status)
// {
//     // Don't free env here, it's already freed in cleanup
//     // free_env(env);  <- Remove this line
    
//     // Clear readline history
//     rl_clear_history();
    
//     // Print a newline for better UX when exiting with Ctrl+D
//     ft_putstr_fd("exit\n", STDOUT_FILENO);
    
//     exit(exit_status);
// }

// // void	process_input(char *input, t_env **env)
// // {
// // 	t_token	*tokens;
// // 	t_cmd	*cmds;

// // 	tokens = tokenize_input(input);
// // 	if (!tokens)
// // 	{
// // 		free(input); //added valgrind 28.04
// // 		return ;
// // 	}
// // 	expand_variables(tokens, *env);
// // 	cmds = parse_tokens(tokens);
// // 	if (!cmds)
// // 	{
// // 		free_tokens(tokens);
// // 		free(input); //added valgrind 28.04
// // 		return ;
// // 	}
// // 	if (cmds->args && cmds->args[0])
// // 	{
// // 		setup_signals_for_command();
// // 		execute(cmds, env);
// // 		setup_signals_for_prompt();
// // 	}
// // 	clean_cmds_tokens_input(cmds, tokens, input);
// // }

// //2 leaki
// // void process_input(char *input, t_env **env)
// // {
// //     t_token *tokens = NULL;
// //     t_cmd *cmds = NULL;

// //     tokens = tokenize_input(input);
// //     if (!tokens) {
// //         free(input); // Free input if tokenization fails
// //         return;
// //     }
// //     expand_variables(tokens, *env);
// //     cmds = parse_tokens(tokens);
// //     if (!cmds) {
// //         free_tokens(tokens); // Free tokens if parsing fails
// //         free(input);       // Free input if parsing fails
// //         return;
// //     }

// //     if (cmds->args && cmds->args[0]) {
// //         setup_signals_for_command();
// //         // *** Pass tokens and input to execute ***
// //         execute(cmds, env);
// //         setup_signals_for_prompt();
// //     }

// //     // If execute finished normally (didn't call exit), clean up.
// //     // If bi_exit was called successfully, it exited, and we won't reach here.
// //     // If bi_exit failed validation and returned, we WILL reach here and clean up.
// //     // Note: We assume execute itself doesn't free these if exit wasn't called.
// //     // If exit was called successfully, these pointers are invalid anyway after cleanup in bi_exit.
// //     if (cmds)
// // 		free_cmds(cmds);
// //     if (tokens)
// // 		free_tokens(tokens);
// //     if (input)
// // 		free(input);
// // }

// // //2 leaki, ok
// // void process_input(char *input, t_env **env)
// // {
// //     t_token *tokens = NULL;
// //     t_cmd *cmds = NULL;

// //     tokens = tokenize_input(input);
// //     if (!tokens) {
// //         free(input);
// //         return;
// //     }
    
// //     expand_variables(tokens, *env);
    
// //     cmds = parse_tokens(tokens);
// //     if (!cmds) {
// //         free_tokens(tokens);
// //         free(input);
// //         return;
// //     }

// //     if (cmds->args && cmds->args[0]) 
// //     {
// //         setup_signals_for_command();
// //         execute(cmds, env);
// //         setup_signals_for_prompt();
// //     }

// //     // Always free resources
// //     free_cmds(cmds);
// //     free_tokens(tokens);
// //     free(input);
// // }

// //moje stare
// void process_input(char *input, t_env **env)
// {
//     t_token *tokens = NULL;
//     t_cmd *cmds = NULL;
//     t_resources res = {0};  // Initialize resources structure
    
//     // Track ALL resources from the beginning
//     res.input = input;
//     res.env = *env;
    
//     tokens = tokenize_input(input);
//     if (!tokens) {
//         free(input);
//         return;
//     }
    
//     res.tokens = tokens;  // Save tokens in resources
    
//     expand_variables(tokens, *env);
    
//     cmds = parse_tokens(tokens);
//     if (!cmds) {
//         free_tokens(tokens);
//         free(input);
//         return;
//     }
    
//     res.cmds = cmds;  // Save commands in resources
    
//     if (cmds->args && cmds->args[0]) {
//         setup_signals_for_command();
        
//         // Pass the COMPLETE resources structure
//         if (is_builtin(cmds->args[0])) 
//         {
//             // Create and initialize t_fds structure with the duplicated file descriptors
//             t_fds fds;
//             fds.stdin = dup(STDIN_FILENO);
//             fds.stdout = dup(STDOUT_FILENO);
            
//             // Pass the properly initialized t_fds to handle_parent_builtin
//             handle_parent_builtin(cmds, fds, &res);
//         } else {
//             execute(cmds, env);
//         }
        
//         setup_signals_for_prompt();
//     }

//     // Only clean up if we didn't call exit
//     free_cmds(cmds);
//     free_tokens(tokens);
//     free(input);
// }

// void	clean_cmds_tokens_input(t_cmd *cmds, t_token *tokens, char *input)
// {
// 	if (cmds)
// 		free_cmds(cmds);
// 	if (tokens)
// 		free_tokens(tokens);
// 	if (input)
// 		free(input);
// }
// #include "inc/minishell.h"

// /**
//  * Obsługuje tokeny przekierowań w parserze.
//  * - tokens: podwójny wskaźnik do aktualnego tokenu (np. `>` lub `<<`).
//  * - cmd: bieżąca komenda, do której dodawane jest przekierowanie.
//  */
// void	handle_redirection_parser(t_token **tokens, t_cmd *cmd)
// {
// 	t_redir	*new_redir;

// 	new_redir = malloc(sizeof(t_redir));
// 	if (!new_redir)
// 	{
// 		perror("minishell");
// 		return ;
// 	}
// 	// Set redirection type based on token
// 	new_redir->type = (*tokens)->type;
// 	// Move to next token (should be filename)
// 	*tokens = (*tokens)->next;
// 	if (!*tokens || ((*tokens)->type != T_WORD
// 			&& (*tokens)->type != T_SINGLE_QUOTED
// 			&& (*tokens)->type != T_DOUBLE_QUOTED))
// 	{
// 		ft_putstr_fd("minishell: syntax error near unexpected token\n",
// 			STDERR_FILENO);
// 		free(new_redir);
// 		return ;
// 	}
// 	// Assign filename and add redirection to command
// 	new_redir->file = ft_strdup((*tokens)->value);
// 	new_redir->next = NULL;
// 	add_redir_to_cmd(cmd, new_redir);
// 	// Move to next token
// 	*tokens = (*tokens)->next;
// }

// /// do usuniecia, ponizej mam ta sama wersje
// void	free_cmd_contents(t_cmd *cmd)
// {
// 	t_redir	*redir;
// 	t_redir	*next_redir;
// 	int		i;

// 	if (cmd->args != NULL)
// 	{
// 		i = 0;
// 		while (cmd->args[i] != NULL)
// 		{
// 			free(cmd->args[i]);
// 			i++;
// 		}
// 		free(cmd->args);
// 	}
// 	redir = cmd->redirections;
// 	while (redir != NULL)
// 	{
// 		next_redir = redir->next;
// 		free(redir->file);
// 		free(redir);
// 		redir = next_redir;
// 	}
// }

// void	free_cmds(t_cmd *cmds)
// {
// 	t_cmd	*current;
// 	t_cmd	*next;

// 	current = cmds;
// 	while (current != NULL)
// 	{
// 		next = current->next;
// 		free_cmd_contents(current);
// 		if (current->pipe_fd[0] != -1)
// 			close(current->pipe_fd[0]);
// 		if (current->pipe_fd[1] != -1)
// 			close(current->pipe_fd[1]);
// 		free(current);
// 		current = next;
// 	}
// }

// t_cmd	*process_token(t_token **tokens, t_cmd *current_cmd, t_cmd **cmds)
// {
// 	char	*arg_copy;

// 	if (is_redirection((*tokens)->type))
// 	{
// 		handle_redirection_parser(tokens, current_cmd);
// 	}
// 	else if ((*tokens)->type == T_PIPE)
// 	{
// 		*tokens = (*tokens)->next; // Skip pipe token
// 	}
// 	else
// 	{
// 		arg_copy = ft_strdup((*tokens)->value);
// 		if (!arg_copy)
// 		{
// 			perror("minishell");
// 			free_cmds(*cmds);
// 			free_tokens(*tokens);
// 			return (NULL);
// 		}
// 		add_arg_to_cmd(current_cmd, arg_copy);
// 		*tokens = (*tokens)->next;
// 	}
// 	return (current_cmd);
// }

// t_cmd	*parse_tokens(t_token *tokens)
// {
// 	t_cmd	*cmds;
// 	t_cmd	*current_cmd;
// 	int		cmd_count;

// 	cmds = NULL;
// 	current_cmd = NULL;
// 	cmd_count = 0;
// 	while (tokens)
// 	{
// 		// Create new command if needed (start or after pipe)
// 		if (!current_cmd || tokens->type == T_PIPE)
// 		{
// 			cmd_add_back(&cmds, cmd_new());
// 			current_cmd = cmd_last(cmds);
// 			cmd_count++;
// 		}
// 		// Process current token
// 		current_cmd = process_token(&tokens, current_cmd, &cmds);
// 		if (!current_cmd) // Check for allocation failure
// 		{
// 			free_cmds(cmds);
// 			free_tokens(tokens);
// 			return (NULL);
// 		}
// 	}
// 	return (cmds);
// }
// #include "inc/minishell.h"

// /**
//  * Dodaje przekierowanie do listy w komendzie.
//  */
// void	add_redir_to_cmd(t_cmd *cmd, t_redir *new_redir)
// {
// 	t_redir	*last;

// 	if (!cmd->redirections)
// 	{
// 		cmd->redirections = new_redir;
// 	}
// 	else
// 	{
// 		last = cmd->redirections;
// 		while (last->next)
// 		{
// 			last = last->next;
// 		}
// 		last->next = new_redir;
// 	}
// }

// t_cmd	*cmd_new(void)
// {
// 	t_cmd	*cmd;

// 	cmd = malloc(sizeof(t_cmd));
// 	if (!cmd)
// 		return (NULL);
// 	cmd->args = NULL;
// 	cmd->redirections = NULL;
// 	cmd->next = NULL;
// 	cmd->pipe_fd[0] = -1;
// 	cmd->pipe_fd[1] = -1;
// 	return (cmd);
// }

// t_cmd	*cmd_last(t_cmd *cmds)
// {
// 	while (cmds && cmds->next)
// 	{
// 		cmds = cmds->next;
// 	}
// 	return (cmds);
// }

// // a pointer to a pointer allows the function to modify 
// //the head of the list (first element)
// // used to add a new command (new_cmd) t the end of the list of commands.
// // appends a new t_cmd structure to the list,
// //	it either reates the list if empty,
// // or adds to the end if the list already has elemensrs
// void	cmd_add_back(t_cmd **cmds, t_cmd *new_cmd)
// {
// 	t_cmd	*last;

// 	if (!*cmds)
// 	{
// 		*cmds = new_cmd;
// 	}
// 	else
// 	{
// 		last = cmd_last(*cmds);
// 		last->next = new_cmd;
// 	}
// }

// int	is_redirection(t_token_type type)
// {
// 	return (type == T_REDIR_IN || type == T_REDIR_OUT || type == T_APPEND
// 		|| type == T_HEREDOC);
// }
// #include "inc/minishell.h"

// void	add_arg_to_cmd(t_cmd *cmd, char *arg)
// {
// 	int		count;
// 	char	**new_args;
// 	int		i;

// 	count = 0;
// 	i = 0;
// 	if (cmd->args)
// 		while (cmd->args[count])
// 			count++;
// 	new_args = malloc(sizeof(char *) * (count + 2));
// 	if (!new_args)
// 	{
// 		perror("minishell");
// 		exit(EXIT_FAILURE); // Or handle more gracefully
// 	}
// 	while (i < count)
// 	{
// 		new_args[i] = cmd->args[i];
// 		i++;
// 	}
// 	new_args[count] = arg;
// 	new_args[count + 1] = NULL;
// 	if (cmd->args)
// 		free(cmd->args);
// 	cmd->args = new_args;
// }
// #include "inc/minishell.h"

// void	prepare_pipeline_execution(t_cmd *cmds, int *prev_pipe_read)
// {
// 	*prev_pipe_read = -1;
// 	setup_signals_for_command();
// 	(void)cmds;
// }
// //moje
// void	execute_pipeline(t_cmd *cmds, t_env **env, int *prev_pipe_read)
// {
// 	int		pipe_fd[2];

// 	while (cmds)
// 	{
// 		pipe_fd[0] = -1;
// 		pipe_fd[1] = -1;
// 		process_command(cmds, env, *prev_pipe_read, pipe_fd);
// 		update_pipe_status(cmds, prev_pipe_read, pipe_fd);
// 		cmds = cmds->next;
// 	}
// 	wait_for_children();
// }

// void	cleanup_pipes(int *pipe_fd, int *prev_pipe_read)
// {
// 	if (*prev_pipe_read != -1)
// 		close(*prev_pipe_read);
// 	if (pipe_fd[1] != -1)
// 		close(pipe_fd[1]);
// 	*prev_pipe_read = pipe_fd[0];
// }

// void	update_pipe_status(t_cmd *cmd, int *prev_pipe_read, int *pipe_fd)
// {
// 	if (cmd->next)
// 		cleanup_pipes(pipe_fd, prev_pipe_read);
// 	else if (pipe_fd[0] != -1)
// 		close(pipe_fd[0]);
// }

// // // Dodaje pojedynczy znak do stringa
// // char	*ft_strjoin_char(char *s, char c)
// // {
// // 	char	*new;
// // 	int		len;

// // 	len = 0;
// // 	if (s)
// // 		len = ft_strlen(s);
// // 	new = malloc(len + 2); // +1 for char, +1 for '\0'
// // 	if (!new)
// // 		return (NULL);
// // 	if (s)
// // 		ft_memcpy(new, s, len);
// // 	new[len] = c;
// // 	new[len + 1] = '\0';
// // 	free(s);
// // 	return (new);
// // }


// // char *expand_str(char *str, t_env *env)
// // {
// //     char *result;
// //     int i;

// //     result = ft_strdup("");
// //     if (!result)
// //         return (NULL);
// //     i = 0;
// //     while (str[i])
// //     {
// //         if (is_special_var(str, i))
// //             i = handle_special_var(&result, i);
// //         else if (is_braced_var(str, i))
// //             i = handle_braced_var(&result, str, i, env);
// //         else if (is_standard_var(str, i))
// //             i = handle_standard_var(&result, str, i, env);
// //         else
// //         {
// //             char *old_result = result;
// //             result = ft_strjoin_char(old_result, str[i]);
// //             if (!result)
// //             {
// //                 free(old_result);
// //                 return (NULL);
// //             }
// //             i++;
// //         }
// //     }
// //     return (result);
// // }


// // void expand_variables(t_token *tokens, t_env *env)
// // {
// //     char *expanded;

// //     while (tokens)
// //     {
// //         if (tokens->type == T_DOUBLE_QUOTED || tokens->type == T_WORD)
// //         {
// //             expanded = expand_str(tokens->value, env);
// //             if (expanded)
// //             {
// //                 free(tokens->value); // Free the old value
// //                 tokens->value = expanded;
// //             }
// //         }
// //         tokens = tokens->next;
// //     }
// // }



// // void process_input(char *input, t_env **env)
// // {
// //     t_token *tokens = NULL;
// //     t_cmd *cmds = NULL;

// //     tokens = tokenize_input(input);
// //     if (!tokens) {
// //         free(input);
// //         return;
// //     }
    
// //     expand_variables(tokens, *env);
    
// //     cmds = parse_tokens(tokens);
// //     if (!cmds) {
// //         free_tokens(tokens);
// //         free(input);
// //         return;
// //     }

// //     if (cmds->args && cmds->args[0]) {
// //         setup_signals_for_command();
// //         execute(cmds, env);
// //         setup_signals_for_prompt();
// //     }

// //     // Always free resources
// //     free_cmds(cmds);
// //     free_tokens(tokens);
// //     free(input);
// // }




// // int main(int argc, char **argv, char **envp)
// // {
// //     t_env *env;
// //     char *input;
// //     t_resources res;

// //     (void)argc;
// //     (void)argv;
// //     init_shell(envp, &env);

// //     while (1)
// //     {
// //         input = readline("minishell$ ");
// //         if (!input)
// //         {
// //             res.env = env;
// //             res.cmds = NULL;
// //             res.tokens = NULL;
// //             res.input = NULL;
// //             cleanup(&res); // Pass the t_resources structure
// //             handle_eof(env, g_exit_status);
// //         }
// //         if (*input)
// //         {
// //             add_history(input);
// //             process_input(input, &env);
// //         }
// //         else
// //             free(input);
// //     }
// //     return (0);
// // }

// // =6450== 5 bytes in 1 blocks are still reachable in loss record 2 of 66
// // ==6450==    at 0x484880F: malloc (vg_replace_malloc.c:446)
// // ==6450==    by 0x10BF1D: ft_strjoin_char (utils.c:29)
// // ==6450==    by 0x10A452: expand_str (expander.c:248)
// // ==6450==    by 0x10A146: expand_variables (expander.c:52)
// // ==6450==    by 0x10B485: process_input (main_utils.c:149)
// // ==6450==    by 0x10B335: main (main.c:58)#include "inc/minishell.h"

// void	handle_sigint_prompt(int sig)
// {
// 	(void)sig;
// 	write(STDOUT_FILENO, "\n", 1);
// 	rl_on_new_line();
// 	rl_replace_line("", 0);
// 	rl_redisplay();
// 	g_exit_status = 130; // Set exit status to 130 for Ctrl-C
// }

// void	handle_sigint_command(int sig)
// {
// 	(void)sig;
// 	write(STDOUT_FILENO, "\n", 1);
// 	g_exit_status = 130; // Set exit status to 130 for Ctrl-C
// }

// void	setup_signals_for_prompt(void)
// {
// 	signal(SIGINT, handle_sigint_prompt);
// 	signal(SIGQUIT, SIG_IGN);
// }

// void	setup_signals_for_command(void)
// {
// 	signal(SIGINT, handle_sigint_command);
// 	signal(SIGQUIT, SIG_IGN);
// }
// #include "inc/minishell.h"

// // Sprawdza, czy znak jest białym znakiem
// int	ft_isspace(char c)
// {
// 	return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f'
// 		|| c == '\v');
// }

// // Łączy dwa stringi, zwalniając s1
// char	*ft_strjoin_free(char *s1, char *s2)
// {
// 	char	*result;

// 	result = ft_strjoin(s1, s2);
// 	free(s1);
// 	return (result);
// }

// // Dodaje pojedynczy znak do stringa
// char	*ft_strjoin_char(char *s, char c)
// {
// 	char	*new;
// 	int		len;

// 	len = 0;
// 	if (s)
// 		len = ft_strlen(s);
// 	new = malloc(len + 2); // +1 for char, +1 for '\0'
// 	if (!new)
// 		return (NULL);
// 	if (s)
// 		ft_memcpy(new, s, len);
// 	new[len] = c;
// 	new[len + 1] = '\0';
// 	free(s);
// 	return (new);
// }

// int	ft_strcmp(const char *s1, const char *s2)
// {
// 	while (*s1 && (*s1 == *s2))
// 	{
// 		s1++;
// 		s2++;
// 	}
// 	return (*(unsigned char *)s1 - *(unsigned char *)s2);
// }
// #include "inc/minishell.h"

// /**
//  * Łączy trzy stringi w jeden (np. "dir", "/", "cmd" → "dir/cmd").
//  * Zwraca nowy string lub NULL przy błędzie alokacji.
//  */
// char	*ft_strjoin3(const char *s1, const char *s2, const char *s3)
// {
// 	char	*result;
// 	char	*tmp;

// 	tmp = ft_strjoin(s1, s2);
// 	if (!tmp)
// 		return (NULL);
// 	result = ft_strjoin(tmp, s3);
// 	free(tmp);
// 	return (result);
// }

// void	ft_free_split(char **split)
// {
// 	int	i;

// 	i = 0;
// 	if (!split)
// 		return ;
// 	while (split[i])
// 	{
// 		free(split[i]);
// 		i++;
// 	}
// 	free(split);
// }

// /* Helper function to check if command is an existing executable path */
// char	*check_absolute_path(char *cmd)
// {
// 	if (!cmd)
// 		return (NULL);
// 	// Check if command contains a slash (absolute/relative path)
// 	if (ft_strchr(cmd, '/'))
// 	{
// 		if (access(cmd, X_OK) == 0)
// 			return (ft_strdup(cmd));
// 	}
// 	return (NULL);
// }

// /* Main function to find executable path */
// //leaki
// char	*get_exec_path(char *cmd, t_env *env)
// {
// 	char	*path_value;
// 	char	**path_dirs;
// 	char	*exec_path;

// 	if (!cmd || !env)
// 		return (NULL);
// 	// First check if it's already an absolute/relative path
// 	exec_path = check_absolute_path(cmd);
// 	if (exec_path)
// 		return (exec_path);
// 	// Get PATH environment variable
// 	path_value = get_env_value("PATH", env);
// 	if (!path_value)
// 		return (NULL);
// 	// Split PATH into directories
// 	path_dirs = ft_split(path_value, ':');
// 	if (!path_dirs)
// 		return (NULL);
// 	// Search through each directory in PATH
// 	exec_path = search_in_path_dirs(cmd, path_dirs);
// 	ft_free_split(path_dirs);
// 	return (exec_path);
// }
// /* Helper function to search through PATH directories */
// char	*search_in_path_dirs(char *cmd, char **dirs)
// {
// 	int			i;
// 	char		*full_path;
// 	const char	*dir;

// 	i = 0;
// 	while (dirs[i])
// 	{
// 		dir = dirs[i];
// 		if (dir[0] == '\0')
// 			dir = "."; // Handle empty paths (e.g., PATH=:/bin)
// 		full_path = ft_strjoin3(dir, "/", cmd);
// 		if (!full_path)
// 			continue ;
// 		// w oryginale ponizej jeszcze w ifie ft_split_free
// 		if (access(full_path, X_OK) == 0)
// 			return (full_path);
// 		free(full_path);
// 		i++;
// 	}
// 	return (NULL);
// }
