#include "../inc/minishell.h"

// calculate the length of an environment variable's value
size_t	env_len(t_env *env)
{
	if (!env || !env->value)
		return (0);
	return (ft_strlen(env->value));
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

//moje, ponizej proba z usunieciem leakow
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
