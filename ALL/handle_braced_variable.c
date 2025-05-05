/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_braced_variable.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhurnik <hhurnik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:00:43 by hhurnik           #+#    #+#             */
/*   Updated: 2025/05/05 16:00:43 by hhurnik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

int	extract_braced_var_name(char *str, int i, char **var_name_ptr,
		int *end_index_ptr)
{
	int		start;
	int		end;
	char	*extracted_name;

	start = i + 2;
	end = start;
	extracted_name = NULL;
	*var_name_ptr = NULL;
	while (str[end] && str[end] != '}')
		end++;
	if (str[end] != '}')
		return (1);
	extracted_name = ft_substr(str, start, end - start);
	if (!extracted_name)
	{
		perror("minishell: ft_substr failed in extract_braced_var_name");
		return (-1);
	}
	*var_name_ptr = extracted_name;
	*end_index_ptr = end;
	return (0);
}

int	append_braced_var_value(char **result, char *var_name, t_env *env,
		int end_index)
{
	char	*var_value;
	char	*new_result;
	char	*value_to_join;

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
		free(var_name);
		*result = NULL;
		return (-1);
	}
	*result = new_result;
	free(var_name);
	return (end_index + 1);
}

int	handle_braced_var(char **result, char *str, int i, t_env *env)
{
	char	*var_name;
	int		end_index;
	int		extract_status;
	int		final_index;

	var_name = NULL;
	end_index = 0;
	extract_status = 0;
	final_index = 0;
	extract_status = extract_braced_var_name(str, i, &var_name, &end_index);
	if (extract_status == 1)
		return (i);
	else if (extract_status == -1)
		return (-1);
	final_index = append_braced_var_value(result, var_name, env, end_index);
	return (final_index);
}
