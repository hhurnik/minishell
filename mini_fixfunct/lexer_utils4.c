#include "inc/minishell.h"


void	handle_redirect_token(char *input, int *i, char **word,
		t_token **tokens)
{
	if (*word)
		finalize_current_word(tokens, word, 0);
	handle_redirection(input, i, tokens);
}

void	validate_redirection_syntax(t_token **tokens)
{
	if (*tokens && (*tokens)->next)
	{
		if ((*tokens)->next->type != T_WORD
			&& (*tokens)->next->type != T_SINGLE_QUOTED
			&& (*tokens)->next->type != T_DOUBLE_QUOTED)
		{
			ft_putstr_fd("minishell: syntax error:\n", STDERR_FILENO);
			ft_putstr_fd("Missing file name after redirection\n",
				STDERR_FILENO);
			*tokens = NULL;
				// To indicate an error and avoid further processing
		}
	}
}
