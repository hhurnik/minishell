/*It is used to mark variables and functions to be passed to child 
processes. Basically, a variable will be included in child 
process environments without affecting other environments. */

/*When a program is invoked it is given an array of strings called the environment. This is a list of name-value pairs, of the form name=value.

The shell provides several ways to manipulate the environment. 
On invocation, the shell scans its own environment and creates 
a parameter for each name found, automatically marking it for 
export to child processes. Executed commands inherit the environment.
 The export and declare  -x commands allow parameters and functions 
 to be added to and deleted from the environment. If the value of 
 a parameter in the environment is modified, the new value becomes 
 part of the environment, replacing the old. The environment 
 inherited by any executed command consists of the shell's initial 
 environment, whose values may be modified in the shell, less any 
 pairs removed by the unset command, plus any additions via the export and declare -x commands.*/

/*The export command in a shell (like bash, zsh, etc.) is used to set environment variables 
and make them available to child processes (like programs launched from the shell).*/

//     -------- without arguments it lists all environment variables that are marked -----------
//for export
//why use export? because by default, shell variables are local to the shell,
//when you export them, you make them part of the environment, which is passed to child processes like ls, vim, gcc
 
 #include <stdio.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include "ms.h"



void bi_export(char *envp[])
{
    int i = 0;

    while (envp[i])
    {
        write(1, envp[i], ft_strlen(envp[i]));
        write(1, "\n", 1);
        i++;
    }
}

