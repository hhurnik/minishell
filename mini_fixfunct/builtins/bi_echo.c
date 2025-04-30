#include "../inc/minishell.h"

int	bi_echo(char *argv[])
{
	int	i;
	int	newline;
	int	first_arg;

	// Parse options and get starting index
	i = handle_n_option(argv, &newline);
	// Print arguments
	first_arg = 1;
	while (argv[i])
	{
		if (!first_arg)
			ft_putchar_fd(' ', STDOUT_FILENO);
		ft_putstr_fd(argv[i], STDOUT_FILENO);
		first_arg = 0;
		i++;
	}
	// Print newline if needed
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}

int	handle_n_option(char *argv[], int *newline_flag)
{
	int	i;
	int	j;

	i = 1;
	*newline_flag = 1; // Default to printing newline
	// Handle -n option(s)
	while (argv[i] && ft_strncmp(argv[i], "-n", 2) == 0)
	{
		// Check if it's only "-n" or "-nnn..." without other characters
		j = 2;
		while (argv[i][j] && argv[i][j] == 'n')
			j++;
		if (argv[i][j] == '\0') // Valid -n option
		{
			*newline_flag = 0;
			i++;
		}
		else
			break ;
	}
	return (i); // Return the index where arguments start
}
