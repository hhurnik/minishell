/*Komenda env służy do wyświetlania zmiennych środowiskowych.
Może być również używana do uruchamiania programu w zmodyfikowanym środowisku.
Kiedy jest wywoływana bez opcji lub argumentów,
	po prostu wypisuje wszystkie zmienne środowiskowe.

jak be argumentow - wypisuje wszystkie zmienne środowiskowe,
	jedna na każdej linii, w formacie NAZWA=ZAWARTOŚĆ.
Wyświetla to środowisko, w którym działa aktualna powłoka.

na pryklad: env

HOME=/home/user
PATH=/usr/bin:/bin:/usr/sbin:/sbin
SHELL=/bin/bash
LANG=en_US.UTF-8*/

#include "../inc/minishell.h"


/**
 * env builtin command - prints all environment variables
 * Format: NAME=VALUE (one per line)
 */
int bi_env(char **args, t_env **env)
{
    t_env *current;
    
    // Check for unsupported options/arguments
    if (args[1])
    {
        ft_putstr_fd("env: too many arguments\n", STDERR_FILENO);
        return (ERROR);
    }
    
    // Print all environment variables
    current = *env;
    while (current)
    {
        // Only print variables with values (standard env behavior)
        if (current->value && current->value[0] != '\0')
        {
            ft_putstr_fd(current->key, STDOUT_FILENO);
            ft_putstr_fd("=", STDOUT_FILENO);
            ft_putstr_fd(current->value, STDOUT_FILENO);
            ft_putstr_fd("\n", STDOUT_FILENO);
        }
        
        current = current->next;
    }
    
    return (SUCCESS);
}