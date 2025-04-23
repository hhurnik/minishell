#include "inc/minishell.h"


/**
 * Wyodrębnia nazwę zmiennej środowiskowej (np. z "$USER" → "USER").
 * - str: wskaźnik do znaku '$' (np. "USER=john").
 * Zwraca: nazwa zmiennej (malloc).
 */
char	*extract_var_name(char *str)
{
	int len = 0;
	while (str[len] && ((ft_isalnum(str[len]) && str[len] == '{' && str[len] =='}') || str[len] == '_'))
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
//stare, dziala ale nie na echo "$"
// char	*expand_str(char *str, t_env *env)
// {
// 	char *result = ft_strdup("");
// 	int i = 0;

// 	while (str[i])
// 	{
// 		if (str[i] == '$' && str[i + 1] == '?')
// 		{
// 			result = ft_strjoin_free(result, ft_itoa(g_exit_status));
// 			i += 2;
// 		}
// 		else if (str[i] == '$')
// 		{
// 			char *var_name = extract_var_name(str + i + 1);
// 			char *var_value = get_env_value(var_name, env);
// 			result = ft_strjoin_free(result, var_value);
// 			i += ft_strlen(var_name) + 1;
// 			free(var_name);
// 		}
// 		else
// 		{
// 			char c[2] = {str[i], '\0'};
//             result = ft_strjoin_free(result, c);
// 		}
// 		i++;
// 	}
// 	return (result);
// }

char	*expand_str(char *str, t_env *env)
{
    char *result = ft_strdup("");
    int i = 0;

    while (str[i])
    {
        if (str[i] == '$' && (str[i + 1] == '{' || str[i + 1] == '('))
        {
            ft_putstr_fd("Squish: syntax error: Squish doesn't handle brackets \n", STDERR_FILENO);
            break;
        }
        if (str[i] == '$' && str[i + 1] == '?') // Handle special case for $?
        {
            result = ft_strjoin_free(result, ft_itoa(g_exit_status));
            i += 2;
        }
        else if (str[i] == '$') // Handle variable expansion
        {
            char *var_name = extract_var_name(str + i + 1);
            if (var_name[0] == '\0') // If no valid variable name, treat as literal $
            {
                result = ft_strjoin_free(result, "$");
                i++; // Skip the '$'
            }

            else
            {
                char *var_value = get_env_value(var_name, env);
                result = ft_strjoin_free(result, var_value ? var_value : ""); // Use empty string if var_value is NULL
                i += ft_strlen(var_name) + 1;
            }
            free(var_name);
        }
        else // Handle regular characters
        {
            char c[2] = {str[i], '\0'};
            result = ft_strjoin_free(result, c);
            i++;
        }
    }
    return (result);
}
