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

// int	run_builtin(t_cmd *cmd, t_resources *res)
// {
// 	if (!strcmp(cmd->args[0], "echo"))
// 		return (bi_echo(cmd->args));
// 	if (!strcmp(cmd->args[0], "pwd"))
// 		return (bi_pwd(cmd->args));
// 	if (!strcmp(cmd->args[0], "exit"))
// 		return (bi_exit(cmd->args, res));
// 	if (!strcmp(cmd->args[0], "unset"))
// 		return (bi_unset(cmd->args, &res->env));
// 	if (!strcmp(cmd->args[0], "export"))
// 		return (bi_export(cmd->args, &res->env));
// 	if (!strcmp(cmd->args[0], "env"))
// 		return (bi_env(cmd->args, &res->env));
// 	if (!strcmp(cmd->args[0], "cd"))
// 		return (bi_cd(cmd->args, &res->env));
// 	return (0);
// }

// int	handle_parent_builtin(t_cmd *cmd, t_fds fds, t_resources *res)
// {
// 	if (setup_redirections(cmd->redirections) < 0)
// 	{
// 		restore_std_fds(fds.stdin, fds.stdout);
// 		g_exit_status = 1;
// 		return (1);
// 	}
// 	g_exit_status = run_builtin(cmd, res);
// 	restore_std_fds(fds.stdin, fds.stdout);
// 	return (1);
// }

// int	handle_single_builtin(t_cmd *cmds, t_resources *res)
// {
// 	t_fds	fds;

// 	if (is_builtin(cmds->args[0]) && !cmds->next)
// 	{
// 		fds.stdin = dup(STDIN_FILENO);
// 		fds.stdout = dup(STDOUT_FILENO);
// 		handle_parent_builtin(cmds, fds, res);
// 		return (1);
// 	}
// 	return (0);
// }
// #include "inc/minishell.h"

// void	free_env_node(t_env *node)
// {
// 	if (node)
// 	{
// 		free(node->key);
// 		free(node->value);
// 		free(node);
// 	}
// }

// t_env	*env_new(const char *key, const char *value)
// {
// 	t_env	*new_var;

// 	new_var = (t_env *)malloc(sizeof(t_env));
// 	if (!new_var)
// 		return (NULL);
// 	new_var->key = ft_strdup(key);
// 	if (!new_var->key)
// 	{
// 		free_env_node(new_var);
// 		return (NULL);
// 	}
// 	if (value)
// 		new_var->value = ft_strdup(value);
// 	else
// 		new_var->value = ft_strdup("");
// 	if (!new_var->value)
// 	{
// 		free_env_node(new_var);
// 		return (NULL);
// 	}
// 	new_var->next = NULL;
// 	return (new_var);
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

// 	if (!*env)
// 	{
// 		*env = new_node;
// 	}
// 	else
// 	{
// 		last = *env;
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

// // // //moje
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



// // // //dobre
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
//     free_env(env);          // Free the t_env list //usunelam 405
//     free_cmds(cmd);         // Free the t_cmd structure //usunelam 405

//     exit(exit_code); // Exit with the determined code
// }




// #include "inc/minishell.h"

// //moje
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
// 	int printed_pipe_msg = 0; 

// 	while ((pid = waitpid(-1, &status, 0)) > 0) // Wait for ANY child
// 	{
// 		if (pid == last_cmd_pid)
// 		{
// 			found_last = 1;
// 			if (WIFEXITED(status))
// 			{
// 				last_cmd_status = WEXITSTATUS(status);
// 			}
// 			else if (WIFSIGNALED(status))
// 			{
// 				int signal_num = WTERMSIG(status);
// 				last_cmd_status = 128 + signal_num;
// 			}
// 		}
// 		if (WIFSIGNALED(status))
// 		{
// 			int signal_num = WTERMSIG(status);
// 			if (signal_num == SIGQUIT)
// 			{
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
// 			else if (signal_num == SIGPIPE)
// 			{
//                 if (!printed_pipe_msg)
//                 {
// 				    ft_putstr_fd("Broken pipe\n", STDERR_FILENO);
//                     printed_pipe_msg = 1;
//                 }
// 			}
// 		}
// 	}
// 	if (last_cmd_pid > 0 && found_last)
// 	{

