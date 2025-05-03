#include "inc/minishell.h"

/**
 * Obsługuje tokeny przekierowań w parserze.
 * - tokens: podwójny wskaźnik do aktualnego tokenu (np. `>` lub `<<`).
 * - cmd: bieżąca komenda, do której dodawane jest przekierowanie.
 */


//nowe, mniej bledow, ju moje
// void handle_redirection_parser(t_token **tokens, t_cmd *cmd)
// {
//     t_redir *new_redir;

//     new_redir = malloc(sizeof(t_redir));
//     if (!new_redir)
//     {
//         perror("minishell: malloc");
//         // Consider more robust error handling (e.g., freeing existing cmd structure)
//         return; // Or exit, or set an error flag
//     }
//     new_redir->type = (*tokens)->type;
//     new_redir->heredoc_fd = -1; // <<< Initialize here

//     // Move to next token (should be filename/delimiter)
//     *tokens = (*tokens)->next;
//     if (!*tokens || ((*tokens)->type != T_WORD && (*tokens)->type != T_SINGLE_QUOTED &&
//                       (*tokens)->type != T_DOUBLE_QUOTED))
//     {
//         ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
//         // Print the unexpected token if possible, or just a newline
//          if (*tokens && (*tokens)->value) {
//              ft_putstr_fd((*tokens)->value, STDERR_FILENO);
//              ft_putstr_fd("'\n", STDERR_FILENO);
//          } else {
//             ft_putstr_fd("newline'\n", STDERR_FILENO);
//          }
//         g_exit_status = 2; // Syntax error
//         free(new_redir);
//         // Signal error back up the call chain if possible
//         return;
//     }
//     // Assign filename/delimiter
//     new_redir->file = ft_strdup((*tokens)->value);
//      if (!new_redir->file) { // Handle strdup failure
//         perror("minishell: malloc");
//         free(new_redir);
//         return; // Or signal error
//     }
//     new_redir->next = NULL;
//     add_redir_to_cmd(cmd, new_redir);
//     // Move to next token
//     *tokens = (*tokens)->next;
// }

void handle_redirection_parser(t_token **tokens, t_cmd *cmd)
{
    t_redir *new_redir;

    new_redir = malloc(sizeof(t_redir));
    if (!new_redir)
    {
        perror("minishell: malloc");
        return; // Exit on allocation failure
    }
    new_redir->type = (*tokens)->type;
    new_redir->heredoc_fd = -1;

    *tokens = (*tokens)->next;
    if (!*tokens || ((*tokens)->type != T_WORD && (*tokens)->type != T_SINGLE_QUOTED &&
                      (*tokens)->type != T_DOUBLE_QUOTED))
    {
        ft_putstr_fd("minishell: syntax error near unexpected token\n", STDERR_FILENO);
        free(new_redir); // Free allocated memory
        return;
    }

    new_redir->file = ft_strdup((*tokens)->value);
    if (!new_redir->file)
    {
        perror("minishell: malloc");
        free(new_redir); // Free allocated memory
        return;
    }

    new_redir->next = NULL;
    add_redir_to_cmd(cmd, new_redir);
    *tokens = (*tokens)->next;
}

/// do usuniecia, ponizej mam ta sama wersje
void	free_cmd_contents(t_cmd *cmd)
{
	t_redir	*redir;
	t_redir	*next_redir;
	int		i;

	if (cmd->args != NULL)
	{
		i = 0;
		while (cmd->args[i] != NULL)
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
	}
	redir = cmd->redirections;
	while (redir != NULL)
	{
		next_redir = redir->next;
		free(redir->file);
		free(redir);
		redir = next_redir;
	}
}

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*current;
	t_cmd	*next;

	current = cmds;
	while (current != NULL)
	{
		next = current->next;
		free_cmd_contents(current);
		if (current->pipe_fd[0] != -1)
			close(current->pipe_fd[0]);
		if (current->pipe_fd[1] != -1)
			close(current->pipe_fd[1]);
		free(current);
		current = next;
	}
}

t_cmd	*process_token(t_token **tokens, t_cmd *current_cmd, t_cmd **cmds)
{
	char	*arg_copy;

	if (is_redirection((*tokens)->type))
	{
		handle_redirection_parser(tokens, current_cmd);
	}
	else if ((*tokens)->type == T_PIPE)
	{
		*tokens = (*tokens)->next; // Skip pipe token
	}
	else
	{
		arg_copy = ft_strdup((*tokens)->value);
		if (!arg_copy)
		{
			perror("minishell");
			free_cmds(*cmds);
			free_tokens(*tokens);
			return (NULL);
		}
		add_arg_to_cmd(current_cmd, arg_copy);
		*tokens = (*tokens)->next;
	}
	return (current_cmd);
}

t_cmd	*parse_tokens(t_token *tokens)
{
	t_cmd	*cmds;
	t_cmd	*current_cmd;
	int		cmd_count;

	cmds = NULL;
	current_cmd = NULL;
	cmd_count = 0;
	while (tokens)
	{
		// Create new command if needed (start or after pipe)
		if (!current_cmd || tokens->type == T_PIPE)
		{
			cmd_add_back(&cmds, cmd_new());
			current_cmd = cmd_last(cmds);
			cmd_count++;
		}
		// Process current token
		current_cmd = process_token(&tokens, current_cmd, &cmds);
		if (!current_cmd) // Check for allocation failure
		{
			free_cmds(cmds);
			free_tokens(tokens);
			return (NULL);
		}
	}
	return (cmds);
}
