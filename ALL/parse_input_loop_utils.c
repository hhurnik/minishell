/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input_loop_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhurnik <hhurnik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:02:40 by hhurnik           #+#    #+#             */
/*   Updated: 2025/05/05 16:04:14 by hhurnik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

void	finalize_and_reset_word(t_parse_state *state)
{
	finalize_current_word(&state->tokens, &state->current_word,
		state->single_quote_used);
	state->single_quote_used = 0;
}

/*
 * Handles whitespace characters during parsing.
 */
void	handle_whitespace(t_parse_state *state)
{
	finalize_and_reset_word(state);
	state->i++;
}

/*
 * Handles pipe characters during parsing.
 */
void	handle_pipe_parser(t_parse_state *state)
{
	finalize_and_reset_word(state);
	add_token(&state->tokens, "|", T_PIPE);
	state->i++;
}
