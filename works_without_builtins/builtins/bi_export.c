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
 
#include "../inc/minishell.h"


size_t env_len(t_env *env)
{
    size_t len = 0;

    if (!env || !env->value) //envnv or its value is NULL
        return (0);

    while (env->value[len])
        len++;

    return (len);
}

int bi_export(t_env **env)
{
    int i = 0;

    if (!env) // Check if env is NULL
        return (1); // Return failure status

    while (env[i])
    {
        if (!env[i]->value) // Check if the current env value is NULL
        {
            i++;
            continue; // Skip NULL values
        }
        write(1, "declare -x ", 11); // Prefix for export-like output
        write(1, env[i]->value, env_len(env[i])); // Use env_len to calculate length
        write(1, "\n", 1);
        i++;
    }
    return (0); // Return success status
}

