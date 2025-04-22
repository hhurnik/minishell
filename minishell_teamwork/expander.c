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


void expand_variables(t_token *tokens, t_env *env) 
{
    while (tokens) 
	{
        if (tokens->type == T_DOUBLE_QUOTED || tokens->type == T_WORD) 
		{
            // Rozwijaj zmienne tylko w podwójnych cudzysłowach i zwykłych słowach
            char *expanded = expand_str(tokens->value, env);
            free(tokens->value);
            tokens->value = expanded;
        }
        // Dla T_SINGLE_QUOTED nie wykonuj ekspansji!
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
			char c[2] = {str[i], '\0'};
            result = ft_strjoin_free(result, c);
		}
		i++;
	}
	return (result);
}
