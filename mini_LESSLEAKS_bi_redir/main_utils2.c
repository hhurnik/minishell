#include "inc/minishell.h"

// // int	handle_quoted_append(char *input, int *i, char **current_word, int *error,
// // 		t_parse_state *state)
// // {
// // 	t_quote	current_quote;
// // 	char	quote_char;
// // 	int		start;
// // 	int		len;
// // 	char	*content;
// // 	char	*old_word;

// // 	current_quote = get_quote_type(input[*i]);
// // 	quote_char = (current_quote == SINGLE_QUOTE) ? '\'' : '"';
// // 	(*i)++;
// // 	start = *i;
// // 	while (input[*i] && input[*i] != quote_char)
// // 		(*i)++;
// // 	if (input[*i] != quote_char)
// // 	{
// // 		ft_putstr_fd("Minishell: syntax error: unclosed quote\n",
// // 			STDERR_FILENO);
// // 		*error = 1;
// // 		return (1);
// // 	}
// // 	if (current_quote == SINGLE_QUOTE)
// // 		state->single_quote_used = 1;
// // 	if (!ensure_current_word(current_word))
// // 	{
// // 		*error = 1;
// // 		return (1);
// // 	}
// // 	len = *i - start;
// // 	if (len > 0)
// // 	{
// // 		content = ft_substr(input, start, len);
// // 		if (!content)
// // 		{
// // 			perror("minishell: malloc error");
// // 			*error = 1;
// // 			return (1);
// // 		}
// // 		old_word = *current_word;
// // 		*current_word = ft_strjoin(old_word, content);
// // 		free(old_word);
// // 		free(content);
// // 		if (!*current_word)
// // 		{
// // 			perror("minishell: malloc error");
// // 			*error = 1;
// // 			return (1);
// // 		}
// // 	}
// // 	(*i)++;
// // 	return (0);
// // }


// void	finalize_current_word(t_token **tokens, char **current_word,
// 		int single_quote_used)
// {
// 	t_token_type	type;

// 	if (*current_word != NULL)
// 	{
// 		type = single_quote_used ? T_SINGLE_QUOTED : T_WORD;
// 		add_token(tokens, *current_word, type);
// 		free(*current_word);
// 		*current_word = NULL;
// 	}
// }

// void	handle_redirection(char *input, int *i, t_token **tokens)
// {
// 	char			first_char;
// 	t_token_type	type;
// 	char			op_str[3];

// 	first_char = input[*i];
// 	op_str[0] = first_char;
// 	(*i)++;
// 	if (input[*i] == first_char)
// 	{
// 		op_str[1] = first_char;
// 		op_str[2] = '\0';
// 		(*i)++;
// 		if (first_char == '>')
// 			type = T_APPEND;
// 		else
// 			type = T_HEREDOC;
// 		add_token(tokens, op_str, type);
// 	}
// 	else
// 	{
// 		op_str[1] = '\0';
// 		if (first_char == '>')
// 			type = T_REDIR_OUT;
// 		else
// 			type = T_REDIR_IN;
// 		add_token(tokens, op_str, type);
// 	}
// }

// void	parse_input_loop(char *input, t_parse_state *state)
// {
// 	while (input[state->i] && !(state->error))
// 	{
// 		if (input[state->i] == '#' && (state->i == 0
// 				|| ft_isspace(input[state->i - 1])))
// 			break ;
// 		if (ft_isspace(input[state->i]))
// 		{
// 			finalize_current_word(&state->tokens, &state->current_word,
// 				state->single_quote_used);
// 			state->i++;
// 		}
// 		else if (is_pipe(input[state->i]))
// 		{
// 			finalize_current_word(&state->tokens, &state->current_word,
// 				state->single_quote_used);
// 			add_token(&state->tokens, "|", T_PIPE);
// 			state->i++;
// 		}
// 		else if (is_redirect(input[state->i]))
// 		{
// 			finalize_current_word(&state->tokens, &state->current_word,
// 				state->single_quote_used);
// 			handle_redirection(input, &state->i, &state->tokens);
// 		}
// 		else if (is_quote(input[state->i]))
// 		{
// 			if (handle_quoted_append(input, &state->i, &state->current_word,
// 					&state->error, state) != 0)
// 				state->error = 1;
// 		}
// 		else
// 		{
// 			append_char_to_word(&state->current_word, input[state->i]);
// 			if (state->current_word == NULL && errno == ENOMEM)
// 				state->error = 1;
// 			state->i++;
// 		}
// 	}
// 	if (!state->error)
// 		finalize_current_word(&state->tokens, &state->current_word,
// 			state->single_quote_used);
// }




