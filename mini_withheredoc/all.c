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
// 		free_cmds(cmds);
// 		free_env(*env);
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

// void	handle_child_process(t_cmd *cmd, t_env **env, int prev_pipe_read,
// 		int *pipe_fd)
// {
// 	t_resources	res;

// 	res.env = *env;
// 	res.cmds = cmd;
// 	res.tokens = NULL; // Tokens are not used here
// 	res.input = NULL;  // Input is not used here
// 	signal(SIGINT, SIG_DFL);
// 	signal(SIGQUIT, SIG_DFL);
// 	if (prev_pipe_read != -1)
// 	{
// 		dup2(prev_pipe_read, STDIN_FILENO);
// 		close(prev_pipe_read);
// 	}
// 	if (cmd->next)
// 		dup2(pipe_fd[1], STDOUT_FILENO);
// 	if (pipe_fd[0] != -1)
// 		close(pipe_fd[0]);
// 	if (pipe_fd[1] != -1)
// 		close(pipe_fd[1]);
// 	if (setup_redirections(cmd->redirections) < 0)
// 	{
// 		free_cmds(cmd);
// 		free_env(*env);
// 		exit(1);
// 	}
// 	if (is_builtin(cmd->args[0]))
// 		exit(run_builtin(cmd, &res)); // Pass res instead of env
// 	else
// 		execute_external(cmd, *env);
// }

// void	execute_external(t_cmd *cmd, t_env *env)
// {
// 	char *path = NULL;
// 	char **env_arr = NULL;

// 	path = get_exec_path(cmd->args[0], env);
// 	env_arr = env_to_arr(env);

// 	if (!path)
// 	{
// 		// get_exec_path couldn't find it (either not in PATH or explicit path didn't exist)
// 		ft_putstr_fd("minishell: ", STDERR_FILENO);
// 		ft_putstr_fd(cmd->args[0], STDERR_FILENO);
// 		// Check if original command contained '/' to mimic bash message difference
// 		if (ft_strchr(cmd->args[0], '/'))
// 		{
// 			ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
// 		}
// 		else
// 		{
// 			ft_putstr_fd(": command not found\n", STDERR_FILENO);
// 		}
// 		ft_free_split(env_arr);
// 		// Don't free path, it's NULL
// 		exit(127);
// 	}

// 	// We have a path, attempt execution
// 	execve(path, cmd->args, env_arr);

// 	// ---- execve failed if we reach here ----
// 	int saved_errno = errno;
// 	struct stat statbuf;

// 	ft_putstr_fd("minishell: ", STDERR_FILENO);
// 	ft_putstr_fd(cmd->args[0], STDERR_FILENO);
// 		// Report error using the original command name
// 	ft_putstr_fd(": ", STDERR_FILENO);

// 	if (saved_errno == ENOENT)
// 	{
// 		// This case might be less likely now if get_exec_path uses stat,
// 		// but keep for robustness / edge cases.
// 		ft_putstr_fd("No such file or directory\n", STDERR_FILENO);
// 		ft_free_split(env_arr);
// 		free(path);
// 		exit(127);
// 	}
// 	else if (saved_errno == EACCES)
// 	{
// 		// Check if it's a directory first
// 		if (stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
// 		{
// 			ft_putstr_fd("Is a directory\n", STDERR_FILENO);
// 		}
// 		else
// 		{
// 			ft_putstr_fd("Permission denied\n", STDERR_FILENO);
// 		}
// 		ft_free_split(env_arr);
// 		free(path);
// 		exit(126);
// 	}
// 	// Add other specific errno checks if needed (e.g.,
// 	//	ENOEXEC for script format error)
// 	else
// 	{
// 		// Generic fallback
// 		ft_putstr_fd(strerror(saved_errno), STDERR_FILENO);
// 		ft_putstr_fd("\n", STDERR_FILENO);
// 		ft_free_split(env_arr);
// 		free(path);
// 		exit(126); // 126 for "command invoked cannot execute" type errors
// 	}
// }#include "inc/minishell.h"


// void	cleanup(t_resources *res)
// {
// 	if (res->input)
// 		free(res->input);
// 	if (res->tokens)
// 		free_tokens(res->tokens);
// 	if (res->cmds)
// 		free_cmds(res->cmds);
// 	if (res->env)
// 		free_env(res->env);

// 	// Clear readline history
// 	rl_clear_history();
// }
// // //moje
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
// // puste ify - usunac czy przydadza sie na
// // usuniecie malo czytelnych w innych funkcjach?
// void	wait_for_children(pid_t last_cmd_pid)
// {
// 	int status;
// 	pid_t pid;
// 	int last_cmd_status = 0; // Store the status of the specific last command
// 	int found_last = 0;
// 	// Flag to track if we found the last command's status

