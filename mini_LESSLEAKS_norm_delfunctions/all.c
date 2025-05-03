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

// void free_env_node(t_env *node)
// {
//     if (node)
//     {
//         free(node->key);
//         free(node->value);
//         free(node);
//     }
// }

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

// void	execute(t_cmd *cmds, t_env **env)
// {
// 	int	stdin_copy;
// 	int	stdout_copy;
// 	int	prev_pipe_read;

// 	t_resources res = {0}; // Initialize resources structure
// 	// Populate resources for proper cleanup in case of exit
// 	res.env = *env;
// 	res.cmds = cmds;
// 	// Note: tokens and input would need to be provided from the caller
// 	// For now, set them to NULL
// 	res.tokens = NULL;
// 	res.input = NULL;
// 	if (!init_execution(cmds, &stdin_copy, &stdout_copy))
// 	{
// 		cleanup(&res);
// 		//free_cmds(cmds);
// 		//free_env(*env);
// 		return ;
// 	}
// 	// Pass the resources struct to handle_single_builtin
// 	if (handle_single_builtin(cmds, &res))
// 		return ;
// 	prepare_pipeline_execution(cmds, &prev_pipe_read);
// 	execute_pipeline(cmds, env, &prev_pipe_read);
// 	cleanup_execution(stdin_copy, stdout_copy);
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

// void handle_child_process(t_cmd *cmd, t_env **env, int prev_pipe_read,
//                           int *pipe_fd)
// {
//     t_resources res; // Used primarily if bi_exit is called
//     int         builtin_status;

//     // Initialize resources struct for potential cleanup by bi_exit
//     // If the child exits directly here, we use explicit free calls below.
//     res.env = *env;
//     res.cmds = cmd;    // Child's view of the command(s) it should handle/clean
//     res.tokens = NULL; // Tokens usually processed before fork, not relevant here
//     res.input = NULL;  // Input string usually processed before fork

//     // Set default signal handling for child processes
//     signal(SIGINT, SIG_DFL); // Terminate on SIGINT
//     signal(SIGQUIT, SIG_DFL); // Terminate and dump core on SIGQUIT

//     // --- Setup Input/Output Piping ---
//     // Redirect stdin from the previous command's pipe output if applicable
//     if (prev_pipe_read != -1)
//     {
//         if (dup2(prev_pipe_read, STDIN_FILENO) == -1)
//         {
//             perror("minishell: dup2 stdin");
//             free_env(*env); // Cleanup before exit
//             free_cmds(cmd); // Cleanup before exit
//             exit(EXIT_FAILURE);
//         }
//         close(prev_pipe_read); // Close the original descriptor
//     }

//     // Redirect stdout to the current command's pipe input if there's a next command
//     if (cmd->next && pipe_fd[1] != -1) // Ensure pipe write end is valid
//     {
//         if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
//         {
//             perror("minishell: dup2 stdout");
//             if (pipe_fd[0] != -1) close(pipe_fd[0]); // Close read end if open
//             close(pipe_fd[1]); // Close write end
//             free_env(*env);    // Cleanup before exit
//             free_cmds(cmd);    // Cleanup before exit
//             exit(EXIT_FAILURE);
//         }
//     }

//     // Close pipe descriptors that are no longer needed in the child
//     if (pipe_fd[0] != -1) // Close the read end of the *current* pipe
//         close(pipe_fd[0]);
//     if (pipe_fd[1] != -1) // Close the write end of the *current* pipe
//         close(pipe_fd[1]);


//     // --- Setup File Redirections ---
//     if (setup_redirections(cmd->redirections) < 0)
//     {
//         // setup_redirections should have printed an error message
//         // g_exit_status is likely set to 1 by setup_redirections on error
//         free_env(*env); // Cleanup child's env copy
//         free_cmds(cmd); // Cleanup child's cmd list copy
//         exit(1); // Exit with status 1 for redirection errors
//     }

//     // --- Execute Command ---
//     // Check if the command is empty after redirections (e.g., "> file")
//     if (!cmd->args || !cmd->args[0])
//     {
//         free_env(*env); // Nothing to execute, just cleanup
//         free_cmds(cmd);
//         exit(EXIT_SUCCESS); // No command is not an error in this context
//     }

//     // Check if it's a builtin command
//     if (is_builtin(cmd->args[0]))
//     {
//         // Run the builtin. run_builtin might call bi_exit which cleans up via 'res' and exits.
//         builtin_status = run_builtin(cmd, &res);

//         // If run_builtin did NOT exit (i.e., it wasn't bi_exit), we need to clean up.
//         // We assume only bi_exit exits internally from run_builtin.
//         free_env(*env); // Free child's env copy
//         free_cmds(cmd); // Free child's cmd list copy

//         exit(builtin_status); // Exit with the status returned by the builtin
//     }
//     else
//     {
//         // Execute external command. execute_external MUST handle its own cleanup and exit.
//         execute_external(cmd, *env);

//         // If execute_external somehow returns (it shouldn't), treat as error.
//         ft_putstr_fd("minishell: execute_external returned unexpectedly\n", STDERR_FILENO);
//         free_env(*env);
//         free_cmds(cmd);
//         exit(EXIT_FAILURE); // Should not happen
//     }
// }

// void execute_external(t_cmd *cmd, t_env *env)
// {
//     char *path = NULL;
//     char **env_arr = NULL;
//     int exec_errno = 0; // Store errno from execve if it fails

//     // Find the executable path
//     // Pass the *head* of the env list
//     path = get_exec_path(cmd->args[0], env);

//     // Convert minishell environment list to char** for execve
//     // Pass the *head* of the env list
//     env_arr = env_to_arr(env);
//     if (!env_arr && errno == ENOMEM) // Check if env_to_arr failed due to malloc
//     {
//         perror("minishell: failed to allocate environment array");
//         // Need to clean up before exiting
//         if (path) free(path);
//         free_env(env); // Free the t_env list
//         free_cmds(cmd); // Free the t_cmd structure
//         exit(EXIT_FAILURE); // Indicate general failure
//     }


//     // --- Case 1: Path not found by get_exec_path ---
//     if (!path)
//     {
//         ft_putstr_fd("minishell: ", STDERR_FILENO);
//         ft_putstr_fd(cmd->args[0], STDERR_FILENO);
//         // Check if the original command contained '/' to mimic bash message difference
//         if (ft_strchr(cmd->args[0], '/'))
//         {
//             ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
//         }
//         else
//         {
//             ft_putstr_fd(": command not found\n", STDERR_FILENO);
//         }

//         // Cleanup before exiting
//         ft_free_split(env_arr); // Free the environment array
//         free_env(env);          // Free the t_env list
//         free_cmds(cmd);         // Free the t_cmd structure
//         exit(127); // Standard exit code for command not found
//     }

//     // --- Case 2: Path found, attempt execution ---
//     execve(path, cmd->args, env_arr);

//     // ---- If execve returns, it failed. ----
//     exec_errno = errno; // Save errno immediately

//     // --- Report the specific execve error ---
//     ft_putstr_fd("minishell: ", STDERR_FILENO);
//     ft_putstr_fd(cmd->args[0], STDERR_FILENO); // Report error using the original command name
//     ft_putstr_fd(": ", STDERR_FILENO);

//     int exit_code = 126; // Default exit code for execution errors

//     if (exec_errno == ENOENT)
//     {
//         // This might happen if the path was valid when get_exec_path checked,
//         // but was removed before execve, or if path pointed to an interpreter
//         // that doesn't exist.
//         ft_putstr_fd("No such file or directory\n", STDERR_FILENO);
//         exit_code = 127; // Use 127 if execve fails with ENOENT
//     }
//     else if (exec_errno == EACCES)
//     {
//         struct stat statbuf;
//         // Check if it's a directory *after* EACCES
//         if (stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
//         {
//             ft_putstr_fd("Is a directory\n", STDERR_FILENO);
//         }
//         else
//         {
//             ft_putstr_fd("Permission denied\n", STDERR_FILENO);
//         }
//         exit_code = 126;
//     }
//     else if (exec_errno == ENOEXEC)
//     {
//         ft_putstr_fd("Exec format error\n", STDERR_FILENO);
//         exit_code = 126;
//     }
//     // Add other specific errno checks if needed (e.g., ENOMEM)
//     else
//     {
//         // Generic fallback using strerror
//         ft_putstr_fd(strerror(exec_errno), STDERR_FILENO);
//         ft_putstr_fd("\n", STDERR_FILENO);
//         exit_code = 126; // Or sometimes 1 for general errors
//     }

//     // --- Cleanup before exiting due to execve failure ---
//     ft_free_split(env_arr); // Free the environment array
//     free(path);             // Free the path string
//     free_env(env);          // Free the t_env list
//     free_cmds(cmd);         // Free the t_cmd structure

//     exit(exit_code); // Exit with the determined code
// }#include "inc/minishell.h"


// void cleanup(t_resources *res)
// {
//     if (!res) return; // Safety check

//     if (res->input)
//         free(res->input);
//     if (res->tokens)
//         free_tokens(res->tokens);
//     if (res->cmds)
//         free_cmds(res->cmds);
//     if (res->env)
//         free_env(res->env); // Frees the environment - ONLY call on exit

//     rl_clear_history();
//     // Nullify pointers in res after freeing to prevent accidental reuse if needed
//     // res->input = NULL; res->tokens = NULL; res->cmds = NULL; res->env = NULL;
// }

// void	process_command(t_cmd *cmd, t_env **env, int prev_pipe_read,
// 		int *pipe_fd)
// {
// 	pid_t pid;

// 	pid = setup_pipe_and_fork(cmd, pipe_fd);
// 	if (pid == 0)
// 		handle_child_process(cmd, env, prev_pipe_read, pipe_fd);
// }

// int init_execution(t_cmd *cmds, int *stdin_copy, int *stdout_copy)
// {
//     *stdin_copy = dup(STDIN_FILENO);
//     *stdout_copy = dup(STDOUT_FILENO);
//     if (!cmds || !cmds->args || !cmds->args[0])
//     {
//         ft_putstr_fd("minishell: null command\n", STDERR_FILENO);
//         if (*stdin_copy != -1)
//             close(*stdin_copy);
//         if (*stdout_copy != -1)
//             close(*stdout_copy);
//         return (0);
//     }
//     return (1);
// }

// void	cleanup_execution(int stdin_copy, int stdout_copy)
// {
// 	setup_signals_for_prompt();
// 	restore_std_fds(stdin_copy, stdout_copy);
// }


// void	wait_for_children(pid_t last_cmd_pid)
// {
// 	int status;
// 	pid_t pid;
// 	int last_cmd_status = 0; // Store the status of the specific last command
// 	int found_last = 0;      // Flag to track if we found the last command's status
// 	int printed_pipe_msg = 0; // Flag to potentially avoid printing "Broken pipe" multiple times

// 	// Initial status if last_cmd_pid is invalid (e.g., fork failed before loop)
// 	// If fork failed for the last command, execution might have already set
// 	// g_exit_status to an error code (like 1). We should preserve that if
// 	// no children are successfully waited for.
// 	// If last_cmd_pid <= 0, we can't rely on found_last/last_cmd_status later.

// 	while ((pid = waitpid(-1, &status, 0)) > 0) // Wait for ANY child
// 	{
// 		// --- Check status of the specific last command ---
// 		if (pid == last_cmd_pid)
// 		{
// 			found_last = 1; // Mark that we found it
// 			if (WIFEXITED(status))
// 			{
// 				last_cmd_status = WEXITSTATUS(status); // Get its normal exit status
// 			}
// 			else if (WIFSIGNALED(status)) // If the last command itself was signaled
// 			{
// 				int signal_num = WTERMSIG(status);
// 				last_cmd_status = 128 + signal_num; // Calculate its signal exit status
//                 // Don't print signal messages here yet, handle below for consistency
// 			}
//             // else: Handle other termination types if necessary (stopped, continued)
// 		}

