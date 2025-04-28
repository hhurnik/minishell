#include "inc/minishell.h"

int		g_exit_status = 0; // Globalna zmienna stanu

// Count the number of nodes in the environment list
int	count_env_nodes(t_env *env)
{
	int		count;
	t_env	*current;
	// var: env(param), count, current = 3 <= 5 OK

	count = 0;
	current = env;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

//Create a single "KEY=VALUE" string entry.
//Returns the allocated string or NULL on error (malloc fail, NULL key/value).
char	*create_env_entry(t_env *node)
{
	char	*key_eq;
	char	*entry;
	// var: node(param), key_eq, entry = 3 <= 5 OK
	// Check for NULL key or value within the node first
	if (!node || !node->key || !node->value)
		return (NULL);
	key_eq = ft_strjoin(node->key, "=");
	if (!key_eq)
		return (NULL);
	entry = ft_strjoin(key_eq, node->value);
	free(key_eq); // Free intermediate string immediately
	if (!entry)
		return (NULL); // Check final string allocation
	return (entry);
}

//Free the partially allocated array on error
//Frees arr[0] up to arr[count-1], then frees arr itself
void	free_array_on_error(char **arr, int count)
{
	int	i; // var: arr(param), count(param), i = 3 <= 5 OK
	i = 0;
	while (i < count)
	{
		free(arr[i]); // Free only the elements that were successfully allocated
		i++;
	}
	free(arr); // Free the array container
}

int	populate_env_array(t_env *env, char **arr)
{
	int		i;
	t_env	*current;
	char	*entry;

	i = 0;
	current = env;
	while (current)
	{
		entry = create_env_entry(current);
		if (!entry)
			return (-1); // Signal error during entry creation
		arr[i] = entry;
		i++;
		current = current->next;
	}
	arr[i] = NULL; // Null-terminate the array
	return (i); // Return the count of successfully added entries
}

/*
 * Main Function: Converts the environment linked list to a NULL-terminated
 * array of strings ("KEY=VALUE"). Returns NULL on any error.
 */
char	**env_to_arr(t_env *env)
{
	int		count;
	char	**arr;
	int		populate_result;

	if (!env)
		return (NULL);
	count = count_env_nodes(env);
	arr = malloc((count + 1) * sizeof(char *));
	if (!arr)
	{
		perror("minishell: malloc");
		return (NULL);
	}
	populate_result = populate_env_array(env, arr);
	if (populate_result == -1) // Check for failure
	{
		free_array_on_error(arr, count); // Free all allocated entries
		return (NULL);
	}
	return (arr);
}
