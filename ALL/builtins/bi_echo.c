/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhurnik <hhurnik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:04:55 by hhurnik           #+#    #+#             */
/*   Updated: 2025/05/05 16:04:55 by hhurnik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	bi_echo(char *argv[])
{
	int	i;
	int	newline;
	int	first_arg;

	i = handle_n_option(argv, &newline);
	first_arg = 1;
	while (argv[i])
	{
		if (!first_arg)
			ft_putchar_fd(' ', STDOUT_FILENO);
		ft_putstr_fd(argv[i], STDOUT_FILENO);
		first_arg = 0;
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}

int	handle_n_option(char *argv[], int *newline_flag)
{
	int	i;
	int	j;

	i = 1;
	*newline_flag = 1;
	while (argv[i] && ft_strncmp(argv[i], "-n", 2) == 0)
	{
		j = 2;
		while (argv[i][j] && argv[i][j] == 'n')
			j++;
		if (argv[i][j] == '\0')
		{
			*newline_flag = 0;
			i++;
		}
		else
			break ;
	}
	return (i);
}
