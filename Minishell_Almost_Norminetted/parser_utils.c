#include "inc/minishell.h"

/**
 * Dodaje przekierowanie do listy w komendzie.
 */
void	add_redir_to_cmd(t_cmd *cmd, t_redir *new_redir)
{
	t_redir	*last;

	if (!cmd->redirections)
	{
		cmd->redirections = new_redir;
	}
	else
	{
		last = cmd->redirections;
		while (last->next)
		{
			last = last->next;
		}
		last->next = new_redir;
	}
}

t_cmd	*cmd_new(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirections = NULL;
	cmd->next = NULL;
	cmd->pipe_fd[0] = -1;
	cmd->pipe_fd[1] = -1;
	return (cmd);
}

t_cmd	*cmd_last(t_cmd *cmds)
{
	while (cmds && cmds->next)
	{
		cmds = cmds->next;
	}
	return (cmds);
}

// a pointer to a pointer allows the function to modify 
//the head of the list (first element)
// used to add a new command (new_cmd) t the end of the list of commands.
// appends a new t_cmd structure to the list,
//	it either reates the list if empty,
// or adds to the end if the list already has elemensrs
void	cmd_add_back(t_cmd **cmds, t_cmd *new_cmd)
{
	t_cmd	*last;

	if (!*cmds)
	{
		*cmds = new_cmd;
	}
	else
	{
		last = cmd_last(*cmds);
		last->next = new_cmd;
	}
}

int	is_redirection(t_token_type type)
{
	return (type == T_REDIR_IN || type == T_REDIR_OUT || type == T_APPEND
		|| type == T_HEREDOC);
}
