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

//nowe proba naleienia leakow
int handle_braced_var(char **result, char *str, int i, t_env *env)
{
    int start = i + 2; // Start after "${"
    int end = start;
    char *var_name = NULL;
    char *var_value = NULL;
    char *new_result = NULL;

    // Find the closing brace '}'
    while (str[end] && str[end] != '}')
        end++;

    if (str[end] != '}') // Closing brace not found
    {
         // Syntax error? Or just let it be? Bash might treat "${VAR" as literal.
         // For now, let's assume it should be treated literally if no '}'
         // We don't modify *result and return original index to continue parsing.
         // Or, if this should be an error, return -1. Let's return original index for now.
         // ft_putstr_fd("minishell: syntax error: unclosed brace\n", 2); // Optional error
         return (i); // Treat as literal, don't advance significantly
    }

    // Extract variable name
    var_name = ft_substr(str, start, end - start);
    if (!var_name)
    {
        perror("minishell: ft_substr failed in handle_braced_var");
        // *result is untouched, return error
        return (-1);
    }

    var_value = get_env_value(var_name, env); // Does not allocate

    // ft_strjoin_free will free the memory pointed to by *result
    new_result = ft_strjoin_free(*result, var_value ? var_value : "");

    if (!new_result)
    {
        perror("minishell: malloc error ft_strjoin_free (braced_var)");
        // *result was freed by ft_strjoin_free, var_name still exists
        free(var_name); // Free the allocated var_name
        *result = NULL; // Set caller's pointer to NULL to indicate failure state
        return (-1);    // Signal error
    }

    // Success
    *result = new_result; // Update caller's pointer
    free(var_name);       // Free the allocated var_name
    return (end + 1);     // Return index *after* the closing '}'
}

int handle_standard_var(char **result, char *str, int i, t_env *env)
{
    char *var_name = NULL;
    char *var_value = NULL;
    char *new_result = NULL;
    int name_len = 0;

    var_name = extract_var_name(str + i + 1); // Allocates var_name
    if (!var_name)
    {
        perror("minishell: extract_var_name failed");
        return (-1); // Error
    }

    name_len = ft_strlen(var_name);

    if (name_len == 0) // Special case: just '$' followed by non-alphanum/_
    {
        // ft_strjoin_free will free the memory pointed to by *result
        new_result = ft_strjoin_free(*result, "$");
        if (!new_result)
        {
            perror("minishell: malloc error ft_strjoin_free (standard_var '$')");
            free(var_name); // Free the empty var_name
            *result = NULL;
            return (-1);
        }
        *result = new_result; // Update caller's pointer
        free(var_name);       // Free the empty var_name
        return (i + 1);       // Advance past '$' only
    }

    // Regular variable name found
    var_value = get_env_value(var_name, env); // Does not allocate

    // ft_strjoin_free will free the memory pointed to by *result
    new_result = ft_strjoin_free(*result, var_value ? var_value : "");
    if (!new_result)
    {
        perror("minishell: malloc error ft_strjoin_free (standard_var)");
        // *result was freed by ft_strjoin_free, var_name still exists
        free(var_name); // Free the allocated var_name
        *result = NULL;
        return (-1);
    }

    // Success
    *result = new_result;  // Update caller's pointer
    free(var_name);        // Free the allocated var_name
    return (i + name_len + 1); // Advance past '$' and the variable name
}


char *expand_str(char *str, t_env *env)
{
    char *result = NULL;
    char *temp_result = NULL;
    int i = 0;
    // int original_i = 0; // No longer needed with -1 error check
    bool error_occurred = false; // Flag to track errors

    result = ft_strdup(""); // Allocate initial empty string
    if (!result) {
        perror("minishell: malloc error in expand_str initial allocation");
        return (NULL); // Cannot even start
    }

    while (str[i] && !error_occurred) {
        // original_i = i; // Store index before potential modification by handlers

        if (is_special_var(str, i)) {
            i = handle_special_var(&result, i);
            if (i < 0) error_occurred = true; // Handler signals error (-1)
        }
        else if (is_braced_var(str, i)) {
             // Handle return value: If it returns < 0, it's an error.
             // If it returns >= original_i, it's success.
            int next_i = handle_braced_var(&result, str, i, env);
            if (next_i < 0) { // Check for explicit error signal
                error_occurred = true;
            } else {
                i = next_i; // Update index only on success
            }
        }
        else if (is_standard_var(str, i)) {
             int next_i = handle_standard_var(&result, str, i, env);
             if (next_i < 0) { // Check for explicit error signal
                 error_occurred = true;
             } else {
                 i = next_i; // Update index only on success
             }
        }
        else { // Append regular character
            temp_result = ft_strjoin_char(result, str[i]); // Pass current result
            if (!temp_result) {
                perror("minishell: malloc error in ft_strjoin_char within expand_str");
                error_occurred = true; // Set error flag
                // Don't update 'result' yet, it needs to be freed below
            } else {
                // If successful, ft_strjoin_char already freed the old 'result'.
                result = temp_result; // Update result to point to the new string
                i++;
            }
        }

        // If an error occurred in this iteration, break the loop
        if (error_occurred) {
            break;
        }

    } // end while

    // --- Cleanup Check ---
    if (error_occurred) {
        if (result) { // Check if result holds any allocated memory
            free(result); // Free the partially constructed string
        }
        return (NULL); // Signal error to the caller by returning NULL
    }

    // --- Success ---
    return (result); // Return the final allocated string
}


////from expandet utils.c

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

void	append_char(char **result, char c)
{
	char tmp[2];
	char *new_result;

	tmp[0] = c;
	tmp[1] = '\0';
	new_result = ft_strjoin(*result, tmp); // Don't free here
	if (!new_result)
	{
		free(*result);
		*result = NULL;
		return ;
	}
	*result = new_result;
}


//nowe
int handle_special_var(char **result, int i)
{
    char *exit_status_str = NULL;
    char *new_result = NULL;

    exit_status_str = ft_itoa(g_exit_status);
    if (!exit_status_str)
    {
        perror("minishell: ft_itoa failed in handle_special_var");
        // *result still holds the old value, signal error without modifying it
        return (-1); // Error indicator
    }

    // ft_strjoin_free will free the memory pointed to by *result
    new_result = ft_strjoin_free(*result, exit_status_str);
    free(exit_status_str); // Free itoa result AFTER join attempt

    if (!new_result)
    {
        perror("minishell: malloc error ft_strjoin_free (special_var)");
        // *result was freed by ft_strjoin_free, but new allocation failed.
        *result = NULL; // Set caller's pointer to NULL to indicate failure state
        return (-1);    // Signal error
    }

    // Success
    *result = new_result; // Update caller's pointer
    return (i + 2);       // Advance past '$?'
}