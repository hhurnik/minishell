#include "../inc/minishell.h"

// check if a variable name is valid for unset
// valid names: start with letter or underscore, then letters,
//	numbers or underscores
//it checks all, key and also a value after "=", 
//wiec ponizej napisalam nowy
// int	is_valid_identifier(char *name)
// {
// 	int	i;

// 	if (!name || !*name)
// 		return (0);
// 	// First character must be letter or underscore
// 	if (!ft_isalpha(name[0]) && name[0] != '_')
// 		return (0);
// 	// Remaining characters must be letters, digits or underscores
// 	i = 1;
// 	while (name[i])
// 	{
// 		if (!ft_isalnum(name[i]) && name[i] != '_')
// 			return (0);
// 		i++;
// 	}
// 	return (1);
// }







// //moje
// int	is_valid_identifier(char *name)
// {
// 	int	i;
// 	char *equals_pos;

// 	if (!name || !*name)
// 		return (0);

// 	equals_pos = ft_strchr(name, '=');
// 	//utnij jesli =
// 	if (equals_pos)
// 		*equals_pos = '\0';

// 	// 1st character must be a letter or underscore
// 	if (!ft_isalpha(name[0]) && name[0] != '_')
// 	{
// 		if (equals_pos)
// 			*equals_pos = '='; // Restore '=' if we modified it
// 		return (0);
// 	}

// 	// Check remaining characters (only up to '=' if it exists)
// 	i = 1;
// 	while (name[i] && name[i] != '=')
// 	{
// 		if (!ft_isalnum(name[i]) && name[i] != '_')
// 		{
// 				if (equals_pos)
// 					*equals_pos = '='; // Restore '=' if modified
// 				return (0);
// 			}
// 			i++;
// 		}
		
// 		// Restore '=' if we modified it (for further processing)
// 		if (equals_pos)
// 			*equals_pos = '=';
// 		return (1);
// }

/**
 * Remove an environment variable from the linked list
 */
// //fukcja 25 linijek bez komentarzy OK
int	remove_env_var(char *varname, t_env **env)
{
	t_env	*current;
	t_env	*previous;

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
int	bi_unset(char **args, t_env **env)
{
	int	i;
	int	status;

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
		{
			if (remove_env_var(args[i], env) != SUCCESS)
				free_env(*env); //dodane valgrind
		}
		i++;
	}
	return (status);
}

