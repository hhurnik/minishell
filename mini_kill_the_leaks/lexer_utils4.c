#include "inc/minishell.h"

void	handle_redirect_token(char *input, int *i, char **word,
		t_token **tokens)
{
	if (*word)
		finalize_current_word(tokens, word);
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
			*tokens = NULL; // To indicate an error and avoid further processing
		}
	}
}

void	handle_redirection(char *input, int *i, t_token **tokens)
{
	char			first_char;
	t_token_type	type;

	first_char = input[*i];
	(*i)++;
	// Move to the next character to check for redirection
	if (input[*i] == first_char) // Handle double operator (>> or <<)
	{
		(*i)++;
		if (first_char == '>')
			type = T_APPEND;
		else
			type = T_HEREDOC;
		add_token(tokens, (char[]){first_char, first_char, '\0'}, type);
	}
	else // Handle single operator (> or <)
	{
		if (first_char == '>')
			type = T_REDIR_OUT;
		else
			type = T_REDIR_IN;
		add_token(tokens, (char[]){first_char, '\0'}, type);
	}
	validate_redirection_syntax(tokens); // Call the validation function
}