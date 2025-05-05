/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_standard_var.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhurnik <hhurnik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:46:04 by hhurnik           #+#    #+#             */
/*   Updated: 2025/05/05 17:14:29 by hhurnik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

int	process_empty_var(t_svar_state *state, int i)
{
	char	*new_result;

	new_result = ft_strjoin_free(*(state->result_ptr), "$");
	if (!new_result)
	{
		perror("minishell: malloc error ft_strjoin_free (standard_var '$')");
		free(state->var_name);
		*(state->result_ptr) = NULL;
		return (-1);
	}
	*(state->result_ptr) = new_result;
	free(state->var_name);
	return (i + 1);
}

int	process_non_empty_var(t_svar_state *state, int i)
{
	char	*var_value;
	char	*new_result;
	int		name_len;

	name_len = ft_strlen(state->var_name);
	var_value = get_env_value(state->var_name, state->env);
	if (var_value)
		new_result = ft_strjoin_free(*(state->result_ptr), var_value);
	else
		new_result = ft_strjoin_free(*(state->result_ptr), "");
	if (!new_result)
	{
		perror("minishell: malloc error ft_strjoin_free (standard_var)");
		free(state->var_name);
		*(state->result_ptr) = NULL;
		return (-1);
	}
	*(state->result_ptr) = new_result;
	free(state->var_name);
	return (i + name_len + 1);
}

int	execute_svar_expansion(t_svar_state *state, int i)
{
	int	name_len;

	name_len = ft_strlen(state->var_name);
	if (name_len == 0)
	{
		return (process_empty_var(state, i));
	}
	else
	{
		return (process_non_empty_var(state, i));
	}
}

int	handle_standard_var(char **result, char *str, int i, t_env *env)
{
	t_svar_state	state;
	char			*var_name_extracted;

	var_name_extracted = extract_var_name(str + i + 1);
	if (!var_name_extracted)
	{
		perror("minishell: extract_var_name failed");
		return (-1);
	}
	state.result_ptr = result;
	state.env = env;
	state.var_name = var_name_extracted;
	return (execute_svar_expansion(&state, i));
}