// 	// If last_cmd_pid is 0 or -1 (e.g., fork failed),
// 	//	default status might be appropriate
// 	if (last_cmd_pid <= 0)
// 	{
// 		// Decide on default behavior, maybe keep g_exit_status as is,
// 		//	or set to error?
// 		// Setting to 1 if last_cmd_pid indicates failure might be reasonable.
// 		// For now,
// 		//	we'll let it default based on what was last set if no children are waited for.
// 		// Or, if fork failed, an error might have already been set.
// 	}

// 	while ((pid = waitpid(-1, &status, 0)) > 0) // Wait for ANY child
// 	{
// 		if (pid == last_cmd_pid)
// 		// Is this the specific last command we care about?
// 		{
// 			found_last = 1; // Mark that we found it
// 			if (WIFEXITED(status))
// 			{
// 				last_cmd_status = WEXITSTATUS(status); // Get its exit status
// 			}
// 			else if (WIFSIGNALED(status))
// 			{
// 				int signal_num = WTERMSIG(status);
// 				last_cmd_status = 128 + signal_num;
// 				// Calculate signal exit status

// 				// Optional: Print signal messages like bash
// 				if (signal_num == SIGQUIT)
// 				{
// 					// Bash prints "Quit (core dumped)" - adjust as needed
// 					ft_putstr_fd("Quit\n", STDERR_FILENO);
// 				}
// 				else if (signal_num == SIGSEGV)
// 				{
// 					ft_putstr_fd("Segmentation fault\n", STDERR_FILENO);
// 				}
// 				// SIGINT is handled by the signal handler setting g_exit_status,
// 				// but waitpid will still report it here. last_cmd_status will be 130.
// 			}
// 			// No need to set g_exit_status here inside the loop
// 		}
// 		// We continue waiting even after finding the last command to ensure
// 		// all children in the pipeline are reaped and no zombies are left.
// 	}

// 	// After waiting for all children, set the global exit status
// 	// ONLY if we actually successfully waited for the last command.
// 	// If last_cmd_pid was valid (>0) but we never found it (waitpid error?),
// 	// maybe indicate an error. For now, we only set if found.
// 	if (last_cmd_pid > 0 && found_last)
// 	{
// 		g_exit_status = last_cmd_status;
// 	}
// 	else if (last_cmd_pid <= 0)
// 	{
// 		// Handle cases where there was no pipeline / fork failed for last cmd.
// 		// Perhaps g_exit_status was already set by a builtin or init_execution failure.
// 		// If no children were waited for at all (waitpid returned
// 		//	-1 immediately with ECHILD),
// 		// g_exit_status should reflect whatever happened before the wait loop.
// 		// So, maybe do nothing here if last_cmd_pid was invalid/not found.
// 	}

// 	// Error handling for waitpid itself
// 	if (pid == -1 && errno != ECHILD)
// 	{
// 		perror("minishell: waitpid");
// 		// Optionally set g_exit_status to indicate a wait error
// 		// g_exit_status = 1;
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

// int	handle_braced_var(char **result, char *str, int i, t_env *env)
// {
// 	int start;
// 	char *var_name;
// 	char *var_value;

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
// 	char *var_name;
// 	char *var_value;

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

// char	*expand_str(char *str, t_env *env)
// {
// 	char *result;
// 	int i;

// 	result = ft_strdup("");
// 	if (!result)
// 		return (NULL); // bylo samo return null
// 	i = 0;
// 	while (str[i])
// 	{
// 		if (is_special_var(str, i))
// 			i = handle_special_var(&result, i);
// 		else if (is_braced_var(str, i))
// 			i = handle_braced_var(&result, str, i, env);
// 		else if (is_standard_var(str, i))
// 			i = handle_standard_var(&result, str, i, env);
// 		else
// 		{
// 			char *old_result = result;
// 			result = ft_strjoin_char(old_result, str[i]);
// 			if (!result)
// 			{
// 				free(old_result);
// 				return (NULL);
// 			}
// 			i++;
// 		}
// 	}
// 	return (result);
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

// int	handle_special_var(char **result, int i)
// {
// 	char *exit_status_str = ft_itoa(g_exit_status);
// 	if (!exit_status_str)
// 		return (-1); // Error handling

// 	char *new_result = ft_strjoin_free(*result, exit_status_str);
// 	free(exit_status_str); // Free the string after using it

// 	if (!new_result)
// 		return (-1);

// 	*result = new_result;
// 	return (i + 2);
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

// void	free_tokens(t_token *tokens)
// {
// 	t_token *tmp;

// 	while (tokens)
// 	{
// 		tmp = tokens;
// 		if (tokens->value)
// 			free(tokens->value); // <<< Free the string!
// 		tokens = tokens->next;
// 		free(tmp);
// 	}
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

// void	free_env(t_env *env)
// {
// 	t_env	*tmp;

