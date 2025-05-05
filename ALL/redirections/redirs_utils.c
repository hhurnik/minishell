/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhurnik <hhurnik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:04:40 by hhurnik           #+#    #+#             */
/*   Updated: 2025/05/05 16:04:41 by hhurnik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// Helper functions
// int create_heredoc_pipe(int pipe_fd[2]);
// int handle_heredoc2(t_redir *redir);
// void print_redirection_error(t_redir *redir);
int	create_heredoc_pipe(int pipe_fd[2])
{
	if (pipe(pipe_fd) == -1)
	{
		perror("minishell: pipe");
		return (-1);
	}
	return (0);
}

int	handle_heredoc2(t_redir *redir)
{
	if (redir->heredoc_fd < 0)
	{
		ft_putstr_fd("minishell: heredoc error: ", STDERR_FILENO);
		ft_putstr_fd(redir->file, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		return (-1);
	}
	if (dup2(redir->heredoc_fd, STDIN_FILENO) < 0)
		return (-1);
	close(redir->heredoc_fd);
	return (0);
}

void	print_redirection_error(t_redir *redir)
{
	int	saved_errno;

	saved_errno = errno;
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(redir->file, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(strerror(saved_errno), STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	g_exit_status = 1;
}
