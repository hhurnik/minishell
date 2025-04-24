#include "../inc/minishell.h"

int	create_heredoc_pipe(int pipe_fd[2])
{
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
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

int	handle_heredoc(const char *delimiter)
{
	char	*line;
	int		pipe_fd[2];

	line = NULL;
	if (create_heredoc_pipe(pipe_fd) < 0)
		return (-1);
	while (1)
	{
		line = readline("> ");
		if (!line) // Handle Ctrl+D
		{
			write(1, "\n", 1);
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		process_heredoc_line(pipe_fd[1], line);
	}
	close(pipe_fd[1]);
	return (pipe_fd[0]);
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
