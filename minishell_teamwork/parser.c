#include "inc/minishell.h"


/**
 * Obsługuje tokeny przekierowań w parserze.
 * - tokens: podwójny wskaźnik do aktualnego tokenu (np. `>` lub `<<`).
 * - cmd: bieżąca komenda, do której dodawane jest przekierowanie.
 */
//stare - dziala, ale ulepszone chyba nizej
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

	// Przypisz nazwę pliku i dodaj przek ierowanie do komendy
	new_redir->file = ft_strdup((*tokens)->value);
	new_redir->next = NULL;
	add_redir_to_cmd(cmd, new_redir);

	// Przesuń się do następnego tokenu
	*tokens = (*tokens)->next;
}



// * Zamienia tokeny na strukturę komend.
//* Sprawdza składnię (np. czy pipe nie jest na końcu).
// void	print_parsed_commands(t_cmd *cmds)
// {
// 	// int i = 0;
// 	while (cmds)
// 	{
// 		// printf("\nCommand %d:\n", ++i);
// 		// printf("  Args: ");
// 		// for (int j = 0; cmds->args && cmds->args[j]; j++)
// 		//     printf("[%s] ", cmds->args[j]);
// 		// printf("\n");
		
// 		// printf("  Redirections:\n");
// 		t_redir *redir = cmds->redirections;
// 		while (redir)
// 		{
// 			printf("    Type: %d, File: %s\n", redir->type, redir->file);
// 			redir = redir->next;
// 		}
		
// 		cmds = cmds->next;
// 	}
// }

void	free_cmds(t_cmd *cmds)
{
	t_cmd *current = cmds;
	
	while (current != NULL)
	{
		t_cmd *next = current->next;
		
		// Free arguments array
		if (current->args != NULL)
		{
			for (int i = 0; current->args[i] != NULL; i++)
			{
				free(current->args[i]); // Free each argument string
			}
			free(current->args); // Free the array itself
		}
		
		// Free redirections
		t_redir *redir = current->redirections;
		while (redir != NULL)
		{
			t_redir *next_redir = redir->next;
			free(redir->file); // Free the filename string
			free(redir);       // Free the redirection struct
			redir = next_redir;
		}
		
		// Close pipe file descriptors if they're open
		if (current->pipe_fd[0] != -1)
		close(current->pipe_fd[0]);
		if (current->pipe_fd[1] != -1)
		close(current->pipe_fd[1]);
		
		free(current); // Free the command struct itself
		current = next;
	}
}

t_cmd	*parse_tokens(t_token *tokens)
{
	t_cmd *cmds = NULL;
	t_cmd *current_cmd = NULL;
	int cmd_count = 0;

	while (tokens)
	{
		// Create new command if needed
		if (!current_cmd || tokens->type == T_PIPE)
		{
			cmd_add_back(&cmds, cmd_new());
			current_cmd = cmd_last(cmds);
			cmd_count++;
		}

		// Process current token
		if (is_redirection(tokens->type))
		{
			//printf("Found redirection: %s\n", tokens->value);
			handle_redirection_parser(&tokens, current_cmd);
		}
		else if (tokens->type == T_PIPE)
		{
			printf("Skipping pipe token\n"); // cy to tu powinno byc? cy debugging step?
			tokens = tokens->next; // Move to next token after pipe
		}
		else
		{
			char *arg_copy = ft_strdup(tokens->value);
			if (!arg_copy)
			{
				perror("minishell");
				free_cmds(cmds);
				free_tokens(tokens);
				return (NULL);
			}
			add_arg_to_cmd(current_cmd, arg_copy);
			tokens = tokens->next;
		}
	}

	//print_parsed_commands(cmds);
	return (cmds);
}
