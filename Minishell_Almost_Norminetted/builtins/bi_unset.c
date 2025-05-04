#include "../inc/minishell.h"

/**
 * Remove an environment variable from the linked list
 */
int	remove_env_var(char *varname, t_env **env)
{
	t_env	*current;
	t_env	*previous;

	if (!env || !*env || !varname)
		return (ERROR);
	current = *env;
	previous = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, varname) == 0)
		{
			if (previous == NULL)
				*env = current->next;
			else
				previous->next = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return (SUCCESS);
		}
		previous = current;
		current = current->next;
	}
	return (SUCCESS);
}

/**
 * Unset environment variables (no options supported)
 */
int	bi_unset(char **args, t_env **env)
{
	int	i;
	int	status;

	status = SUCCESS;
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
				free_env(*env);
		}
		i++;
	}
	return (status);
}