// 		// --- Check for signaled termination for *any* child waited for in this loop ---
// 		// This is where we print messages like "Broken pipe", "Quit", etc.
// 		if (WIFSIGNALED(status))
// 		{
// 			int signal_num = WTERMSIG(status);

//             // Print signal messages like bash - DO THIS FOR *ANY* CHILD THAT SIGNALED
//             // We print these messages even if the process wasn't the 'last_cmd_pid'
// 			if (signal_num == SIGQUIT)
// 			{
// 				// Bash prints "Quit (core dumped)" - adjust as needed
// 				// Check WCOREDUMP(status) if you want to add "(core dumped)"
// 				ft_putstr_fd("Quit", STDERR_FILENO);
//                 if (WCOREDUMP(status))
//                     ft_putstr_fd(" (core dumped)", STDERR_FILENO);
//                 ft_putstr_fd("\n", STDERR_FILENO);
// 			}
// 			else if (signal_num == SIGSEGV)
// 			{
// 				ft_putstr_fd("Segmentation fault", STDERR_FILENO);
//                 if (WCOREDUMP(status))
//                     ft_putstr_fd(" (core dumped)", STDERR_FILENO);
//                 ft_putstr_fd("\n", STDERR_FILENO);
// 			}
// 			else if (signal_num == SIGPIPE) // SIGPIPE is usually signal 13
// 			{
//                 // Optional: Only print "Broken pipe" once per pipeline wait
//                 if (!printed_pipe_msg)
//                 {
// 				    ft_putstr_fd("Broken pipe\n", STDERR_FILENO);
//                     printed_pipe_msg = 1; // Set flag after printing
//                 }
//                 // Note: If multiple processes get SIGPIPE concurrently, Bash might
//                 // technically print it multiple times, but often only one matters.
//                 // Printing once is usually sufficient for mimicking the common case.
// 			}
//             // else if (signal_num == SIGINT) {
//             //     // We usually don't print anything extra for SIGINT here,
//             //     // as the newline is typically handled by the signal handler.
//             //     // The exit status (130) will be set correctly if it was the last command.
//             // }
// 			// Add checks for other signals if needed
// 		}
// 		// We continue waiting even after finding the last command to ensure
// 		// all children in the pipeline are reaped and no zombies are left.
// 	} // End of while loop

// 	// --- Set the final global exit status ---
// 	// After waiting for all children, set the global exit status based
// 	// on the termination status of the *last* command in the pipeline.
// 	if (last_cmd_pid > 0 && found_last)
// 	{
// 		// We successfully found and waited for the last command.
// 		// last_cmd_status contains either its exit code or signal status (128+sig).
// 		g_exit_status = last_cmd_status;
// 	}
// 	else if (last_cmd_pid <= 0)
// 	{
// 		// Handle cases where there was no valid last command pid
// 		// (e.g., single command, fork failure before loop).
// 		// In this scenario, g_exit_status should likely retain the value
// 		// it had before wait_for_children was called (e.g., error status
// 		// from setup, or status from a single builtin).
// 		// If waitpid returned ECHILD immediately (no children), we also fall here.
// 		// So, we explicitly *don't* modify g_exit_status here.
// 	}
//     else // (last_cmd_pid > 0 && !found_last)
//     {
//         // This case is strange: We had a valid last PID, but waitpid finished
//         // without finding it (perhaps waitpid failed with EINTR or another error).
//         // It might be safer to not modify g_exit_status or set it to a generic error.
//         // Let's leave g_exit_status as is for now.
//     }


// 	// --- Error handling for waitpid itself ---
// 	if (pid == -1 && errno != ECHILD)
// 	{
// 		// waitpid failed for a reason other than "no child processes"
// 		perror("minishell: waitpid");

// 	}
// }#include "inc/minishell.h"

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
// 	int len;

// 	len = 0;
// 	while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
// 	{
// 		len++;
// 	}
// 	return (ft_substr(str, 0, len));
// }

// void	expand_variables(t_token *tokens, t_env *env)
// {
// 	char *expanded;

// 	while (tokens)
// 	{
// 		if (tokens->type == T_SINGLE_QUOTED)
// 		{
// 			// NIE ROBIMY NIC
// 		}
// 		else if (tokens->type == T_DOUBLE_QUOTED || tokens->type == T_WORD)
// 		{
// 			expanded = expand_str(tokens->value, env);
// 			if (expanded)
// 			{
// 				free(tokens->value);
// 				tokens->value = expanded;
// 			}
// 		}
// 		tokens = tokens->next;
// 	}
// }

// //nowe proba naleienia leakow
// int handle_braced_var(char **result, char *str, int i, t_env *env)
// {
//     int start = i + 2; // Start after "${"
//     int end = start;
//     char *var_name = NULL;
//     char *var_value = NULL;
//     char *new_result = NULL;

//     // Find the closing brace '}'
//     while (str[end] && str[end] != '}')
//         end++;

//     if (str[end] != '}') // Closing brace not found
//     {
//          // Syntax error? Or just let it be? Bash might treat "${VAR" as literal.
//          // For now, let's assume it should be treated literally if no '}'
//          // We don't modify *result and return original index to continue parsing.
//          // Or, if this should be an error, return -1. Let's return original index for now.
//          // ft_putstr_fd("minishell: syntax error: unclosed brace\n", 2); // Optional error
//          return (i); // Treat as literal, don't advance significantly
//     }

//     // Extract variable name
//     var_name = ft_substr(str, start, end - start);
//     if (!var_name)
//     {
//         perror("minishell: ft_substr failed in handle_braced_var");
//         // *result is untouched, return error
//         return (-1);
//     }

//     var_value = get_env_value(var_name, env); // Does not allocate

//     // ft_strjoin_free will free the memory pointed to by *result
//     new_result = ft_strjoin_free(*result, var_value ? var_value : "");

//     if (!new_result)
//     {
//         perror("minishell: malloc error ft_strjoin_free (braced_var)");
//         // *result was freed by ft_strjoin_free, var_name still exists
//         free(var_name); // Free the allocated var_name
//         *result = NULL; // Set caller's pointer to NULL to indicate failure state
//         return (-1);    // Signal error
//     }

//     // Success
//     *result = new_result; // Update caller's pointer
//     free(var_name);       // Free the allocated var_name
//     return (end + 1);     // Return index *after* the closing '}'
// }

// int handle_standard_var(char **result, char *str, int i, t_env *env)
// {
//     char *var_name = NULL;
//     char *var_value = NULL;
//     char *new_result = NULL;
//     int name_len = 0;

//     var_name = extract_var_name(str + i + 1); // Allocates var_name
//     if (!var_name)
//     {
//         perror("minishell: extract_var_name failed");
//         return (-1); // Error
//     }

//     name_len = ft_strlen(var_name);

//     if (name_len == 0) // Special case: just '$' followed by non-alphanum/_
//     {
//         // ft_strjoin_free will free the memory pointed to by *result
//         new_result = ft_strjoin_free(*result, "$");
//         if (!new_result)
//         {
//             perror("minishell: malloc error ft_strjoin_free (standard_var '$')");
//             free(var_name); // Free the empty var_name
//             *result = NULL;
//             return (-1);
//         }
//         *result = new_result; // Update caller's pointer
//         free(var_name);       // Free the empty var_name
//         return (i + 1);       // Advance past '$' only
//     }

//     // Regular variable name found
//     var_value = get_env_value(var_name, env); // Does not allocate

//     // ft_strjoin_free will free the memory pointed to by *result
//     new_result = ft_strjoin_free(*result, var_value ? var_value : "");
//     if (!new_result)
//     {
//         perror("minishell: malloc error ft_strjoin_free (standard_var)");
//         // *result was freed by ft_strjoin_free, var_name still exists
//         free(var_name); // Free the allocated var_name
//         *result = NULL;
//         return (-1);
//     }

//     // Success
//     *result = new_result;  // Update caller's pointer
//     free(var_name);        // Free the allocated var_name
//     return (i + name_len + 1); // Advance past '$' and the variable name
// }


// char *expand_str(char *str, t_env *env)
// {
//     char *result = NULL;
//     char *temp_result = NULL;
//     int i = 0;
//     // int original_i = 0; // No longer needed with -1 error check
//     bool error_occurred = false; // Flag to track errors

//     result = ft_strdup(""); // Allocate initial empty string
//     if (!result) {
//         perror("minishell: malloc error in expand_str initial allocation");
//         return (NULL); // Cannot even start
//     }

//     while (str[i] && !error_occurred) {
//         // original_i = i; // Store index before potential modification by handlers

//         if (is_special_var(str, i)) {
//             i = handle_special_var(&result, i);
//             if (i < 0) error_occurred = true; // Handler signals error (-1)
//         }
//         else if (is_braced_var(str, i)) {
//              // Handle return value: If it returns < 0, it's an error.
//              // If it returns >= original_i, it's success.
//             int next_i = handle_braced_var(&result, str, i, env);
//             if (next_i < 0) { // Check for explicit error signal
//                 error_occurred = true;
//             } else {
//                 i = next_i; // Update index only on success
//             }
//         }
//         else if (is_standard_var(str, i)) {
//              int next_i = handle_standard_var(&result, str, i, env);
//              if (next_i < 0) { // Check for explicit error signal
//                  error_occurred = true;
//              } else {
//                  i = next_i; // Update index only on success
//              }
//         }
//         else { // Append regular character
//             temp_result = ft_strjoin_char(result, str[i]); // Pass current result
//             if (!temp_result) {
//                 perror("minishell: malloc error in ft_strjoin_char within expand_str");
//                 error_occurred = true; // Set error flag
//                 // Don't update 'result' yet, it needs to be freed below
//             } else {
//                 // If successful, ft_strjoin_char already freed the old 'result'.
//                 result = temp_result; // Update result to point to the new string
//                 i++;
//             }
//         }

//         // If an error occurred in this iteration, break the loop
//         if (error_occurred) {
//             break;
//         }

//     } // end while

//     // --- Cleanup Check ---
//     if (error_occurred) {
//         if (result) { // Check if result holds any allocated memory
//             free(result); // Free the partially constructed string
//         }
//         return (NULL); // Signal error to the caller by returning NULL
//     }

//     // --- Success ---
//     return (result); // Return the final allocated string
// }


// ////from expandet utils.c

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

// void	append_char(char **result, char c)
// {
// 	char tmp[2];
// 	char *new_result;

// 	tmp[0] = c;
// 	tmp[1] = '\0';
// 	new_result = ft_strjoin(*result, tmp); // Don't free here
// 	if (!new_result)
// 	{
// 		free(*result);
// 		*result = NULL;
// 		return ;
// 	}
// 	*result = new_result;
// }


// //nowe
// int handle_special_var(char **result, int i)
// {
//     char *exit_status_str = NULL;
//     char *new_result = NULL;

//     exit_status_str = ft_itoa(g_exit_status);
//     if (!exit_status_str)
//     {
//         perror("minishell: ft_itoa failed in handle_special_var");
//         // *result still holds the old value, signal error without modifying it
//         return (-1); // Error indicator
//     }

//     // ft_strjoin_free will free the memory pointed to by *result
//     new_result = ft_strjoin_free(*result, exit_status_str);
//     free(exit_status_str); // Free itoa result AFTER join attempt

