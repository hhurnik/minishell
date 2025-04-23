#include "inc/minishell.h"

void	restore_std_fds(int stdin_copy, int stdout_copy)
{
	dup2(stdin_copy, STDIN_FILENO);
	dup2(stdout_copy, STDOUT_FILENO);
	close(stdin_copy);
	close(stdout_copy);
}


int setup_redirections(t_redir *redir_list)
{
    int status;

    while (redir_list)
    {
        // Validate that the redirection has a valid file name
        if (!redir_list->file || redir_list->file[0] == '\0')
        {
            ft_putstr_fd("minishell: syntax error: missing file name after redirection\n", STDERR_FILENO);
            return (-1);
        }

        status = 0;
        if (redir_list->type == T_REDIR_IN)
            status = handle_input_redir(redir_list->file);
        else if (redir_list->type == T_REDIR_OUT)
            status = handle_output_redir(redir_list->file);
        else if (redir_list->type == T_APPEND)
            status = handle_append_redir(redir_list->file);
        else if (redir_list->type == T_HEREDOC)
            status = handle_heredoc_redir(redir_list->file);

        if (status < 0)
        {
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(redir_list->file, STDERR_FILENO);
            ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
            return (-1);
        }
        redir_list = redir_list->next;
    }
    return (0);
}

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
        if (setup_redirections(cmds->redirections) < 0) // Check redirections
        {
            restore_std_fds(stdin_copy, stdout_copy);
            return; // Stop execution if redirection fails
        }
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

            if (setup_redirections(cmds->redirections) < 0) // Check redirections
                exit(1); // Exit child process if redirection fails

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
        free_env(env);
}
