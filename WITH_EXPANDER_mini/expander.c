#include "inc/minishell.h"

int handle_standard_var(char **result, char *str, int i, t_env *env)
{
    char *var_name = NULL;
    char *var_value = NULL;
    char *new_result = NULL;
    int name_len = 0;

    var_name = extract_var_name(str + i + 1);
    if (!var_name)
    {
        perror("minishell: extract_var_name failed");
        return (-1);
    }
    name_len = ft_strlen(var_name);
    if (name_len == 0)
    {
        new_result = ft_strjoin_free(*result, "$");
        if (!new_result)
        {
            perror("minishell: malloc error ft_strjoin_free (standard_var '$')");
            free(var_name);
            *result = NULL;
            return (-1);
        }
        *result = new_result;
        free(var_name);
        return (i + 1);
    }
    var_value = get_env_value(var_name, env);
    new_result = ft_strjoin_free(*result, var_value ? var_value : "");
    if (!new_result)
    {
        perror("minishell: malloc error ft_strjoin_free (standard_var)");
        free(var_name);
        *result = NULL;
        return (-1);
    }

    // Success
    *result = new_result;
    free(var_name);
    return (i + name_len + 1);
}


// char *expand_str(char *str, t_env *env)
// {
//     char *result = NULL;
//     char *temp_result = NULL;
//     int i = 0;
//     bool error_occurred = false;

//     result = ft_strdup("");
//     if (!result) {
//         perror("minishell: malloc error in expand_str initial allocation");
//         return (NULL);
//     }
//     while (str[i] && !error_occurred)
//     {
//         if (is_special_var(str, i)) {
//             i = handle_special_var(&result, i);
//             if (i < 0) error_occurred = true;
//         }
//         else if (is_braced_var(str, i)) {
//             int next_i = handle_braced_var(&result, str, i, env);
//             if (next_i < 0) {
//                 error_occurred = true;
//             } else {
//                 i = next_i;
//             }
//         }
//         else if (is_standard_var(str, i)) {
//              int next_i = handle_standard_var(&result, str, i, env);
//              if (next_i < 0) {
//                  error_occurred = true;
//              } else {
//                  i = next_i;
//              }
//         }
//         else {
//             temp_result = ft_strjoin_char(result, str[i]);
//             if (!temp_result) {
//                 perror("minishell: malloc error in ft_strjoin_char within expand_str");
//                 error_occurred = true;
//             } else {
//                 result = temp_result;
//                 i++;
//             }
//         }
//         if (error_occurred) {
//             break;
//         }
//     }
//     if (error_occurred) {
//         if (result) {
//             free(result);
//         }
//         return (NULL);
//     }
//     return (result);
// }













void	append_char(char **result, char c)
{
	char tmp[2];
	char *new_result;

	tmp[0] = c;
	tmp[1] = '\0';
	new_result = ft_strjoin(*result, tmp);
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
        return (-1);
    }
    new_result = ft_strjoin_free(*result, exit_status_str);
    free(exit_status_str);
    if (!new_result)
    {
        perror("minishell: malloc error ft_strjoin_free (special_var)");
        *result = NULL;
        return (-1);
    }
    *result = new_result;
    return (i + 2);
}