// 		g_exit_status = last_cmd_status;
// 	}
// 	else if (last_cmd_pid <= 0)
// 	{
// 	}
//     else // (last_cmd_pid > 0 && !found_last)
//     {
//     }
// 	if (pid == -1 && errno != ECHILD)

// 		perror("minishell: waitpid");
// }


// #include "inc/minishell.h"

// int		g_exit_status = 0; // Globalna zmienna stanu

// // Count the number of nodes in the environment list
// int	count_env_nodes(t_env *env)
// {
// 	int		count;
// 	t_env	*current;

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

// 	if (!node || !node->key || !node->value)
// 		return (NULL);
// 	key_eq = ft_strjoin(node->key, "=");
// 	if (!key_eq)
// 		return (NULL);
// 	entry = ft_strjoin(key_eq, node->value);
// 	free(key_eq);
// 	if (!entry)
// 		return (NULL);
// 	return (entry);
// }

// //Free the partially allocated array on error
// //Frees arr[0] up to arr[count-1], then frees arr itself
// void	free_array_on_error(char **arr, int count)
// {
// 	int	i;

// 	i = 0;
// 	while (i < count)
// 	{
// 		free(arr[i]);
// 		i++;
// 	}
// 	free(arr);
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
// 			return (-1);
// 		arr[i] = entry;
// 		i++;
// 		current = current->next;
// 	}
// 	arr[i] = NULL;
// 	return (i);
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
// 	if (populate_result == -1)
// 	{
// 		free_array_on_error(arr, count);
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
//     bool error_occurred = false;

//     result = ft_strdup("");
//     if (!result) 
//     {
//         perror("minishell: malloc error in expand_str initial allocation");
//         return (NULL); // Cannot even start
//     }

//     while (str[i] && !error_occurred) 
//     {
//         if (is_special_var(str, i)) 
//         {
//             i = handle_special_var(&result, i);
//             if (i < 0)
//                 error_occurred = true; // Handler signals error (-1)
//         }
//         else if (is_braced_var(str, i)) 
//         {
//             int next_i = handle_braced_var(&result, str, i, env);
//             if (next_i < 0) 
//                 error_occurred = true;
//             else 
//                 i = next_i; // Update index only on success
//         }
//         else if (is_standard_var(str, i)) 
//         {
//              int next_i = handle_standard_var(&result, str, i, env);
//              if (next_i < 0) 
//                  error_occurred = true;
//              else
//                 i = next_i; // Update index only on success
//         }
//         else 
//         { // Append regular character
//             temp_result = ft_strjoin_char(result, str[i]); // Pass current result
//             if (!temp_result) 
//             {
//                 perror("minishell: malloc error in ft_strjoin_char within expand_str");
//                 error_occurred = true; // Set error flag
//                 // Don't update 'result' yet, it needs to be freed below
//             } 
//             else 
//             {
//                 // If successful, ft_strjoin_char already freed the old 'result'.
//                 result = temp_result; // Update result to point to the new string
//                 i++;
//             }
//         }
//         if (error_occurred) 
//            break;
//     }

//     if (error_occurred) 
//     {
//         if (result) 
//             free(result); // Free the partially constructed string
//         return (NULL);
//     }

//     return (result);
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

// int	extract_and_append_content(t_append_info *info, t_input_state *input_state,
// 		char **current_word, int *error)
// {
// 	char	*content;
// 	char	*old_word;

// 	content = ft_substr(input_state->input, info->start, info->len);
// 	if (!content)
// 	{
// 		perror("minishell: malloc error (substr)");
// 		*error = 1;
// 		return (1);
// 	}
// 	old_word = *current_word;
// 	*current_word = ft_strjoin(old_word, content);
// 	free(old_word);
// 	free(content);
// 	if (!*current_word)
// 	{
// 		perror("minishell: malloc error (strjoin)");
// 		*error = 1;
// 		return (1);
// 	}
// 	return (0);
// }

// int	prepare_and_execute_append(t_quote_indices *indices,
// 		t_input_state *input_state, char **current_word, int *error)
// {
// 	int				len;
// 	t_append_info	append_data;

// 	if (ensure_word_allocated(current_word, error))
// 	{
// 		return (1);
// 	}
// 	len = indices->end_pos - indices->start;
// 	if (len > 0)
// 	{
// 		append_data.start = indices->start;
// 		append_data.len = len;
// 		if (extract_and_append_content(&append_data, input_state, current_word,
// 				error))
// 		{
// 			return (1);
// 		}
// 	}
// 	return (0);
// }

