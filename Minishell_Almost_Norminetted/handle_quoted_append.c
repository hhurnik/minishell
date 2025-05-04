#include "inc/minishell.h"

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
		if (extract_and_append_content(&append_data, input_state, current_word,
				error))
		{
			return (1);
		}
	}
	return (0);
}

int	find_and_update_for_quote(t_input_state *input_state, int *error,
		t_parse_state *state, t_quote_indices *indices_out)
{
	t_quote	local_current_quote;
	char	quote_char;
	int		start_index_val;

	local_current_quote = get_quote_type(input_state->input[*(input_state->i)]);
	if (local_current_quote == SINGLE_QUOTE)
		quote_char = '\'';
	else
		quote_char = '"';
	(*(input_state->i))++;
	start_index_val = *(input_state->i);
	indices_out->start = start_index_val;
	if (find_closing_quote(input_state, quote_char) != 0)
	{
		ft_putstr_fd("Minishell: syntax error: unclosed quote\n",
			STDERR_FILENO);
		*error = 1;
		return (1);
	}
	indices_out->end_pos = *(input_state->i);
	if (local_current_quote == SINGLE_QUOTE)
		state->single_quote_used = 1;
	return (0);
}

int	append_and_advance_for_quote(t_input_state *input_state,
		char **current_word, int *error, t_quote_indices *indices)
{
	if (prepare_and_execute_append(indices, input_state, current_word, error))
		return (1);
	(*(input_state->i))++;
	return (0);
}

// --- Original Function (Now Orchestrator) ---
int	handle_quoted_append(t_input_state *input_state, char **current_word,
		int *error, t_parse_state *state)
{
	t_quote_indices	indices;

	if (find_and_update_for_quote(input_state, error, state, &indices) != 0)
		return (1);
	if (append_and_advance_for_quote(input_state, current_word, error,
			&indices) != 0)
		return (1);
	return (0);
}