// 	while (env)
// 	{
// 		tmp = env->next; // Save next pointer before freeing
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
// 	// Don't free env here, it's already freed in cleanup
// 	// free_env(env);  <- Remove this line
// 	// Clear readline history
// 	rl_clear_history();
// 	// Print a newline for better UX when exiting with Ctrl+D
// 	ft_putstr_fd("exit\n", STDOUT_FILENO);
// 	exit(exit_status);
// }


// // //moje
// // void	process_input(char *input, t_env **env)
// // {
// // 	t_token	*tokens;
// // 	t_cmd	*cmds;

// // 	tokens = NULL;
// // 	cmds = NULL;
// // 	t_fds fds;             // For saving/restoring std fds for parent builtins
// // 	t_resources res = {0}; // Resources for cleanup (esp. for bi_exit)
// // 	// --- Setup Resources ---
// // 	res.input = input; // Track original input pointer (though we free it here)
// // 	res.env = *env;    // Track environment pointer
// // 	// --- Tokenize ---
// // 	tokens = tokenize_input(input);
// // 	if (!tokens)
// // 	{
// // 		free(input); // Free original input if tokenization fails
// // 		// g_exit_status might be set by tokenizer on syntax error
// // 		return ;
// // 	}
// // 	res.tokens = tokens; // Track tokens
// // 	// --- Expand Variables ---
// // 	expand_variables(tokens, *env);
// // 	// --- Parse Tokens into Commands ---
// // 	cmds = parse_tokens(tokens);
// // 	if (!cmds)
// // 	{
// // 		// Parsing failed (syntax error should set g_exit_status)
// // 		free_tokens(tokens);
// // 		free(input);
// // 		return ;
// // 	}
// // 	res.cmds = cmds; // Track commands
// // 	// --- Handle Argument Shifting for Empty Command Names (like $EMPTY cmd) ---
// // 	shift_empty_args(cmds); // Modify cmds->args in place if needed
// // 	// --- Handle Commands that are NOW Empty (like $EMPTY alone,
// // 	//	or after shift) ---
// // 	// Check if the *first* command is now effectively empty (no args[0] or args[0] is "")
// // 	// Note: shift_empty_args ensures args[0] isn't "" if args[1] existed.
// // 	// This check catches "$EMPTY" alone or syntax resulting in no args.
// // 	if (!cmds->args || !cmds->args[0] || cmds->args[0][0] == '\0')
// // 	{
// // 		// Check specifically for the "$EMPTY" alone case (single command,
// // 		//	now empty)
// // 		if (!cmds->next && (!cmds->args || !cmds->args[0]
// // 				|| cmds->args[0][0] == '\0'))
// // 		{
// // 			g_exit_status = 0; // Bash exits 0 for just "" or $EMPTY
// // 		}
// // 		// Otherwise, if parsing/shifting resulted in no command (e.g.,
// // 		//	" > file "),
// // 		// let the exit status remain or set to an error? Bash might set 0 or error.
// // 		// For simplicity, we might just let it be 0 or the status from parsing.
// // 		// Cleanup and return, skipping execution
// // 		free_cmds(cmds);
// // 		free_tokens(tokens);
// // 		free(input);
// // 		return ;
// // 	}
// // 	// --- End Empty Command Check ---
// // 	// --- Proceed with Execution ---
// // 	// At this point, cmds, cmds->args,
// // 	//	and cmds->args[0] should be valid and non-empty.
// // 	setup_signals_for_command(); // Set SIGINT/SIGQUIT handlers for execution
// // 	// Check if it's a single command that is a builtin
// // 	if (is_builtin(cmds->args[0]) && !cmds->next)
// // 	{
// // 		// Run single builtin in parent process
// // 		fds.stdin = dup(STDIN_FILENO); // Backup std fds
// // 		fds.stdout = dup(STDOUT_FILENO);
// // 		// Pass full resources in case the builtin (like exit) needs to cleanup everything
// // 		handle_parent_builtin(cmds, fds, &res);
// // 		// handle_parent_builtin restores fds and sets g_exit_status
// // 		// If bi_exit was called, we won't get here.
// // 	}
// // 	// Otherwise, it's an external command, a pipeline,
// // 	// or a builtin in a pipeline
// // 	else
// // 	{
// // 		// Execute external command(s) or pipeline (forks involved)
// // 		execute(cmds, env); // execute() handles waiting and sets g_exit_status
// // 	}
// // 	setup_signals_for_prompt(); // Reset signal handlers for the next prompt
// // 	// --- Cleanup for the current input ---
// // 	// This cleanup runs unless bi_exit was called.
// // 	free_cmds(cmds);
// // 	free_tokens(tokens);
// // 	free(input); // Free the original readline input string
// // 					// Note: 'env' persists across inputs. 'res' is cleaned up if bi_exit called.
// // }


