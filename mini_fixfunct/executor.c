#include "inc/minishell.h"

void cleanup(t_resources *res)
{
    if (res->input)
        free(res->input);
    if (res->tokens)
        free_tokens(res->tokens);
    if (res->cmds)
        free_cmds(res->cmds);
    if (res->env)
        free_env(res->env);
    
    // Clear readline history
    rl_clear_history();
}
// //moje
// void	process_command(t_cmd *cmd, t_env **env, int prev_pipe_read,
// 		int *pipe_fd)
// {
// 	pid_t pid;

// 	pid = setup_pipe_and_fork(cmd, pipe_fd);
// 	if (pid == 0)
// 		handle_child_process(cmd, env, prev_pipe_read, pipe_fd);
// }


void process_command(t_cmd *cmd, t_env **env, int prev_pipe_read, int *pipe_fd)
{
    pid_t pid;

    // If it's export and it's the first command in the pipeline, run it in the parent
    if (cmd->args && !cmd->next && !strcmp(cmd->args[0], "export")) {
        t_resources res = {0};
        res.env = *env;
        res.cmds = cmd;
        g_exit_status = run_builtin(cmd, &res);
        return;
    }

    pid = setup_pipe_and_fork(cmd, pipe_fd);
    if (pid == 0)
        handle_child_process(cmd, env, prev_pipe_read, pipe_fd);
}

int	init_execution(t_cmd *cmds, int *stdin_copy, int *stdout_copy)
{
	*stdin_copy = dup(STDIN_FILENO);
	*stdout_copy = dup(STDOUT_FILENO);
	if (!cmds || !cmds->args || !cmds->args[0])
	{
		ft_putstr_fd("minishell: null command\n", STDERR_FILENO);
		return (0);
	}
	return (1);
}

void	cleanup_execution(int stdin_copy, int stdout_copy)
{
	setup_signals_for_prompt();
	restore_std_fds(stdin_copy, stdout_copy);
}


void wait_for_children(pid_t last_cmd_pid)
{
    int   status;
    pid_t pid;
    int   last_cmd_status = 0; // Store the status of the specific last command
    int   found_last = 0;     // Flag to track if we found the last command's status

    // If last_cmd_pid is 0 or -1 (e.g., fork failed), default status might be appropriate
    if (last_cmd_pid <= 0) {
        // Decide on default behavior, maybe keep g_exit_status as is, or set to error?
        // Setting to 1 if last_cmd_pid indicates failure might be reasonable.
        // For now, we'll let it default based on what was last set if no children are waited for.
        // Or, if fork failed, an error might have already been set.
    }


    while ((pid = waitpid(-1, &status, 0)) > 0) // Wait for ANY child
    {
        if (pid == last_cmd_pid) // Is this the specific last command we care about?
        {
            found_last = 1; // Mark that we found it
            if (WIFEXITED(status))
            {
                last_cmd_status = WEXITSTATUS(status); // Get its exit status
            }
            else if (WIFSIGNALED(status))
            {
                int signal_num = WTERMSIG(status);
                last_cmd_status = 128 + signal_num; // Calculate signal exit status

                // Optional: Print signal messages like bash
                if (signal_num == SIGQUIT) {
                    // Bash prints "Quit (core dumped)" - adjust as needed
                    ft_putstr_fd("Quit\n", STDERR_FILENO);
                } else if (signal_num == SIGSEGV) {
                     ft_putstr_fd("Segmentation fault\n", STDERR_FILENO);
                }
                // SIGINT is handled by the signal handler setting g_exit_status,
                // but waitpid will still report it here. last_cmd_status will be 130.
            }
            // No need to set g_exit_status here inside the loop
        }
        // We continue waiting even after finding the last command to ensure
        // all children in the pipeline are reaped and no zombies are left.
    }

    // After waiting for all children, set the global exit status
    // ONLY if we actually successfully waited for the last command.
    // If last_cmd_pid was valid (>0) but we never found it (waitpid error?),
    // maybe indicate an error. For now, we only set if found.
    if (last_cmd_pid > 0 && found_last) {
         g_exit_status = last_cmd_status;
    } else if (last_cmd_pid <= 0) {
        // Handle cases where there was no pipeline / fork failed for last cmd.
        // Perhaps g_exit_status was already set by a builtin or init_execution failure.
        // If no children were waited for at all (waitpid returned -1 immediately with ECHILD),
        // g_exit_status should reflect whatever happened before the wait loop.
        // So, maybe do nothing here if last_cmd_pid was invalid/not found.
    }


    // Error handling for waitpid itself
    if (pid == -1 && errno != ECHILD) {
        perror("minishell: waitpid");
        // Optionally set g_exit_status to indicate a wait error
        // g_exit_status = 1;
    }
}