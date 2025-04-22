/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 20:19:34 by hhurnik           #+#    #+#             */
/*   Updated: 2025/04/22 15:13:54 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// exit with no options

// but with possible arguments?
// If exit is called with no argument,
//	the shell or program should terminate with an exit status of 0.

// If exit is called with an argument,
//	this argument will be treated as the exit status code.
// argument should be an int, the shell should exit with this status

// exit status code is an integer from 0 to 255, unsigned
// jesli aru=gumentem bedzie ujemna liczba,
//	system zamieni je na unsigned przy uzyciu modulo

// Since m is a string and not a number,
//	Bash will not throw an error for the invalid argument, but instead,
// it will treat the string m as a non-numeric value,
//	which Bash interprets as 0.

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
//stary exit dobrze dziala
// int	bi_exit(char *argv[])
// {
// 	int	exit_code;

// 	// jesli wiecej niz 1 argument, to nie wychodzi z shella - zwraca 1
// 	if (argv[1] && argv[2])
// 	{
// 		write_stderr("exit: too many arguments\n");
// 		// nie wychodzi, wiec nie exit tylko return (return (1));
// 	}
// 	// jesli jest argument za exit
// 	if (argv[1])
// 	{
// 		if (is_integer(argv[1]))
// 		{
// 			exit_code = ft_atoi(argv[1]);
// 			if (exit_code < 0)
// 				exit_code = ((exit_code % 256) + 256) % 256;
// 					// zeby nigdy nie byl negative
// 			exit(exit_code);
// 		}
// 		else
// 		{
// 			write_stderr("exit: numeric argument required\n");
// 			// moj bash tu i tak wykonuje exit(2)
// 			exit(2);
// 		}
// 	}
// 	else
// 		exit(0);
// }


int bi_exit(char *argv[], t_env *env) // Added env parameter
{
    int exit_code = g_exit_status; // Default to last exit status

    ft_putstr_fd("exit\n", STDOUT_FILENO); // Print exit message (optional, often done in caller)

    if (argv[1] && argv[2])
    {
        write_stderr("exit: too many arguments\n");
        g_exit_status = 1; // Set exit status to 1
        return (1);        // Return 1 to indicate error, *don't exit yet*
    }

    if (argv[1])
    {
        if (is_integer(argv[1]))
        {
            exit_code = ft_atoi(argv[1]);
            // Bash calculates exit code modulo 256
            exit_code = exit_code % 256;
            if (exit_code < 0) // Ensure positive result if % gives negative
                exit_code += 256;
        }
        else
        {
            write_stderr("exit: numeric argument required\n");
            exit_code = 2; // Bash exits with 2 in this case
        }
    }

    // Perform cleanup *before* exiting
    cleanup(env, NULL, NULL, NULL); // Pass NULL for tokens/cmds/input as they are cleaned by main loop normally
    exit(exit_code); // Now exit the program
}

