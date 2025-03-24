#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "ms.h"

// tylko echo -n
//wypisuje tekst na standardowe wyjście - terminal most often (echo - na koniec wpisuje \n)
//echo -n - without newline at the end
//argv-tablica stringów: argv[0] == "echo", args[1..] to argumenty

// w bashu echo -n -n "something"  -- tez nie wypisze nowej linii - zaimplementowac?
// nawet jesli jest 

int bi_echo(char *argv[])
{
    int i = 1; // bez argumentow i tak wypisze \n
    int newline = 1; // domyślnie wypisujemy nową linię na koncu

    // Obsługa -n (może być kilka, tak dziala echo -n w bashu: -n, -nnnn)
    // akceptuje -n -n -nnn -nnn  "something" - wypisuje tylko somehtign
    while (argv[i] && ft_strncmp(argv[i], "-n", 2) == 0)
    {
        int j = 2;
        while (argv[i][j] == 'n')
            j++;
        if (argv[i][j] == '\0') // tylko -n lub -nnn...
        {
            newline = 0;
            i++;
        }
        else
            break;
    }


    // Wypisz argumenty oddzielone spacją
    while (argv[i])
    {
        write(1, argv[i], ft_strlen(argv[i]));
        if (argv[i + 1])
            write(1, " ", 1); /// wypisuje spacjw pomiedy, ale nie na koncu
        i++;
    }

    if (newline)
        write(1, "\n", 1);

    return (0);
}

int main(int argc, char *argv[]) 
{
    if (argc == 1) 
    {
        // No arguments passed, so we just print an empty line
        write(1, "\n", 1);
    } 
    else 
    {
        // Call the echo function with the arguments from the command line
        bi_echo(argv);
    }
    
    return (0);
}