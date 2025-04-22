#include "inc/minishell.h"


int		g_exit_status = 0; // Globalna zmienna stanu

char	**env_to_arr(t_env *env)
{
	if (!env)
		return (NULL); // Zabezpieczenie przed NULL-em

	int count = 0;
	t_env *current = env;
	while (current)
	{
		count++;
		current = current->next;
	}

	char **arr = malloc((count + 1) * sizeof(char *));
	if (!arr)
		return (NULL);

	current = env;
	int i = 0;
	int error = 0; // Flaga błędu

	while (current && !error)
	{
		if (!current->key || !current->value)
		{ // Zabezpieczenie przed NULL
			error = 1;
			break ;
		}

		char *key_eq = ft_strjoin(current->key, "=");
		if (!key_eq)
		{
			error = 1;
			break ;
		}

		char *entry = ft_strjoin(key_eq, current->value);
		free(key_eq);
		if (!entry)
		{
			error = 1;
			break ;
		}

		arr[i++] = entry;
		current = current->next;
	}

	if (error)
	{
		// Zwolnij tylko do ostatniego poprawnego elementu (0..i-1)
		while (i > 0)
		{
			free(arr[--i]);
		}
		free(arr);
		return (NULL);
	}

	arr[i] = NULL; // Kończymy tablicę NULL
	return (arr);
}
