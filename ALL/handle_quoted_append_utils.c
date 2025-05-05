/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quoted_append_utils.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhurnik <hhurnik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:00:55 by hhurnik           #+#    #+#             */
/*   Updated: 2025/05/05 16:00:57 by hhurnik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

int	ensure_word_allocated(char **current_word, int *error)
{
	if (*current_word == NULL)
	{
		*current_word = ft_strdup("");
		if (*current_word == NULL)
		{
			perror("minishell: malloc error (ensure_word)");
			*error = 1;
			return (1);
		}
	}
	return (0);
}

int	find_closing_quote(t_input_state *input_state, char quote_char)
{
	int	current_i;

	current_i = *(input_state->i);
	while (input_state->input[current_i]
		&& input_state->input[current_i] != quote_char)
	{
		current_i++;
	}
	*(input_state->i) = current_i;
	if (input_state->input[current_i] != quote_char)
	{
		return (1);
	}
	return (0);
}
