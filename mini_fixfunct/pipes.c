#include "inc/minishell.h"

void	prepare_pipeline_execution(t_cmd *cmds, int *prev_pipe_read)
{
	*prev_pipe_read = -1;
	setup_signals_for_command();
	(void)cmds;
}

void execute_pipeline(t_cmd *cmds, t_env **env, int *prev_pipe_read)
{
    int   pipe_fd[2];
    pid_t pid;
    pid_t last_pid = -1; // Variable to store the PID of the last command
    t_cmd *current_cmd = cmds; // Use a temporary pointer to iterate

    while (current_cmd)
    {
        pipe_fd[0] = -1;
        pipe_fd[1] = -1;

        // Directly call setup_pipe_and_fork here to get the PID
        pid = setup_pipe_and_fork(current_cmd, pipe_fd);

        if (pid < 0) {
             // Fork failed, maybe handle error, break loop?
             // perror("minishell: fork failed in pipeline");
             // Consider setting an error status
             if (*prev_pipe_read != -1) close(*prev_pipe_read); // Cleanup pending read pipe
             break; // Stop processing pipeline
        }
        else if (pid == 0)
        {
            // --- Child Process ---
            // process_command used to call handle_child_process, do it directly
            handle_child_process(current_cmd, env, *prev_pipe_read, pipe_fd);
            // handle_child_process exits, so code below won't run in child
        }
        else
        {
            // --- Parent Process ---
            // If this is the last command in the list, store its PID
            if (!current_cmd->next)
            {
                last_pid = pid;
            }
            // Close pipes in the parent
            update_pipe_status(current_cmd, prev_pipe_read, pipe_fd);
        }

        current_cmd = current_cmd->next; // Move to the next command
    }

    // Wait for all children, passing the PID of the last one
    wait_for_children(last_pid);
}

void	cleanup_pipes(int *pipe_fd, int *prev_pipe_read)
{
	if (*prev_pipe_read != -1)
		close(*prev_pipe_read);
	if (pipe_fd[1] != -1)
		close(pipe_fd[1]);
	*prev_pipe_read = pipe_fd[0];
}

void	update_pipe_status(t_cmd *cmd, int *prev_pipe_read, int *pipe_fd)
{
	if (cmd->next)
		cleanup_pipes(pipe_fd, prev_pipe_read);
	else if (pipe_fd[0] != -1)
		close(pipe_fd[0]);
}
