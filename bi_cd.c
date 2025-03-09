#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//jesli samo cd - go to the home directory
int builtin_cd(char *argv[]) 
{
    // If no arguments, change to the home directory
    if (argv[1] == NULL) 
    {
        // Get home directory from the environment variable
        char *home = getenv("HOME");
        if (home == NULL) 
        {
            // If HOME is not set, print an error
            perror("cd");
            return 1;
        }
        //jesli nie udalo sie mienic na home
        if (chdir(home) != 0) 
        {
            perror("cd");
            return (1);
        }
    }
    // If there's an argument, attempt to change to the specified directory
    else 
    {
        // Change to the specified directory (can be relative or absolute)
        if (chdir(argv[1]) != 0) 
        {
            perror("cd");
            return (1);
        }
    }

    // Success
    return (0);
}