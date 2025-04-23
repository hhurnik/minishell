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

///diala, ale be {}
// char	*expand_str(char *str, t_env *env)
// {
//     char *result = ft_strdup("");
//     int i = 0;

//     while (str[i])
//     {
//         if (str[i] == '$' && str[i + 1] == '?') // Handle special case for $?
//         {
//             result = ft_strjoin_free(result, ft_itoa(g_exit_status));
//             i += 2;
//         }
//         else if (str[i] == '$') // Handle variable expansion
//         {
//             char *var_name = extract_var_name(str + i + 1);
//             if (var_name[0] == '\0') // If no valid variable name, treat as literal $
//             {
//                 result = ft_strjoin_free(result, "$");
//                 i++; // Skip the '$'
//             }
//             else
//             {
//                 char *var_value = get_env_value(var_name, env);
                
//                 // Use empty string if var_value is NULL
//                 if (var_value)
//                     result = ft_strjoin_free(result, var_value);
//                 else
//                     result = ft_strjoin_free(result, "");
//                 i += ft_strlen(var_name) + 1;
//             }
//             free(var_name);
//         }
//         else // Handle regular characters
//         {
//             char c[2] = {str[i], '\0'};
//             result = ft_strjoin_free(result, c);
//             i++;
//         }
//     }
//     return (result);
// }



///handles {}
char *expand_str(char *str, t_env *env)
{
    char *result = ft_strdup("");
    int i = 0;

    while (str[i])
    {
        if (str[i] == '$' && str[i + 1] == '?') // Handle special case for $?
        {
            result = ft_strjoin_free(result, ft_itoa(g_exit_status));
            i += 2;
        }
        else if (str[i] == '$' && str[i + 1] == '{') // Handle ${VAR} format
        {
            i += 2; // Skip ${ characters
            int start = i;
            
            // Find the closing brace
            while (str[i] && str[i] != '}')
                i++;
                
            if (str[i] == '}')
            {
                char *var_name = ft_substr(str, start, i - start);
                char *var_value = get_env_value(var_name, env);
                
                if (var_value)
                    result = ft_strjoin_free(result, var_value);
                else
                    result = ft_strjoin_free(result, "");
                    
                free(var_name);
                i++; // Skip closing brace
            }
        }
        else if (str[i] == '$') // Handle standard $VAR format
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
                
                if (var_value)
                    result = ft_strjoin_free(result, var_value);
                else
                    result = ft_strjoin_free(result, "");
                    
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
