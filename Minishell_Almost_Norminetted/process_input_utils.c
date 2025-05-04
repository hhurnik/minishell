#include "inc/minishell.h"

void	shift_empty_args(t_cmd *cmds)
{
	t_cmd	*current_cmd;
	int		i;
	char	*empty_arg_to_free;

	current_cmd = cmds;
	while (current_cmd)
	{
		if (current_cmd->args && current_cmd->args[0] && current_cmd->args[1]
			&& current_cmd->args[0][0] == '\0')
		{
			i = 0;
			empty_arg_to_free = current_cmd->args[0];
			while (current_cmd->args[i + 1])
			{
				current_cmd->args[i] = current_cmd->args[i + 1];
				i++;
			}
			current_cmd->args[i] = NULL;
			free(empty_arg_to_free);
		}
		current_cmd = current_cmd->next;
	}
}
