// #include "minishell.h"

// void	free_cmd_contents(t_cmd *cmd)
// {
// 	t_redir	*redir;
// 	t_redir	*next_redir;
// 	int		i;

// 	if (cmd->args != NULL)
// 	{
// 		i = 0;
// 		while (cmd->args[i] != NULL)
// 		{
// 			free(cmd->args[i]);
// 			i++;
// 		}
// 		free(cmd->args);
// 	}
	
// 	redir = cmd->redirections;
// 	while (redir != NULL)
// 	{
// 		next_redir = redir->next;
// 		free(redir->file);
// 		free(redir);
// 		redir = next_redir;
// 	}
// }

// void	free_cmds(t_cmd *cmds)
// {
// 	t_cmd	*current;
// 	t_cmd	*next;

// 	current = cmds;
// 	while (current != NULL)
// 	{
// 		next = current->next;
// 		free_cmd_contents(current);


// 		if (current->pipe_fd[0] != -1)
// 			close(current->pipe_fd[0]);
// 		if (current->pipe_fd[1] != -1)
// 			close(current->pipe_fd[1]);
// 		free(current);
// 		current = next;
// 	}
// }