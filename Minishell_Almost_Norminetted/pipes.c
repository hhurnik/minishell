#include "inc/minishell.h"

void	prepare_pipeline_execution(t_cmd *cmds, int *prev_pipe_read)
{
	*prev_pipe_read = -1;
	setup_signals_for_command();
	(void)cmds;
}

void	execute_pipeline(t_cmd *cmds, t_env **env, int *prev_pipe_read)
{
	pid_t	last_pid;
	t_cmd	*current_cmd;

	last_pid = -1;
	current_cmd = cmds;
	while (current_cmd)
	{
		last_pid = process_single_command(current_cmd, env, prev_pipe_read,
				last_pid);
		if (last_pid == -2)
			return ;
		current_cmd = current_cmd->next;
	}
	finalize_pipeline(prev_pipe_read, last_pid);
}

pid_t	process_single_command(t_cmd *cmd, t_env **env, int *prev_pipe_read,
		pid_t last_pid)
{
	int		pipe_fd[2];
	pid_t	pid;

	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
	pid = setup_pipe_and_fork(cmd, pipe_fd);
	if (pid < 0)
	{
		if (*prev_pipe_read != -1)
			close(*prev_pipe_read);
		return (-2);
	}
	else if (pid == 0)
		handle_child_process(cmd, env, *prev_pipe_read, pipe_fd);
	else
	{
		if (!cmd->next)
			last_pid = pid;
		update_pipe_status(cmd, prev_pipe_read, pipe_fd);
	}
	return (last_pid);
}

void	finalize_pipeline(int *prev_pipe_read, pid_t last_pid)
{
	if (*prev_pipe_read != -1)
	{
		close(*prev_pipe_read);
		*prev_pipe_read = -1;
	}
	wait_for_children(last_pid);
}
