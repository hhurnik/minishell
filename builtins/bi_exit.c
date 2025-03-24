#include <unistd.h>
#include <stdlib.h>
#include "ms.h"

//exit with no options

// but with possible arguments?
// If exit is called with no argument, the shell or program should terminate with an exit status of 0.

//If exit is called with an argument, this argument will be treated as the exit status code.
//argument should be an int, the shell should exit with this status

// exit status code is an integer from 0 to 255, unsigned
// jesli aru=gumentem bedzie ujemna liczba, system zamieni je na unsigned przy uzyciu modulo

void write_stderr(char *str)
{
    write(2, str, ft_strlen(str));
}

int is_integer(char *str)
{
    int i = 0;

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

int bi_exit(char *argv[])
{
    int exit_code;
    //jesli jest argument za exit
    if (argv[1])
    {
        if(is_integer(argv[1]))
        {
            exit_code = ft_atoi(argv[1]);

            if (exit_code < 0)
                exit_code = ((exit_code % 256) + 256) % 256; // zeby nigdy nie byl negative

            exit(exit_code);
        }
        else
        {
            write_stderr("exit: numeric argument required\n");
            return (1);
        }
    }
    else
        exit(0);
    
    return (0);
}