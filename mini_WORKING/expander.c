#include "inc/minishell.h"


/**
 * Wyodrębnia nazwę zmiennej środowiskowej (np. z "$USER" → "USER").
 * - str: wskaźnik do znaku '$' (np. "USER=john").
 * Zwraca: nazwa zmiennej (malloc).
 */
char	*extract_var_name(char *str)
{
	int len;

	len = 0;
	while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
	{
		len++;
	}
	return (ft_substr(str, 0, len));
}

void	expand_variables(t_token *tokens, t_env *env)
{
	char *expanded;

	while (tokens)
	{
		if (tokens->type == T_SINGLE_QUOTED)
		{
			// NIE ROBIMY NIC
		}
		else if (tokens->type == T_DOUBLE_QUOTED || tokens->type == T_WORD)
		{
			expanded = expand_str(tokens->value, env);
			if (expanded)
			{
				free(tokens->value);
				tokens->value = expanded;
			}
		}
		tokens = tokens->next;
	}
}

int	handle_braced_var(char **result, char *str, int i, t_env *env)
{
	int start;
	char *var_name;
	char *var_value;

	start = i + 2;
	while (str[i] && str[i] != '}')
		i++;
	if (str[i] == '}')
	{
		var_name = ft_substr(str, start, i - start);
		var_value = get_env_value(var_name, env);
		*result = ft_strjoin_free(*result, var_value ? var_value : "");
		free(var_name);
		i++;
	}
	return (i);
}
int	handle_standard_var(char **result, char *str, int i, t_env *env)
{
	char *var_name;
	char *var_value;

	var_name = extract_var_name(str + i + 1);
	if (var_name[0] == '\0')
	{
		*result = ft_strjoin_free(*result, "$");
		free(var_name);
		return (i + 1);
	}
	var_value = get_env_value(var_name, env);
	*result = ft_strjoin_free(*result, var_value ? var_value : "");
	i += ft_strlen(var_name) + 1;
	free(var_name);
	return (i);
}

char	*expand_str(char *str, t_env *env)
{
	char *result;
	int i;

	result = ft_strdup("");
	if (!result)
		return (NULL); // bylo samo return null
	i = 0;
	while (str[i])
	{
		if (is_special_var(str, i))
			i = handle_special_var(&result, i);
		else if (is_braced_var(str, i))
			i = handle_braced_var(&result, str, i, env);
		else if (is_standard_var(str, i))
			i = handle_standard_var(&result, str, i, env);
		else
		{
			char *old_result = result;
			result = ft_strjoin_char(old_result, str[i]);
			if (!result)
			{
				free(old_result);
				return (NULL);
			}
			i++;
		}
	}
	return (result);
}

// ////from expandet utils.c

// int	is_special_var(char *str, int i)
// {
// 	return (str[i] == '$' && str[i + 1] == '?');
// }

// int	is_braced_var(char *str, int i)
// {
// 	return (str[i] == '$' && str[i + 1] == '{');
// }

// int	is_standard_var(char *str, int i)
// {
// 	return (str[i] == '$');
// }

// void	append_char(char **result, char c)
// {
// 	char tmp[2];
// 	char *new_result;

// 	tmp[0] = c;
// 	tmp[1] = '\0';
// 	new_result = ft_strjoin(*result, tmp); // Don't free here
// 	if (!new_result)
// 	{
// 		free(*result);
// 		*result = NULL;
// 		return ;
// 	}
// 	*result = new_result;
// }

// int	handle_special_var(char **result, int i)
// {
// 	char *exit_status_str = ft_itoa(g_exit_status);
// 	if (!exit_status_str)
// 		return (-1); // Error handling

// 	char *new_result = ft_strjoin_free(*result, exit_status_str);
// 	free(exit_status_str); // Free the string after using it

// 	if (!new_result)
// 		return (-1);

// 	*result = new_result;
// 	return (i + 2);
// }
