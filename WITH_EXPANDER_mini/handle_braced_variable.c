#include "inc/minishell.h"


//divided handle_braced_bar
static int extract_braced_var_name(char *str, int i, char **var_name_ptr, int *end_index_ptr)
{
    int start;
    int end;
    char *extracted_name; // Renamed from var_name to avoid confusion

    // Initialization
    start = i + 2;
    end = start;
    extracted_name = NULL;
    *var_name_ptr = NULL; // Ensure out-parameter is NULL initially

    while (str[end] && str[end] != '}')
        end++;

    if (str[end] != '}')
        return (1); // Signal: closing brace not found
    extracted_name = ft_substr(str, start, end - start);
    if (!extracted_name)
    {
        perror("minishell: ft_substr failed in extract_braced_var_name");
        return (-1); // Signal: allocation error
    }
    *var_name_ptr = extracted_name;
    *end_index_ptr = end;

    return (0); // Signal: success
}


static int append_braced_var_value(char **result, char *var_name, t_env *env, int end_index)
{
    char *var_value;
    char *new_result;
    char *value_to_join; // To handle NULL var_value gracefully

    // Initialization
    var_value = NULL;
    new_result = NULL;
    value_to_join = "";

    var_value = get_env_value(var_name, env);
    if (var_value)
        value_to_join = var_value;
    new_result = ft_strjoin_free(*result, value_to_join);
    if (!new_result)
    {
        perror("minishell: malloc error ft_strjoin_free (braced_var)");
        free(var_name); // Free the name received from the previous function
        *result = NULL; // Indicate *result is now invalid/NULL
        return (-1);    // Signal: allocation error
    }
    *result = new_result;
    free(var_name);
    return (end_index + 1);
}


int handle_braced_var(char **result, char *str, int i, t_env *env)
{
    char *var_name;
    int end_index; // Index of the '}' character
    int extract_status;
    int final_index;

    // Initialization (required by the prompt, though unnecessary for pointers/int)
    var_name = NULL;
    end_index = 0;
    extract_status = 0;
    final_index = 0;

    extract_status = extract_braced_var_name(str, i, &var_name, &end_index);
    if (extract_status == 1) // Closing brace not found
        return (i);
    else if (extract_status == -1) // Allocation error during extraction
        return (-1);

    final_index = append_braced_var_value(result, var_name, env, end_index);

    return (final_index); // Return result from append function (-1 on error, end+1 on success)
}