//     if (!new_result)
//     {
//         perror("minishell: malloc error ft_strjoin_free (special_var)");
//         // *result was freed by ft_strjoin_free, but new allocation failed.
//         *result = NULL; // Set caller's pointer to NULL to indicate failure state
//         return (-1);    // Signal error
//     }

//     // Success
//     *result = new_result; // Update caller's pointer
//     return (i + 2);       // Advance past '$?'
// }#include "inc/minishell.h"

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
// 	t_token *new;

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
// 	t_token *prev;
// 	t_token *current;
// 	t_token *next;

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

// void	handle_input_char(char *input, int *i, char **current_word,
// 		t_token **tokens)
// {
// 	static int error = 0;

// 	if (ft_isspace(input[*i]))
// 		skip_space(i, current_word, tokens);
// 	else if (is_quote(input[*i]))
// 	{
// 		// Zakończ słowo przed obsługą cytatu
// 		if (*current_word)
// 			finalize_current_word(tokens, current_word, 0);
// 				// 0 dla standardowego słowa (brak cytatu)
// 		handle_quoted(input, i, tokens, &error);           
// 			// Obsługujemy cudzysłowy
// 	}
// 	else if (is_pipe(input[*i]))
// 		handle_pipe(i, current_word, tokens);
// 	else if (is_redirect(input[*i]))
// 		handle_redirect_token(input, i, current_word, tokens);
// 	else
// 		append_char_to_word(current_word, input[(*i)++]);
// }
// #include "inc/minishell.h"


// int	handle_quote(char *input, int *i, t_token **tokens, int *error)
// {
// 	t_quote current_quote;
// 	char *quoted;
// 	t_token_type quote_type;

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
// 	char quote_char = (quote == SINGLE_QUOTE) ? '\'' : '"';
// 	int start = *i;
// 	while (input[*i] && input[*i] != quote_char)
// 		(*i)++;

// 	if (input[*i] != quote_char)
// 	{
// 		*error = 1; // Ustaw błąd jeśli nie znaleziono zamykającego cytatu
// 		return (NULL);
// 	}

// 	char *content = ft_substr(input, start, *i - start);
// 	(*i)++; // Zwiększ indeks, aby przejść po zamykającym cudzysłowie
// 	return (content);
// }


// void free_tokens(t_token *tokens)
// {
//     t_token *tmp;

//     while (tokens)
//     {
//         tmp = tokens->next; // Store next *before* freeing current
//         if (tokens->value)
//             free(tokens->value); // Free the string value
//         free(tokens);          // Free the token node
//         tokens = tmp;         // Move to the next node
//     }
// }

// // //debugging
// // void free_tokens(t_token *tokens)
// // {
// //     t_token *current = tokens;
// //     t_token *next_node;
// //     int count = 0; // Debug counter

// //     // --- ADDED: Initial check and head pointer ---
// //     printf("DEBUG: free_tokens STARTING with head=%p\n", (void*)current);
// //     if (!current) {
// //         printf("DEBUG: free_tokens received NULL list, doing nothing.\n");
// //         return;
// //     }
// //     // --- END ADDED ---

// //     while (current != NULL)
// //     {
// //         count++;
// //         // --- ADDED: Print current node info ---
// //         printf("DEBUG: free_tokens loop %d: current node=%p, next=%p, type=%d\n",
// //                count, (void*)current, (void*)current->next, current->type);
// //         // --- END ADDED ---

// //         next_node = current->next; // Store the next pointer *before* freeing anything

// //         if (current->value)
// //         {
// //             // --- ADDED: Print value being freed ---
// //             printf("DEBUG: free_tokens loop %d: freeing value=%p ('%s')\n",
// //                    count, (void*)current->value, current->value);
// //             // --- END ADDED ---
// //             free(current->value);
// //             current->value = NULL; // Optional: Prevent dangling pointer issues
// //         } else {
// //             // --- ADDED: Indicate NULL value ---
// //              printf("DEBUG: free_tokens loop %d: value pointer was NULL\n", count);
// //             // --- END ADDED ---
// //         }

// //         // --- ADDED: Print node being freed ---
// //         printf("DEBUG: free_tokens loop %d: freeing node %p itself\n", count, (void*)current);
// //         // --- END ADDED ---
// //         free(current);          // Free the token node itself
// //         current = next_node;    // Move to the next node using the stored pointer
// //     }

// //     // --- ADDED: Final count ---
// //     printf("DEBUG: free_tokens FINISHED after attempting to free %d nodes\n", count);
// //     // --- END ADDED ---
// // }
// #include "inc/minishell.h"


// void	skip_space(int *i, char **word, t_token **tokens)
// {
// 	if (*word)
// 		finalize_current_word(tokens, word, 0);
// 	(*i)++;
// }

// void	handle_pipe(int *i, char **word, t_token **tokens)
// {
// 	if (*word)
// 		finalize_current_word(tokens, word, 0);
// 	add_token(tokens, "|", T_PIPE);
// 	(*i)++;
// }

// int	handle_quoted(char *input, int *i, t_token **tokens, int *error)
// {
// 	t_quote current_quote;
// 	char *quoted;

// 	current_quote = get_quote_type(input[*i]);
// 	(*i)++; // Pomijamy otwierający cudzysłów

// 	quoted = read_quoted_content(input, i, current_quote, error);
// 	if (*error)
// 	{
// 		ft_putstr_fd("Minishell: syntax error: unclosed quote\n",
// 			STDERR_FILENO);
// 		return (1);
// 	}

// 	// Dodajemy token z odpowiednim typem w zależności od typu cytatu
// 	t_token_type quote_type = get_token_type_from_quote(current_quote);
// 	add_token(tokens, quoted, quote_type);

// 	free(quoted); // Zwolnij pamięć
// 	return (0);
// }

// t_token	*handle_unexpected_quote(char *word, t_token *tokens)
// {
// 	free(word);
// 	free_tokens(tokens);
// 	return (NULL);
// }
// #include "inc/minishell.h"


// void	handle_redirect_token(char *input, int *i, char **word,
// 		t_token **tokens)
// {
// 	if (*word)
// 		finalize_current_word(tokens, word, 0);
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
// 			*tokens = NULL;
// 			// To indicate an error and avoid further processing
// 		}
// 	}
// }
// #include "inc/minishell.h"

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
// #include "inc/minishell.h"

// int	handle_quoted_token(char *input, int *i, t_token **tokens, int *error)
// {
// 	t_quote			current_quote;
// 	char			*quoted_content;
// 	t_token_type	quote_type;

// 	current_quote = get_quote_type(input[*i]);
// 	(*i)++; // Move past the opening quote
// 	// read_quoted_content reads *only* the inner content
// 	quoted_content = read_quoted_content(input, i, current_quote, error);
// 	if (*error || quoted_content == NULL) // Check error flag OR malloc failure
// 	{
// 		// read_quoted_content might set error but return NULL without msg
// 		if (*error && quoted_content == NULL)
// 		{
// 			ft_putstr_fd("Minishell: syntax error: unclosed quote\n",
// 				STDERR_FILENO);
// 		}
// 		else if (quoted_content == NULL)
// 		{
// 			// Malloc error likely
// 			perror("minishell: malloc error in read_quoted_content");
// 			*error = 1; // Ensure error is set
// 		}
// 		// Do not free quoted_content if it's NULL
// 		// If *error is set but quoted_content is not NULL (shouldn't happen?),
// 		//	free it.
// 		if (quoted_content)
// 			free(quoted_content);
// 		return (1); // Indicate failure
// 	}
// 	// Determine token type based on the quote character used
// 	quote_type = get_token_type_from_quote(current_quote);
// 	// Add the token with the correct type and the *inner* content
// 	add_token(tokens, quoted_content, quote_type);
// 	free(quoted_content); // Free the temporary content string
// 	// i is already advanced past the closing quote by read_quoted_content
// 	return (0); // Indicate success
// }

// // //moje
// // void	free_env(t_env *env)
// // {
// // 	t_env	*tmp;

// // 	while (env)
// // 	{
// // 		tmp = env->next; // Save next pointer before freeing
// // 		free(env->key);
// // 		free(env->value);
// // 		free(env);
// // 		env = tmp;
// // 	}
// // }

// void free_env(t_env *env)
// {
//     t_env *tmp;
//     while (env)
//     {
//         tmp = env->next;   // 1. Save next pointer
//         free(env->key);    // 2. Free key string
//         free(env->value);  // 3. Free value string
//         free(env);         // 4. Free the node itself
//         env = tmp;         // 5. Move to the next node
//     }
// }

// void	init_shell(char **envp, t_env **env)
// {
// 	setup_signals_for_prompt();
// 	*env = NULL;
// 	init_env(envp, env);
// }

// void	handle_eof(int exit_status)
// {
// 	// Don't free env here, it's already freed in cleanup
// 	// free_env(env);  <- Remove this line
// 	// Clear readline history
// 	rl_clear_history();
// 	// Print a newline for better UX when exiting with Ctrl+D
// 	ft_putstr_fd("exit\n", STDOUT_FILENO);
// 	exit(exit_status);
// }

// void process_input(char *input, t_env **env)
// {
//     t_resources res = {0}; // Mainly for passing to bi_exit if called
//     t_token     *tokens = NULL;
//     t_cmd       *cmds = NULL;

//     // Keep track of resources mainly for potential cleanup by bi_exit
//     res.input = input;
//     res.env = *env;
//     // Note: We'll free tokens and cmds manually unless bi_exit happens

//     tokens = tokenize_input(input);
//     if (!tokens)
//     {
//         free(input); // Free only input on tokenize failure
//         return;
//     }
//     res.tokens = tokens; // Track for bi_exit

//     expand_variables(tokens, *env);

//     cmds = parse_tokens(tokens);
//     if (!cmds)
//     {
//         free_tokens(tokens); // Free tokens
//         free(input);      // Free input
//         return;
//     }
//     res.cmds = cmds; // Track for bi_exit

//     // Shift arguments *after* parsing and *before* heredoc/execution
//     // Important if expansion results in empty args that affect command name
//     shift_empty_args(cmds); // Assuming this handles ["", "cmd"] -> ["cmd"]

//     // Handle Heredocs first
//     if (handle_all_heredocs(cmds) != 0)
//     {
//         close_heredoc_fds(cmds); // Close any FDs opened before failure
//         free_cmds(cmds);         // Free cmds
//         free_tokens(tokens);     // Free tokens
//         free(input);          // Free input
//         return;
//     }

//     // Check for empty commands *after* heredocs are processed
//     // (e.g., just "> file" or "$EMPTY_VAR")
//     if (!cmds->args || !cmds->args[0] || cmds->args[0][0] == '\0') // <<< NEW CHECK
//     {
//          // Action remains the same: cleanup and return
//          close_heredoc_fds(cmds);
//          free_cmds(cmds);
//          free_tokens(tokens);
//          free(input);
//          // g_exit_status will be 0 unless handle_all_heredocs or earlier
//          // parsing/redirection setup failed and set it to non-zero.
//          return; // Nothing to execute
//     }


//     setup_signals_for_command();

//     if (is_builtin(cmds->args[0]) && !cmds->next)
//     {
//         t_fds fds = {dup(STDIN_FILENO), dup(STDOUT_FILENO)};
//         // Pass 'res' so bi_exit can clean up *everything* if called
//         handle_parent_builtin(cmds, fds, &res);
//         // If handle_parent_builtin called bi_exit, we won't reach the code below.
//         // If it didn't exit, fds should have been restored within it.
//     }
//     else
//     {
//         execute(cmds, env); // execute handles waiting and sets g_exit_status
//     }

