#include "inc/minishell.h"

void	skip_space(int *i, char **word, t_token **tokens)
{
	if (*word)
		finalize_current_word(tokens, word, 0);
	(*i)++;
}

void	handle_pipe(int *i, char **word, t_token **tokens)
{
	if (*word)
		finalize_current_word(tokens, word, 0);
	add_token(tokens, "|", T_PIPE);
	(*i)++;
}

t_token	*handle_unexpected_quote(char *word, t_token *tokens)
{
	free(word);
	free_tokens(tokens);
	return (NULL);
}