// void process_input(char *input, t_env **env)
// {
//     t_token     *tokens;
//     t_cmd       *cmds;
//     t_fds       fds;
//     t_resources res = {0};

//     // --- Setup Resources ---
//     res.input = input;
//     res.env = *env;

//     // --- Tokenize ---
//     tokens = tokenize_input(input);
//     if (!tokens) { /* ... existing error handling ... */ return; }
//     res.tokens = tokens;

//     // --- Expand Variables ---
//     expand_variables(tokens, *env);

//     // --- Parse Tokens into Commands ---
//     cmds = parse_tokens(tokens);
//     if (!cmds) { /* ... existing error handling ... */ return; }
//     res.cmds = cmds;

//     // --- Handle Argument Shifting ---
//     shift_empty_args(cmds);

//     // --- Handle Empty Commands ---
//     if (!cmds->args || !cmds->args[0] || cmds->args[0][0] == '\0') {
//         /* ... existing empty command handling ... */
//         // Make sure to free resources before returning
//         free_cmds(cmds);
//         free_tokens(tokens);
//         free(input);
//         return;
//     }

//     // --- >>> NEW: Handle Heredocs <<< ---
//     if (handle_all_heredocs(cmds) != 0) {
//         // Heredoc failed, error message/warning printed inside handle_heredoc
//         // g_exit_status is set inside handle_all_heredocs or handle_heredoc
//         close_heredoc_fds(cmds); // Close any FDs that might have been opened before failure
//         free_cmds(cmds);
//         free_tokens(tokens);
//         free(input);
//         return; // Skip execution
//     }
//     // --- >>> END NEW <<< ---

//     // --- Proceed with Execution ---
//     setup_signals_for_command();

//     if (is_builtin(cmds->args[0]) && !cmds->next) {
//         fds.stdin = dup(STDIN_FILENO);
//         fds.stdout = dup(STDOUT_FILENO);
//         // Pass full resources in case of exit
//         handle_parent_builtin(cmds, fds, &res); // Note: exit() won't return
//         // handle_parent_builtin now handles redirection setup internally
//     } else {
//         execute(cmds, env); // Handles waiting and sets g_exit_status
//     }

//     setup_signals_for_prompt(); // Reset signal handlers

//     // --- >>> NEW: Close Heredoc FDs in Parent <<< ---
//     close_heredoc_fds(cmds);
//     // --- >>> END NEW <<< ---


//     // --- Cleanup for the current input ---
//     // Don't call cleanup(&res) here unless exit was called,
//     // otherwise env will be freed.
//     // cleanup is primarily for bi_exit.
//     free_cmds(cmds);
//     free_tokens(tokens);
//     free(input);
// }








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
// // //moje
// // void	handle_redirection_parser(t_token **tokens, t_cmd *cmd)
// // {
// // 	t_redir	*new_redir;

// // 	new_redir = malloc(sizeof(t_redir));
// // 	if (!new_redir)
// // 	{
// // 		perror("minishell");
// // 		return ;
// // 	}
// // 	// Set redirection type based on token
// // 	new_redir->type = (*tokens)->type;
// // 	// Move to next token (should be filename)
// // 	*tokens = (*tokens)->next;
// // 	if (!*tokens || ((*tokens)->type != T_WORD
// // 			&& (*tokens)->type != T_SINGLE_QUOTED
// // 			&& (*tokens)->type != T_DOUBLE_QUOTED))
// // 	{
// // 		ft_putstr_fd("minishell: syntax error near unexpected token\n",
// // 			STDERR_FILENO);
// // 		free(new_redir);
// // 		return ;
// // 	}
// // 	// Assign filename and add redirection to command
// // 	new_redir->file = ft_strdup((*tokens)->value);
// // 	new_redir->next = NULL;
// // 	add_redir_to_cmd(cmd, new_redir);
// // 	// Move to next token
// // 	*tokens = (*tokens)->next;
// // }

// void handle_redirection_parser(t_token **tokens, t_cmd *cmd)
// {
//     t_redir *new_redir;

//     new_redir = malloc(sizeof(t_redir));
//     if (!new_redir)
//     {
//         perror("minishell: malloc");
//         // Consider more robust error handling (e.g., freeing existing cmd structure)
//         return; // Or exit, or set an error flag
//     }
//     new_redir->type = (*tokens)->type;
//     new_redir->heredoc_fd = -1; // <<< Initialize here