//     setup_signals_for_prompt();
//     close_heredoc_fds(cmds); // Close heredoc FDs *after* execution

//     // --- Manually Clean up command-specific resources ---
//     // Do NOT call cleanup(&res) here as it frees the environment!
//     free_cmds(cmds);
//     free_tokens(tokens);
//     free(input);
//     // 'res' is now potentially dangling if bi_exit wasn't called, but it's local
//     // and goes out of scope, so it's okay.
// }


// ///env | sort | grep -v SHLVL | grep -v ^_




// void	shift_empty_args(t_cmd *cmds)
// {
// 	t_cmd	*current_cmd;
// 	int		i;
// 	char	*empty_arg_to_free;

// 	current_cmd = cmds;
// 	while (current_cmd)
// 	{
// 		// Check if args exists, has at least two elements, and args[0] is empty
// 		if (current_cmd->args && current_cmd->args[0] && current_cmd->args[1]
// 			&& current_cmd->args[0][0] == '\0')
// 		{
// 			i = 0;
// 			empty_arg_to_free = current_cmd->args[0];
// 			// Save pointer to free
// 			// Shift pointers left
// 			while (current_cmd->args[i + 1])
// 			{
// 				current_cmd->args[i] = current_cmd->args[i + 1];
// 				i++;
// 			}
// 			current_cmd->args[i] = NULL; // Null-terminate the shifted array
// 			free(empty_arg_to_free);     // Free the original empty string arg
// 		}
// 		current_cmd = current_cmd->next;
// 	}
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

// static int	ensure_current_word(char **current_word)
// {
// 	if (*current_word == NULL)
// 	{
// 		*current_word = ft_strdup("");
// 		if (!*current_word)
// 		{
// 			perror("minishell: malloc error");
// 			return (0);
// 		}
// 	}
// 	return (1);
// }

// void	append_char_to_word(char **word, char c)
// {
// 	char	*old_word;

// 	if (!ensure_current_word(word))
// 	{
// 		return ;
// 	}
// 	old_word = *word;
// 	*word = ft_strjoin_char(old_word, c);
// 	if (!*word)
// 	{
// 		perror("minishell: malloc error");
// 	}
// }

// int	handle_quoted_append(char *input, int *i, char **current_word, int *error,
// 		t_parse_state *state)
// {
// 	t_quote	current_quote;
// 	char	quote_char;
// 	int		start;
// 	int		len;
// 	char	*content;
// 	char	*old_word;

// 	current_quote = get_quote_type(input[*i]);
// 	quote_char = (current_quote == SINGLE_QUOTE) ? '\'' : '"';
// 	(*i)++;
// 	start = *i;
// 	while (input[*i] && input[*i] != quote_char)
// 	{
// 		(*i)++;
// 	}
// 	if (input[*i] != quote_char)
// 	{
// 		ft_putstr_fd("Minishell: syntax error: unclosed quote\n",
// 			STDERR_FILENO);
// 		*error = 1;
// 		return (1);
// 	}
// 	// Ustawiamy flagę, jeśli napotkaliśmy pojedynczy cudzysłów
// 	if (current_quote == SINGLE_QUOTE)
// 	{
// 		state->single_quote_used = 1;
// 	}
// 	if (!ensure_current_word(current_word))
// 	{
// 		*error = 1;
// 		return (1);
// 	}
// 	len = *i - start;
// 	if (len > 0)
// 	{
// 		content = ft_substr(input, start, len);
// 		if (!content)
// 		{
// 			perror("minishell: malloc error");
// 			*error = 1;
// 			return (1);
// 		}
// 		old_word = *current_word;
// 		*current_word = ft_strjoin(old_word, content);
// 		free(old_word);
// 		free(content);
// 		if (!*current_word)
// 		{
// 			perror("minishell: malloc error");
// 			*error = 1;
// 			return (1);
// 		}
// 	}
// 	(*i)++;
// 	return (0);
// }

// void	finalize_current_word(t_token **tokens, char **current_word,
// 		int single_quote_used)
// {
// 	t_token_type	type;

// 	if (*current_word != NULL)
// 	{
// 		type = single_quote_used ? T_SINGLE_QUOTED : T_WORD;
// 		add_token(tokens, *current_word, type);
// 		free(*current_word);
// 		*current_word = NULL;
// 	}
// }

// void	handle_redirection(char *input, int *i, t_token **tokens)
// {
// 	char			first_char;
// 	t_token_type	type;
// 	char			op_str[3];

// 	first_char = input[*i];
// 	op_str[0] = first_char;
// 	(*i)++;
// 	if (input[*i] == first_char)
// 	{
// 		op_str[1] = first_char;
// 		op_str[2] = '\0';
// 		(*i)++;
// 		if (first_char == '>')
// 			type = T_APPEND;
// 		else
// 			type = T_HEREDOC;
// 		add_token(tokens, op_str, type);
// 	}
// 	else
// 	{
// 		op_str[1] = '\0';
// 		if (first_char == '>')
// 			type = T_REDIR_OUT;
// 		else
// 			type = T_REDIR_IN;
// 		add_token(tokens, op_str, type);
// 	}
// }

// void	parse_input_loop(char *input, t_parse_state *state)
// {
// 	while (input[state->i] && !(state->error))
// 	{
// 		//|| input[state->i - 1] == ' '))
// 		if (input[state->i] == '#' && (state->i == 0
// 				|| ft_isspace(input[state->i - 1])))
// 			// Zatrzymaj przetwarzanie po napotkaniu znaku komentarza
// 			break ;
// 		if (ft_isspace(input[state->i]))
// 		{
// 			// Finalizuj jakiekolwiek słowo przed pominięciem spacji
// 			finalize_current_word(&state->tokens, &state->current_word,
// 				state->single_quote_used);
// 			state->i++; // Pomijamy spację
// 		}
// 		else if (is_pipe(input[state->i]))
// 		{
// 			// Finalizuj słowo, dodaj token dla pipe
// 			finalize_current_word(&state->tokens, &state->current_word,
// 				state->single_quote_used);
// 			add_token(&state->tokens, "|", T_PIPE);
// 			state->i++;
// 		}
// 		else if (is_redirect(input[state->i]))
// 		{
// 			// Finalizuj słowo przed obsługą operatora przekierowania
// 			finalize_current_word(&state->tokens, &state->current_word,
// 				state->single_quote_used);
// 			handle_redirection(input, &state->i, &state->tokens);
// 			// Dodaje token przekierowania
// 		}
// 		else if (is_quote(input[state->i]))
// 		{
// 			// Dodaj zawartość cytatu do aktualnie budowanego słowa
// 			if (handle_quoted_append(input, &state->i, &state->current_word,
// 					&state->error, state) != 0)
// 			{
// 				// Wystąpił błąd (niezamknięty cudzysłów lub błąd malloc)
// 				state->error = 1; // Ustawiamy flagę błędu
// 			}
// 		}
// 		else
// 		{
// 			// Dodaj zwykły znak do aktualnie budowanego słowa
// 			append_char_to_word(&state->current_word, input[state->i]);
// 			if (state->current_word == NULL && errno == ENOMEM)
// 			{ // Sprawdzamy, czy append_char się nie udał
// 				state->error = 1;
// 			}
// 			state->i++;
// 		}
// 	}
// 	// Po zakończeniu pętli, finalizuj jakiekolwiek pozostałe słowo
// 	if (!state->error)
// 	{ // Finalizujemy tylko, jeśli nie wystąpił błąd w trakcie pętli
// 		finalize_current_word(&state->tokens, &state->current_word,
// 			state->single_quote_used);
// 	}
// }

// t_token	*tokenize_input(char *input)
// {
// 	t_parse_state	state;

// 	state.tokens = NULL;
// 	state.current_word = NULL;
// 	state.i = 0;
// 	state.error = 0;
// 	state.single_quote_used = 0; // Inicjalizujemy na 0
// 	parse_input_loop(input, &state);
// 	// Finalne słowo obsługiwane *wewnątrz* i *po* parse_input_loop
// 	if (state.error)
// 	{
// 		// Czyszczenie potencjalnie częściowo zbudowanego słowa i tokenów
// 		if (state.current_word)
// 			free(state.current_word);
// 		free_tokens(state.tokens);
// 		return (NULL);
// 	}
// 	return (reverse_tokens(state.tokens));
// }
// #include "inc/minishell.h"

// /**
//  * Obsługuje tokeny przekierowań w parserze.
//  * - tokens: podwójny wskaźnik do aktualnego tokenu (np. `>` lub `<<`).
//  * - cmd: bieżąca komenda, do której dodawane jest przekierowanie.
//  */

// void handle_redirection_parser(t_token **tokens, t_cmd *cmd)
// {
//     t_redir *new_redir;

//     new_redir = malloc(sizeof(t_redir));
//     if (!new_redir)
//     {
//         perror("minishell: malloc");
//         return; // Exit on allocation failure
//     }
//     new_redir->type = (*tokens)->type;
//     new_redir->heredoc_fd = -1;

//     *tokens = (*tokens)->next;
//     if (!*tokens || ((*tokens)->type != T_WORD && (*tokens)->type != T_SINGLE_QUOTED &&
//                       (*tokens)->type != T_DOUBLE_QUOTED))
//     {
//         ft_putstr_fd("minishell: syntax error near unexpected token\n", STDERR_FILENO);
//         free(new_redir); // Free allocated memory
//         return;
//     }

//     new_redir->file = ft_strdup((*tokens)->value);
//     if (!new_redir->file)
//     {
//         perror("minishell: malloc");
//         free(new_redir); // Free allocated memory
//         return;
//     }

//     new_redir->next = NULL;
//     add_redir_to_cmd(cmd, new_redir);
//     *tokens = (*tokens)->next;
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

// void	prepare_pipeline_execution(t_cmd *cmds, int *prev_pipe_read)
// {
// 	*prev_pipe_read = -1;
// 	setup_signals_for_command();
// 	(void)cmds;
// }

// void	execute_pipeline(t_cmd *cmds, t_env **env, int *prev_pipe_read)
// {
// 	int		pipe_fd[2];
// 	pid_t	pid;

// 	pid_t last_pid = -1;       // Variable to store the PID of the last command
// 	t_cmd *current_cmd = cmds; // Use a temporary pointer to iterate
// 	while (current_cmd)
// 	{
// 		pipe_fd[0] = -1;
// 		pipe_fd[1] = -1;
// 		// Directly call setup_pipe_and_fork here to get the PID
// 		pid = setup_pipe_and_fork(current_cmd, pipe_fd);
// 		if (pid < 0)
// 		{
// 			// Fork failed, maybe handle error, break loop?
// 			// perror("minishell: fork failed in pipeline");
// 			// Consider setting an error status
// 			if (*prev_pipe_read != -1)
// 				close(*prev_pipe_read); // Cleanup pending read pipe
// 			break ;                      // Stop processing pipeline
// 		}
// 		else if (pid == 0)
// 		{
// 			// --- Child Process ---
// 			// process_command used to call handle_child_process, do it directly
// 			handle_child_process(current_cmd, env, *prev_pipe_read, pipe_fd);
// 			// handle_child_process exits, so code below won't run in child
// 		}
// 		else
// 		{
// 			// --- Parent Process ---
// 			// If this is the last command in the list, store its PID
// 			if (!current_cmd->next)
// 			{
// 				last_pid = pid;
// 			}
// 			// Close pipes in the parent
// 			update_pipe_status(current_cmd, prev_pipe_read, pipe_fd);
// 		}
// 		current_cmd = current_cmd->next; // Move to the next command
// 	}
// 	if (*prev_pipe_read != -1)
// 	{
// 		close(*prev_pipe_read);
// 		*prev_pipe_read = -1; // Optional: Reset to indicate closed
// 	}
// 	// Wait for all children, passing the PID of the last one
// 	wait_for_children(last_pid);
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
// #include "inc/minishell.h"

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


