#include "inc/minishell.h"


/**
 * Obsługuje tokeny przekierowań w parserze.
 * - tokens: podwójny wskaźnik do aktualnego tokenu (np. `>` lub `<<`).
 * - cmd: bieżąca komenda, do której dodawane jest przekierowanie.
 */
void	handle_redirection_parser(t_token **tokens, t_cmd *cmd)
{
	t_redir *new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
	{
		perror("minishell");
		return ;
	}

	// Ustaw typ przekierowania na podstawie tokenu
	new_redir->type = (*tokens)->type;

	// Przesuń się do następnego tokenu (powinien być nazwa pliku)
	*tokens = (*tokens)->next;
	if (!*tokens || (*tokens)->type != T_WORD)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token\n",
			STDERR_FILENO);
		free(new_redir);
		return ;
	}

	// Przypisz nazwę pliku i dodaj przekierowanie do komendy
	new_redir->file = ft_strdup((*tokens)->value);
	new_redir->next = NULL;
	add_redir_to_cmd(cmd, new_redir);

	// Przesuń się do następnego tokenu
	*tokens = (*tokens)->next;
}

/**
 * Zamienia tokeny na strukturę komend.
 * Sprawdza składnię (np. czy pipe nie jest na końcu).
 */
t_cmd	*parse_tokens(t_token *tokens)
{
	t_cmd *cmds = NULL;
	t_cmd *current_cmd = NULL;

	while (tokens)
	{
		// Utwórz nową komendę
		if (!current_cmd || tokens->type == T_PIPE)
		{
			cmd_add_back(&cmds, cmd_new());
			current_cmd = cmd_last(cmds);
		}
		// Dodaj argumenty/redirekcje
		if (is_redirection(tokens->type))
		{
			handle_redirection_parser(&tokens, current_cmd);
		}
		else if (tokens->type != T_PIPE)
		{
			add_arg_to_cmd(current_cmd, tokens->value);
		}
		tokens = tokens->next;
	}
	return (cmds);
}
