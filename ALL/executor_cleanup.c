/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_cleanup.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhurnik <hhurnik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:59:48 by hhurnik           #+#    #+#             */
/*   Updated: 2025/05/05 15:59:50 by hhurnik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

void	cleanup(t_resources *res)
{
	if (!res)
		return ;
	if (res->input)
		free(res->input);
	if (res->tokens)
		free_tokens(res->tokens);
	if (res->cmds)
		free_cmds(res->cmds);
	if (res->env)
		free_env(res->env);
	rl_clear_history();
}

void	cleanup_execution(int stdin_copy, int stdout_copy)
{
	setup_signals_for_prompt();
	restore_std_fds(stdin_copy, stdout_copy);
}

void	finalize_exit_status(pid_t last_pid, int found, int last_status)
{
	if (last_pid > 0 && found)
		g_exit_status = last_status;
}

void	run_builtin_and_exit(t_cmd *cmd, t_resources *res)
{
	int	status;

	status = run_builtin(cmd, res);
	free_env(res->env);
	free_cmds(cmd);
	exit(status);
}