// /**
//  * Łączy trzy stringi w jeden (np. "dir", "/", "cmd" → "dir/cmd").
//  * Zwraca nowy string lub NULL przy błędzie alokacji.
//  */
// char	*ft_strjoin3(const char *s1, const char *s2, const char *s3)
// {
// 	char *result;
// 	char *tmp;

// 	tmp = ft_strjoin(s1, s2);
// 	if (!tmp)
// 		return (NULL);
// 	result = ft_strjoin(tmp, s3);
// 	free(tmp);
// 	return (result);
// }

// void	ft_free_split(char **split)
// {
// 	int i;

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

// char	*get_exec_path(char *cmd, t_env *env)
// {
// 	char *path_value;
// 	char **path_dirs;
// 	char *exec_path;
// 	struct stat statbuf; // Declare statbuf here for the explicit path check

// 	// Basic validation
// 	if (!cmd || cmd[0] == '\0') // Also check for empty command string
// 		return (NULL);
// 	if (!env)          // Need env to check PATH
// 		return (NULL); // Or decide how to handle no env

// 	// --- Check if cmd is an explicit path (contains '/') ---
// 	if (ft_strchr(cmd, '/'))
// 	{
// 		// Use stat() to check if the path exists. We don't check permissions here.
// 		// Let execve handle permissions, directory execution errors etc.
// 		if (stat(cmd, &statbuf) == 0)
// 		{
// 			// Path exists (it could be a file, directory, etc.)
// 			return (ft_strdup(cmd)); // Return a copy of the path
// 		}
// 		else
// 		{
// 			// Stat failed (errno is likely ENOENT - No such file or directory)
// 			return (NULL); // Indicate failure to find the explicit path
// 		}
// 	}

// 	// --- If not an explicit path, search in PATH ---
// 	path_value = get_env_value("PATH", env);
// 	if (!path_value || path_value[0] == '\0')
// 	{
// 		// PATH not set or empty, cannot search
// 		return (NULL);
// 	}

// 	path_dirs = ft_split(path_value, ':');
// 	if (!path_dirs)
// 	{
// 		// ft_split failed (malloc error likely)
// 		// Optionally: perror("minishell: ft_split failed for PATH");
// 		return (NULL);
// 	}

// 	// Search through each directory listed in PATH
// 	exec_path = search_in_path_dirs(cmd, path_dirs);
// 		// This might return NULL or a malloc'd path

// 	// Clean up the allocated path directory array
// 	ft_free_split(path_dirs);

// 	// Return the result (NULL if not found in PATH,
// 	//	or the allocated full path)
// 	return (exec_path);
// }

// // //moje
// char	*search_in_path_dirs(char *cmd, char **dirs)
// {
// 	int i;
// 	char *full_path;
// 	const char *dir;

// 	i = 0;
// 	while (dirs[i])
// 	{
// 		dir = dirs[i];
// 		if (dir[0] == '\0')
// 			dir = "."; // Handle empty paths (e.g., PATH=:/bin)
// 		full_path = ft_strjoin3(dir, "/", cmd);
// 		if (!full_path)
// 			continue ;

// 		if (access(full_path, X_OK) == 0)
// 			return (full_path);
// 		free(full_path);
// 		i++;
// 	}
// 	return (NULL);
// }




// //nowy proba cleanup 2.05
// void cleanup_all_resources(t_resources *res, int *fds_to_close, int fd_count)
// {
//     int i;

//     // Clean up file descriptors
//     for (i = 0; i < fd_count; i++)
//     {
//         if (fds_to_close[i] >= 0)
//             close(fds_to_close[i]);
//     }

//     // Clean up allocated memory
//     if (res)
//     {
//         if (res->input)
//             free(res->input);
//         if (res->tokens)
//             free_tokens(res->tokens);
//         if (res->cmds)
//             free_cmds(res->cmds);
//         if (res->env)
//             free_env(res->env);
//     }

//     // Clear readline history
//     rl_clear_history();
// }#include "inc/minishell.h"

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
// #include "../inc/minishell.h"
// //////////start moje

// ///moje
// // int	create_heredoc_pipe(int pipe_fd[2])
// // {
// // 	if (pipe(pipe_fd) == -1)
// // 	{
// // 		perror("pipe");
// // 		return (-1);
// // 	}
// // 	return (0);
// // }



// int create_heredoc_pipe(int pipe_fd[2])
// {
//     if (pipe(pipe_fd) == -1)
//     {
//         perror("minishell: pipe");
//         return (-1);
//     }
//     return (0);
// }




// void	process_heredoc_line(int write_fd, char *line)
// {
// 	// Write to pipe (with newline)
// 	write(write_fd, line, ft_strlen(line));
// 	write(write_fd, "\n", 1);
// 	free(line);
// }


// // //moje
// // int	handle_heredoc(const char *delimiter)
// // {
// // 	char	*line;
// // 	int		pipe_fd[2];

// // 	line = NULL;
// // 	if (create_heredoc_pipe(pipe_fd) < 0)
// // 		return (-1);
// // 	while (1)
// // 	{
// // 		line = readline("> ");
// // 		if (!line) // Handle Ctrl+D
// // 		{
// // 			write(1, "\n", 1);
// // 			break ;
// // 		}
// // 		if (ft_strcmp(line, delimiter) == 0)
// // 		{
// // 			free(line);
// // 			break ;
// // 		}
// // 		process_heredoc_line(pipe_fd[1], line);
// // 	}
// // 	close(pipe_fd[1]);
// // 	return (pipe_fd[0]);
// // }


// // ///////NAJLEPSZY, ZOSTAWIC, DZIALA DLA 3 HEREDOCOW
// int handle_heredoc(char *delimiter)
// {
//     char *line;
//     int pipe_fd[2];
//     int return_fd = -1;
//     bool delimiter_found = false;

//     if (create_heredoc_pipe(pipe_fd) < 0) // create_heredoc_pipe just calls pipe()
//         return (-1);

//     return_fd = pipe_fd[0]; // Store the read end

//     // Optional: Temporarily ignore SIGINT for heredoc input? Complex.
//     // Setup signals appropriate for interactive input if needed.

//     while (1)
//     {
//         line = readline("> ");

//         if (!line) // Handle Ctrl+D (EOF) or readline error
//         {
//             ft_putstr_fd("minishell: warning: here-document delimited by end-of-file (wanted `", STDERR_FILENO);
//             ft_putstr_fd(delimiter, STDERR_FILENO);
//             ft_putstr_fd("')\n", STDERR_FILENO);
//             break; // Exit loop - unsuccessful termination
//         }

//         if (ft_strcmp(line, delimiter) == 0)
//         {
//             delimiter_found = true; // Mark as successful
//             free(line);
//             break; // Exit loop - successful termination
//         }
//         // Write line + newline to the pipe
//         write(pipe_fd[1], line, ft_strlen(line));
//         write(pipe_fd[1], "\n", 1);
//         free(line);
//     }

//     close(pipe_fd[1]); // Close write end

//     if (delimiter_found) {
//         return (return_fd); // Return the read FD on success
//     } else {
//         close(return_fd); // Close the read end as it's not needed/valid
//         return (-1); // Return an error code
//     }
// }

// int	handle_heredoc_redir(char *delimiter)
// {
// 	int	fd;

// 	fd = handle_heredoc(delimiter); // Call heredoc handler
// 	if (fd < 0)
// 		return (-1);
// 	if (dup2(fd, STDIN_FILENO) < 0)
// 	{
// 		perror("dup2");
// 		close(fd);
// 		return (-1);
// 	}
// 	close(fd);
// 	return (0);
// }

// ////////////koniec moje

// //////////////////////ponizej nowe

// int handle_all_heredocs(t_cmd *cmds)
// {
//     t_cmd   *current_cmd;
//     t_redir *current_redir;
//     int     fd;

//     current_cmd = cmds;
//     while (current_cmd)
//     {
//         current_redir = current_cmd->redirections;
//         while (current_redir)
//         {
//             if (current_redir->type == T_HEREDOC)
//             {
//                 // Make sure heredoc_fd is reset (important if function called multiple times, though unlikely here)
//                 if (current_redir->heredoc_fd != -1) {
//                    close(current_redir->heredoc_fd);
//                    current_redir->heredoc_fd = -1;
//                 }

//                 // 'file' field holds the delimiter for T_HEREDOC
//                 fd = handle_heredoc(current_redir->file);

//                 if (fd < 0)
//                 {
//                     // handle_heredoc should print the warning on EOF
//                     g_exit_status = 1; // Set error status
//                     // No need to close FDs here, as handle_heredoc closes on error
//                     return (-1); // Signal failure
//                 }
//                 current_redir->heredoc_fd = fd; // Store the read end FD
//             }
//             current_redir = current_redir->next;
//         }
//         current_cmd = current_cmd->next;
//     }
//     return (0); // Success
// }

// void close_heredoc_fds(t_cmd *cmds)
// {
//     t_cmd   *current_cmd;
//     t_redir *current_redir;

//     current_cmd = cmds;
//     while (current_cmd)
//     {
//         current_redir = current_cmd->redirections;
//         while (current_redir)
//         {
//             if (current_redir->type == T_HEREDOC && current_redir->heredoc_fd != -1)
//             {
//                 close(current_redir->heredoc_fd);
//                 current_redir->heredoc_fd = -1; // Mark as closed
//             }
//             current_redir = current_redir->next;
//         }
//         current_cmd = current_cmd->next;
//     }
// }
// #include "minishell.h"

// // dup - tworzy kopie file descriptora oldfd, using the lowest-numbered
// // unused file descriptor fot the new descriptor
// // dup2
// //	- to samo tylko uzywa konkretnego file descriptora podanego mu do funkcji
// // int dup2(int oldfd, int newfd);

// // < file
// // // zamiast czytac z klawiatury (stdin), czytaj z tego file
// int	handle_input_redir(char *file)
// {
// 	int	fd;

// 	fd = open(file, O_RDONLY);
// 	if (fd < 0)
// 		return (-1);
// 	if (dup2(fd, STDIN_FILENO) < 0)
// 	{
// 		close(fd);
// 		return (-1);
// 	}
// 	close(fd);
// 	return (0);
// }

// int	handle_output_redir(char *file)
// {
// 	int	fd;

// 	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	if (fd < 0)
// 		return (-1);
// 	if (dup2(fd, STDOUT_FILENO) < 0)
// 	{
// 		close(fd);
// 		return (-1);
// 	}
// 	close(fd);
// 	return (0);
// }

// int	handle_append_redir(char *file)
// {
// 	int	fd;

// 	fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 	if (fd < 0)
// 		return (-1);
// 	if (dup2(fd, STDOUT_FILENO) < 0)
// 	{
// 		close(fd);
// 		return (-1);
// 	}
// 	close(fd);
// 	return (0);
// }
// #include "../inc/minishell.h"

// void	restore_std_fds(int stdin_copy, int stdout_copy)
// {
// 	// Ensure descriptors are valid before dup2
// 	if (stdin_copy >= 0)
// 	{
// 		dup2(stdin_copy, STDIN_FILENO);
// 		close(stdin_copy);
// 	}
// 	if (stdout_copy >= 0)
// 	{
// 		dup2(stdout_copy, STDOUT_FILENO);
// 		close(stdout_copy);
// 	}
// }

