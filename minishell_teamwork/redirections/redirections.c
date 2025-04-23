/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhurnik <hhurnik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 15:32:38 by hhurnik           #+#    #+#             */
/*   Updated: 2025/04/23 19:53:09 by hhurnik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// dup - tworzy kopie file descriptora oldfd, using the lowest-numbered
// unused file descriptor fot the new descriptor
// dup2
//	- to samo tylko uzywa konkretnego file descriptora podanego mu do funkcji
// int dup2(int oldfd, int newfd);

// < file
// // zamiast czytac z klawiatury (stdin), czytaj z tego file
int handle_input_redir(char *file)
{
    int fd = open(file, O_RDONLY);
    if (fd < 0)
        return (-1); // Do not print error message here

    if (dup2(fd, STDIN_FILENO) < 0)
    {
        close(fd);
        return (-1);
    }
    close(fd);
    return (0);
}

int handle_output_redir(char *file)
{
    int fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
        return (-1); // Do not print error message here

    if (dup2(fd, STDOUT_FILENO) < 0)
    {
        close(fd);
        return (-1);
    }
    close(fd);
    return (0);
}

int handle_append_redir(char *file)
{
    int fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd < 0)
        return (-1); // Do not print error message here

    if (dup2(fd, STDOUT_FILENO) < 0)
    {
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


// int setup_redirections(t_redir *redir_list)
// {
//     int status;

//     while (redir_list)
//     {
//         status = 0;
//         if (redir_list->type == T_REDIR_IN)
//             status = handle_input_redir(redir_list->file);
//         else if (redir_list->type == T_REDIR_OUT)
//             status = handle_output_redir(redir_list->file);
//         else if (redir_list->type == T_APPEND)
//             status = handle_append_redir(redir_list->file);
//         else if (redir_list->type == T_HEREDOC)
//             status = handle_heredoc_redir(redir_list->file);

//         if (status < 0) // If any redirection fails
//         {
//             ft_putstr_fd("minishell: ", STDERR_FILENO);
//             ft_putstr_fd(redir_list->file, STDERR_FILENO);
//             ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
//             return (-1); // Stop processing further redirections
//         }
//         redir_list = redir_list->next;
//     }
//     return (0);
// }
int setup_redirections(t_redir *redir_list)
{
    int status;

    while (redir_list)
    {
        // Validate that the redirection has a valid file name
        if (!redir_list->file || redir_list->file[0] == '\0')
        {
            ft_putstr_fd("minishell: syntax error: missing file name after redirection\n", STDERR_FILENO);
            return (-1);
        }

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
        {
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(redir_list->file, STDERR_FILENO);
            ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
            return (-1);
        }
        redir_list = redir_list->next;
    }
    return (0);
}