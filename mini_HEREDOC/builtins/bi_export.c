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
// // format: declare -x KEY="VALUE"
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

//diala, leakami
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

int	set_env_var(const char *var, t_env **env)
{
    char	*key;
    char	*value;
    t_env	*existing;

    if (parse_env_var(var, &key, &value) == ERROR)
    {
        free(key); // Added for safety
        free(value); // Added for safety
        return (ERROR);
    }
    existing = get_env(key, *env);
    if (existing)
    {
        if (ft_strchr(var, '='))
        {
            free(existing->value);
            existing->value = value;
        }
        existing->is_exported = 1; // Mark as exported
        free(key);
    }
    else
    {
        t_env *new_node = env_new(key, value);
        if (new_node)
            new_node->is_exported = 1; // Mark as exported
        env_add_back(env, new_node);
        free(key);
        free(value);
    }
    return (SUCCESS);
}

// //moje
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



char	*ft_strndup(const char *s, size_t n)
{
	char	*new;
	size_t	len;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	if (n < len)
		len = n;
	new = (char *)malloc(len + 1);
	if (!new)
		return (NULL);
	ft_memcpy(new, s, len);
	new[len] = '\0';
	return (new);
}



// // //tego uywam w wieksosci
int is_valid_identifier(const char *arg)
{
	char	*name;
	char	*equal_pos;
	int		i;

	if (!arg || !*arg)
		return (0);

	// Get the position of '=' if present
	equal_pos = ft_strchr(arg, '=');

	// Extract the variable name (up to '=' or full string if no '=')
	if (equal_pos)
		name = ft_strndup(arg, equal_pos - arg);
	else
		name = ft_strdup(arg);

	if (!name)
		return (0);

	// Check first character: must be alpha or underscore
	if (!ft_isalpha(name[0]) && name[0] != '_')
	{
		free(name);
		return (0);
	}

	// Check remaining characters
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
		{
			free(name);
			return (0);
		}
		i++;
	}

	free(name);
	return (1);
}