// //function 1 deepsel
// /////TO JEST DOBRA FUNKCJA, DZIALA DLA 3 HEREDOCOW, POWYZEJ TYLKO TESTUJE
// // int process_single_redirection(t_redir *redir)
// // {
// //     int status = 0;
// //     int saved_errno;

// //     if (!redir || !redir->file || redir->file[0] == '\0') 
// //     {
// //         ft_putstr_fd("minishell: syntax error: missing filename or delimiter\n", STDERR_FILENO);
// //         g_exit_status = 2;
// //         return (-1);
// //     }
// //     if (redir->type == T_REDIR_IN)
// //         status = handle_input_redir(redir->file);
// //     else if (redir->type == T_REDIR_OUT)
// //         status = handle_output_redir(redir->file);
// //     else if (redir->type == T_APPEND)
// //         status = handle_append_redir(redir->file);
// //     else if (redir->type == T_HEREDOC)
// //     {
// //         if (redir->heredoc_fd < 0) 
// //         {
// //             ft_putstr_fd("minishell: internal error: invalid heredoc fd for ", STDERR_FILENO);
// //              ft_putstr_fd(redir->file, STDERR_FILENO);
// //              ft_putstr_fd("\n", STDERR_FILENO);
// //             status = -1;
// //         }
// //         else 
// //         {
// //             if (dup2(redir->heredoc_fd, STDIN_FILENO) < 0)
// //                  status = -1;
// //             close(redir->heredoc_fd);
// //         }
// //     }
// //     if (status < 0)
// //     {
// //         saved_errno = errno;
// //         ft_putstr_fd("minishell: ", STDERR_FILENO);
// //         ft_putstr_fd(redir->file, STDERR_FILENO);
// //         ft_putstr_fd(": ", STDERR_FILENO);
// //         ft_putstr_fd(strerror(saved_errno), STDERR_FILENO);
// //         ft_putstr_fd("\n", STDERR_FILENO);
// //         g_exit_status = 1;
// //         return (-1);
// //     }
// //     return (0);
// // }

// ///////////////////podzial process_single_redirection//////////////////////

// int	validate_redirection(t_redir *redir)
// {
// 	if (!redir)
// 	{
// 		ft_putstr_fd("minishell: syntax error\n", STDERR_FILENO);
// 		g_exit_status = 2;
// 		return (-1);
// 	}
// 	if (!redir->file || redir->file[0] == '\0')
// 	{
// 		ft_putstr_fd("minishell: missing filename\n", STDERR_FILENO);
// 		g_exit_status = 2;
// 		return (-1);
// 	}
// 	return (0);
// }

// /* heredoc_utils.c */
// int	handle_heredoc2(t_redir *redir)
// {
// 	if (redir->heredoc_fd < 0)
// 	{
// 		ft_putstr_fd("minishell: heredoc error: ", STDERR_FILENO);
// 		ft_putstr_fd(redir->file, STDERR_FILENO);
// 		ft_putstr_fd("\n", STDERR_FILENO);
// 		return (-1);
// 	}
// 	if (dup2(redir->heredoc_fd, STDIN_FILENO) < 0)
// 		return (-1);
// 	close(redir->heredoc_fd);
// 	return (0);
// }

// /* redirection_utils.c */
// int	handle_redirection_type(t_redir *redir)
// {
// 	if (redir->type == T_REDIR_IN)
// 		return (handle_input_redir(redir->file));
// 	else if (redir->type == T_REDIR_OUT)
// 		return (handle_output_redir(redir->file));
// 	else if (redir->type == T_APPEND)
// 		return (handle_append_redir(redir->file));
// 	else if (redir->type == T_HEREDOC)
// 		return (handle_heredoc2(redir));
// 	return (0);
// }

// /* error_utils.c */
// void	print_redirection_error(t_redir *redir)
// {
// 	int	saved_errno;

// 	saved_errno = errno;
// 	ft_putstr_fd("minishell: ", STDERR_FILENO);
// 	ft_putstr_fd(redir->file, STDERR_FILENO);
// 	ft_putstr_fd(": ", STDERR_FILENO);
// 	ft_putstr_fd(strerror(saved_errno), STDERR_FILENO);
// 	ft_putstr_fd("\n", STDERR_FILENO);
// 	g_exit_status = 1;
// }

// /* process_redirection.c */
// int	process_single_redirection(t_redir *redir)
// {
// 	int	status;

// 	if (validate_redirection(redir) < 0)
// 		return (-1);
// 	status = handle_redirection_type(redir);
// 	if (status < 0)
// 	{
// 		print_redirection_error(redir);
// 		return (-1);
// 	}
// 	return (0);
// }




// ///////////////////koniec podzial process_single_redirection//////////////////////


// // potrojny heredoc dziala
// /* Main function that processes all redirections in the list */
// int	setup_redirections(t_redir *redir_list)
// {
// 	while (redir_list)
// 	{
// 		if (process_single_redirection(redir_list) < 0)
// 			return (-1);
// 		redir_list = redir_list->next;
// 	}
// 	return (0);
// }
// #include "../inc/minishell.h"

// int	handle_tilde_path(char **path, char *arg, t_env **env, char *old_pwd)
// {
// 	char	*home;

// 	home = get_env_value("HOME", *env);
// 	if (!home)
// 	{
// 		ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
// 		free(old_pwd);
// 		return (ERROR);
// 	}
// 	*path = ft_strjoin(home, &arg[1]);
// 	if (!*path)
// 	{
// 		free(old_pwd);
// 		return (ERROR);
// 	}
// 	return (CONTINUE);
// }

// int	handle_regular_path(char **path, char *arg, char *old_pwd)
// {
// 	*path = ft_strdup(arg);
// 	if (!*path)
// 	{
// 		free(old_pwd);
// 		return (ERROR);
// 	}
// 	return (CONTINUE);
// }

// int	process_path_argument(char **args, t_env **env, char **old_pwd)
// {
// 	char	*path;
// 	int		result;

// 	if (args[1][0] == '~' && args[1][1] == '/')
// 		result = handle_tilde_path(&path, args[1], env, *old_pwd);
// 	else
// 		result = handle_regular_path(&path, args[1], *old_pwd);
// 	if (result != CONTINUE)
// 		return (result);
// 	if (chdir(path) == -1)
// 	{
// 		perror("minishell: cd");
// 		free(path);
// 		free(*old_pwd);
// 		return (ERROR);
// 	}
// 	free(path);
// 	return (update_pwds(env, *old_pwd));
// }

// int	bi_cd(char **args, t_env **env)
// {
// 	char	*old_pwd;
// 	int		special_case_result;

// 	if (args[1] && args[2])
// 	{
// 		ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
// 		return (ERROR);
// 	}
// 	old_pwd = getcwd(NULL, 0);
// 	if (!old_pwd)
// 	{
// 		if (get_env_value("PWD", *env))
// 			old_pwd = ft_strdup(get_env_value("PWD", *env));
// 		else
// 			old_pwd = ft_strdup("");
// 	}
// 	if (!args[1])
// 		return (handle_no_args(env, &old_pwd));
// 	special_case_result = process_special_case(args[1], env, &old_pwd);
// 	if (special_case_result != CONTINUE)
// 	{
// 		free(old_pwd);
// 		return (special_case_result);
// 	}
// 	return (process_path_argument(args, env, &old_pwd));
// }
// #include "../inc/minishell.h"

// int	handle_home_case(t_env **env, char **old_pwd)
// {
// 	char	*home;
// 	char	*pwd_value;

// 	home = get_env_value("HOME", *env);
// 	if (!home)
// 	{
// 		ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
// 		return (ERROR);
// 	}
// 	pwd_value = get_env_value("PWD", *env);
// 	if (pwd_value)
// 		*old_pwd = ft_strdup(pwd_value);
// 	else
// 		*old_pwd = getcwd(NULL, 0);
// 	if (chdir(home) == -1)
// 	{
// 		perror("minishell: cd");
// 		free(*old_pwd);
// 		return (ERROR);
// 	}
// 	return (SUCCESS);
// }

// int	handle_dash_case(t_env **env, char **old_pwd)
// {
// 	char	*pwd_value;
// 	char	*oldpwd;

// 	oldpwd = get_env_value("OLDPWD", *env);
// 	if (!oldpwd)
// 	{
// 		ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR_FILENO);
// 		return (ERROR);
// 	}
// 	pwd_value = get_env_value("PWD", *env);
// 	if (pwd_value)
// 		*old_pwd = ft_strdup(pwd_value);
// 	else
// 		*old_pwd = getcwd(NULL, 0);
// 	if (chdir(oldpwd) == -1)
// 	{
// 		perror("minishell: cd");
// 		free(*old_pwd);
// 		return (ERROR);
// 	}
// 	ft_putendl_fd(oldpwd, STDOUT_FILENO);
// 	return (SUCCESS);
// }

// int	process_special_case(char *arg, t_env **env, char **old_pwd)
// {
// 	if (!arg[0] || (arg[0] == '~' && (ft_isspace(arg[1]) || arg[1] == '\0')))
// 		return (handle_home_case(env, old_pwd));
// 	if (arg[0] == '-')
// 		return (handle_dash_case(env, old_pwd));
// 	return (CONTINUE);
// }

// int	handle_no_args(t_env **env, char **old_pwd)
// {
// 	if (handle_home_case(env, old_pwd) == SUCCESS)
// 		return (update_pwds(env, *old_pwd));
// 	return (ERROR);
// }
// #include "../inc/minishell.h"

// void	update_or_create_pwd(t_env **env, t_env *pwd_var, char *new_pwd)
// {
// 	if (pwd_var)
// 	{
// 		free(pwd_var->value);
// 		pwd_var->value = new_pwd;
// 	}
// 	else
// 	{
// 		env_add_back(env, env_new("PWD", new_pwd));
// 		free(new_pwd);
// 	}
// }

// void	update_or_create_oldpwd(t_env **env, t_env *oldpwd_var,
// 		char *old_pwd_copy)
// {
// 	if (oldpwd_var)
// 	{
// 		free(oldpwd_var->value);
// 		if (old_pwd_copy)
// 			oldpwd_var->value = old_pwd_copy;
// 		else
// 			oldpwd_var->value = ft_strdup("");
// 	}
// 	else if (old_pwd_copy)
// 	{
// 		env_add_back(env, env_new("OLDPWD", old_pwd_copy));
// 		free(old_pwd_copy);
// 	}
// }

// int	update_pwds(t_env **env, char *old_pwd)
// {
// 	t_env	*pwd_var;
// 	t_env	*oldpwd_var;
// 	char	*new_pwd;
// 	char	*old_pwd_copy;

// 	old_pwd_copy = NULL;
// 	if (old_pwd)
// 		old_pwd_copy = ft_strdup(old_pwd);
// 	pwd_var = get_env("PWD", *env);
// 	oldpwd_var = get_env("OLDPWD", *env);
// 	new_pwd = getcwd(NULL, 0);
// 	if (!new_pwd)
// 	{
// 		perror("minishell: cd");
// 		free(old_pwd);
// 		free(old_pwd_copy);
// 		return (ERROR);
// 	}
// 	update_or_create_pwd(env, pwd_var, new_pwd);
// 	update_or_create_oldpwd(env, oldpwd_var, old_pwd_copy);
// 	free(old_pwd);
// 	return (SUCCESS);
// }
// #include "../inc/minishell.h"

// int	bi_echo(char *argv[])
// {
// 	int	i;
// 	int	newline;
// 	int	first_arg;

