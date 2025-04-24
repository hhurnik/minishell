#include "../inc/minishell.h"

//check if a variable name is valid for unset
//valid names: start with letter or underscore, then letters, numbers or underscores
int is_valid_identifier(char *name)
{
    int i;
    
    if (!name || !*name)
        return (0);
    
    // First character must be letter or underscore
    if (!ft_isalpha(name[0]) && name[0] != '_')
        return (0);
    
    // Remaining characters must be letters, digits or underscores
    i = 1;
    while (name[i])
    {
        if (!ft_isalnum(name[i]) && name[i] != '_')
            return (0);
        i++;
    }
    
    return (1);
}

/**
 * Remove an environment variable from the linked list
 */
int remove_env_var(char *varname, t_env **env)
{
    t_env *current;
    t_env *previous;
    
    if (!env || !*env || !varname)
        return (ERROR);
    
    current = *env;
    previous = NULL;
    
    // Find the variable in the linked list
    while (current)
    {
        // Compare variable names exactly (not just a prefix)
        if (ft_strcmp(current->key, varname) == 0)
        {
            // Remove from beginning of list
            if (previous == NULL)
                *env = current->next;
            // Remove from middle/end of list
            else
                previous->next = current->next;
                
            // Free node memory
            free(current->key);
            free(current->value);
            free(current);
            return (SUCCESS);
        }
        
        previous = current;
        current = current->next;
    }
    
    return (SUCCESS); // Variable not found, not an error
}

/**
 * Unset environment variables (no options supported)
 */
int bi_unset(char **args, t_env **env)
{
    int i;
    int status;
    
    status = SUCCESS;
    
    // No arguments is not an error
    if (!args[1])
        return (SUCCESS);
    
    i = 1;
    while (args[i])
    {
        if (!is_valid_identifier(args[i]))
        {
            ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
            ft_putstr_fd(args[i], STDERR_FILENO);
            ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
            status = ERROR;
        }
        else
            remove_env_var(args[i], env);
        
        i++;
    }
    
    return (status);
}