//     // Move to next token (should be filename/delimiter)
//     *tokens = (*tokens)->next;
//     if (!*tokens || ((*tokens)->type != T_WORD && (*tokens)->type != T_SINGLE_QUOTED &&
//                       (*tokens)->type != T_DOUBLE_QUOTED))
//     {
//         ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
//         // Print the unexpected token if possible, or just a newline
//          if (*tokens && (*tokens)->value) {
//              ft_putstr_fd((*tokens)->value, STDERR_FILENO);
//              ft_putstr_fd("'\n", STDERR_FILENO);
//          } else {
//             ft_putstr_fd("newline'\n", STDERR_FILENO);
//          }
//         g_exit_status = 2; // Syntax error
//         free(new_redir);
//         // Signal error back up the call chain if possible
//         return;
//     }
//     // Assign filename/delimiter
//     new_redir->file = ft_strdup((*tokens)->value);
//      if (!new_redir->file) { // Handle strdup failure
//         perror("minishell: malloc");
//         free(new_redir);
//         return; // Or signal error
//     }
//     new_redir->next = NULL;
//     add_redir_to_cmd(cmd, new_redir);
//     // Move to next token
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
// 		return (-1); // Do not print error message here
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
// 		return (-1); // Do not print error message here
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
// 		return (-1); // Do not print error message here
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
// // ////dziala dla naszych testow ale nie heredoc
// // //moje
// // int	process_single_redirection(t_redir *redir)
// // {
// // 	int	status;

// // 	int saved_errno; // Variable to store errno immediately after failure
// // 	// Check for missing filename (syntax error)
// // 	if (!redir || !redir->file || redir->file[0] == '\0')
// // 	{
// // 		ft_putstr_fd("minishell: syntax error: "
// // 						"missing filename after redirection\n",
// // 						STDERR_FILENO);
// // 		g_exit_status = 2; // Syntax errors often have exit code 2 in bash
// // 		return (-1);
// // 	}
// // 	status = 0; // Initialize status
// // 	// --- Perform the specific redirection ---
// // 	// These handle_* functions should return -1 on error and set errno.
// // 	if (redir->type == T_REDIR_IN)
// // 		status = handle_input_redir(redir->file);
// // 	else if (redir->type == T_REDIR_OUT)
// // 		status = handle_output_redir(redir->file);
// // 	else if (redir->type == T_APPEND)
// // 		status = handle_append_redir(redir->file);
// // 	else if (redir->type == T_HEREDOC)
// // 		status = handle_heredoc_redir(redir->file);
// // 	// Add else if for other types if necessary
// // 	// --- Check for errors and print appropriate message ---
// // 	if (status < 0)
// // 	{
// // 		saved_errno = errno; // Save errno immediately after open/dup2 failure
// // 		ft_putstr_fd("minishell: ", STDERR_FILENO);
// // 		ft_putstr_fd(redir->file, STDERR_FILENO);
// // 		ft_putstr_fd(": ", STDERR_FILENO);
// // 		// Print the actual system error message
// // 		ft_putstr_fd(strerror(saved_errno), STDERR_FILENO);
// // 		ft_putstr_fd("\n", STDERR_FILENO);
// // 		g_exit_status = 1; // Redirection errors typically yield exit code 1
// // 		return (-1);       // Return error
// // 	}
// // 	return (0); // Success
// // }

// int process_single_redirection(t_redir *redir)
// {
//     int status = 0; // Initialize status to success
//     int saved_errno;

//     if (!redir || !redir->file || redir->file[0] == '\0') {
//         // This check is particularly important for non-heredoc cases
//         // For heredoc, redir->file is the delimiter and should exist
//         ft_putstr_fd("minishell: syntax error: missing filename or delimiter\n", STDERR_FILENO);
//         g_exit_status = 2;
//         return (-1);
//     }

//     // --- Perform the specific redirection ---
//     if (redir->type == T_REDIR_IN)
//         status = handle_input_redir(redir->file);
//     else if (redir->type == T_REDIR_OUT)
//         status = handle_output_redir(redir->file);
//     else if (redir->type == T_APPEND)
//         status = handle_append_redir(redir->file);
//     else if (redir->type == T_HEREDOC)
//     {
//         // Heredoc already processed, use the stored FD
//         if (redir->heredoc_fd < 0) {
//             // Should not happen if handle_all_heredocs succeeded
//             ft_putstr_fd("minishell: internal error: invalid heredoc fd for ", STDERR_FILENO);
//              ft_putstr_fd(redir->file, STDERR_FILENO); // Print delimiter for context
//              ft_putstr_fd("\n", STDERR_FILENO);
//             status = -1; // Signal internal error
//         } else {
//              // Duplicate the read end of the pipe to standard input
//             if (dup2(redir->heredoc_fd, STDIN_FILENO) < 0) {
//                  status = -1; // dup2 failed, errno is set
//             }
//              // *** CRUCIAL ***: Close the original heredoc FD in the child
//              //                 after dup2. We don't need it anymore.
//             close(redir->heredoc_fd);
//         }
//     }
//     // Add else if for other types if necessary

