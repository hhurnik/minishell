#include "../inc/minishell.h"
// char cwd[PATH_MAX]; // buffer to store the current directory path
// int	bi_pwd(char *argv[])
// {
// 	char	cwd[PATH_MAX];

// 	// check if there are any arguments other than the command
// 	if (argv[1])
// 	{
// 		ft_putstr_fd("pwd: too many arguments\n", STDERR_FILENO);
// 		return (1);
// 	}
// 	// get the current working directory
// 	if (getcwd(cwd, sizeof(cwd)) != NULL)
// 	{
// 		ft_putstr_fd(cwd, STDOUT_FILENO); // print the current directory
// 		ft_putstr_fd("\n", STDOUT_FILENO); // print newline
// 		return (0); // successs
// 	}
// 	// if getcwd fails, print the error
// 	perror("minishell: pwd");
// 	return (1);
// }

int bi_pwd(char *argv[])
{
    char cwd[PATH_MAX];

    // REMOVE or comment out this check:
    /*
    if (argv[1])
    {
        ft_putstr_fd("pwd: too many arguments\n", STDERR_FILENO);
        return (1);
    }
    */
    (void)argv; // Indicate argv is intentionally unused

    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        ft_putstr_fd(cwd, STDOUT_FILENO);
        ft_putstr_fd("\n", STDOUT_FILENO);
        return (0); // Success
    }
    perror("minishell: pwd");
    return (1); // Error (getcwd failed)
}