#include "inc/minishell.h"

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

char *read_quoted_content(char *input, int *i, t_quote quote, int *error)
{
    char quote_char = (quote == SINGLE_QUOTE) ? '\'' : '"';
    int start = *i;
    while (input[*i] && input[*i] != quote_char)
        (*i)++;

    if (input[*i] != quote_char) {
        *error = 1;  // Ustaw błąd jeśli nie znaleziono zamykającego cytatu
        return NULL;
    }

    char *content = ft_substr(input, start, *i - start);
    (*i)++;  // Zwiększ indeks, aby przejść po zamykającym cudzysłowie
    return content;
}


//2 leaks
void free_tokens(t_token *tokens)
{
    t_token *tmp;

    while (tokens)
    {
        tmp = tokens;
        if (tokens->value)
            free(tokens->value); // <<< Free the string!
        tokens = tokens->next;
        free(tmp);
    }
}
