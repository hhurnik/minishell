#include "ms.h"

typedef struct s_cmd
{
    char **args;
    t_redir *redirs;
    struct s_cmd *next;
} t_cmd


// pipe(int pipefd[2]) - pipefd[0]: Read end, pipefd[1]: Write end
// 1. use dup2() to redirect stdin/stdout to/from a pipe
// 2. fork a process for each command in the pipeline
// 3. The first command’s output goes into the pipe
// The last command reads from the last pipe, and outputs to stdout
// Any middle command reads from previous pipe and writes to next

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

// Helper to count commands
int	count_commands(char **commands)
{
	int	i;

	i = 0;
	while (commands[i])
		i++;
	return (i);
}

void	execute_pipeline(char **commands, char *argv[])
{
	int		i;
	int		prev_fd;
	int		pipes[2];
	int		cmd_count;
	pid_t	pid;

	i = 0;
	prev_fd = -1;
	cmd_count = count_commands(commands);
	// looping over each command
	// create a pipe for the next command if needed
	// fork a process
	// setup stdin/stdout redirection using dup2
	// exec the command in the chidl
	// prepare parent for the next iteration
	while (i < cmd_count)
	{
		// If not the last command, create pipe
		if (i < cmd_count - 1)
		{
			if (pipe(pipes) == -1)
			{
				perror("pipe");
				exit(1);
			}
		}
		pid = fork();
		if (pid < 0)
		{
			perror("fork");
			exit(1);
		}
		if (pid == 0)
		{
			// CHILD PROCESS
			// If not first command, read from prev_fd
			if (i > 0)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}
			// If not last command, write to pipe
			// bo potrzebuje pipe tylko jesli istnieje kolejny command
			if (i < cmd_count - 1)
			{
				dup2(pipes[1], STDOUT_FILENO);
					// duplicates pipes[1] to file descriptor 1 - stdout
				// ^ po tym stdout and pipes[1] both point to the same pipe write end
				// i pipes[1] (original fd) juz nei jest potrzebny w procesie
				//	- it's duplicate
				close(pipes[0]); // to tez zamykam,
				//bo nie uzywam read end in the child process
				//	- it may prevent EOF signals from propagating correctly through the pipe
				close(pipes[1]); // wiec zamykam, bo jest now redundant
			}
			// Close any other descriptors in child
			// Parse args and exec
			if (access(argv[0], X_OK) != 0)
			{
				// Optional: try searching PATH if needed
				perror("exec error");
				exit(1);
			}
			execve(argv[0], argv, environ);
			perror("execve");
			exit(1);
		}
		// PARENT PROCESS
		// Close previous pipe read end
		if (prev_fd != -1)
			close(prev_fd);
		// Prepare prev_fd for next command
		if (i < cmd_count - 1)
		{
			close(pipes[1]);    // Parent doesn’t need write end
			prev_fd = pipes[0]; // Save read end for next child
		}
		i++; // advance to next command
	}
	// Parent waits for all children
	i = 0;
	while (i < cmd_count)
	{
		wait(NULL);
		i++;
	}
}
