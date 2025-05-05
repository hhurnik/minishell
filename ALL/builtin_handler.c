/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhurnik <hhurnik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:58:55 by hhurnik           #+#    #+#             */
/*   Updated: 2025/05/05 15:58:58 by hhurnik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

int	is_builtin(char *cmd)
{
	if ((!strcmp(cmd, "echo") || !strcmp(cmd, "cd") || !strcmp(cmd, "pwd")
			|| !strcmp(cmd, "export") || !strcmp(cmd, "unset") || !strcmp(cmd,
				"env") || !strcmp(cmd, "exit")))
		return (1);
	else
		return (0);
}

int	run_builtin(t_cmd *cmd, t_resources *res)
{
	if (!strcmp(cmd->args[0], "echo"))
		return (bi_echo(cmd->args));
	if (!strcmp(cmd->args[0], "pwd"))
		return (bi_pwd(cmd->args));
	if (!strcmp(cmd->args[0], "exit"))
		return (bi_exit(cmd->args, res));
	if (!strcmp(cmd->args[0], "unset"))
		return (bi_unset(cmd->args, &res->env));
	if (!strcmp(cmd->args[0], "export"))
		return (bi_export(cmd->args, &res->env));
	if (!strcmp(cmd->args[0], "env"))
		return (bi_env(cmd->args, &res->env));
	if (!strcmp(cmd->args[0], "cd"))
		return (bi_cd(cmd->args, &res->env));
	return (0);
}

int	handle_parent_builtin(t_cmd *cmd, t_fds fds, t_resources *res)
{
	if (setup_redirections(cmd->redirections) < 0)
	{
		restore_std_fds(fds.stdin, fds.stdout);
		g_exit_status = 1;
		return (1);
	}
	g_exit_status = run_builtin(cmd, res);
	restore_std_fds(fds.stdin, fds.stdout);
	return (1);
}

int	handle_single_builtin(t_cmd *cmds, t_resources *res)
{
	t_fds	fds;

	if (is_builtin(cmds->args[0]) && !cmds->next)
	{
		fds.stdin = dup(STDIN_FILENO);
		fds.stdout = dup(STDOUT_FILENO);
		handle_parent_builtin(cmds, fds, res);
		return (1);
	}
	return (0);
}
