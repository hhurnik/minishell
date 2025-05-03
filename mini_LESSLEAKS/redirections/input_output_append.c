#include "minishell.h"

// dup - tworzy kopie file descriptora oldfd, using the lowest-numbered
// unused file descriptor fot the new descriptor
// dup2
//	- to samo tylko uzywa konkretnego file descriptora podanego mu do funkcji
// int dup2(int oldfd, int newfd);

// < file
// // zamiast czytac z klawiatury (stdin), czytaj z tego file
int	handle_input_redir(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
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

int	handle_output_redir(char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
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

int	handle_append_redir(char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
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
