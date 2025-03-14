/*Komenda env służy do wyświetlania zmiennych środowiskowych. 
Może być również używana do uruchamiania programu w zmodyfikowanym środowisku. 
Kiedy jest wywoływana bez opcji lub argumentów, po prostu wypisuje wszystkie zmienne środowiskowe.

jak be argumentow - wypisuje wszystkie zmienne środowiskowe, jedna na każdej linii, w formacie NAZWA=ZAWARTOŚĆ. 
Wyświetla to środowisko, w którym działa aktualna powłoka.

na pryklad: env

HOME=/home/user
PATH=/usr/bin:/bin:/usr/sbin:/sbin
SHELL=/bin/bash
LANG=en_US.UTF-8*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "ms.h"

int bi_env(char *argv[]) 
{
    // are there args after env?
    if (argv[1] != NULL) 
    {
        write(2, "env: no arguments are allowed\n", 30);
        return (1);
    }

    // Get the first environment variable
    char *env_var = getenv("HOME"); // Start from the HOME variable (or any other environment variable)
    
    // Print all environment variables until we reach the end
    while (env_var != NULL) 
    {
        write(1, env_var, ft_strlen(env_var));  // Print the environment variable value
        write(1, "\n", 1);                    // Print a newline after each variable
        env_var = getenv("NEXT_ENV_VAR");   // Get the next environment variable
    }
    return (0);
}