#include "inc/minishell.h"

void	skip_space(int *i, char **word, t_token **tokens)
{
	if (*word)
		finalize_current_word(tokens, word);
	(*i)++;
}

void	handle_pipe(int *i, char **word, t_token **tokens)
{
	if (*word)
		finalize_current_word(tokens, word);
	add_token(tokens, "|", T_PIPE);
	(*i)++;
}

int handle_quoted(char *input, int *i, t_token **tokens, int *error)
{
    t_quote current_quote;
    char *quoted;

    current_quote = get_quote_type(input[*i]);
    (*i)++;
    quoted = read_quoted_content(input, i, current_quote, error);
    if (*error)
        return (ft_putstr_fd("Minishell: syntax error: unclosed quote\n", STDERR_FILENO), 1);

    // Create a new token with the appropriate type
    t_token_type quote_type = get_token_type_from_quote(current_quote);
    add_token(tokens, quoted, quote_type);

    free(quoted);
    return (0);
}

t_token	*handle_unexpected_quote(char *word, t_token *tokens)
{
	free(word);
	free_tokens(tokens);
	return (NULL);
}