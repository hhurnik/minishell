/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_pipes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhurnik <hhurnik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:59:33 by hhurnik           #+#    #+#             */
/*   Updated: 2025/05/05 15:59:35 by hhurnik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

int	setup_pipe_and_fork(t_cmd *cmd, int *pipe_fd)
{
	pid_t	pid;

	if (cmd->next)
	{
		if (pipe(pipe_fd) == -1)
		{
			perror("minishell: pipe");
			return (0);
		}
	}
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		if (pipe_fd[0] != -1)
			close(pipe_fd[0]);
		if (pipe_fd[1] != -1)
			close(pipe_fd[1]);
		return (0);
	}
	return (pid);
}

void	setup_child_pipes(t_cmd *cmd, int prev_pipe_read, int *pipe_fd,
		t_env **env)
{
	if (prev_pipe_read != -1)
		redirect_stdin(prev_pipe_read, env, cmd);
	if (cmd->next)
	{
		if (pipe_fd[1] != -1)
			redirect_stdout(pipe_fd[1], env, cmd, pipe_fd);
	}
	if (pipe_fd[0] != -1)
		close(pipe_fd[0]);
	if (pipe_fd[1] != -1)
		close(pipe_fd[1]);
}

void	init_child_resources(t_resources *res, t_cmd *cmd, t_env **env)
{
	res->env = *env;
	res->cmds = cmd;
	res->tokens = NULL;
	res->input = NULL;
}

void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	exit_child_success(t_env **env, t_cmd *cmd)
{
	free_env(*env);
	free_cmds(cmd);
	exit(EXIT_SUCCESS);
}
