#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
int g_exit_status = 0;
extern char **environ;

typedef enum e_token_type 
{
TOKEN_WORD, // zwykłe słowo, polecenie, argument
TOKEN_PIPE, // |
TOKEN_REDIRECT_IN, //
TOKEN_REDIRECT_OUT, // >
TOKEN_APPEND, // >>
TOKEN_HEREDOC, //
} t_token_type;

typedef struct s_token
{
    t_token_type type;
    char *value;
    struct s_token *next;
} t_token;


// void setup_signals(void);

void handle_signal(int sig)
{
    if (sig == SIGINT)
    {
        rl_replace_line("\0", 0);
        write(1, "\n", 1);
        rl_on_new_line();
        rl_redisplay();
        g_exit_status = 1;
    }
}

void setup_signals(void)
{
    signal(SIGINT, handle_signal);
    signal(SIGQUIT, SIG_IGN);
}
void init_shell()
{
    printf("\n\n\n\n******************"
        "************************");
    printf("\n\n\n\t****MY SHELL****");
    printf("\n\n\t-USE AT YOUR OWN RISK-");
    printf("\n\n\n\n*******************"
        "***********************");
    char* username = getenv("USER");
    printf("\n\n\nUSER is: @%s", username);
    printf("\n");
    sleep(1);
}

int main()
{
    char *input;
    // char **tokens;

    setup_signals();
    init_shell();
    while (1)
    {
        input = readline("Squish🦆> ");
        if (!input)
        {
            printf("exit\n");
            break;
        }
        if (input[0] == '\0')
        {
            free(input);
            continue;
        }
        if (*input)
            // add_history(input);
        // execute_command(input, env);
        free(input);
        // free(tokens);
    }
    // clear_history();
    return EXIT_SUCCESS;
}

