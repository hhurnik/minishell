#include "../inc/minishell.h"
//////////start moje

///moje
// int	create_heredoc_pipe(int pipe_fd[2])
// {
// 	if (pipe(pipe_fd) == -1)
// 	{
// 		perror("pipe");
// 		return (-1);
// 	}
// 	return (0);
// }



int create_heredoc_pipe(int pipe_fd[2])
{
    if (pipe(pipe_fd) == -1)
    {
        perror("minishell: pipe");
        return (-1);
    }
    return (0);
}




void	process_heredoc_line(int write_fd, char *line)
{
	// Write to pipe (with newline)
	write(write_fd, line, ft_strlen(line));
	write(write_fd, "\n", 1);
	free(line);
}


// //moje
// int	handle_heredoc(const char *delimiter)
// {
// 	char	*line;
// 	int		pipe_fd[2];

// 	line = NULL;
// 	if (create_heredoc_pipe(pipe_fd) < 0)
// 		return (-1);
// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (!line) // Handle Ctrl+D
// 		{
// 			write(1, "\n", 1);
// 			break ;
// 		}
// 		if (ft_strcmp(line, delimiter) == 0)
// 		{
// 			free(line);
// 			break ;
// 		}
// 		process_heredoc_line(pipe_fd[1], line);
// 	}
// 	close(pipe_fd[1]);
// 	return (pipe_fd[0]);
// }
int handle_heredoc(char *delimiter)
{
    char *line;
    int pipe_fd[2];
    int return_fd = -1;
    bool delimiter_found = false;

    if (create_heredoc_pipe(pipe_fd) < 0) // create_heredoc_pipe just calls pipe()
        return (-1);

    return_fd = pipe_fd[0]; // Store the read end

    // Optional: Temporarily ignore SIGINT for heredoc input? Complex.
    // Setup signals appropriate for interactive input if needed.

    while (1)
    {
        line = readline("> ");

        if (!line) // Handle Ctrl+D (EOF) or readline error
        {
            ft_putstr_fd("minishell: warning: here-document delimited by end-of-file (wanted `", STDERR_FILENO);
            ft_putstr_fd(delimiter, STDERR_FILENO);
            ft_putstr_fd("')\n", STDERR_FILENO);
            break; // Exit loop - unsuccessful termination
        }

        if (ft_strcmp(line, delimiter) == 0)
        {
            delimiter_found = true; // Mark as successful
            free(line);
            break; // Exit loop - successful termination
        }
        // Write line + newline to the pipe
        write(pipe_fd[1], line, ft_strlen(line));
        write(pipe_fd[1], "\n", 1);
        free(line);
    }

    close(pipe_fd[1]); // Close write end

    if (delimiter_found) {
        return (return_fd); // Return the read FD on success
    } else {
        close(return_fd); // Close the read end as it's not needed/valid
        return (-1); // Return an error code
    }
}

int	handle_heredoc_redir(char *delimiter)
{
	int	fd;

	fd = handle_heredoc(delimiter); // Call heredoc handler
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

////////////koniec moje

//////////////////////ponizej nowe

int handle_all_heredocs(t_cmd *cmds)
{
    t_cmd   *current_cmd;
    t_redir *current_redir;
    int     fd;

    current_cmd = cmds;
    while (current_cmd)
    {
        current_redir = current_cmd->redirections;
        while (current_redir)
        {
            if (current_redir->type == T_HEREDOC)
            {
                // Make sure heredoc_fd is reset (important if function called multiple times, though unlikely here)
                if (current_redir->heredoc_fd != -1) {
                   close(current_redir->heredoc_fd);
                   current_redir->heredoc_fd = -1;
                }

                // 'file' field holds the delimiter for T_HEREDOC
                fd = handle_heredoc(current_redir->file);

                if (fd < 0)
                {
                    // handle_heredoc should print the warning on EOF
                    g_exit_status = 1; // Set error status
                    // No need to close FDs here, as handle_heredoc closes on error
                    return (-1); // Signal failure
                }
                current_redir->heredoc_fd = fd; // Store the read end FD
            }
            current_redir = current_redir->next;
        }
        current_cmd = current_cmd->next;
    }
    return (0); // Success
}

void close_heredoc_fds(t_cmd *cmds)
{
    t_cmd   *current_cmd;
    t_redir *current_redir;

    current_cmd = cmds;
    while (current_cmd)
    {
        current_redir = current_cmd->redirections;
        while (current_redir)
        {
            if (current_redir->type == T_HEREDOC && current_redir->heredoc_fd != -1)
            {
                close(current_redir->heredoc_fd);
                current_redir->heredoc_fd = -1; // Mark as closed
            }
            current_redir = current_redir->next;
        }
        current_cmd = current_cmd->next;
    }
}
