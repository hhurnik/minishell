#include "inc/minishell.h"

static t_redir *create_redir(t_token *token)
{
	t_redir *new_redir;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
	{
		perror("minishell: malloc");
		return (NULL);
	}
	new_redir->type = token->type;
	new_redir->heredoc_fd = -1;
	new_redir->next = NULL;
	return (new_redir);
}

static int	check_redir_syntax(t_token *token)
{
	if (!token || (token->type != T_WORD &&
		token->type != T_SINGLE_QUOTED &&
		token->type != T_DOUBLE_QUOTED))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token\n", STDERR_FILENO);
		return (0);
	}
	return (1);
}

void	handle_redirection_parser(t_token **tokens, t_cmd *cmd)
{
	t_redir *redir;

	redir = create_redir(*tokens);
	if (!redir)
		return;
	*tokens = (*tokens)->next;
	if (!check_redir_syntax(*tokens))
		return (free(redir));
	redir->file = ft_strdup((*tokens)->value);
	if (!redir->file)
	{
		perror("minishell: malloc");
		return (free(redir));
	}
	add_redir_to_cmd(cmd, redir);
	*tokens = (*tokens)->next;
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
		*tokens = (*tokens)->next;
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
		if (!current_cmd || tokens->type == T_PIPE)
		{
			cmd_add_back(&cmds, cmd_new());
			current_cmd = cmd_last(cmds);
			cmd_count++;
		}
		current_cmd = process_token(&tokens, current_cmd, &cmds);
		if (!current_cmd)
		{
			free_cmds(cmds);
			free_tokens(tokens);
			return (NULL);
		}
	}
	return (cmds);
}
