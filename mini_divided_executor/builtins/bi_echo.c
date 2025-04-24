#include "../inc/minishell.h"

int	bi_echo(char *argv[])
{
    int i = 1;
    int newline = 1; // Default to printing newline

    // Handle -n option(s)
    while (argv[i] && ft_strncmp(argv[i], "-n", 2) == 0)
    {
        // Check if it's only "-n" or "-nnn..." without other characters
        int j = 2;
        while (argv[i][j] && argv[i][j] == 'n')
            j++;
            
        if (argv[i][j] == '\0') // Valid -n option
        {
            newline = 0;
            i++;
        }
        else
            break;
    }

    // Print arguments
    int first_arg = 1;
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

    return (0); // Always return 0 for echo
}