// int	find_and_update_for_quote(t_input_state *input_state, int *error,
// 		t_parse_state *state, t_quote_indices *indices_out)
// {
// 	t_quote	local_current_quote;
// 	char	quote_char;
// 	int		start_index_val;

// 	local_current_quote = get_quote_type(input_state->input[*(input_state->i)]);
// 	if (local_current_quote == SINGLE_QUOTE)
// 		quote_char = '\'';
// 	else
// 		quote_char = '"';
// 	(*(input_state->i))++;
// 	start_index_val = *(input_state->i);
// 	indices_out->start = start_index_val;
// 	if (find_closing_quote(input_state, quote_char) != 0)
// 	{
// 		ft_putstr_fd("Minishell: syntax error: unclosed quote\n",
// 			STDERR_FILENO);
// 		*error = 1;
// 		return (1);
// 	}
// 	indices_out->end_pos = *(input_state->i);
// 	if (local_current_quote == SINGLE_QUOTE)
// 		state->single_quote_used = 1;
// 	return (0);
// }

// int	append_and_advance_for_quote(t_input_state *input_state,
// 		char **current_word, int *error, t_quote_indices *indices)
// {
// 	if (prepare_and_execute_append(indices, input_state, current_word, error))
// 		return (1);
// 	(*(input_state->i))++;
// 	return (0);
// }

// // --- Original Function (Now Orchestrator) ---
// int	handle_quoted_append(t_input_state *input_state, char **current_word,
// 		int *error, t_parse_state *state)
// {
// 	t_quote_indices	indices;

// 	if (find_and_update_for_quote(input_state, error, state, &indices) != 0)
// 		return (1);
// 	if (append_and_advance_for_quote(input_state, current_word, error,
// 			&indices) != 0)
// 		return (1);
// 	return (0);
// }
// #include "inc/minishell.h"

// int	ensure_word_allocated(char **current_word, int *error)
// {
// 	if (*current_word == NULL)
// 	{
// 		*current_word = ft_strdup("");
// 		if (*current_word == NULL)
// 		{
// 			perror("minishell: malloc error (ensure_word)");
// 			*error = 1;
// 			return (1);
// 		}
// 	}
// 	return (0);
// }

// int	find_closing_quote(t_input_state *input_state, char quote_char)
// {
// 	int	current_i;

// 	current_i = *(input_state->i);
// 	while (input_state->input[current_i]
// 		&& input_state->input[current_i] != quote_char)
// 	{
// 		current_i++;
// 	}
// 	*(input_state->i) = current_i;
// 	if (input_state->input[current_i] != quote_char)
// 	{
// 		return (1);
// 	}
// 	return (0);
// }
// #include "inc/minishell.h"

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
// 	new->value = ft_strdup(value);
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

// // //moje
// // void free_tokens(t_token *tokens)
// // {
// //     t_token *tmp;

// //     while (tokens)
// //     {
// //         tmp = tokens->next; // Store next *before* freeing current
// //         if (tokens->value)
// //             free(tokens->value); // Free the string value
// //         free(tokens);          // Free the token node
// //         tokens = tmp;         // Move to the next node
// //     }
// // }



// // //debugging
// void free_tokens(t_token *tokens)
// {
//     t_token *current = tokens;
//     t_token *next_node;
//     int count = 0; // Debug counter

//     // --- ADDED: Initial check and head pointer ---
//     printf("DEBUG: free_tokens STARTING with head=%p\n", (void*)current);
//     if (!current) {
//         printf("DEBUG: free_tokens received NULL list, doing nothing.\n");
//         return;
//     }
//     // --- END ADDED ---

//     while (current != NULL)
//     {
//         count++;
//         // --- ADDED: Print current node info ---
//         printf("DEBUG: free_tokens loop %d: current node=%p, next=%p, type=%d\n",
//                count, (void*)current, (void*)current->next, current->type);
//         // --- END ADDED ---

//         next_node = current->next; // Store the next pointer *before* freeing anything

