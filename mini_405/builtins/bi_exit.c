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
		return (1);
	}
	if (argv[1])
	{
		if (is_integer(argv[1]))
		{
			*exit_code = ft_atoi(argv[1]);
			*exit_code %= 256;
			if (*exit_code < 0)
				*exit_code += 256;
		}
		else
		{
			write_stderr("exit: numeric argument required\n");
			*exit_code = 2;
		}
	}
	return (0);
}

//moje, usunelam tylko 405 w tym - doloyc  powrotem chyba
// //poniej proba
int	bi_exit(char *argv[], t_resources *res)
{
	int	exit_code;
	int	parse_error;

	exit_code = g_exit_status;
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	parse_error = parse_exit_args(argv, &exit_code);
	if (parse_error == 1)
	{
		g_exit_status = exit_code;
		return (exit_code);
	}
	cleanup(res);
	rl_clear_history(); //usuniete 405
	exit(exit_code);
}

