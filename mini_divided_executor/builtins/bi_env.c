#include "../inc/minishell.h"


/**
 * env builtin command - wypisuje all env variables
 * Format: NAME=VALUE (one per line)
 */
int bi_env(char **args, t_env **env)
{
    t_env *current;
    
    // Check for unsupported options/arguments
    if (args[1])
    {
        ft_putstr_fd("env: too many arguments\n", STDERR_FILENO);
        return (ERROR);
    }
    
    // Print all env variables
    current = *env;
    while (current)
    {
        // wypis tylko variables with values (standard env behavior)
        if (current->value && current->value[0] != '\0')
        {
            ft_putstr_fd(current->key, STDOUT_FILENO);
            ft_putstr_fd("=", STDOUT_FILENO);
            ft_putstr_fd(current->value, STDOUT_FILENO);
            ft_putstr_fd("\n", STDOUT_FILENO);
        }
        
        current = current->next;
    }
    return (SUCCESS);
}