// 	i = handle_n_option(argv, &newline);
// 	first_arg = 1;
// 	while (argv[i])
// 	{
// 		if (!first_arg)
// 			ft_putchar_fd(' ', STDOUT_FILENO);
// 		ft_putstr_fd(argv[i], STDOUT_FILENO);
// 		first_arg = 0;
// 		i++;
// 	}
// 	if (newline)
// 		ft_putchar_fd('\n', STDOUT_FILENO);
// 	return (0);
// }

// int	handle_n_option(char *argv[], int *newline_flag)
// {
// 	int	i;
// 	int	j;

// 	i = 1;
// 	*newline_flag = 1;
// 	while (argv[i] && ft_strncmp(argv[i], "-n", 2) == 0)
// 	{
// 		j = 2;
// 		while (argv[i][j] && argv[i][j] == 'n')
// 			j++;
// 		if (argv[i][j] == '\0')
// 		{
// 			*newline_flag = 0;
// 			i++;
// 		}
// 		else
// 			break ;
// 	}
// 	return (i);
// }
// #include "../inc/minishell.h"

// /**
//  * env builtin command - wypisuje all env variables
//  * Format: NAME=VALUE (one per line)
//  */
// //moje, be grep halo=123 cos tam
// int	bi_env(char **args, t_env **env)
// {
// 	t_env	*current;

// 	if (args[1])
// 	{
// 		ft_putstr_fd("env: too many arguments\n", STDERR_FILENO);
// 		return (ERROR);
// 	}
// 	current = *env;
// 	while (current)
// 	{
// 		if (current->value && current->value[0] != '\0')
// 		{
// 			ft_putstr_fd(current->key, STDOUT_FILENO);
// 			ft_putstr_fd("=", STDOUT_FILENO);
// 			ft_putstr_fd(current->value, STDOUT_FILENO);
// 			ft_putstr_fd("\n", STDOUT_FILENO);
// 		}
// 		current = current->next;
// 	}
// 	return (SUCCESS);
// }
// #include "../inc/minishell.h"

// void	write_stderr(char *str)
// {
// 	write(2, str, ft_strlen(str));
// }

// int	is_integer(char *str)
// {
// 	int	i;

// 	i = 0;
// 	if (str[i] == '-' || str[i] == '+')
// 		i++;
// 	while (str[i])
// 	{
// 		if (str[i] < '0' || str[i] > '9')
// 			return (0);
// 		i++;
// 	}
// 	return (1);
// }

// int	parse_exit_args(char *argv[], int *exit_code)
// {
// 	if (argv[1] && argv[2])
// 	{
// 		write_stderr("exit: too many arguments\n");
// 		*exit_code = 1;
// 		return (1);
// 	}
// 	if (argv[1])
// 	{
// 		if (is_integer(argv[1]))
// 		{
// 			*exit_code = ft_atoi(argv[1]);
// 			*exit_code %= 256;
// 			if (*exit_code < 0)
// 				*exit_code += 256;
// 		}
// 		else
// 		{
// 			write_stderr("exit: numeric argument required\n");
// 			*exit_code = 2;
// 		}
// 	}
// 	return (0);
// }

// int	bi_exit(char *argv[], t_resources *res)
// {
// 	int	exit_code;
// 	int	parse_error;

// 	exit_code = g_exit_status;
// 	ft_putstr_fd("exit\n", STDOUT_FILENO);
// 	parse_error = parse_exit_args(argv, &exit_code);
// 	if (parse_error == 1)
// 	{
// 		g_exit_status = exit_code;
// 		return (exit_code);
// 	}
// 	cleanup(res);
// 	rl_clear_history();
// 	exit(exit_code);
// }
// // without arguments it lists all environment variables that are marked
// // why use export? because by default, shell variables are local to the shell,
// // when you export them, you make them part of the environment,
// //	which is passed to child processes like ls, vim, gcc

// #include "../inc/minishell.h"

// // do setenv var - po podziale
// int	update_existing_env(t_env *existing, const char *var, char *value)
// {
// 	if (ft_strchr(var, '='))
// 	{
// 		free(existing->value);
// 		existing->value = value;
// 	}
// 	existing->is_exported = 1;
// 	return (SUCCESS);
// }

// // do setenv var - po podziale
// int	add_new_env(t_env **env, char *key, char *value)
// {
// 	t_env	*new_node;

// 	new_node = env_new(key, value);
// 	if (!new_node)
// 	{
// 		free(key);
// 		free(value);
// 		return (ERROR);
// 	}
// 	new_node->is_exported = 1;
// 	env_add_back(env, new_node);
// 	free(key);
// 	free(value);
// 	return (SUCCESS);
// }

// int	set_env_var(const char *var, t_env **env)
// {
// 	char	*key;
// 	char	*value;
// 	t_env	*existing;

// 	if (parse_env_var(var, &key, &value) == ERROR)
// 	{
// 		free(key);
// 		free(value);
// 		return (ERROR);
// 	}
// 	existing = get_env(key, *env);
// 	if (existing)
// 	{
// 		update_existing_env(existing, var, value);
// 		free(key);
// 	}
// 	else
// 	{
// 		if (add_new_env(env, key, value) == ERROR)
// 			return (ERROR);
// 	}
// 	return (SUCCESS);
// }

// int	bi_export(char **args, t_env **env)
// {
// 	int	i;
// 	int	status;

// 	if (!env)
// 		return (ERROR);
// 	if (!args[1])
// 	{
// 		print_env_export_format(*env);
// 		return (SUCCESS);
// 	}
// 	status = SUCCESS;
// 	i = 0;
// 	while (args[++i] && (status == SUCCESS || 1))
// 	{
// 		if (!is_valid_identifier(args[i]))
// 		{
// 			ft_putstr_fd("minishell: export: `", 2);
// 			ft_putstr_fd(args[i], 2);
// 			ft_putstr_fd("': not a valid identifier\n", 2);
// 			status = ERROR;
// 		}
// 		else if (set_env_var(args[i], env) != SUCCESS)
// 			status = ERROR;
// 	}
// 	return (status);
// }

// int	is_valid_identifier(const char *arg)
// {
// 	int	i;

// 	if (!arg || (!ft_isalpha(arg[0]) && arg[0] != '_'))
// 		return (0);
// 	i = 1;
// 	while (arg[i] && arg[i] != '=')
// 	{
// 		if (!ft_isalnum(arg[i]) && arg[i] != '_')
// 			return (0);
// 		i++;
// 	}
// 	return (1);
// }
// #include "../inc/minishell.h"

// // calculate the length of an environment variable's value
// size_t	env_len(t_env *env)
// {
// 	if (!env || !env->value)
// 		return (0);
// 	return (ft_strlen(env->value));
// }

// char	*ft_strndup(const char *s, size_t n)
// {
// 	char	*new;
// 	size_t	len;

// 	if (!s)
// 		return (NULL);
// 	len = ft_strlen(s);
// 	if (n < len)
// 		len = n;
// 	new = (char *)malloc(len + 1);
// 	if (!new)
// 		return (NULL);
// 	ft_memcpy(new, s, len);
// 	new[len] = '\0';
// 	return (new);
// }

// // print all environment variables in export format
// // // format: declare -x KEY="VALUE"
// void	print_env_export_format(t_env *env)
// {
// 	while (env)
// 	{
// 		ft_putstr_fd("declare -x ", STDOUT_FILENO);
// 		ft_putstr_fd(env->key, STDOUT_FILENO);
// 		ft_putstr_fd("=\"", STDOUT_FILENO);
// 		ft_putstr_fd(env->value, STDOUT_FILENO);
// 		ft_putstr_fd("\"\n", STDOUT_FILENO);
// 		env = env->next;
// 	}
// }

// //moje, ponizej proba z usunieciem leakow
// int	parse_env_var(const char *var, char **key, char **value)
// {
// 	char	*equals;

// 	equals = ft_strchr(var, '=');
// 	if (!equals)
// 	{
// 		*key = ft_strdup(var);
// 		*value = ft_strdup("");
// 	}
// 	else
// 	{
// 		*key = ft_substr(var, 0, equals - var);
// 		*value = ft_strdup(equals + 1);
// 	}
// 	if (!*key || !*value)
// 	{
// 		free(*key);
// 		free(*value);
// 		return (ERROR);
// 	}
// 	return (SUCCESS);
// }
// #include "../inc/minishell.h"

// int	bi_pwd(char *argv[])
// {
// 	char	cwd[PATH_MAX];

// 	(void)argv;
// 	if (getcwd(cwd, sizeof(cwd)) != NULL)
// 	{
// 		ft_putstr_fd(cwd, STDOUT_FILENO);
// 		ft_putstr_fd("\n", STDOUT_FILENO);
// 		return (0);
// 	}
// 	perror("minishell: pwd");
// 	return (1);
// }
// #include "../inc/minishell.h"

// /**
//  * Remove an environment variable from the linked list
//  */
// int	remove_env_var(char *varname, t_env **env)
// {
// 	t_env	*current;
// 	t_env	*previous;

// 	if (!env || !*env || !varname)
// 		return (ERROR);
// 	current = *env;
// 	previous = NULL;
// 	while (current)
// 	{
// 		if (ft_strcmp(current->key, varname) == 0)
// 		{
// 			if (previous == NULL)
// 				*env = current->next;
// 			else
// 				previous->next = current->next;
// 			free(current->key);
// 			free(current->value);
// 			free(current);
// 			return (SUCCESS);
// 		}
// 		previous = current;
// 		current = current->next;
// 	}
// 	return (SUCCESS);
// }

// /**
//  * Unset environment variables (no options supported)
//  */
// int	bi_unset(char **args, t_env **env)
// {
// 	int	i;
// 	int	status;

// 	status = SUCCESS;
// 	if (!args[1])
// 		return (SUCCESS);
// 	i = 1;
// 	while (args[i])
// 	{
// 		if (!is_valid_identifier(args[i]))
// 		{
// 			ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
// 			ft_putstr_fd(args[i], STDERR_FILENO);
// 			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
// 			status = ERROR;
// 		}
// 		else
// 		{
// 			if (remove_env_var(args[i], env) != SUCCESS)
// 				free_env(*env);
// 		}
// 		i++;
// 	}
// 	return (status);
// }
// #ifndef MINISHELL_H
// # define MINISHELL_H

// # include "../libft/libft.h"
// # include <errno.h>
// # include <fcntl.h>
// # include <limits.h>
// # include <readline/history.h>
// # include <readline/readline.h>
// # include <signal.h>
// # include <stdbool.h>
// # include <stdint.h>
// # include <stdio.h>
// # include <sys/signal.h>
// # include <sys/types.h>
// # include <sys/wait.h>
// # include <unistd.h>
// #include <sys/stat.h> 

// // constants
// # define SUCCESS 0
// # define ERROR 1
// # define CMD_NOT_FOUND 127     // moze do skorzystania
// # define PERMISSION_DENIED 126 // same
// # define ERROR_CD -1           // usunac?
// # define DOLLAR '$'
// # define CONTINUE 2

// extern int g_exit_status;
// // extern volatile sig_atomic_t g_in_command;

// // ------------------------------------------
// // Struktury danych
// // ------------------------------------------

// typedef struct s_env
// {
// 	char *key;
// 	char *value;
// 	int is_exported; /// added for export, env , pipes ghost=123
// 	struct s_env *next;
// } t_env;

// typedef enum e_token_type
// {
// 	T_WORD,
// 	T_PIPE,
// 	T_REDIR_IN,
// 	T_REDIR_OUT,
// 	T_APPEND,
// 	T_HEREDOC,
// 	T_SINGLE_QUOTED,
// 	T_DOUBLE_QUOTED,
// } t_token_type;

