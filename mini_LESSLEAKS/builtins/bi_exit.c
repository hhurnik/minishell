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

// //moje
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

int bi_exit(char *argv[], t_resources *res)
{
    int exit_code;
    int parse_error; // Use a clearer name

    exit_code = g_exit_status; // Default to last status
    ft_putstr_fd("exit\n", STDOUT_FILENO);

    parse_error = parse_exit_args(argv, &exit_code); // exit_code updated by parse_exit_args

    if (parse_error == 1) // "too many arguments" or "numeric argument required"
    {
        // parse_exit_args already set the appropriate exit_code (1 or 2).
         g_exit_status = exit_code; // Update global status for the shell
        return (exit_code); // Return the error status (1 or 2)
    }

    // If parse_exit_args returned 0, it means we SHOULD exit the shell.
    // Clean up *everything* right before the final exit.
    cleanup(res); // This frees input, tokens, cmds, AND ENV.
    rl_clear_history();
    exit(exit_code); // Exit the entire minishell process.
}

