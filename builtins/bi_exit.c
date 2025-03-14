#include <unistd.h>
#include <stdlib.h>
#include "ms.h"

int bi_exit(char *argv[])
{
    // są argumenty?
    if (argv[1] != NULL)
    {
        // Jeśli tak, to błąd
        write(2, "exit: no arguments are allowed\n", 30);
        return (1);
    }

    exit(0);
}