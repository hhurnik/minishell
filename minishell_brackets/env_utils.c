#include "inc/minishell.h"

t_env	*env_new(const char *key, const char *value)
{
	t_env	*new_var;

	new_var = (t_env *)malloc(sizeof(t_env));
	if (!new_var)
		return (NULL);
	new_var->key = ft_strdup(key);
	if (!new_var->key)
	{
		free(new_var);
		return (NULL);
	}
	if (value)
		new_var->value = ft_strdup(value);
	else
		new_var->value = ft_strdup("");
	if (!new_var->value)
	{
		free(new_var->key);
		free(new_var);
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

//dodaje new node to the end of a linked list of t_env nodes
void	env_add_back(t_env **env, t_env *new_node)
{
	//if empty list, zrob nowy node
	if (!*env)
	{
		*env = new_node;
	}
	else
	{
		t_env *last = *env;
		//dopoki ten node ma nastepny node, keep going (last idzie a do final node)
		while (last->next)
		{
			last = last->next;
		}
		last->next = new_node;
	}
}

void init_env(char **envp, t_env **env)
{
    int i = 0;
    while (envp[i])
    {
        char *eq = ft_strchr(envp[i], '=');
        if (eq)
        {
            int key_len = eq - envp[i];
            char *key = ft_substr(envp[i], 0, key_len);
            if (!key) { /* handle error */ continue; }
            char *value = ft_strdup(eq + 1);
            if (!value) 
            { 
                free(key); /* handle error */ 
                continue; 
            }
            t_env *new_node = env_new(key, value);
            if (!new_node) 
            {
                free(key);
                free(value);
                perror("minishell: malloc");
                free_env(*env);
                exit(EXIT_FAILURE);
            }
            // Since env_new made copies, free the originals
            free(key);
            free(value);
            env_add_back(env, new_node);
        }
        i++;
    }
}

