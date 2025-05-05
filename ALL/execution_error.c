/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhurnik <hhurnik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:59:25 by hhurnik           #+#    #+#             */
/*   Updated: 2025/05/05 15:59:28 by hhurnik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

void	print_basic_exec_error(int err, t_cmd *cmd)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd->args[0], STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (err == ENOENT)
	{
		ft_putstr_fd("No such file or directory\n", STDERR_FILENO);
	}
	else if (err == ENOEXEC)
	{
		ft_putstr_fd("Exec format error\n", STDERR_FILENO);
	}
	else if (err != EACCES)
	{
		ft_putstr_fd(strerror(err), STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
	}
}

void	print_exec_access_error(char *path)
{
	struct stat	statbuf;
	int			status;

	status = stat(path, &statbuf);
	if (status == 0)
	{
		if (S_ISDIR(statbuf.st_mode))
			ft_putstr_fd("Is a directory\n", STDERR_FILENO);
		else
			ft_putstr_fd("Permission denied\n", STDERR_FILENO);
	}
}

void	print_exec_error(int err, t_cmd *cmd, char *path)
{
	if (err == EACCES)
	{
		print_basic_exec_error(err, cmd);
		print_exec_access_error(path);
	}
	else
	{
		print_basic_exec_error(err, cmd);
	}
}
