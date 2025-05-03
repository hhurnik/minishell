#include "inc/minishell.h"

int ensure_word_allocated(char **current_word, int *error)
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
	while (input_state->input[current_i] && input_state->input[current_i] != quote_char)
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

int	extract_and_append_content(t_append_info *info, t_input_state *input_state,
		char **current_word, int *error)
{
	char	*content;
	char	*old_word;

	content = ft_substr(input_state->input, info->start, info->len);
	if (!content)
	{
		perror("minishell: malloc error (substr)");
		*error = 1;
		return (1);
	}
	old_word = *current_word;
	*current_word = ft_strjoin(old_word, content);
	free(old_word);
	free(content);
	if (!*current_word)
	{
		perror("minishell: malloc error (strjoin)");
		*error = 1;
		return (1);
	}
	return (0);
}

int	prepare_and_execute_append(t_quote_indices *indices,
		t_input_state *input_state, char **current_word, int *error)
{
	int				len;
	t_append_info	append_data;

	if (ensure_word_allocated(current_word, error))
	{
		return (1);
	}

	len = indices->end_pos - indices->start;
	if (len > 0)
	{
		append_data.start = indices->start;
		append_data.len = len;
		if (extract_and_append_content(&append_data, input_state, current_word, error))
		{
			return (1);
		}
	}
	return (0);
}


int	handle_quoted_append(t_input_state *input_state, char **current_word, int *error,
		t_parse_state *state)
{
	t_quote			local_current_quote;
	char			quote_char;
	t_quote_indices	indices;
	int				start_index_val;

	local_current_quote = get_quote_type(input_state->input[*(input_state->i)]);
	quote_char = (local_current_quote == SINGLE_QUOTE) ? '\'' : '"';

	(*(input_state->i))++;
	start_index_val = *(input_state->i);
	indices.start = start_index_val;

	if (find_closing_quote(input_state, quote_char) != 0)
	{
		ft_putstr_fd("Minishell: syntax error: unclosed quote\n", STDERR_FILENO);
		*error = 1;
		return (1);
	}
	indices.end_pos = *(input_state->i);
	if (local_current_quote == SINGLE_QUOTE)
		state->single_quote_used = 1;
	if (prepare_and_execute_append(&indices, input_state, current_word, error))
		return (1);
	(*(input_state->i))++;
	return (0);
}