//     // --- Check for errors and print appropriate message ---
//     if (status < 0)
//     {
//         saved_errno = errno; // Save errno immediately after failure
//         ft_putstr_fd("minishell: ", STDERR_FILENO);
//         // For T_HEREDOC error (likely dup2 fail), print delimiter as context
//         // For others, print filename.
//         ft_putstr_fd(redir->file, STDERR_FILENO);
//         ft_putstr_fd(": ", STDERR_FILENO);
//         ft_putstr_fd(strerror(saved_errno), STDERR_FILENO);
//         ft_putstr_fd("\n", STDERR_FILENO);
//         g_exit_status = 1;
//         return (-1); // Return error
//     }

//     return (0); // Success
// }


// // moje, nie dziala na podwojego heredoca
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


// // // //with debugging steps
// // int	setup_redirections(t_redir *redir_list)
// // {
// // 	int	status;

// // 	printf("DEBUG: Entering setup_redirections\n");
// // 	while (redir_list)
// // 	{
// // 		printf("DEBUG: Processing redirection type %d, file/delim '%s'\n",
// // 			redir_list->type, redir_list->file);
// // 		status = process_single_redirection(redir_list);
// // 		printf("DEBUG: process_single_redirection returned %d\n", status);
// // 		if (status < 0)
// // 		{
// // 			printf("DEBUG: Error detected, returning -1\n");
// // 			return (-1);
// // 		}
// // 		redir_list = redir_list->next;
// // 	}
// // 	printf("DEBUG: Exiting setup_redirections successfully\n");
// // 	return (0);
// // }


// // minishell$ cat <<HERE <<DOC
// // DEBUG: Entering setup_redirections
// // DEBUG: Processing redirection type 5, file/delim 'HERE'  // First heredoc
// // > oi                                                    // Input for HERE
// // > HERE                                                  // Delimiter for HERE
// // DEBUG: process_single_redirection returned 0            // First heredoc OK
// // DEBUG: Processing redirection type 5, file/delim 'DOC'  // Second heredoc
// // > oi                                                    // Input for DOC (You typed 'oi' again)
// // >                                                       // Input for DOC (You typed ENTER - empty line)
// // DEBUG: process_single_redirection returned 0            // Second heredoc *claims* OK (Incorrect behavior!)
// // DEBUG: Exiting setup_redirections successfully
// // oi                                                      // Output of cat
// // minishell$#include "../inc/minishell.h"

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
// //MOJE
// // int	bi_cd(char **args, t_env **env)
// // {
// // 	char	*old_pwd;
// // 	int		special_case_result;

// // 	old_pwd = getcwd(NULL, 0);
// // 	if (!old_pwd)
// // 	{
// // 		if (get_env_value("PWD", *env))
// // 			old_pwd = ft_strdup(get_env_value("PWD", *env));
// // 		else
// // 			old_pwd = ft_strdup("");
// // 	}
// // 	if (!args[1])
// // 		return (handle_no_args(env, &old_pwd));
// // 	special_case_result = process_special_case(args[1], env, &old_pwd);
// // 	if (special_case_result != CONTINUE)
// // 	{
// // 		free(old_pwd); //dodane valgrind
// // 		return (special_case_result);
// // 	}
// // 	return (process_path_argument(args, env, &old_pwd));
// // }

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
//     if (!old_pwd)
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
// 		free(old_pwd); //dodane valgrind
// 		return (special_case_result);
// 	}
// 	return (process_path_argument(args, env, &old_pwd));
// }






// // //NOWE
// // int	bi_cd(char **args, t_env **env)
// // {
// //     char	*old_pwd;
// //     int		special_case_result;

// //     old_pwd = getcwd(NULL, 0);
// //     if (!old_pwd)
// //     {
// //         if (get_env_value("PWD", *env))
// //             old_pwd = ft_strdup(get_env_value("PWD", *env));
// //         else
// //             old_pwd = ft_strdup("");
// //         if (!old_pwd) // Handle strdup failure
// //             return (ERROR);
// //     }
// //     if (!args[1])
// //     {
// //         int result = handle_no_args(env, &old_pwd);
// //         free(old_pwd); // Free old_pwd after handling no args
// //         return (result);
// //     }
// //     special_case_result = process_special_case(args[1], env, &old_pwd);
// //     if (special_case_result != CONTINUE)
// //     {
// //         free(old_pwd); // Free old_pwd if special case fails
// //         return (special_case_result);
// //     }
// //     return (process_path_argument(args, env, &old_pwd));
// // }
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
// 	// Make a copy of the string instead of storing the direct pointer
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
// 	// Make a copy of the string
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
// 	// Make a copy of old_pwd for OLDPWD
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
// 	// Always free the original old_pwd
// 	free(old_pwd);
// 	return (SUCCESS);
// }
// #include "../inc/minishell.h"

