#include "inc/minishell.h"

int		g_exit_status = 0;

int	count_env_nodes(t_env *env)
{
	int		count;
	t_env	*current;

	count = 0;
	current = env;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

char	*create_env_entry(t_env *node)
{
	char	*key_eq;
	char	*entry;

	if (!node || !node->key || !node->value)
		return (NULL);
	key_eq = ft_strjoin(node->key, "=");
	if (!key_eq)
		return (NULL);
	entry = ft_strjoin(key_eq, node->value);
	free(key_eq);
	if (!entry)
		return (NULL);
	return (entry);
}

void	free_array_on_error(char **arr, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
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
			return (-1);
		arr[i] = entry;
		i++;
		current = current->next;
	}
	arr[i] = NULL;
	return (i);
}

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
	if (populate_result == -1)
	{
		free_array_on_error(arr, count);
		return (NULL);
	}
	return (arr);
}
