#include "ms.h"

// typedef struct s_cmd
// {
//     char **args;
//     t_redir *redirs;
//     struct s_cmd *next;
// } t_cmd


// pipe(int pipefd[2]) - pipefd[0]: Read end, pipefd[1]: Write end
// 1. use dup2() to redirect stdin/stdout to/from a pipe
// 2. fork a process for each command in the pipeline
// 3. The first command’s output goes into the pipe
// The last command reads from the last pipe, and outputs to stdout
// Any middle command reads from previous pipe and writes to next

/* NOW
- Traverse a linked list of commands
-Set up pipes between them
-Use dup2() for stdin/stdout redirection
-fork() each process
-execve() each command using cmd->args
-Wait for all children at the end
*/
//pipe - one way communication channel between processes 
// fds are not tied to the processes, but to resources that a process is using
// a file, directory, pipe, socket, terminal, device
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>


extern char **environ;

// one node = one command
typedef struct s_cmd
{
    char **args;
    // handle redirs later
    struct s_cmd *next;
} t_cmd;

void execute_pipeline(t_cmd *cmd_list)
{
    int prev_fd = -1;         // For previous pipe's read end - initially none
    int pipefd[2];            // pipefd[0] = read, pipefd[1] = write
    pid_t pid;

    // przejsc przez kazdy argument listy
    while (cmd_list)
    {
        // Create pipe only if there’s a next command
        if (cmd_list->next)
        {
            // returns -1 on error - failed to create pipe, 0 on success
            // (system out of fd/ process has too many open fd/kernel resources are exhausted/ invalid memory for pipefd array)
            if (pipe(pipefd) == -1) 
            {
                perror("pipe");
                exit(EXIT_FAILURE); //exit(1);
            }
        }

        // fork a child process to run the
        pid = fork();
        if (pid < 0)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0)
        {
            // === CHILD PROCESS ===

            // If not the first command, connect stdin to prev_fd
            if (prev_fd != -1)
            {
                //// Replace stdin with prev_fd
                if (dup2(prev_fd, STDIN_FILENO) == -1)
                {
                    perror("dup2 prev_fd");
                    exit(EXIT_FAILURE);
                }
                close(prev_fd);
            }

            // If not the last command, connect stdout to pipe write end
            if (cmd_list->next)
            {
                if (dup2(pipefd[1], STDOUT_FILENO) == -1)
                {
                    perror("dup2 pipe write");
                    exit(EXIT_FAILURE);
                }
                close(pipefd[0]); // Not used in child
                close(pipefd[1]); // Already duplicated
            }

            // Execute the command
            if (!cmd_list->args || !cmd_list->args[0])
            {
                fprintf(stderr, "Empty command\n");
                exit(EXIT_FAILURE);
            }

            // Optional: handle PATH lookup manually here if needed
            if (access(cmd_list->args[0], X_OK) != 0)
            {
                perror("exec error");
                exit(EXIT_FAILURE);
            }

            execve(cmd_list->args[0], cmd_list->args, environ);
            perror("execve");
            exit(EXIT_FAILURE);
        }

        // === PARENT PROCESS ===

        // Close prev_fd if open
        if (prev_fd != -1)
            close(prev_fd);

        // If there is a next command, store current pipe's read end
        if (cmd_list->next)
        {
            close(pipefd[1]);    // Parent doesn't write
            prev_fd = pipefd[0]; // Will be used in next iteration
        }

        cmd_list = cmd_list->next;
    }

    // Wait for all children
    while (wait(NULL) > 0);
}



// zasymuluj: echo hello.txt | grep txt

int main(void)
{
    // echo hello.txt
    t_cmd *cmd1 = malloc(sizeof(t_cmd));
    cmd1->args = malloc(sizeof(char *) * 3);
    cmd1->args[0] = strdup("/bin/echo");
    cmd1->args[1] = strdup("hello.txt");
    cmd1->args[2] = NULL;
    cmd1->next = NULL;

    // grep txt
    t_cmd *cmd2 = malloc(sizeof(t_cmd));
    cmd2->args = malloc(sizeof(char *) * 3);
    cmd2->args[0] = strdup("/usr/bin/grep");
    cmd2->args[1] = strdup("txt");
    cmd2->args[2] = NULL;
    cmd2->next = NULL;

    cmd1->next = cmd2;

    execute_pipeline(cmd1);

    // cleanup
    free(cmd1->args[0]); free(cmd1->args[1]); free(cmd1->args); free(cmd1);
    free(cmd2->args[0]); free(cmd2->args[1]); free(cmd2->args); free(cmd2);

    return 0;
}
































// // version with arrays - it needs to count commands

// // Helper to count commands
// int	count_commands(char **commands)
// {
// 	int	i;

// 	i = 0;
// 	while (commands[i])
// 		i++;
// 	return (i);
// }

// void	execute_pipeline(char **commands, char *argv[])
// {
// 	int		i;
// 	int		prev_fd;
// 	int		pipes[2];
// 	int		cmd_count;
// 	pid_t	pid;

// 	i = 0;
// 	prev_fd = -1;
// 	cmd_count = count_commands(commands);
// 	// looping over each command
// 	// create a pipe for the next command if needed
// 	// fork a process
// 	// setup stdin/stdout redirection using dup2
// 	// exec the command in the chidl
// 	// prepare parent for the next iteration
// 	while (i < cmd_count)
// 	{
// 		// If not the last command, create pipe
// 		if (i < cmd_count - 1)
// 		{
// 			if (pipe(pipes) == -1)
// 			{
// 				perror("pipe");
// 				exit(1);
// 			}
// 		}
// 		pid = fork();
// 		if (pid < 0)
// 		{
// 			perror("fork");
// 			exit(1);
// 		}
// 		if (pid == 0)
// 		{
// 			// CHILD PROCESS
// 			// If not first command, read from prev_fd
// 			if (i > 0)
// 			{
// 				dup2(prev_fd, STDIN_FILENO);
// 				close(prev_fd);
// 			}
// 			// If not last command, write to pipe
// 			// bo potrzebuje pipe tylko jesli istnieje kolejny command
// 			if (i < cmd_count - 1)
// 			{
// 				dup2(pipes[1], STDOUT_FILENO);
// 					// duplicates pipes[1] to file descriptor 1 - stdout
// 				// ^ po tym stdout and pipes[1] both point to the same pipe write end
// 				// i pipes[1] (original fd) juz nei jest potrzebny w procesie
// 				//	- it's duplicate
// 				close(pipes[0]); // to tez zamykam,
// 				//bo nie uzywam read end in the child process
// 				//	- it may prevent EOF signals from propagating correctly through the pipe
// 				close(pipes[1]); // wiec zamykam, bo jest now redundant
// 			}
// 			// Close any other descriptors in child
// 			// Parse args and exec
// 			if (access(argv[0], X_OK) != 0)
// 			{
// 				// Optional: try searching PATH if needed
// 				perror("exec error");
// 				exit(1);
// 			}
// 			execve(argv[0], argv, environ);
// 			perror("execve");
// 			exit(1);
// 		}
// 		// PARENT PROCESS
// 		// Close previous pipe read end
// 		if (prev_fd != -1)
// 			close(prev_fd);
// 		// Prepare prev_fd for next command
// 		if (i < cmd_count - 1)
// 		{
// 			close(pipes[1]);    // Parent doesn’t need write end
// 			prev_fd = pipes[0]; // Save read end for next child
// 		}
// 		i++; // advance to next command
// 	}
// 	// Parent waits for all children
// 	i = 0;
// 	while (i < cmd_count)
// 	{
// 		wait(NULL);
// 		i++;
// 	}
// }