//         if (current->value)
//         {
//             // --- ADDED: Print value being freed ---
//             printf("DEBUG: free_tokens loop %d: freeing value=%p ('%s')\n",
//                    count, (void*)current->value, current->value);
//             // --- END ADDED ---
//             free(current->value);
//             current->value = NULL; // Optional: Prevent dangling pointer issues
//         } else {
//             // --- ADDED: Indicate NULL value ---
//              printf("DEBUG: free_tokens loop %d: value pointer was NULL\n", count);
//             // --- END ADDED ---
//         }

//         // --- ADDED: Print node being freed ---
//         printf("DEBUG: free_tokens loop %d: freeing node %p itself\n", count, (void*)current);
//         // --- END ADDED ---
//         free(current);          // Free the token node itself
//         current = next_node;    // Move to the next node using the stored pointer
//     }

//     // --- ADDED: Final count ---
//     printf("DEBUG: free_tokens FINISHED after attempting to free %d nodes\n", count);
//     // --- END ADDED ---
// }

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

// t_token	*handle_unexpected_quote(char *word, t_token *tokens)
// {
// 	free(word);
// 	free_tokens(tokens);
// 	return (NULL);
// }
// #include "inc/minishell.h"

// #include "inc/minishell.h"

// //moje
// static void	handle_no_input(t_env *env)
// {
// 	t_resources	res;

// 	res.env = env;
// 	res.cmds = NULL;
// 	res.tokens = NULL;
// 	res.input = NULL;
// 	cleanup(&res);
// 	handle_eof(g_exit_status);
// }

// int	main(int argc, char **argv, char **envp)
// {
// 	t_env	*env;
// 	char	*input;

// 	(void)argc;
// 	(void)argv;
// 	init_shell(envp, &env);
// 	while (1)
// 	{
// 		input = readline("minishell$ ");
// 		if (!input)
// 			handle_no_input(env);
// 		if (*input)
// 		{
// 			add_history(input);
// 			process_input(input, &env);
// 		}
// 		else
// 		{
// 			free(input);
// 		}

// 	}
// 	return (0);
// }
// #include "inc/minishell.h"

// void	free_env(t_env *env)
// {
// 	t_env	*tmp;

// 	while (env)
// 	{
// 		tmp = env->next;
// 		free(env->key);
// 		free(env->value);
// 		free(env);
// 		env = tmp;
// 	}
// }

// void	init_shell(char **envp, t_env **env)
// {
// 	setup_signals_for_prompt();
// 	*env = NULL;
// 	init_env(envp, env);
// }

// void	handle_eof(int exit_status)
// {
// 	rl_clear_history();
// 	ft_putstr_fd("exit\n", STDOUT_FILENO);
// 	exit(exit_status);
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
// #include "inc/minishell.h"


// // --- Your Parsing Logic (Modified) ---

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
// 	t_input_state	input_state;

// 	input_state.input = input;
// 	input_state.i = &state->i;

// 	while (input[state->i] && !(state->error))
// 	{
// 		if (input[state->i] == '#' && (state->i == 0
// 				|| ft_isspace(input[state->i - 1])))
// 			break ;

// 		if (ft_isspace(input[state->i]))
// 		{
// 			finalize_current_word(&state->tokens, &state->current_word,
// 				state->single_quote_used);
// 			state->single_quote_used = 0;
// 			state->i++;
// 		}
// 		else if (is_pipe(input[state->i]))
// 		{
// 			finalize_current_word(&state->tokens, &state->current_word,
// 				state->single_quote_used);
// 			state->single_quote_used = 0;
// 			add_token(&state->tokens, "|", T_PIPE);
// 			state->i++;
// 		}
// 		else if (is_redirect(input[state->i]))
// 		{
// 			finalize_current_word(&state->tokens, &state->current_word,
// 				state->single_quote_used);
// 			state->single_quote_used = 0;
// 			handle_redirection(input, &state->i, &state->tokens);
// 		}
// 		else if (is_quote(input[state->i]))
// 		{
// 			// Function call remains the same, error checked via loop condition
// 			handle_quoted_append(&input_state, &state->current_word,
// 					&state->error, state);
// 		}
// 		else
// 		{
// 			// Call append_char_to_word (void return type)
// 			append_char_to_word(&state->current_word, input[state->i]);
// 			// Assume append_char_to_word sets state->error internally on failure
// 			state->i++;
// 		}
// 	}

