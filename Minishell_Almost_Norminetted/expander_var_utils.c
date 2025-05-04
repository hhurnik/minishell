#include "inc/minishell.h"

int	is_special_var(char *str, int i)
{
	return (str[i] == '$' && str[i + 1] == '?');
}

int	is_braced_var(char *str, int i)
{
	return (str[i] == '$' && str[i + 1] == '{');
}

int	is_standard_var(char *str, int i)
{
	return (str[i] == '$');
}

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
