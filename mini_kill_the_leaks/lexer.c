#include "inc/minishell.h"

// dodaje tokeny for a single command line, to the beginning of the linked list
void	add_token(t_token **tokens, char *value, t_token_type type)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return ;
	new->value = ft_strdup(value); // MUST duplicate the string
	if (!new->value)
	{
		free(new);
		return ;
	}
	new->type = type;
	new->next = *tokens;
	*tokens = new;
}

t_token	*reverse_tokens(t_token *tokens)
{
	t_token	*prev;
	t_token	*current;
	t_token	*next;

	prev = NULL;
	current = tokens;
	next = NULL;
	while (current)
	{
		next = current->next;
		current->next = prev;
		prev = current;
		current = next;
	}
	return (prev);
}

void	parse_input_loop(char *input, t_parse_state *state)
{
	while (input[state->i] && !(state->error))
	{
		if (state->current_quote != NO_QUOTE)
		{
			state->tokens = handle_unexpected_quote(state->current_word,
					state->tokens);
			return ;
		}
		else if (is_quote(input[state->i])) // Sprawdzanie, czy to cytat
		{
			state->error = handle_quoted(input, &state->i, &state->tokens,
					&state->error);
		}
		else // Inne przypadki
		{
			handle_input_char(input, &state->i, &state->current_word,
				&state->tokens);
		}
	}
}

t_token	*tokenize_input(char *input)
{
	t_parse_state	state;

	state.tokens = NULL;
	state.current_word = NULL;
	state.current_quote = NO_QUOTE;
	state.i = 0;
	state.error = 0;
	parse_input_loop(input, &state);
	if (state.current_word)
		finalize_current_word(&state.tokens, &state.current_word);
	if (state.error)
	{
		free_tokens(state.tokens);
		return (NULL);
	}
	return (reverse_tokens(state.tokens));
}

void	handle_input_char(char *input, int *i, char **current_word,
		t_token **tokens)
{
	if (ft_isspace(input[*i]))
		skip_space(i, current_word, tokens);
	else if (is_pipe(input[*i]))
		handle_pipe(i, current_word, tokens);
	else if (is_redirect(input[*i]))
		handle_redirect_token(input, i, current_word, tokens);
	else
		append_char_to_word(current_word, input[(*i)++]);
}