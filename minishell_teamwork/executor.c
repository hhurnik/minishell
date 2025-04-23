#include "inc/minishell.h"

void	restore_std_fds(int stdin_copy, int stdout_copy)
{
	dup2(stdin_copy, STDIN_FILENO);
	dup2(stdout_copy, STDOUT_FILENO);
	close(stdin_copy);
	close(stdout_copy);
}
/// poprzednia wersja dziala ale odpala 2 minishell jesli bez globalnych
// void execute(t_cmd *cmds, t_env **env)
// {
//     // Set flag that we're in a command
//     //g_in_command = 1;
    
//     int stdin_copy = dup(STDIN_FILENO);
//     int stdout_copy = dup(STDOUT_FILENO);
//     int status;
//     int prev_pipe_read = -1;

//     if (!cmds || !cmds->args || !cmds->args[0])
//     {
//         ft_putstr_fd("minishell: null command\n", STDERR_FILENO);
//         //g_in_command = 0;  // Reset the flag before returning
//         return;
//     }

//     // Builtin in parent process if no pipes
//     if (is_builtin(cmds->args[0]) && !cmds->next)
//     {
//         handle_redirections(cmds);
//         g_exit_status = run_builtin(cmds, env);
//         restore_std_fds(stdin_copy, stdout_copy);
//         //g_in_command = 0;  // Reset the flag before returning
//         return;
//     }

//     // Continue forking path (pipelines or non-builtin)
//     while (cmds)
//     {
//         int pipe_fd[2] = {-1, -1};
        
//         // Create pipe if there's a next command
//         if (cmds->next && pipe(pipe_fd) == -1)
//         {
//             perror("minishell: pipe");
//             break;
//         }

//         pid_t pid = fork();
//         if (pid == -1)
//         {
//             perror("minishell: fork");
//             if (pipe_fd[0] != -1) close(pipe_fd[0]);
//             if (pipe_fd[1] != -1) close(pipe_fd[1]);
//             break;
//         }
//         else if (pid == 0)  // Child process
//         {
//             // Set up input from previous command if needed
//             if (prev_pipe_read != -1)
//             {
//                 dup2(prev_pipe_read, STDIN_FILENO);
//                 close(prev_pipe_read);
//             }
            
//             // Set up output to next command if needed
//             if (cmds->next)
//             {
//                 dup2(pipe_fd[1], STDOUT_FILENO);
//             }
            
//             // Close unused pipe ends
//             if (pipe_fd[0] != -1) close(pipe_fd[0]);
//             if (pipe_fd[1] != -1) close(pipe_fd[1]);

//             handle_redirections(cmds);

//             // If it's a builtin, run it in the child process
//             if (is_builtin(cmds->args[0]))
//             {
//                 exit(run_builtin(cmds, env));
//             }
//             else  // External command
//             {
//                 // Ensure the environment is up to date for the child
//                 char *path = get_exec_path(cmds->args[0], *env);
//                 char **env_arr = env_to_arr(*env);

//                 // Execute the command
//                 if (path)
//                     execve(path, cmds->args, env_arr);
//                 else
//                     execvp(cmds->args[0], cmds->args);  // Try direct execution as fallback
                    
//                 perror("minishell");
//                 exit(127);  // Command not found
//             }
//         }

//         // Parent process
        
//         // Close previous pipe input if we had one
//         if (prev_pipe_read != -1)
//             close(prev_pipe_read);
        
//         // Set up for next iteration
//         if (cmds->next)
//         {
//             prev_pipe_read = pipe_fd[0];  // Save read end for next command
//             close(pipe_fd[1]);            // Close write end in parent
//         }
//         else if (pipe_fd[0] != -1)
//         {
//             // Last command, close any remaining pipe ends
//             close(pipe_fd[0]);
//         }

//         cmds = cmds->next;
//     }

//     // Wait for all child processes to finish
//     while (waitpid(-1, &status, 0) > 0)
//     {
//         if (WIFEXITED(status))
//             g_exit_status = WEXITSTATUS(status);
//     }

//     // At the end of function
//     //g_in_command = 0;  // Reset the flag
//     restore_std_fds(stdin_copy, stdout_copy);
// }


