#include "inc/minishell.h"
#include <mcheck.h>

int main(int argc, char **argv, char **envp)
{
    t_env *env;
    char *input;
    t_resources res;

    (void)argc;
    (void)argv;
    init_shell(envp, &env);

    while (1)
    {
        input = readline("minishell$ ");
        if (!input)
        {
            res.env = env;
            res.cmds = NULL;
            res.tokens = NULL;
            res.input = NULL;
            cleanup(&res); // Pass the t_resources structure
            //handle_eof(env, g_exit_status);
            handle_eof(g_exit_status);
        }
        if (*input)
        {
            add_history(input);
            process_input(input, &env);
        }
        else
            free(input);
    }
    return (0);
}
