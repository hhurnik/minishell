#include "inc/minishell.h"


// --- Your Parsing Logic (Modified) ---

void	finalize_current_word(t_token **tokens, char **current_word,
		int single_quote_used)
{
	t_token_type	type;

	if (*current_word != NULL)
	{
		type = single_quote_used ? T_SINGLE_QUOTED : T_WORD;
		add_token(tokens, *current_word, type);
		free(*current_word);
		*current_word = NULL;
	}
}

void	handle_redirection(char *input, int *i, t_token **tokens)
{
	char			first_char;
	t_token_type	type;
	char			op_str[3];

	first_char = input[*i];
	op_str[0] = first_char;
	(*i)++;

	if (input[*i] == first_char)
	{
		op_str[1] = first_char;
		op_str[2] = '\0';
		(*i)++;
		if (first_char == '>')
			type = T_APPEND;
		else
			type = T_HEREDOC;
		add_token(tokens, op_str, type);
	}
	else
	{
		op_str[1] = '\0';
		if (first_char == '>')
			type = T_REDIR_OUT;
		else
			type = T_REDIR_IN;
		add_token(tokens, op_str, type);
	}
}

void	parse_input_loop(char *input, t_parse_state *state)
{
	t_input_state	input_state;

	input_state.input = input;
	input_state.i = &state->i;

	while (input[state->i] && !(state->error))
	{
		if (input[state->i] == '#' && (state->i == 0
				|| ft_isspace(input[state->i - 1])))
			break ;

		if (ft_isspace(input[state->i]))
		{
			finalize_current_word(&state->tokens, &state->current_word,
				state->single_quote_used);
			state->single_quote_used = 0;
			state->i++;
		}
		else if (is_pipe(input[state->i]))
		{
			finalize_current_word(&state->tokens, &state->current_word,
				state->single_quote_used);
			state->single_quote_used = 0;
			add_token(&state->tokens, "|", T_PIPE);
			state->i++;
		}
		else if (is_redirect(input[state->i]))
		{
			finalize_current_word(&state->tokens, &state->current_word,
				state->single_quote_used);
			state->single_quote_used = 0;
			handle_redirection(input, &state->i, &state->tokens);
		}
		else if (is_quote(input[state->i]))
		{
			// Function call remains the same, error checked via loop condition
			handle_quoted_append(&input_state, &state->current_word,
					&state->error, state);
		}
		else
		{
			// Call append_char_to_word (void return type)
			append_char_to_word(&state->current_word, input[state->i]);
			// Assume append_char_to_word sets state->error internally on failure
			state->i++;
		}
	}

	if (!state->error)
	{
		finalize_current_word(&state->tokens, &state->current_word,
			state->single_quote_used);
	}
}


t_token	*tokenize_input(char *input)
{
	t_parse_state	state;

	state.tokens = NULL;
	state.current_word = NULL;
	state.i = 0;
	state.error = 0;
	state.single_quote_used = 0;
	state.current_quote = NO_QUOTE; // Initialize based on struct definition

	parse_input_loop(input, &state);

	if (state.error)
	{
		if (state.current_word)
			free(state.current_word);
		free_tokens(state.tokens);
		return (NULL);
	}

	return (reverse_tokens(state.tokens));
}