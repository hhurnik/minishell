// without arguments it lists all environment variables that are marked
// why use export? because by default, shell variables are local to the shell,
// when you export them, you make them part of the environment,
//	which is passed to child processes like ls, vim, gcc

#include "../inc/minishell.h"

// do setenv var - po podziale
int	update_existing_env(t_env *existing, const char *var, char *value)
{
	if (ft_strchr(var, '='))
	{
		free(existing->value);
		existing->value = value;
	}
	existing->is_exported = 1;
	return (SUCCESS);
}

// do setenv var - po podziale
int	add_new_env(t_env **env, char *key, char *value)
{
	t_env	*new_node;

	new_node = env_new(key, value);
	if (!new_node)
	{
		free(key);
		free(value);
		return (ERROR);
	}
	new_node->is_exported = 1;
	env_add_back(env, new_node);
	free(key);
	free(value);
	return (SUCCESS);
}

int	set_env_var(const char *var, t_env **env)
{
	char	*key;
	char	*value;
	t_env	*existing;

	if (parse_env_var(var, &key, &value) == ERROR)
	{
		free(key);
		free(value);
		return (ERROR);
	}
	existing = get_env(key, *env);
	if (existing)
	{
		update_existing_env(existing, var, value);
		free(key);
	}
	else
	{
		if (add_new_env(env, key, value) == ERROR)
			return (ERROR);
	}
	return (SUCCESS);
}

int	bi_export(char **args, t_env **env)
{
	int	i;
	int	status;

	if (!env)
		return (ERROR);
	if (!args[1])
	{
		print_env_export_format(*env);
		return (SUCCESS);
	}
	status = SUCCESS;
	i = 0;
	while (args[++i] && (status == SUCCESS || 1))
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			status = ERROR;
		}
		else if (set_env_var(args[i], env) != SUCCESS)
			status = ERROR;
	}
	return (status);
}

int	is_valid_identifier(const char *arg)
{
	int	i;

	if (!arg || (!ft_isalpha(arg[0]) && arg[0] != '_'))
		return (0);
	i = 1;
	while (arg[i] && arg[i] != '=')
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
