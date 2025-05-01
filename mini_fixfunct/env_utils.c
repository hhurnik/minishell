#include "inc/minishell.h"

void free_env_node(t_env *node)
{
    if (node)
    {
        free(node->key);
        free(node->value);
        free(node);
    }
}

t_env *env_new(const char *key, const char *value)
{
    t_env *new_var;

    new_var = (t_env *)malloc(sizeof(t_env));
    if (!new_var)
        return (NULL);

    new_var->key = ft_strdup(key);
    if (!new_var->key)
    {
        free_env_node(new_var); // Use helper function for cleanup
        return (NULL);
    }

    if (value)
        new_var->value = ft_strdup(value);
    else
        new_var->value = ft_strdup("");

    if (!new_var->value)
    {
        free_env_node(new_var); // Use helper function for cleanup
        return (NULL);
    }

    new_var->next = NULL;
    return (new_var);
}

t_env	*get_env(const char *key, t_env *env)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}


char	*get_env_value(const char *key, t_env *env)
{
	t_env	*var;

	var = get_env(key, env);
	if (!var)
		return (NULL);
	return (var->value);
}



// dodaje new node to the end of a linked list of t_env nodes
void	env_add_back(t_env **env, t_env *new_node)
{
	t_env	*last;
	// if empty list, zrob nowy node
	if (!*env)
	{
		*env = new_node;
	}
	else
	{
		last = *env;
		// dopoki ten node ma nastepny node,
		// keep going(last idzie a do final node)
		while (last->next)
		{
			last = last->next;
		}
		last->next = new_node;
	}
}
