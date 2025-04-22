/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 15:32:38 by hhurnik           #+#    #+#             */
/*   Updated: 2025/04/22 19:02:42 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// dup - tworzy kopie file descriptora oldfd, using the lowest-numbered
// unused file descriptor fot the new descriptor
// dup2
//	- to samo tylko uzywa konkretnego file descriptora podanego mu do funkcji
// int dup2(int oldfd, int newfd);

// < file
// zamiast czytac z klawiatury (stdin), czytaj z tego file
int	handle_input_redir(char *file)
{
	int	fd;

	// otwor file in readonly
	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		perror(file); // print error jesli nie otworzy
		return (-1);
	}
	// dup2 tu replaces stin (0) with the fd
	// make newfd (0) point to the same thing oldfd points to
	// first argument is the source, the second is the target
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		// on error -1 is returned
		// on success the new fd is returned
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}
// > file
// zamiast wypisywac na stdout, wypisz do file
//| - bitwise OR
int	handle_output_redir(char *file)
{
	// open or create a file if it doesnt exist for writing only
	// jesli juz istnieje, truncate it - clears contents
	// 0644 (rw-r--r--)
	int fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror(file);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}
// >>
// O-APPEND - If set, the file offset shall be set to the end of the file prior
// to each write
// jak >, tylko nie nadpisuje zawartosci

int	handle_append_redir(char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror(file);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int handle_heredoc(const char *delimiter)
{
    char *line = NULL;
    int pipe_fd[2];

    if (pipe(pipe_fd) == -1)
    {
        perror("pipe");
        return (-1);
    }

    while (1)
    {
        line = readline("> ");
        if (!line) // Handle Ctrl+D
        {
            write(1, "\n", 1);
            break;
        }

        // Check if line matches delimiter (without newline)
        if (ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }

        // Write to pipe (with newline)
        write(pipe_fd[1], line, strlen(line));
        write(pipe_fd[1], "\n", 1);
        free(line);
    }

    close(pipe_fd[1]);
    return (pipe_fd[0]);
}

int	handle_heredoc_redir(char *delimiter)
{
	int fd = handle_heredoc(delimiter); // Call heredoc handler
	if (fd < 0)
		return (-1);
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	setup_redirections(t_redir *redir_list)
{
	int	status;

	while (redir_list)
	{
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
			return (-1);
		redir_list = redir_list->next;
	}
	return (0);
}