// 	if (!state->error)
// 	{
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
// 	state.single_quote_used = 0;
// 	state.current_quote = NO_QUOTE; // Initialize based on struct definition

// 	parse_input_loop(input, &state);

// 	if (state.error)
// 	{
// 		if (state.current_word)
// 			free(state.current_word);
// 		free_tokens(state.tokens);
// 		return (NULL);
// 	}

// 	return (reverse_tokens(state.tokens));
// }#include "inc/minishell.h"

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

// // //moje
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

// //moje
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
// 		exit(EXIT_FAILURE);
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

// void	execute_pipeline(t_cmd *cmds, t_env **env, int *prev_pipe_read)
// {
// 	pid_t	last_pid;
// 	t_cmd	*current_cmd;

// 	last_pid = -1;
// 	current_cmd = cmds;
// 	while (current_cmd)
// 	{
// 		last_pid = process_single_command(current_cmd, env, prev_pipe_read,
// 				last_pid);
// 		if (last_pid == -2)
// 			return ;
// 		current_cmd = current_cmd->next;
// 	}
// 	finalize_pipeline(prev_pipe_read, last_pid);
// }

// pid_t	process_single_command(t_cmd *cmd, t_env **env, int *prev_pipe_read,
// 		pid_t last_pid)
// {
// 	int		pipe_fd[2];
// 	pid_t	pid;

// 	pipe_fd[0] = -1;
// 	pipe_fd[1] = -1;
// 	pid = setup_pipe_and_fork(cmd, pipe_fd);
// 	if (pid < 0)
// 	{
// 		if (*prev_pipe_read != -1)
// 			close(*prev_pipe_read);
// 		return (-2);
// 	}
// 	else if (pid == 0)
// 		handle_child_process(cmd, env, *prev_pipe_read, pipe_fd);
// 	else
// 	{
// 		if (!cmd->next)
// 			last_pid = pid;
// 		update_pipe_status(cmd, prev_pipe_read, pipe_fd);
// 	}
// 	return (last_pid);
// }

// void	finalize_pipeline(int *prev_pipe_read, pid_t last_pid)
// {
// 	if (*prev_pipe_read != -1)
// 	{
// 		close(*prev_pipe_read);
// 		*prev_pipe_read = -1;
// 	}
// 	wait_for_children(last_pid);
// }
// #include "inc/minishell.h"

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

// void	process_input(char *input, t_env **env)
// {
// 	t_resources	res;
// 	t_token		*tokens;
// 	t_cmd		*cmds;

// 	tokens = NULL;
// 	cmds = NULL;
// 	res.input = NULL;
// 	res.tokens = NULL;
// 	res.cmds = NULL;
// 	res.env = NULL;
// 	res.input = input;
// 	res.env = *env;
// 	if (!initialize_and_tokenize(input, &tokens))
// 		return ;
// 	res.tokens = tokens;
// 	if (!prepare_commands(tokens, &cmds, &res))
// 	{
// 		free_tokens(tokens);
// 		free(input);
// 		return ;
// 	}
// 	res.cmds = cmds;
// 	execute_commands(cmds, env, &res);
	
// }

// bool	initialize_and_tokenize(char *input, t_token **tokens)
// {
// 	*tokens = tokenize_input(input);
// 	if (!*tokens)
// 	{
// 		free(input);
// 		return (false);
// 	}
// 	return (true);
// }

// bool	prepare_commands(t_token *tokens, t_cmd **cmds, t_resources *res)
// {
// 	expand_variables(tokens, res->env);
// 	*cmds = parse_tokens(tokens);
// 	if (!*cmds)
// 		return (false);
// 	shift_empty_args(*cmds);
// 	if (handle_all_heredocs(*cmds) != 0)
// 	{
// 		close_heredoc_fds(*cmds);
// 		free_cmds(*cmds);
// 		return (false);
// 	}
// 	if (!(*cmds)->args || !(*cmds)->args[0] || (*cmds)->args[0][0] == '\0')
// 	{
// 		close_heredoc_fds(*cmds);
// 		free_cmds(*cmds);
// 		return (false);
// 	}
// 	return (true);
// }

// void	execute_commands(t_cmd *cmds, t_env **env, t_resources *res)
// {
// 	t_fds	fds;

