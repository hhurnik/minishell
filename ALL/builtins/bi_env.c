/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhurnik <hhurnik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:04:59 by hhurnik           #+#    #+#             */
/*   Updated: 2025/05/05 16:04:59 by hhurnik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/**
 * env builtin command - wypisuje all env variables
 * Format: NAME=VALUE (one per line)
 */
//moje, be grep halo=123 cos tam
int	bi_env(char **args, t_env **env)
{
	t_env	*current;

	if (args[1])
	{
		ft_putstr_fd("env: too many arguments\n", STDERR_FILENO);
		return (ERROR);
	}
	current = *env;
	while (current)
	{
		if (current->value && current->value[0] != '\0')
		{
			ft_putstr_fd(current->key, STDOUT_FILENO);
			ft_putstr_fd("=", STDOUT_FILENO);
			ft_putstr_fd(current->value, STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		current = current->next;
	}
	return (SUCCESS);
}
