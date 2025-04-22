

// deletes the variables during program execution
//- both functions and shell variables - only shell variables

// if a variable is readonly you cannot unset it

// search internal environment variable list, remove the matching entry

#include "../inc/minishell.h"


// // Compare a variable name with the key in a t_env structure
// int	env_cmpr(t_env *env, char *varname)
// {
// 	if (!env || !env->key || !varname) // Check for NULL pointers
// 		return (0);

// 	// Compare the key in t_env with varname
// 	return (ft_strncmp(env->key, varname, ft_strlen(varname)) == 0);
// }

// // Remove an environment variable from the list
// void	remove_env_var(char *varname, t_env **env)
// {
// 	int i = 0;

// 	while (env[i])
// 	{
// 		// Use env_cmpr to compare the variable name
// 		if (env_cmpr(env[i], varname))
// 		{
// 			free(env[i]->key);
// 			free(env[i]->value);
// 			free(env[i]);

// 			// Shift all remaining entries up
// 			while (env[i + 1])
// 			{
// 				env[i] = env[i + 1];
// 				i++;
// 			}
// 			env[i] = NULL;
// 			break ; // Only one instance possible, so break
// 		}
// 		i++;
// 	}
// }

// // Updated bi_unset function to return an int (g_state)
// int	bi_unset(char **argv, t_env **env)
// {
// 	int i = 1;
// 	int g_state = 0; // Initialize g_state to success (0)

// 	if (!argv || !env) // Check for NULL pointers
// 		return (1);    // Return failure status

// 	while (argv[i])
// 	{
// 		// Optional: validate variable name here (skip invalid names)
// 		remove_env_var(argv[i], env);
// 		i++;
// 	}

// 	return (g_state); // Return the status
// }


/**
 * Check if a variable name is valid for unset
 * Valid names: start with letter or underscore, followed by letters, numbers, or underscores
 */
static int is_valid_identifier(char *name)
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