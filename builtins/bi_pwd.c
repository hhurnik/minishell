#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int builtin_pwd(char *argv[]) 
{
    // Check if there are any arguments (pwd with no options)
    if (argv[1] != NULL) 
    {
        // If there are arguments, print an error (pwd doesn't take options)
        perror("pwd: no arguments are allowed");
        return (1);
    }

    // Allocate a buffer to store the current working directory
    char *cwd = malloc(1024); // Or any other buffer size ????
    if (!cwd) 
    {
        perror("malloc failed");
        return (1);
    }

    // Get the current working directory
    //getcwd - kopiuje an absolute pathname of the current working directory to the array pointed to by buf, which is of length size.
    if (getcwd(cwd, 1024) == NULL)  
    {
        // If getcwd fails, print an error message
        perror("pwd");
        free(cwd);
        return (1);
    }

    // Print the current working directory
    write(1, cwd, strlen(cwd));
    write(1, "\n", 1); // Add a newline at the end - wróc prompta

    free(cwd);
    return (0);
}