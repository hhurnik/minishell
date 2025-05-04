#include "inc/minishell.h"

static void finalize_and_reset_word(t_parse_state *state)
{
	finalize_current_word(&state->tokens, &state->current_word,
		state->single_quote_used);
	state->single_quote_used = 0;
}

/*
 * Handles whitespace characters during parsing.
 */
static void handle_whitespace(t_parse_state *state)
{
	finalize_and_reset_word(state);
	state->i++;
}

/*
 * Handles pipe characters during parsing.
 */
static void handle_pipe_parser(t_parse_state *state)
{
	finalize_and_reset_word(state);
	// Assuming add_token exists and handles token creation/addition
	add_token(&state->tokens, "|", T_PIPE);
	state->i++;
}

/*
 * Handles redirection characters ('<' or '>') during parsing.
 * Wraps the call to the original handle_redirection.
 */
static void handle_redirect_wrapper(char *input, t_parse_state *state)
{
	finalize_and_reset_word(state);
	// Assuming handle_redirection exists and updates state->i internally
	handle_redirection(input, &state->i, &state->tokens);
}

/*
 * Handles quote characters (' or ") during parsing.
 * Wraps the call to the original handle_quoted_append.
 */
static void handle_quote_wrapper(char *input, t_parse_state *state)
{
	t_input_state	input_state;

	input_state.input = input;
	input_state.i = &state->i;
	// Assuming handle_quoted_append exists and updates state->i and state->error
	handle_quoted_append(&input_state, &state->current_word,
			&state->error, state);
}

/*
 * Handles regular (non-special) characters during parsing.
 */
static void handle_regular_char(char *input, t_parse_state *state)
{
	// Assuming append_char_to_word exists
	append_char_to_word(&state->current_word, input[state->i]);
	state->i++;
}

/*
 * Checks if the current position indicates a comment that should end parsing.
 * Returns true if parsing should stop, false otherwise.
 */
static bool check_for_comment_break(char *input, t_parse_state *state)
{
	if (input[state->i] == '#' && (state->i == 0
			|| ft_isspace(input[state->i - 1])))
	{
		return (true); // Indicates loop should break
	}
	return (false); // Indicates loop should continue
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
