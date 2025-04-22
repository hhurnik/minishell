#include "../inc/minishell.h"

int	bi_pwd(char *argv[])
{
	// check if there are any arguments other than the command 
	if (argv[1])
	{
		ft_putstr_fd("pwd: too many arguments\n", STDERR_FILENO);
		return (1);                                                
	}

	char cwd[PATH_MAX]; //buffer to store the current directory path
	//get the current working directory
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		ft_putstr_fd(cwd, STDOUT_FILENO);  // print the current directory
		ft_putstr_fd("\n", STDOUT_FILENO); // print newline
		return (0); //successs
	}

	//if getcwd fails, print the error
	perror("minishell: pwd");
	return (1);
}