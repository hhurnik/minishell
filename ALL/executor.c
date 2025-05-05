/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhurnik <hhurnik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:00:05 by hhurnik           #+#    #+#             */
/*   Updated: 2025/05/05 16:00:07 by hhurnik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

void	handle_signal_output(int status, int *printed_pipe_msg)
{
	int	sig;

	if (!WIFSIGNALED(status))
		return ;
	sig = WTERMSIG(status);
	if (sig == SIGQUIT)
	{
		ft_putstr_fd("Quit", STDERR_FILENO);
		if (WCOREDUMP(status))
			ft_putstr_fd(" (core dumped)", STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
	}
	else if (sig == SIGSEGV)
	{
		ft_putstr_fd("Segmentation fault", STDERR_FILENO);
		if (WCOREDUMP(status))
			ft_putstr_fd(" (core dumped)", STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
	}
	else if (sig == SIGPIPE && *printed_pipe_msg == 0)
	{
		ft_putstr_fd("Broken pipe\n", STDERR_FILENO);
		*printed_pipe_msg = 1;
	}
}

void	process_child_status(pid_t pid, int status, pid_t last_pid,
		t_status_tracker *tracker)
{
	if (pid == last_pid)
	{
		tracker->found = 1;
		if (WIFEXITED(status))
			tracker->last_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			tracker->last_status = 128 + WTERMSIG(status);
	}
}

void	process_command(t_cmd *cmd, t_env **env, int prev_pipe_read,
		int *pipe_fd)
{
	pid_t	pid;

	pid = setup_pipe_and_fork(cmd, pipe_fd);
	if (pid == 0)
		handle_child_process(cmd, env, prev_pipe_read, pipe_fd);
}

int	init_execution(t_cmd *cmds, int *stdin_copy, int *stdout_copy)
{
	int	tmp_in;
	int	tmp_out;

	tmp_in = dup(STDIN_FILENO);
	tmp_out = dup(STDOUT_FILENO);
	*stdin_copy = tmp_in;
	*stdout_copy = tmp_out;
	if (!cmds)
		return (0);
	if (!cmds->args)
		return (0);
	if (!cmds->args[0])
	{
		ft_putstr_fd("minishell: null command\n", STDERR_FILENO);
		if (tmp_in != -1)
			close(tmp_in);
		if (tmp_out != -1)
			close(tmp_out);
		return (0);
	}
	return (1);
}

void	wait_for_children(pid_t last_cmd_pid)
{
	int					status;
	pid_t				pid;
	int					printed_pipe_msg;
	t_status_tracker	tracker;

	tracker.found = 0;
	tracker.last_status = 0;
	printed_pipe_msg = 0;
	pid = waitpid(-1, &status, 0);
	while (pid > 0)
	{
		process_child_status(pid, status, last_cmd_pid, &tracker);
		handle_signal_output(status, &printed_pipe_msg);
		pid = waitpid(-1, &status, 0);
	}
	finalize_exit_status(last_cmd_pid, tracker.found, tracker.last_status);
	if (pid == -1 && errno != ECHILD)
		perror("minishell: waitpid");
}
