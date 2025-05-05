/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhurnik <hhurnik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:00:31 by hhurnik           #+#    #+#             */
/*   Updated: 2025/05/05 16:00:31 by hhurnik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

int	handle_standard_var(char **result, char *str, int i, t_env *env)
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
	*result = new_result;
	free(var_name);
	return (i + name_len + 1);
}

void	append_char(char **result, char c)
{
	char	tmp[2];
	char	*new_result;

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

int	handle_special_var(char **result, int i)
{
	char	*exit_status_str;
	char	*new_result;

	exit_status_str = NULL;
	new_result = NULL;
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
