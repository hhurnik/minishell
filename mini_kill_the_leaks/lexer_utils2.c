#include "inc/minishell.h"

void	finalize_current_word(t_token **tokens, char **current_word)
{
	add_token(tokens, *current_word, T_WORD);
	free(*current_word);
	*current_word = NULL;
}

int	handle_quote(char *input, int *i, t_token **tokens, int *error)
{
	t_quote			current_quote;
	char			*quoted;
	t_token_type	quote_type;

	current_quote = get_quote_type(input[*i]);
	(*i)++;
	quoted = read_quoted_content(input, i, current_quote, error);
	if (*error)
		return (ft_putstr_fd("Minishell: syntax error: unclosed quote\n",
				STDERR_FILENO), 1);
	quote_type = get_token_type_from_quote(current_quote);
	add_token(tokens, quoted, quote_type);
	free(quoted);
	return (0);
}

char	*read_quoted_content(char *input, int *i, t_quote quote, int *error)
{
	char	quote_char;
	int		start;
	char	*content;

	if (quote == SINGLE_QUOTE)
		quote_char = '\'';
	else
		quote_char = '"';
	start = *i;
	while (input[*i] && input[*i] != quote_char)
		(*i)++;
	if (input[*i] != quote_char)
	{
		*error = 1;
		return (NULL);
	}
	content = ft_substr(input, start, *i - start);
	(*i)++;
	return (content);
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->value);
		free(tmp);
	}
}