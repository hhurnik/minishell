#include <stdio.h>
#include <string.h>
#include <unistd.h>
//wypisuje tekst na standardowe wyjście - terminal most often (echo - na koniec wpisuje \n)
//echo -n - without newline at the end
//argv-tablica stringów: argv[0] == "echo", args[1..] to argumenty

int builtin_echo(char *argv[])
{
    int i = 1;
    int newline = 1; // domyślnie wypisujemy nową linię na koncu

    // Obsługa -n (może być kilka: -n, -nnnn)
    if (argv[i] && strncmp(argv[i], "-n", 2) == 0)
    {
        int j = 2;
        while (argv[i][j] == 'n')
            j++;
        if (argv[i][j] == '\0') // tylko -n lub -nnn...
        {
            newline = 0;
            i++;
        }
    }

    // Wypisz argumenty oddzielone spacją
    while (argv[i])
    {
        write(1, argv[i], strlen(argv[i]));
        if (argv[i + 1])
            write(1, " ", 1); /// wypisuje spacjw pomiedy, ale nie na koncu
        i++;
    }

    if (newline)
        write(1, "\n", 1);

    return 0;
}