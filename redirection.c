// • Implement redirections:
// ◦ < should redirect input.
// ◦ > should redirect output.
// ◦ << should be given a delimiter, then read the input until a line containing the
// delimiter is seen. However, it doesn’t have to update the history!
// ◦ >> should redirect output in append mode


/*what it does:
<	Redirect stdin from file
>	Redirect stdout to file (truncate)
>>	Redirect stdout to file (append)
<<	Here-document (stdin from inline input until a delimiter line is found)*/

// execve(): Used when you want to replace the current process (the shell) 
//with an external program (e.g., cat, ls, etc.). 
//This is the typical way to run external programs in a shell.

//why to use fork?
/*fork() is used to create a new child process that is a copy of the current process 
(the parent process). After fork(), you will have two processes running: the parent process 
(which is the shell) and the child process (which will execute the command).*/

// echo accepts only parameters, cat accepts anything from the stdin
// echo -display the line of text
// cat - concatenate files and print on the stdout


// data in entered via stdin (keyboard ususally)
// the resulting output goes to stdout (usually the shell) --> these are called streams

//it;s possible to alter these input and ouput locations - the computer then gets
// information from somewhere else than stdin or send the results somewhere 
// other than stdout - this is called redirection


//  ------- https://www.redhat.com/en/blog/redirect-operators-bash

// REGULAR OUTPUT > OPERATOR       the output redirector overwrites any existing data in the file -- easy to accidentally
// overwrite existing information

// REGULAR OUTPUT APPEND >> OPERATOR
// The append >> operator adds the output to 
// the existing content instead of overwriting it

// REGULAR INPUT < OPERATOR
// the input redirector pulls data in a stream from a given source
// data can be pulled from another source, such as a file

/*sort < mylist.txt 
cat
cow
dog
horse*/

// << is known as here-document structure.
/*You let the program know what will be the ending text, and whenever that 
delimiter is seen, the program will read all the stuff you've given to the program 
as input and perform a task upon it.*/

/*$ wc << EOF
> one two three
> four five
> EOF
 2  5 24
In this example we tell wc program to wait for EOF string, then type in five words, 
and then type in EOF to signal that we're done giving input.*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "ms.h"

extern char **environ;

// Function to handle redirections for input/output
int handle_redirection(char *infile, char *outfile, int append) 
{
    int fd;

    // Input redirection (<)
    if (infile) 
    {
        fd = open(infile, O_RDONLY);
        if (fd == -1) 
        {
            perror("minishell");
            return (-1);
        }
        if (dup2(fd, STDIN_FILENO) == -1) 
        {
            perror("minishell");
            close(fd);
            return (-1);
        }
        close(fd);
    }

    // Output redirection (>)
    if (outfile) 
    {
        if (append) 
        {
            // Output append (>>)
            fd = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
        } 
        else 
        {
            // Output overwrite (>)
            fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        }
        if (fd == -1) 
        {
            perror("minishell");
            return (-1);
        }
        if (dup2(fd, STDOUT_FILENO) == -1) 
        {
            perror("minishell");
            close(fd);
            return (-1);
        }
        close(fd);
    }

    return (0);
}

// Function to handle heredoc redirection (<<)
int handle_heredoc(char *delimiter) 
{
    int pipefds[2];
    pid_t pid;

    // Create a pipe
    if (pipe(pipefds) == -1) 
    {
        perror("minishell");
        return -1;
    }

    pid = fork();
    if (pid == 0) 
    {
        // Child process: read lines from stdin until delimiter is encountered
        char *line = NULL;
        size_t len = 0;

        close(pipefds[0]);  // Close read end of pipe

        while (1) 
        {
            printf("heredoc> ");
            getline(&line, &len, stdin);

            // Check if the line matches the delimiter
            if (strncmp(line, delimiter, ft_strlen(delimiter)) == 0) 
            {
                free(line);
                break;  // Exit the heredoc loop
            }

            // Write the line to the pipe
            write(pipefds[1], line, strlen(line));
        }

        close(pipefds[1]);  // Close write end of pipe
        exit(0);
    } 
    else if (pid > 0) 
    {
        // Parent process: redirect stdin to pipe and wait for child
        close(pipefds[1]);  // Close write end of pipe

        if (dup2(pipefds[0], STDIN_FILENO) == -1) 
        {
            perror("minishell");
            close(pipefds[0]);
            return (-1);
        }
        close(pipefds[0]);  // Close read end of pipe after redirecting

        waitpid(pid, NULL, 0);  // Wait for the heredoc child to finish
    } 
    else 
    {
        perror("fork failed");
        return (-1);
    }

    return 0;
}

// Function to execute the command with redirection
void execute_command_with_redirection(char *cmd, char *infile, char *outfile, int append, char *delimiter) 
{
    pid_t pid = fork();
    if (pid == 0) 
    {
        // Child process

        // Handle heredoc redirection
        if (delimiter) 
        {
            if (handle_heredoc(delimiter) == -1) 
            {
                exit(1);
            }
        }

        // Handle input and output redirection
        if (handle_redirection(infile, outfile, append) == -1) 
        {
            exit(1);
        }

        // Now execute the command
        char *args[] = {cmd, NULL};
        execve(cmd, args, environ);

        // If execve fails
        perror("execve failed");
        exit(1);
    } 
    else if (pid > 0) 
    {
        // Parent process
        int status;
        waitpid(pid, &status, 0);  // Wait for the child to finish
    } 
    else 
    {
        perror("fork failed");
    }
}

// Main function for testing
int main() 
{
    char *cmd = "echo";  // Example command
    char *infile = "input.txt";  // Example input redirection file
    char *outfile = "output.txt";  // Example output redirection file
    char *delimiter = "END";  // Example heredoc delimiter
    int append = 0;  // Use 1 for append (>>), 0 for overwrite (>)

    // Run the command with redirection
    execute_command_with_redirection(cmd, infile, outfile, append, delimiter);

    return (0);
}