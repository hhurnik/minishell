#include "inc/minishell.h"

void	prepare_pipeline_execution(t_cmd *cmds, int *prev_pipe_read)
{
	*prev_pipe_read = -1;
	setup_signals_for_command();
	(void)cmds;
}

void	execute_pipeline(t_cmd *cmds, t_env **env, int *prev_pipe_read)
{
	int		pipe_fd[2];

	while (cmds)
	{
		pipe_fd[0] = -1;
		pipe_fd[1] = -1;
		process_command(cmds, env, *prev_pipe_read, pipe_fd);
		update_pipe_status(cmds, prev_pipe_read, pipe_fd);
		cmds = cmds->next;
	}
	wait_for_children();
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