// typedef enum e_quote
// {
// 	NO_QUOTE,
// 	SINGLE_QUOTE,
// 	DOUBLE_QUOTE
// } t_quote;

// typedef struct s_token
// {
// 	char *value;
// 	t_token_type type;
// 	struct s_token *next;
// } t_token;

// typedef struct s_redir
// {
// 	char *file;  //delimiter for heredoc, filename otherwise
// 	t_token_type type;
// 	struct s_redir *next;
// 	int heredoc_fd; //dodane pole przy naprawie heredoca
// } t_redir;

// typedef struct s_cmd
// {
// 	char **args;
// 	t_redir *redirections;
// 	struct s_cmd *next;
// 	int pipe_fd[2];
// } t_cmd;

// typedef struct s_parse_state
// {
// 	int i;
// 	int error;
// 	int single_quote_used;
// 	char *current_word;
// 	t_quote current_quote;
// 	t_token *tokens;
// } t_parse_state;

// // dla handle_parend ... dodaje poniej
// typedef struct s_resources
// {
// 	t_env *env;
// 	t_token *tokens;
// 	t_cmd *cmds;
// 	char *input;
// } t_resources;

// typedef struct s_fds
// {
// 	int stdin;
// 	int stdout;
// } t_fds;

// // ------------------------------------------
// // Prototypy funkcji
// // ------------------------------------------

// ////podzielony executor.c:  /////////////////////
// // redirs_execute.c
// int	process_single_redirection(t_redir *redir);
// int	setup_redirections(t_redir *redir_list);
// void	restore_std_fds(int stdin_copy, int stdout_copy);

// // execution.c
// int	setup_pipe_and_fork(t_cmd *cmd, int *pipe_fd);
// void	handle_child_process(t_cmd *cmd, t_env **env, int prev_pipe_read,
// 		int *pipe_fd);

// void	execute_external(t_cmd *cmd, t_env *env);
// // void	execute(t_cmd *cmds, t_env **env);
// void	execute(t_cmd *cmds, t_env **env);

// // executor.c
// // void	cleanup(t_env *env, t_token *tokens, t_cmd *cmds, char *input);
// void	cleanup(t_resources *res);
// void	process_command(t_cmd *cmd, t_env **env, int prev_pipe_read,
// 		int *pipe_fd);
// int	init_execution(t_cmd *cmds, int *stdin_copy, int *stdout_copy);
// void	cleanup_execution(int stdin_copy, int stdout_copy);
// // void	wait_for_children(void);
// void	wait_for_children(pid_t last_cmd_pid);

// // pipes.c
// void	cleanup_pipes(int *pipe_fd, int *prev_pipe_read);
// void	prepare_pipeline_execution(t_cmd *cmds, int *prev_pipe_read);
// void	execute_pipeline(t_cmd *cmds, t_env **env, int *prev_pipe_read);
// // void execute_pipeline(t_cmd *cmds, t_resources *res, int *prev_pipe_read);
// void	update_pipe_status(t_cmd *cmd, int *prev_pipe_read, int *pipe_fd);

// // i builtinhandler

// /////////////koniec podzielonego executora

// // builtin_handler.c
// int	is_builtin(char *cmd);
// // int		run_builtin(t_cmd *cmd, t_env **env);
// int	run_builtin(t_cmd *cmd, t_resources *res);
// // int		handle_parent_builtin(t_cmd *cmd, t_env **env, int stdin_copy,
// // 			int stdout_copy);
// int	handle_parent_builtin(t_cmd *cmd, t_fds fds, t_resources *res);

// // int		handle_single_builtin(t_cmd *cmds, t_env **env, int stdin_copy,
// // 			int stdout_copy);
// int	handle_single_builtin(t_cmd *cmds, t_resources *res);

// // env_utils.c
// t_env	*get_env(const char *key, t_env *env);
// char	*get_env_value(const char *key, t_env *env);
// t_env	*env_new(const char *key, const char *value);
// void	env_add_back(t_env **env, t_env *new_node);

// // init_env.c
// char	*extract_key(char *env_var, int *key_len);
// int	handle_env_node_creation(char *key, char *value, t_env **env);
// int	process_single_env_var(char *env_var, t_env **env);
// void	init_env(char **envp, t_env **env);

// // executor_utils.c
// int	count_env_nodes(t_env *env);
// char	*create_env_entry(t_env *node);
// void	free_array_on_error(char **arr, int count);
// int	populate_env_array(t_env *env, char **arr);
// char	**env_to_arr(t_env *env);

// // expander.c
// char	*extract_var_name(char *str);
// void	expand_variables(t_token *tokens, t_env *env);
// int	handle_braced_var(char **result, char *str, int i, t_env *env);
// int	handle_standard_var(char **result, char *str, int i, t_env *env);
// char	*expand_str(char *str, t_env *env);
// // char	*remove_quotes(char *str, char quote_type);

// // expander_utils.c
// int	is_special_var(char *str, int i);
// int	is_braced_var(char *str, int i);
// int	is_standard_var(char *str, int i);
// void	append_char(char **result, char c);
// int	handle_special_var(char **result, int i);

// // lexer.c
// void	add_token(t_token **tokens, char *value, t_token_type type);
// t_token	*reverse_tokens(t_token *tokens);
// void	parse_input_loop(char *input, t_parse_state *state);
// t_token	*tokenize_input(char *input);
// void	handle_input_char(char *input, int *i, char **current_word,
// 		t_token **tokens);

// // lexer_utils.c
// int	is_pipe(char c);
// int	is_redirect(char c);
// int	is_quote(char c);
// t_quote	get_quote_type(char c);
// t_token_type	get_token_type_from_quote(t_quote quote);

// // lexer_utils2.c
// void finalize_current_word(t_token **tokens, char **current_word, int single_quote_used);

// int	handle_quote(char *input, int *i, t_token **tokens, int *error);
// char	*read_quoted_content(char *input, int *i, t_quote quote, int *error);
// void	free_tokens(t_token *tokens);

// // lexer_utils3.c
// void	skip_space(int *i, char **word, t_token **tokens);
// void	handle_pipe(int *i, char **word, t_token **tokens);

// // int	handle_quoted(char *input, int *i, char **current_word, int *error);
// int	handle_quoted(char *input, int *i, t_token **tokens, int *error);

// void	append_char_to_word(char **word, char c);
// t_token	*handle_unexpected_quote(char *word, t_token *tokens);

// // lexer_utils4.c
// void	handle_redirect_token(char *input, int *i, char **word,
// 		t_token **tokens);
// void	validate_redirection_syntax(t_token **tokens);
// void	handle_redirection(char *input, int *i, t_token **tokens);

// // main_utils.c
// void	free_env(t_env *env);
// void	init_shell(char **envp, t_env **env);
// // void	handle_eof(t_env *env, int exit_status);
// void	handle_eof(int exit_status);
// // void	process_input(char *input, t_env **env);
// void	process_input(char *input, t_env **env);
// void	clean_cmds_tokens_input(t_cmd *cmds, t_token *tokens, char *input);

// // parser_utils.c
// void	add_redir_to_cmd(t_cmd *cmd, t_redir *new_redir);
// t_cmd	*cmd_new(void);
// t_cmd	*cmd_last(t_cmd *cmds);
// void	cmd_add_back(t_cmd **cmds, t_cmd *new_cmd);
// int	is_redirection(t_token_type type);

// // parser_utils2.c
// void	add_arg_to_cmd(t_cmd *cmd, char *arg);

// // parser.c
// void	handle_redirection_parser(t_token **tokens, t_cmd *cmd);
// void	free_cmd_contents(t_cmd *cmd);
// void	free_cmds(t_cmd *cmds);
// t_cmd	*process_token(t_token **tokens, t_cmd *current_cmd, t_cmd **cmds);
// t_cmd	*parse_tokens(t_token *tokens);

// // signals.c
// void	handle_sigint_prompt(int sig);
// void	handle_sigint_command(int sig);
// void	setup_signals_for_prompt(void);
// void	setup_signals_for_command(void);

// // utils.c
// int	ft_isspace(char c);
// char	*ft_strjoin_free(char *s1, char *s2);
// char	*ft_strjoin_char(char *s, char c);
// int	ft_strcmp(const char *s1, const char *s2);

// // utils2.c
// char	*ft_strjoin3(const char *s1, const char *s2, const char *s3);
// char	*check_absolute_path(char *cmd);
// char	*get_exec_path(char *cmd, t_env *env);
// char	*search_in_path_dirs(char *cmd, char **dirs);
// void	ft_free_split(char **split);

// //--------------BUILTINS--------------
// // bi_cd_special_case.c
// int	handle_home_case(t_env **env, char **old_pwd);
// int	handle_dash_case(t_env **env, char **old_pwd);
// int	process_special_case(char *arg, t_env **env, char **old_pwd);
// int	handle_no_args(t_env **env, char **old_pwd);

// // bi_cd.c
// int	handle_tilde_path(char **path, char *arg, t_env **env, char *old_pwd);
// int	handle_regular_path(char **path, char *arg, char *old_pwd);
// int	process_path_argument(char **args, t_env **env, char **old_pwd);
// int	bi_cd(char **args, t_env **env);

// // bi_cd_utils.c
// void	update_or_create_pwd(t_env **env, t_env *pwd_var, char *new_pwd);
// void	update_or_create_oldpwd(t_env **env, t_env *oldpwd_var,
// 		char *old_pwd_copy);
// int		update_pwds(t_env **env, char *old_pwd);

// // bi_echo.c
// int		bi_echo(char *argv[]);
// int		handle_n_option(char *argv[], int *newline_flag);

// // bi_env.c
// int		bi_env(char **args, t_env **env);

// // bi_exit.c
// void	write_stderr(char *str);
// int		is_integer(char *str);
// int		parse_exit_args(char *argv[], int *exit_code);
// int		bi_exit(char *argv[], t_resources *res);


// // bi_export_utils.c
// size_t	env_len(t_env *env);
// char	*ft_strndup(const char *s, size_t n);
// void	print_env_export_format(t_env *env);
// int		parse_env_var(const char *var, char **key, char **value);

// // bi_export.c
// int		update_existing_env(t_env *existing, const char *var, char *value);
// int		add_new_env(t_env **env, char *key, char *value);
// int		set_env_var(const char *var, t_env **env);
// int		bi_export(char **args, t_env **env);
// int		is_valid_identifier(const char *arg);

// // bi_pwd.c
// int		bi_pwd(char *argv[]);

// // bi_unset.c
// int		remove_env_var(char *varname, t_env **env);
// int		bi_unset(char **args, t_env **env);

// ////--------------REDIRECTIONS----------------
// // input_output_append.c
// int	handle_input_redir(char *file);
// int	handle_output_redir(char *file);
// int	handle_append_redir(char *file);

// // heredoc.c
// int	create_heredoc_pipe(int pipe_fd[2]);
// void	process_heredoc_line(int write_fd, char *line);
// // int	handle_heredoc(const char *delimiter);
// int	handle_heredoc(char *delimiter);
// int	handle_heredoc2(t_redir *redir);

// int	handle_heredoc_redir(char *delimiter);
// void shift_empty_args(t_cmd *cmds);
// int handle_all_heredocs(t_cmd *cmds);
// void close_heredoc_fds(t_cmd *cmds);
// int create_heredoc_pipe(int pipe_fd[2]);

// // int handle_quoted_token(char *input, int *i, t_token **tokens, int *error);
// // int append_char_to_word(char **word, char c);
// #endif