// t_token	*tokenize_input(char *input)
// {
// 	t_parse_state	state;

// 	state.tokens = NULL;
// 	state.current_word = NULL;
// 	state.i = 0;
// 	state.error = 0;
// 	state.single_quote_used = 0;
// 	parse_input_loop(input, &state);
// 	if (state.error)
// 	{
// 		if (state.current_word)
// 			free(state.current_word);
// 		free_tokens(state.tokens);
// 		return (NULL);
// 	}
// 	return (reverse_tokens(state.tokens));
// }

///////////////////////////////////////////////KONIEC MOJE------------------------------





///JUZ JEST W HANDLE_QUOTED_APPEND.C

// static int ensure_word_allocated(char **current_word, int *error)
// {
// 	if (*current_word == NULL)
// 	{
// 		*current_word = ft_strdup("");
// 		if (*current_word == NULL)
// 		{
// 			perror("minishell: malloc error (ensure_word)");
// 			*error = 1;
// 			return (1);
// 		}
// 	}
// 	return (0);
// }


// static int	find_closing_quote(t_input_state *input_state, char quote_char)
// {
// 	int	current_i;

// 	current_i = *(input_state->i);
// 	while (input_state->input[current_i] && input_state->input[current_i] != quote_char)
// 	{
// 		current_i++;
// 	}
// 	*(input_state->i) = current_i;
// 	if (input_state->input[current_i] != quote_char)
// 	{
// 		return (1);
// 	}
// 	return (0);
// }

// static int	extract_and_append_content(t_append_info *info, t_input_state *input_state,
// 		char **current_word, int *error)
// {
// 	char	*content;
// 	char	*old_word;

// 	content = ft_substr(input_state->input, info->start, info->len);
// 	if (!content)
// 	{
// 		perror("minishell: malloc error (substr)");
// 		*error = 1;
// 		return (1);
// 	}
// 	old_word = *current_word;
// 	*current_word = ft_strjoin(old_word, content);
// 	free(old_word);
// 	free(content);
// 	if (!*current_word)
// 	{
// 		perror("minishell: malloc error (strjoin)");
// 		*error = 1;
// 		return (1);
// 	}
// 	return (0);
// }

// static int	prepare_and_execute_append(t_quote_indices *indices,
// 		t_input_state *input_state, char **current_word, int *error)
// {
// 	int				len;
// 	t_append_info	append_data;

// 	if (ensure_word_allocated(current_word, error))
// 	{
// 		return (1);
// 	}

// 	len = indices->end_pos - indices->start;
// 	if (len > 0)
// 	{
// 		append_data.start = indices->start;
// 		append_data.len = len;
// 		if (extract_and_append_content(&append_data, input_state, current_word, error))
// 		{
// 			return (1);
// 		}
// 	}
// 	return (0);
// }


// // --- Main Refactored Function ---

// int	handle_quoted_append(t_input_state *input_state, char **current_word, int *error,
// 		t_parse_state *state)
// {
// 	t_quote			local_current_quote;
// 	char			quote_char;
// 	t_quote_indices	indices;
// 	int				start_index_val;

// 	local_current_quote = get_quote_type(input_state->input[*(input_state->i)]);
// 	quote_char = (local_current_quote == SINGLE_QUOTE) ? '\'' : '"';

// 	(*(input_state->i))++;
// 	start_index_val = *(input_state->i);
// 	indices.start = start_index_val;

// 	if (find_closing_quote(input_state, quote_char) != 0)
// 	{
// 		ft_putstr_fd("Minishell: syntax error: unclosed quote\n", STDERR_FILENO);
// 		*error = 1;
// 		return (1);
// 	}
// 	indices.end_pos = *(input_state->i);

// 	if (local_current_quote == SINGLE_QUOTE)
// 		state->single_quote_used = 1;

// 	if (prepare_and_execute_append(&indices, input_state, current_word, error))
// 	{
// 		return (1);
// 	}

// 	(*(input_state->i))++;

// 	return (0);
// }



//////KONIEC HANDLE_QUOTED_APPEND.C

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