// 	setup_signals_for_command();
// 	if (is_builtin(cmds->args[0]) && !cmds->next)
// 	{
// 		fds.stdin = dup(STDIN_FILENO);
// 		fds.stdout = dup(STDOUT_FILENO);
// 		handle_parent_builtin(cmds, fds, res);
// 	}
// 	else
// 		execute(cmds, env);
// 	setup_signals_for_prompt();
// 	close_heredoc_fds(cmds);
// 	free_cmds(cmds);
// 	free_tokens(res->tokens);
// 	free(res->input);
// }
// #include "inc/minishell.h"

// void	shift_empty_args(t_cmd *cmds)
// {
// 	t_cmd	*current_cmd;
// 	int		i;
// 	char	*empty_arg_to_free;

// 	current_cmd = cmds;
// 	while (current_cmd)
// 	{
// 		if (current_cmd->args && current_cmd->args[0] && current_cmd->args[1]
// 			&& current_cmd->args[0][0] == '\0')
// 		{
// 			i = 0;
// 			empty_arg_to_free = current_cmd->args[0];
// 			while (current_cmd->args[i + 1])
// 			{
// 				current_cmd->args[i] = current_cmd->args[i + 1];
// 				i++;
// 			}
// 			current_cmd->args[i] = NULL;
// 			free(empty_arg_to_free);
// 		}
// 		current_cmd = current_cmd->next;
// 	}
// }#include "inc/minishell.h"

// void	handle_sigint_prompt(int sig)
// {
// 	(void)sig;
// 	write(STDOUT_FILENO, "\n", 1);
// 	rl_on_new_line();
// 	rl_replace_line("", 0);
// 	rl_redisplay();
// 	g_exit_status = 130;
// }

// void	handle_sigint_command(int sig)
// {
// 	(void)sig;
// 	write(STDOUT_FILENO, "\n", 1);
// 	g_exit_status = 130;
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
// 	new = malloc(len + 2);
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
// 	if (ft_strchr(cmd, '/'))
// 	{
// 		if (access(cmd, X_OK) == 0)
// 			return (ft_strdup(cmd));
// 	}
// 	return (NULL);
// }

// char	*get_exec_path(char *cmd, t_env *env)
// {
// 	char		*path_value;
// 	char		**path_dirs;
// 	char		*exec_path;
// 	struct stat	statbuf;

// 	if (!cmd || cmd[0] == '\0')
// 		return (NULL);
// 	if (!env)
// 		return (NULL);
// 	if (ft_strchr(cmd, '/'))
// 	{
// 		if (stat(cmd, &statbuf) == 0)
// 			return (ft_strdup(cmd));
// 		else
// 			return (NULL);
// 	}
// 	path_value = get_env_value("PATH", env);
// 	if (!path_value || path_value[0] == '\0')
// 		return (NULL);
// 	path_dirs = ft_split(path_value, ':');
// 	if (!path_dirs)
// 		return (NULL);
// 	exec_path = search_in_path_dirs(cmd, path_dirs);
// 	ft_free_split(path_dirs);
// 	return (exec_path);
// }

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
// 			dir = ".";
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

// //moje, usunelam tylko 405 w tym - doloyc  powrotem chyba
// // //poniej proba
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
// 	rl_clear_history(); //usuniete 405
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
// #include "../inc/minishell.h"

// // // Heredoc core functionality
// // bool read_until_delimiter(char *delimiter, int write_fd);
// // int handle_heredoc(char *delimiter);
// // int process_heredoc_redirection(t_redir *redir);
// // int handle_all_heredocs(t_cmd *cmds);
// // void close_heredoc_fds(t_cmd *cmds);

// /////podzielony handle_heredoc - read until delimeter and handle heredoc
// // brak leakow, tester dziala
// bool	read_until_delimiter(char *delimiter, int write_fd)
// {
// 	char	*line;

// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (!line)
// 		{
// 			ft_putstr_fd("minishell: warning: here-document "
// 				"delimited by end-of-file (wanted `", STDERR_FILENO);
// 			ft_putstr_fd(delimiter, STDERR_FILENO);
// 			ft_putstr_fd("')\n", STDERR_FILENO);
// 			return (false);
// 		}
// 		if (ft_strcmp(line, delimiter) == 0)
// 		{
// 			free(line);
// 			return (true);
// 		}
// 		write(write_fd, line, ft_strlen(line));
// 		write(write_fd, "\n", 1);
// 		free(line);
// 	}
// }

