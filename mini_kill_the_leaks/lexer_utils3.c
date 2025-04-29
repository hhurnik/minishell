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
//dziala i bez leakow w echo, ale nie dziala dla echo "hello""world"
// int	handle_quoted(char *input, int *i, t_token **tokens, int *error)
// {
// 	char	*quoted;
// 	t_quote	quote;

// 	quote = get_quote_type(input[*i]);
// 	// Określenie typu cytatu
// 	(*i)++;
// 	// Przesunięcie wskaźnika na następny znak
// 	quoted = read_quoted_content(input, i, quote, error);
// 	// Odczytanie zawartości cytatu
// 	if (*error)
// 	{
// 		ft_putstr_fd("Squish: syntax error: unclosed quote\n", 2);
// 		return (1); // Błąd, jeśli cytat nie został zamknięty
// 	}
//	add_token(tokens, quoted, get_token_type_from_quote(quote));
// 	// Dodanie tokena
// 	free(quoted);
// 	// Zwolnienie pamięci
// 	return (0); // Sukces
// }

//works for echo "hello""world", but doesnt add token?
int	handle_quoted(char *input, int *i, char **current_word, int *error)
{
	t_quote current_quote;
	char *quoted;

	current_quote = get_quote_type(input[*i]);
	(*i)++;
	quoted = read_quoted_content(input, i, current_quote, error);
	if (*error)
		return (ft_putstr_fd("Minishell: syntax error: unclosed quote\n", STDERR_FILENO), 1);
	
	// Append quoted content to the current word
	if (*current_word == NULL)
		*current_word = ft_strdup(quoted);
	else
	{
		char *tmp = ft_strjoin(*current_word, quoted);
		free(*current_word);
		*current_word = tmp;
	}
	free(quoted);
	return (0);
}

void	append_char_to_word(char **word, char c)
{
	*word = ft_strjoin_char(*word, c);
}

t_token	*handle_unexpected_quote(char *word, t_token *tokens)
{
	free(word);
	free_tokens(tokens);
	return (NULL);
}