// int	bi_echo(char *argv[])
// {
// 	int	i;
// 	int	newline;
// 	int	first_arg;

// 	// Parse options and get starting index
// 	i = handle_n_option(argv, &newline);
// 	// Print arguments
// 	first_arg = 1;
// 	while (argv[i])
// 	{
// 		if (!first_arg)
// 			ft_putchar_fd(' ', STDOUT_FILENO);
// 		ft_putstr_fd(argv[i], STDOUT_FILENO);
// 		first_arg = 0;
// 		i++;
// 	}
// 	// Print newline if needed
// 	if (newline)
// 		ft_putchar_fd('\n', STDOUT_FILENO);
// 	return (0);
// }

// int	handle_n_option(char *argv[], int *newline_flag)
// {
// 	int	i;
// 	int	j;

// 	i = 1;
// 	*newline_flag = 1; // Default to printing newline
// 	// Handle -n option(s)
// 	while (argv[i] && ft_strncmp(argv[i], "-n", 2) == 0)
// 	{
// 		// Check if it's only "-n" or "-nnn..." without other characters
// 		j = 2;
// 		while (argv[i][j] && argv[i][j] == 'n')
// 			j++;
// 		if (argv[i][j] == '\0') // Valid -n option
// 		{
// 			*newline_flag = 0;
// 			i++;
// 		}
// 		else
// 			break ;
// 	}
// 	return (i); // Return the index where arguments start
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

// 	// Check for unsupported options/arguments
// 	if (args[1])
// 	{
// 		ft_putstr_fd("env: too many arguments\n", STDERR_FILENO);
// 		return (ERROR);
// 	}
// 	// Print all env variables
// 	current = *env;
// 	while (current)
// 	{
// 		// wypis tylko variables with values (standard env behavior)
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
// 		return (1); // Indicate error (don't exit)
// 	}
// 	if (argv[1])
// 	{
// 		if (is_integer(argv[1]))
// 		{
// 			*exit_code = ft_atoi(argv[1]);
// 			// bash calculates exit code modulo 256
// 			*exit_code %= 256;
// 			if (*exit_code < 0) // Ensure positive result
// 				*exit_code += 256;
// 		}
// 		else
// 		{
// 			write_stderr("exit: numeric argument required\n");
// 			*exit_code = 2;
// 		}
// 	}
// 	return (0); // Indicate success (should exit)
// }

// /// the last, 2 leaki
// int bi_exit(char *argv[], t_resources *res)
// {
//     int exit_code;
//     int should_exit;

//     exit_code = g_exit_status;
//     ft_putstr_fd("exit\n", STDOUT_FILENO);
//     should_exit = parse_exit_args(argv, &exit_code);
//     if (should_exit == 1)
// 		return (1);
      
//     // Clean up all resources
//     cleanup(res); // This handles res->input, tokens, cmds, env
// 	rl_clear_history(); // Clear readline history
//     exit(exit_code);
// }
// // without arguments it lists all environment variables that are marked
// // why use export? because by default, shell variables are local to the shell,
// // when you export them, you make them part of the environment,
// //	which is passed to child processes like ls, vim, gcc

// #include "../inc/minishell.h"

// // calculate the length of an environment variable's value
// size_t	env_len(t_env *env)
// {
// 	if (!env || !env->value)
// 		return (0);
// 	return (ft_strlen(env->value));
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

// //diala, leakami
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

// int	set_env_var(const char *var, t_env **env)
// {
//     char	*key;
//     char	*value;
//     t_env	*existing;

//     if (parse_env_var(var, &key, &value) == ERROR)
//     {
//         free(key); // Added for safety
//         free(value); // Added for safety
//         return (ERROR);
//     }
//     existing = get_env(key, *env);
//     if (existing)
//     {
//         if (ft_strchr(var, '='))
//         {
//             free(existing->value);
//             existing->value = value;
//         }
//         existing->is_exported = 1; // Mark as exported
//         free(key);
//     }
//     else
//     {
//         t_env *new_node = env_new(key, value);
//         if (new_node)
//             new_node->is_exported = 1; // Mark as exported
//         env_add_back(env, new_node);
//         free(key);
//         free(value);
//     }
//     return (SUCCESS);
// }

// // //moje
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



// // // //tego uywam w wieksosci
// int is_valid_identifier(const char *arg)
// {
// 	char	*name;
// 	char	*equal_pos;
// 	int		i;

// 	if (!arg || !*arg)
// 		return (0);

// 	// Get the position of '=' if present
// 	equal_pos = ft_strchr(arg, '=');

// 	// Extract the variable name (up to '=' or full string if no '=')
// 	if (equal_pos)
// 		name = ft_strndup(arg, equal_pos - arg);
// 	else
// 		name = ft_strdup(arg);

