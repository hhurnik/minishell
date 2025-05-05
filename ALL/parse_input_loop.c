/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input_loop.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhurnik <hhurnik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:02:48 by hhurnik           #+#    #+#             */
/*   Updated: 2025/05/05 16:02:48 by hhurnik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

/*
 * Handles redirection characters ('<' or '>') during parsing.
 * Wraps the call to the original handle_redirection.
 */
void	handle_redirect_wrapper(char *input, t_parse_state *state)
{
	finalize_and_reset_word(state);
	handle_redirection(input, &state->i, &state->tokens);
}

/*
 * Handles quote characters (' or ") during parsing.
 * Wraps the call to the original handle_quoted_append.
 */
void	handle_quote_wrapper(char *input, t_parse_state *state)
{
	t_input_state	input_state;

	input_state.input = input;
	input_state.i = &state->i;
	handle_quoted_append(&input_state, &state->current_word, &state->error,
		state);
}

/*
 * Handles regular (non-special) characters during parsing.
 */
void	handle_regular_char(char *input, t_parse_state *state)
{
	append_char_to_word(&state->current_word, input[state->i]);
	state->i++;
}

/*
 * Checks if the current position indicates a comment that should end parsing.
 * Returns true if parsing should stop, false otherwise.
 */
bool	check_for_comment_break(char *input, t_parse_state *state)
{
	if (input[state->i] == '#' && (state->i == 0
			|| ft_isspace(input[state->i - 1])))
	{
		return (true);
	}
	return (false);
}

/*
 * Main loop for parsing the input string character by character.
 * Dispatches handling to specific functions based on the character type.
 * (Original function refactored to use helpers)
 */
void	parse_input_loop(char *input, t_parse_state *state)
{
	while (input[state->i] && !(state->error))
	{
		if (check_for_comment_break(input, state))
			break ;
		if (ft_isspace(input[state->i]))
			handle_whitespace(state);
		else if (is_pipe(input[state->i]))
			handle_pipe_parser(state);
		else if (is_redirect(input[state->i]))
			handle_redirect_wrapper(input, state);
		else if (is_quote(input[state->i]))
			handle_quote_wrapper(input, state);
		else
			handle_regular_char(input, state);
	}
	if (!state->error)
	{
		finalize_current_word(&state->tokens, &state->current_word,
			state->single_quote_used);
	}
}