// int	handle_heredoc(char *delimiter)
// {
// 	int	pipe_fd[2];
// 	int	read_fd;

// 	if (create_heredoc_pipe(pipe_fd) < 0)
// 		return (-1);
// 	read_fd = pipe_fd[0];
// 	if (read_until_delimiter(delimiter, pipe_fd[1]))
// 	{
// 		close(pipe_fd[1]);
// 		return (read_fd);
// 	}
// 	close(pipe_fd[1]);
// 	close(read_fd);
// 	return (-1);
// }

// int	process_heredoc_redirection(t_redir *redir)
// {
// 	int	fd;

// 	if (redir->heredoc_fd != -1)
// 	{
// 		close(redir->heredoc_fd);
// 		redir->heredoc_fd = -1;
// 	}
// 	fd = handle_heredoc(redir->file);
// 	if (fd < 0)
// 	{
// 		g_exit_status = 1;
// 		return (-1);
// 	}
// 	redir->heredoc_fd = fd;
// 	return (0);
// }

// int	handle_all_heredocs(t_cmd *cmds)
// {
// 	t_cmd	*cmd;
// 	t_redir	*redir;

// 	cmd = cmds;
// 	while (cmd)
// 	{
// 		redir = cmd->redirections;
// 		while (redir)
// 		{
// 			if (redir->type == T_HEREDOC
// 				&& process_heredoc_redirection(redir) < 0)
// 				return (-1);
// 			redir = redir->next;
// 		}
// 		cmd = cmd->next;
// 	}
// 	return (0);
// }

// void	close_heredoc_fds(t_cmd *cmds)
// {
// 	t_cmd	*current_cmd;
// 	t_redir	*current_redir;

// 	current_cmd = cmds;
// 	while (current_cmd)
// 	{
// 		current_redir = current_cmd->redirections;
// 		while (current_redir)
// 		{
// 			if (current_redir->type == T_HEREDOC
// 				&& current_redir->heredoc_fd != -1)
// 			{
// 				close(current_redir->heredoc_fd);
// 				current_redir->heredoc_fd = -1;
// 			}
// 			current_redir = current_redir->next;
// 		}
// 		current_cmd = current_cmd->next;
// 	}
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

// // Redirection core functionality
// // int validate_redirection(t_redir *redir);
// // int handle_redirection_type(t_redir *redir);
// // int process_single_redirection(t_redir *redir);
// // int setup_redirections(t_redir *redir_list);
// // void restore_std_fds(int stdin_copy, int stdout_copy);

// void	restore_std_fds(int stdin_copy, int stdout_copy)
// {
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

// // Helper functions
// // int create_heredoc_pipe(int pipe_fd[2]);
// // int handle_heredoc2(t_redir *redir);
// // void print_redirection_error(t_redir *redir);
// int	create_heredoc_pipe(int pipe_fd[2])
// {
// 	if (pipe(pipe_fd) == -1)
// 	{
// 		perror("minishell: pipe");
// 		return (-1);
// 	}
// 	return (0);
// }

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

// ///dla main_utils2.c
// typedef struct s_input_state {
// 	char	*input;
// 	int		*i;
// }	t_input_state;

// typedef struct s_append_info {
// 	int	start;
// 	int	len;
// }	t_append_info;

// typedef struct s_quote_indices {
// 	int	start;
// 	int	end_pos;
// }	t_quote_indices;

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
// int	init_execution(t_cmd *cmds, int *stdin_copy, int *stdout_copy);
// void	cleanup_execution(int stdin_copy, int stdout_copy);
// // void	wait_for_children(void);
// void	wait_for_children(pid_t last_cmd_pid);

// // pipes.c
// void	prepare_pipeline_execution(t_cmd *cmds, int *prev_pipe_read);
// void	execute_pipeline(t_cmd *cmds, t_env **env, int *prev_pipe_read);
// pid_t	process_single_command(t_cmd *cmd, t_env **env, int *prev_pipe_read, pid_t last_pid);
// void	finalize_pipeline(int *prev_pipe_read, pid_t last_pid);

// // pipes_utils.c
// void	cleanup_pipes(int *pipe_fd, int *prev_pipe_read);
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

