/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 20:19:34 by hhurnik           #+#    #+#             */
/*   Updated: 2025/04/22 18:43:56 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	write_stderr(char *str)
{
	write(2, str, ft_strlen(str));
}

int	is_integer(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int bi_exit(char *argv[], t_env *env)
{
    int exit_code = g_exit_status;

    ft_putstr_fd("exit\n", STDOUT_FILENO); //? leave it?

    if (argv[1] && argv[2])
    {
        write_stderr("exit: too many arguments\n");
        g_exit_status = 1;
        return (1);
    }

    if (argv[1])
    {
        if (is_integer(argv[1]))
        {
            exit_code = ft_atoi(argv[1]);
            // bash calculates exit code modulo 256
            exit_code = exit_code % 256;
            if (exit_code < 0) // Ensure positive result if % gives negative
                exit_code += 256;
        }
        else
        {
            write_stderr("exit: numeric argument required\n");
            exit_code = 2;
        }
    }

    cleanup(env, NULL, NULL, NULL); // passass NULL for tokens/cmds/input cause they are cleaned by main loop normally
    exit(exit_code);
}


