#include "inc/minishell.h"

//caly file - podzielony init_env
char	*extract_key(char *env_var, int *key_len)
{
	char	*eq;
	char	*key;

	eq = ft_strchr(env_var, '=');
	if (!eq)
		return (NULL);
	*key_len = eq - env_var;
	key = ft_substr(env_var, 0, *key_len);
	return (key);
}

int	handle_env_node_creation(char *key, char *value, t_env **env)
{
	t_env	*new_node;

	new_node = env_new(key, value);
	if (!new_node)
	{
		free(key);
		free(value);
		perror("minishell: malloc");
		free_env(*env);
		exit(EXIT_FAILURE);
	}
	env_add_back(env, new_node);
	free(key);
	free(value);
	return (1);
}

int	process_single_env_var(char *env_var, t_env **env)
{
	char	*key;
	char	*value;
	int		key_len;

	key = extract_key(env_var, &key_len);
	if (!key)
		return (0);
	value = ft_strdup(ft_strchr(env_var, '=') + 1);
	if (!value)
	{
		free(key);
		return (0);
	}
	return (handle_env_node_creation(key, value, env));
}

void	init_env(char **envp, t_env **env)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		process_single_env_var(envp[i], env);
		i++;
	}
}