// 	if (!name)
// 		return (0);

// 	// Check first character: must be alpha or underscore
// 	if (!ft_isalpha(name[0]) && name[0] != '_')
// 	{
// 		free(name);
// 		return (0);
// 	}

// 	// Check remaining characters
// 	i = 1;
// 	while (name[i])
// 	{
// 		if (!ft_isalnum(name[i]) && name[i] != '_')
// 		{
// 			free(name);
// 			return (0);
// 		}
// 		i++;
// 	}

// 	free(name);
// 	return (1);
// }
// #include "../inc/minishell.h"
// // char cwd[PATH_MAX]; // buffer to store the current directory path
// // int	bi_pwd(char *argv[])
// // {
// // 	char	cwd[PATH_MAX];

// // 	// check if there are any arguments other than the command
// // 	if (argv[1])
// // 	{
// // 		ft_putstr_fd("pwd: too many arguments\n", STDERR_FILENO);
// // 		return (1);
// // 	}
// // 	// get the current working directory
// // 	if (getcwd(cwd, sizeof(cwd)) != NULL)
// // 	{
// // 		ft_putstr_fd(cwd, STDOUT_FILENO); // print the current directory
// // 		ft_putstr_fd("\n", STDOUT_FILENO); // print newline
// // 		return (0); // successs
// // 	}
// // 	// if getcwd fails, print the error
// // 	perror("minishell: pwd");
// // 	return (1);
// // }

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
// }#include "../inc/minishell.h"

// // check if a variable name is valid for unset
// // valid names: start with letter or underscore, then letters,
// //	numbers or underscores
// //it checks all, key and also a value after "=", 
// //wiec ponizej napisalam nowy
// // int	is_valid_identifier(char *name)
// // {
// // 	int	i;

// // 	if (!name || !*name)
// // 		return (0);
// // 	// First character must be letter or underscore
// // 	if (!ft_isalpha(name[0]) && name[0] != '_')
// // 		return (0);
// // 	// Remaining characters must be letters, digits or underscores
// // 	i = 1;
// // 	while (name[i])
// // 	{
// // 		if (!ft_isalnum(name[i]) && name[i] != '_')
// // 			return (0);
// // 		i++;
// // 	}
// // 	return (1);
// // }







// // //moje
// // int	is_valid_identifier(char *name)
// // {
// // 	int	i;
// // 	char *equals_pos;

// // 	if (!name || !*name)
// // 		return (0);

// // 	equals_pos = ft_strchr(name, '=');
// // 	//utnij jesli =
// // 	if (equals_pos)
// // 		*equals_pos = '\0';

// // 	// 1st character must be a letter or underscore
// // 	if (!ft_isalpha(name[0]) && name[0] != '_')
// // 	{
// // 		if (equals_pos)
// // 			*equals_pos = '='; // Restore '=' if we modified it
// // 		return (0);
// // 	}

// // 	// Check remaining characters (only up to '=' if it exists)
// // 	i = 1;
// // 	while (name[i] && name[i] != '=')
// // 	{
// // 		if (!ft_isalnum(name[i]) && name[i] != '_')
// // 		{
// // 				if (equals_pos)
// // 					*equals_pos = '='; // Restore '=' if modified
// // 				return (0);
// // 			}
// // 			i++;
// // 		}
		
// // 		// Restore '=' if we modified it (for further processing)
// // 		if (equals_pos)
// // 			*equals_pos = '=';
// // 		return (1);
// // }

// /**
//  * Remove an environment variable from the linked list
//  */
// // //fukcja 25 linijek bez komentarzy OK
// int	remove_env_var(char *varname, t_env **env)
// {
// 	t_env	*current;
// 	t_env	*previous;

// 	if (!env || !*env || !varname)
// 		return (ERROR);
// 	current = *env;
// 	previous = NULL;
// 	// Find the variable in the linked list
// 	while (current)
// 	{
// 		// Compare variable names exactly (not just a prefix)
// 		if (ft_strcmp(current->key, varname) == 0)
// 		{
// 			// Remove from beginning of list
// 			if (previous == NULL)
// 				*env = current->next;
// 			// Remove from middle/end of list
// 			else
// 				previous->next = current->next;
// 			// Free node memory
// 			free(current->key);
// 			free(current->value);
// 			free(current);
// 			return (SUCCESS);
// 		}
// 		previous = current;
// 		current = current->next;
// 	}
// 	return (SUCCESS); // Variable not found, not an error
// }

// /**
//  * Unset environment variables (no options supported)
//  */
// int	bi_unset(char **args, t_env **env)
// {
// 	int	i;
// 	int	status;

// 	status = SUCCESS;
// 	// No arguments is not an error
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
// 				free_env(*env); //dodane valgrind
// 		}
// 		i++;
// 	}
// 	return (status);
// }

