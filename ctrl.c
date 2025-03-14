#include "ms.h"


/*
ctrl-C displays a new prompt on a new line.
◦ ctrl-D exits the shell.
◦ ctrl-\ does nothing
*/

/*
In a typical shell, when the user presses Ctrl+D:

The shell receives an EOF (End Of File) signal, which tells it that no more input 
is coming. Upon receiving EOF, the shell should exit gracefully.
*/

// pod spodem is supposed to exit

void minishell_loop() 
{
    char *input;

    while (1) 
    {
        // Prompt for input
        input = readline("minishell> ");

        // If input is NULL (Ctrl+D), exit the shell
        if (input == NULL) 
        {
            printf("\nExiting minishell... Goodbye!\n");
            exit(0);
        }

        // If the input is not empty, add it to history
        if (input && input[0] != '\0')
            add_history(input);

        // Handle input commands here -> parse and execute the commands)

        free(input);
    }
}


// testowanie
int main()
{

    minishell_loop();
    return (0);
}

