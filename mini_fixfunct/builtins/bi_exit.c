/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 20:19:34 by hhurnik           #+#    #+#             */
/*   Updated: 2025/04/28 19:07:24 by marvin           ###   ########.fr       */
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

int	parse_exit_args(char *argv[], int *exit_code)
{
	if (argv[1] && argv[2])
	{
		write_stderr("exit: too many arguments\n");
		*exit_code = 1;
		return (1); // Indicate error (don't exit)
	}
	if (argv[1])
	{
		if (is_integer(argv[1]))
		{
			*exit_code = ft_atoi(argv[1]);
			// bash calculates exit code modulo 256
			*exit_code %= 256;
			if (*exit_code < 0) // Ensure positive result
				*exit_code += 256;
		}
		else
		{
			write_stderr("exit: numeric argument required\n");
			*exit_code = 2;
		}
	}
	return (0); // Indicate success (should exit)
}

/// the last, 2 leaki
int bi_exit(char *argv[], t_resources *res)
{
    int exit_code;
    int should_exit;

    exit_code = g_exit_status;
    ft_putstr_fd("exit\n", STDOUT_FILENO);
    should_exit = parse_exit_args(argv, &exit_code);
    if (should_exit == 1)
		return (1);
      
    // Clean up all resources
    cleanup(res); // This handles res->input, tokens, cmds, env
	rl_clear_history(); // Clear readline history
    exit(exit_code);
}
