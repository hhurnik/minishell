// without arguments it lists all environment variables that are marked
// why use export? because by default, shell variables are local to the shell,
// when you export them, you make them part of the environment,
//	which is passed to child processes like ls, vim, gcc

#include "../inc/minishell.h"

// calculate the length of an environment variable's value
size_t	env_len(t_env *env)
{
	if (!env || !env->value)
		return (0);
	return (ft_strlen(env->value));
}

// print all environment variables in export format
// format: declare -x KEY="VALUE"
void	print_env_export_format(t_env *env)
{
	while (env)
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(env->key, STDOUT_FILENO);
		ft_putstr_fd("=\"", STDOUT_FILENO);
		ft_putstr_fd(env->value, STDOUT_FILENO);
		ft_putstr_fd("\"\n", STDOUT_FILENO);
		env = env->next;
	}
}

int	parse_env_var(const char *var, char **key, char **value)
{
	char	*equals;

	equals = ft_strchr(var, '=');
	if (!equals)
	{
		*key = ft_strdup(var);
		*value = ft_strdup("");
	}
	else
	{
		*key = ft_substr(var, 0, equals - var);
		*value = ft_strdup(equals + 1);
	}
	if (!*key || !*value)
	{
		free(*key);
		free(*value);
		return (ERROR);
	}
	return (SUCCESS);
}

/* Function 2: Update or add the environment variable to the list */
int	set_env_var(const char *var, t_env **env)
{
	char	*key;
	char	*value;
	t_env	*existing;

	if (parse_env_var(var, &key, &value) == ERROR)
	{
		free(key); //dodane valgrind
		free(value); //dodane valgrind
		return (ERROR);
	}
	existing = get_env(key, *env);
	if (existing)
	{
		if (ft_strchr(var, '='))
			// Only update value if there was an equals sign
		{
			free(existing->value);
			existing->value = value;
		}
		free(key);
	}
	else
	{
		env_add_back(env, env_new(key, value));
		free(key);
		free(value);
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
