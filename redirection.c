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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

extern char **environ;  // To access environment variables

// Function to find the full path of a command in the directories listed in PATH
char *find_command_in_path(const char *cmd) 
{
    char *path = getenv("PATH");
    if (!path) return NULL;

    char *path_copy = strdup(path);
    char *dir = strtok(path_copy, ":");
    while (dir) 
    {
        char *full_cmd = malloc(strlen(dir) + strlen(cmd) + 2);
        if (!full_cmd) 
        {
            free(path_copy);
            return NULL;
        }
        sprintf(full_cmd, "%s/%s", dir, cmd);

        // Check if the command is executable
        if (access(full_cmd, X_OK) == 0) 
        {
            free(path_copy);
            return full_cmd;
        }

        free(full_cmd);
        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return NULL;  // Command not found
}

// Function to execute an external command
void execute_command(char *cmd) 
{
    char *cmd_path = find_command_in_path(cmd);
    if (!cmd_path) {
        fprintf(stderr, "minishell: command not found: %s\n", cmd);
        return;
    }

    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        char *args[] = {cmd, NULL};
        execve(cmd_path, args, environ);  // Execute the command

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

    free(cmd_path);  // Free the path memory after use
}

// Main function with the shell loop
int main() 
{
    char *input;

    // Infinite loop for the shell prompt
    while (1) 
    {
        // Display prompt and read input using readline
        input = readline("minishell> ");
        if (!input) break;  // Exit the loop if input is NULL (Ctrl+D)

        if (*input) 
        {
            add_history(input);  // Add the input to the history

            // Check if it's a built-in command (only "exit" for simplicity)
            if (strncmp(input, "exit", 4) == 0) 
            {
                free(input);
                break;  // Exit the shell
            }

            // Otherwise, execute as an external command
            execute_command(input);
        }

        free(input);  // Free the input after processing
    }

    return 0;
}