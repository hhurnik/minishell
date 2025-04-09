#include "inc/minishell.h"


/**
 * Wyodrębnia nazwę zmiennej środowiskowej (np. z "$USER" → "USER").
 * - str: wskaźnik do znaku '$' (np. "USER=john").
 * Zwraca: nazwa zmiennej (malloc).
 */
char	*extract_var_name(char *str)
{
	int len = 0;
	while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
	{
		len++;
	}
	return (ft_substr(str, 0, len));
}

/**
 * Zamienia zmienne środowiskowe (np. $HOME) na ich wartości.
 * Obsługuje również $? (status ostatniej komendy).
 */
//looks for tokens that are words - of type t_word,
//checks if they contain a $ sign - env variable that needs to be expanded
void	expand_variables(t_token *tokens, t_env *env)
{
	while (tokens)
	{
		if (tokens->type == T_WORD && ft_strchr(tokens->value, '$'))
		{
			char *expanded = expand_str(tokens->value, env);
			free(tokens->value);
			tokens->value = expanded;
		}
		tokens = tokens->next;
	}
}

// Przykład: Zamienia "$USER" na wartość zmiennej USER
char	*expand_str(char *str, t_env *env)
{
	char *result = ft_strdup("");
	int i = 0;

	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] == '?')
		{
			result = ft_strjoin_free(result, ft_itoa(g_exit_status));
			i += 2;
		}
		else if (str[i] == '$')
		{
			char *var_name = extract_var_name(str + i + 1);
			char *var_value = get_env_value(var_name, env);
			result = ft_strjoin_free(result, var_value);
			i += ft_strlen(var_name) + 1;
			free(var_name);
		}
		else
		{
			// Dodaj zwykły znak
		}
		i++;
	}
	return (result);
}
