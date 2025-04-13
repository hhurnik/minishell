/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhurnik <hhurnik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 20:19:34 by hhurnik           #+#    #+#             */
/*   Updated: 2025/04/13 19:12:21 by hhurnik          ###   ########.fr       */
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

int	bi_exit(char *argv[])
{
	int	exit_code;

	// jesli wiecej niz 1 argument, to nie wychodzi z shella - zwraca 1
	if (argv[1] && argv[2])
	{
		write_stderr("exit: too many arguments\n");
		// nie wychodzi, wiec nie exit tylko return (return (1));
	}
	// jesli jest argument za exit
	if (argv[1])
	{
		if (is_integer(argv[1]))
		{
			exit_code = ft_atoi(argv[1]);
			if (exit_code < 0)
				exit_code = ((exit_code % 256) + 256) % 256;
					// zeby nigdy nie byl negative
			exit(exit_code);
		}
		else
		{
			write_stderr("exit: numeric argument required\n");
			// moj bash tu i tak wykonuje exit(2)
			exit(2);
		}
	}
	else
		exit(0);
}

// int main(int argc, char *argv[])
// {
//     printf("Number of arguments: %d\n", argc);
//     int i = 0;
//     while (i < argc)
//     {
//         printf("argv[%d]: %s\n", i, argv[i]);
//         i++;
//     }

//     // Test the bi_exit function with the arguments passed to main
//     printf("\nCalling bi_exit function:\n");
//     int result = bi_exit(argv);

//     // If bi_exit returns a non-zero value, we print a message.
//     if (result != 0) {
//         printf("Error occurred in bi_exit. Returned: %d\n", result);
//     }

//     return (0);
// }

// If a command is not found,
//	the child process created to execute it returns a status of 127.
// If a command is found but is not executable, the return status is 126.

// If a command fails because of an error during expansion or redirection,
//	the exit status is greater than zero.