// // handle_quoted_append.c - from main_utils2.c
// int	extract_and_append_content(t_append_info *info,
// 		t_input_state *input_state, char **current_word, int *error);
// int	prepare_and_execute_append(t_quote_indices *indices,
// 		t_input_state *input_state, char **current_word, int *error);
// int	find_and_update_for_quote(t_input_state *input_state, int *error,
// 	t_parse_state *state, t_quote_indices *indices_out);
// int	append_and_advance_for_quote(t_input_state *input_state,
// 	char **current_word, int *error, t_quote_indices *indices);
// int	handle_quoted_append(t_input_state *input_state, char **current_word,
// 		int *error, t_parse_state *state);



// //handle_quoted_append_utils.c
// int	ensure_word_allocated(char **current_word, int *error);
// int	find_closing_quote(t_input_state *input_state, char quote_char);

// // expander_utils.c
// int	is_special_var(char *str, int i);
// int	is_braced_var(char *str, int i);
// int	is_standard_var(char *str, int i);
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
// char	*read_quoted_content(char *input, int *i, t_quote quote, int *error);
// void	free_tokens(t_token *tokens);

// // lexer_utils3.c
// void	skip_space(int *i, char **word, t_token **tokens);
// void	handle_pipe(int *i, char **word, t_token **tokens);

// void	append_char_to_word(char **word, char c);
// t_token	*handle_unexpected_quote(char *word, t_token *tokens);

// // lexer_utils4.c
// void	handle_redirection(char *input, int *i, t_token **tokens);

// // main_utils.c
// void	free_env(t_env *env);
// void	init_shell(char **envp, t_env **env);
// void	handle_eof(int exit_status);
// //void	process_input(char *input, t_env **env);
// void	shift_empty_args(t_cmd *cmds);

// //process_input.c
// void process_input(char *input, t_env **env);
// bool initialize_and_tokenize(char *input, t_token **tokens);
// bool prepare_commands(t_token *tokens, t_cmd **cmds, t_resources *res);
// void execute_commands(t_cmd *cmds, t_env **env, t_resources *res);


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

// // heredoc.c
// bool read_until_delimiter(char *delimiter, int write_fd);
// int handle_heredoc(char *delimiter);
// int process_heredoc_redirection(t_redir *redir);
// int handle_all_heredocs(t_cmd *cmds);
// void close_heredoc_fds(t_cmd *cmds);

// // input_output_append.c
// int	handle_input_redir(char *file);
// int	handle_output_redir(char *file);
// int	handle_append_redir(char *file);

// // redirs_execute.c
// int validate_redirection(t_redir *redir);
// int handle_redirection_type(t_redir *redir);
// int process_single_redirection(t_redir *redir);
// int setup_redirections(t_redir *redir_list);
// void restore_std_fds(int stdin_copy, int stdout_copy);


// // redirs_utils.c
// int create_heredoc_pipe(int pipe_fd[2]);
// int handle_heredoc2(t_redir *redir);
// void print_redirection_error(t_redir *redir);



// /////DELETED CAUSE THEY WERE NOT USED
// //process_command(t_cmd *cmd, t_env **env, int prev_pipe_read, int *pipe_fd)
// //append_char(char **result, char c) (from expander_utils.c)
// // void	handle_input_char(char *input, int *i, char **current_word,
// // 	t_token **tokens); from lexec.c
// //handle_quote(char *input, int *i, t_token **tokens, int *error) (from lexer_utils2.c)
// //void	handle_redirect_token(char *input, int *i, char **word,
// //	t_token **tokens);   was only used in inadle_input_char
// //int	handle_quoted(char *input, int *i, t_token **tokens, 
// //     int *error); was only used in handle_input_char
// //void	validate_redirection_syntax(t_token **tokens); was not used anywhere
// //handle_quoted_token(char *input, int *i, t_token **tokens, int *error) (from main_utils.c section) -- not used anywhere
// // void	clean_cmds_tokens_input(t_cmd *cmds, t_token *tokens, char *input);
// //from main_utils.c - not used anywhere
// //cleanup_all_resources(t_resources *res, int *fds_to_close, int fd_count) (from utils2.c section)
// //was not usedanywhere
// //process_heredoc_line(int write_fd, char *line) (from heredoc.c)
// //not used anywhere
// //handle_heredoc_redir(char *delimiter) (from heredoc.c) not used anywhere

// #endif
