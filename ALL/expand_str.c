/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_str.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhurnik <hhurnik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:00:12 by hhurnik           #+#    #+#             */
/*   Updated: 2025/05/05 16:00:12 by hhurnik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

int	append_char_to_result(char **result_ptr, char current_char)
{
	char	*current_result;
	char	*new_result;

	current_result = *result_ptr;
	new_result = ft_strjoin_char(current_result, current_char);
	if (!new_result)
	{
		perror("minishell: malloc error in ft_strjoin_char");
		return (0);
	}
	*result_ptr = new_result;
	return (1);
}

int	process_char_type(char **res, char *str, int i, t_env *env)
{
	int	next_i;

	next_i = -2;
	if (is_special_var(str, i))
	{
		next_i = handle_special_var(res, i);
	}
	else if (is_braced_var(str, i))
	{
		next_i = handle_braced_var(res, str, i, env);
	}
	else if (is_standard_var(str, i))
	{
		next_i = handle_standard_var(res, str, i, env);
	}
	else
	{
		if (!append_char_to_result(res, str[i]))
			next_i = -1;
		else
			next_i = i + 1;
	}
	return (next_i);
}

bool	run_expansion_loop(char **res, char *str, int *i_ptr, t_env *env)
{
	int	current_i;
	int	next_i;

	current_i = *i_ptr;
	next_i = process_char_type(res, str, current_i, env);
	if (next_i < 0)
	{
		return (true);
	}
	else
	{
		*i_ptr = next_i;
		return (false);
	}
}

char	*finalize_expansion(char *result, bool error_occurred)
{
	if (error_occurred)
	{
		if (result)
		{
			free(result);
		}
		return (NULL);
	}
	return (result);
}

char	*expand_str(char *str, t_env *env)
{
	char	*result;
	int		i;
	bool	error_occurred;

	result = NULL;
	i = 0;
	error_occurred = false;
	result = ft_strdup("");
	if (!result)
	{
		perror("minishell: malloc error in expand_str initial allocation");
		return (NULL);
	}
	while (str && str[i] && !error_occurred)
		error_occurred = run_expansion_loop(&result, str, &i, env);
	return (finalize_expansion(result, error_occurred));
}
