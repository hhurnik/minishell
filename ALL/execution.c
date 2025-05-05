/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhurnik <hhurnik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:59:40 by hhurnik           #+#    #+#             */
/*   Updated: 2025/05/05 15:59:42 by hhurnik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

void	execute(t_cmd *cmds, t_env **env)
{
	int			stdin_copy;
	int			stdout_copy;
	int			prev_pipe_read;
	t_resources	res;

	init_exec_resources(&res, cmds, env);
	if (!init_execution(cmds, &stdin_copy, &stdout_copy))
	{
		cleanup(&res);
		return ;
	}
	if (handle_single_builtin(cmds, &res))
		return ;
	prepare_pipeline_execution(cmds, &prev_pipe_read);
	execute_pipeline(cmds, env, &prev_pipe_read);
	cleanup_execution(stdin_copy, stdout_copy);
}

void	execute_external(t_cmd *cmd, t_env *env)
{
	char	*path;
	char	**env_arr;

	path = NULL;
	env_arr = NULL;
	prepare_exec_env(cmd, env, &path, &env_arr);
	if (!path)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->args[0], STDERR_FILENO);
		if (ft_strchr(cmd->args[0], '/'))
		{
			ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		}
		else
		{
			ft_putstr_fd(": command not found\n", STDERR_FILENO);
		}
		ft_free_split(env_arr);
		free_env(env);
		free_cmds(cmd);
		exit(127);
	}
	execve(path, cmd->args, env_arr);
	handle_execve_failure(cmd, env, path, env_arr);
}

int	prepare_exec_env(t_cmd *cmd, t_env *env, char **path, char ***env_arr)
{
	char	*tmp_path;
	char	**tmp_arr;

	tmp_path = get_exec_path(cmd->args[0], env);
	tmp_arr = env_to_arr(env);
	*path = tmp_path;
	*env_arr = tmp_arr;
	if (!*env_arr)
	{
		if (errno == ENOMEM)
		{
			perror("minishell: failed to allocate environment array");
			if (*path)
				free(*path);
			free_env(env);
			free_cmds(cmd);
			exit(EXIT_FAILURE);
		}
	}
	return (0);
}

void	handle_execve_failure(t_cmd *cmd, t_env *env, char *path,
		char **env_arr)
{
	int	err;
	int	code;

	err = errno;
	code = 126;
	if (err == ENOENT)
	{
		code = 127;
	}
	print_exec_error(err, cmd, path);
	ft_free_split(env_arr);
	free(path);
	free_env(env);
	free_cmds(cmd);
	exit(code);
}
