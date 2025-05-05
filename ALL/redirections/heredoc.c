/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhurnik <hhurnik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:04:29 by hhurnik           #+#    #+#             */
/*   Updated: 2025/05/05 16:04:30 by hhurnik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// // Heredoc core functionality
// bool read_until_delimiter(char *delimiter, int write_fd);
// int handle_heredoc(char *delimiter);
// int process_heredoc_redirection(t_redir *redir);
// int handle_all_heredocs(t_cmd *cmds);
// void close_heredoc_fds(t_cmd *cmds);

/////podzielony handle_heredoc - read until delimeter and handle heredoc
// brak leakow, tester dziala
bool	read_until_delimiter(char *delimiter, int write_fd)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putstr_fd("minishell: warning: here-document "
				"delimited by end-of-file (wanted `", STDERR_FILENO);
			ft_putstr_fd(delimiter, STDERR_FILENO);
			ft_putstr_fd("')\n", STDERR_FILENO);
			return (false);
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			return (true);
		}
		write(write_fd, line, ft_strlen(line));
		write(write_fd, "\n", 1);
		free(line);
	}
}

int	handle_heredoc(char *delimiter)
{
	int	pipe_fd[2];
	int	read_fd;

	if (create_heredoc_pipe(pipe_fd) < 0)
		return (-1);
	read_fd = pipe_fd[0];
	if (read_until_delimiter(delimiter, pipe_fd[1]))
	{
		close(pipe_fd[1]);
		return (read_fd);
	}
	close(pipe_fd[1]);
	close(read_fd);
	return (-1);
}

int	process_heredoc_redirection(t_redir *redir)
{
	int	fd;

	if (redir->heredoc_fd != -1)
	{
		close(redir->heredoc_fd);
		redir->heredoc_fd = -1;
	}
	fd = handle_heredoc(redir->file);
	if (fd < 0)
	{
		g_exit_status = 1;
		return (-1);
	}
	redir->heredoc_fd = fd;
	return (0);
}

int	handle_all_heredocs(t_cmd *cmds)
{
	t_cmd	*cmd;
	t_redir	*redir;

	cmd = cmds;
	while (cmd)
	{
		redir = cmd->redirections;
		while (redir)
		{
			if (redir->type == T_HEREDOC
				&& process_heredoc_redirection(redir) < 0)
				return (-1);
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (0);
}

void	close_heredoc_fds(t_cmd *cmds)
{
	t_cmd	*current_cmd;
	t_redir	*current_redir;

	current_cmd = cmds;
	while (current_cmd)
	{
		current_redir = current_cmd->redirections;
		while (current_redir)
		{
			if (current_redir->type == T_HEREDOC
				&& current_redir->heredoc_fd != -1)
			{
				close(current_redir->heredoc_fd);
				current_redir->heredoc_fd = -1;
			}
			current_redir = current_redir->next;
		}
		current_cmd = current_cmd->next;
	}
}