void execute(t_cmd *cmds, t_env **env)
{
    int stdin_copy = dup(STDIN_FILENO);
    int stdout_copy = dup(STDOUT_FILENO);
    int status;
    int prev_pipe_read = -1;

    if (!cmds || !cmds->args || !cmds->args[0])
    {
        ft_putstr_fd("minishell: null command\n", STDERR_FILENO);
        return;
    }

    // Handle built-in commands in the parent process if no pipes
    if (is_builtin(cmds->args[0]) && !cmds->next)
    {
        handle_redirections(cmds);
        g_exit_status = run_builtin(cmds, env);
        restore_std_fds(stdin_copy, stdout_copy);
        return;
    }

    // Continue forking path (pipelines or non-builtin)
    while (cmds)
    {
        setup_signals_for_command(); // Set signal handling for command execution

        int pipe_fd[2] = {-1, -1};
        if (cmds->next && pipe(pipe_fd) == -1)
        {
            perror("minishell: pipe");
            break;
        }

        pid_t pid = fork();
        if (pid == -1)
        {
            perror("minishell: fork");
            if (pipe_fd[0] != -1) close(pipe_fd[0]);
            if (pipe_fd[1] != -1) close(pipe_fd[1]);
            break;
        }
        else if (pid == 0) // Child process
        {
            signal(SIGINT, SIG_DFL); // Restore default SIGINT behavior in child
            signal(SIGQUIT, SIG_DFL); // Restore default SIGQUIT behavior in child

            if (prev_pipe_read != -1)
            {
                dup2(prev_pipe_read, STDIN_FILENO);
                close(prev_pipe_read);
            }

            if (cmds->next)
            {
                dup2(pipe_fd[1], STDOUT_FILENO);
            }

            if (pipe_fd[0] != -1) close(pipe_fd[0]);
            if (pipe_fd[1] != -1) close(pipe_fd[1]);

            handle_redirections(cmds);

            if (is_builtin(cmds->args[0]))
            {
                exit(run_builtin(cmds, env));
            }
            else
            {
                char *path = get_exec_path(cmds->args[0], *env);
                char **env_arr = env_to_arr(*env);

                if (path)
                    execve(path, cmds->args, env_arr);
                else
                    execvp(cmds->args[0], cmds->args);

                perror("minishell");
                exit(127);
            }
        }

        // Parent process
        if (prev_pipe_read != -1)
            close(prev_pipe_read);

        if (cmds->next)
        {
            prev_pipe_read = pipe_fd[0];
            close(pipe_fd[1]);
        }
        else if (pipe_fd[0] != -1)
        {
            close(pipe_fd[0]);
        }

        cmds = cmds->next;
    }

    // Wait for all child processes to finish
    while (waitpid(-1, &status, 0) > 0)
    {
        if (WIFEXITED(status))
            g_exit_status = WEXITSTATUS(status);
    }

    setup_signals_for_prompt(); // Restore signal handling for the prompt
    restore_std_fds(stdin_copy, stdout_copy);
}


void	handle_redirections(t_cmd *cmd)
{
	t_redir *redir = cmd->redirections;
	while (redir)
	{
		if (redir->type == T_REDIR_IN)
		{
			int fd = open(redir->file, O_RDONLY);
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (redir->type == T_REDIR_OUT)
		{
			int fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (redir->type == T_APPEND)
		{
			int fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (redir->type == T_HEREDOC)
		{
			int fd = handle_heredoc(redir->file);
            if (fd < 0)
                ft_putstr_fd("minishell: heredoc failed\n", STDERR_FILENO);
                
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
		redir = redir->next;
	}
}

void	close_pipes(t_cmd *cmds)
{
	while (cmds)
	{
		if (cmds->pipe_fd[0] != -1)
			close(cmds->pipe_fd[0]);
		if (cmds->pipe_fd[1] != -1)
			close(cmds->pipe_fd[1]);
		cmds = cmds->next;
	}
}


void cleanup(t_env *env, t_token *tokens, t_cmd *cmds, char *input)
{
    // Free readline specific things (might help with *some* readline leaks)
    rl_clear_history(); // Clear history list maintained by readline
    // Consider rl_deprep_terminal() if necessary, but often not needed.

    // Free your data structures
    if (input)
        free(input);
    if (tokens)
        free_tokens(tokens);
    if (cmds)
        free_cmds(cmds);
    if (env)
        free_env_list(env); // Use the correct name for your env free function
}
