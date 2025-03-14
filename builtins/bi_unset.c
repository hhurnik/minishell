
//deletes the variables during program execution 
//- both functions and shell variables - only shell variables

//if a variable is readonly you cannot unset it


//search internal environment variable list, remove the matching entry

#include <stdlib.h>
#include <string.h>
#include "ms.h"

//bedziemy miec w mainie
char **copy_env(char *envp[])
{
    int i = 0;
    char **env;

    while (envp[i])
        i++;

    env = malloc(sizeof(char *) * (i + 1));
    if (!env)
        return (NULL);


    
    while (envp[i]) 
    {
        env[i] = ft_strdup(envp[i]);
        i++;
    }

    env[i] = NULL;
    return (env);
}

void remove_env_var(char *varname, char **env) 
{
    int i = 0;
    int len = ft_strlen(varname);

    while (env[i]) 
    {
        // Match varname and make sure it ends with '='
        if (ft_strncmp(env[i], varname, len) == 0 && env[i][len] == '=') 
        {
            free(env[i]);

            // Shift all remaining entries up
            while (env[i + 1]) 
            {
                env[i] = env[i + 1];
                i++;
            }
            env[i] = NULL;
            break; // Only one instance possible, so break
        }
        i++;
    }
}

void ft_unset(char **argv, char **env) 
{
    int i = 1;

    while (argv[i]) 
    {
        // Optional: validate variable name here (skip invalid names)
        remove_env_var(argv[i], env);
        i++;
    }
}