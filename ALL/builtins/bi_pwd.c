/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhurnik <hhurnik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:05:24 by hhurnik           #+#    #+#             */
/*   Updated: 2025/05/05 16:05:24 by hhurnik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	bi_pwd(char *argv[])
{
	char	cwd[PATH_MAX];

	(void)argv;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		ft_putstr_fd(cwd, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		return (0);
	}
	perror("minishell: pwd");
	return (1);
}
