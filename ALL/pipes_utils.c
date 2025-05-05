/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhurnik <hhurnik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:03:10 by hhurnik           #+#    #+#             */
/*   Updated: 2025/05/05 16:03:11 by hhurnik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

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
