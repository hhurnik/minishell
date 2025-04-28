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

//clear history at the end
/* 4,016 bytes in 1 blocks are still reachable in loss record 41 of 63
==2074==    at 0x484880F: malloc (vg_replace_malloc.c:446)
==2074==    by 0x48A0BAC: xmalloc (in /usr/lib/x86_64-linux-gnu/libreadline.so.8.1)
==2074==    by 0x4899FD6: add_history (in /usr/lib/x86_64-linux-gnu/libreadline.so.8.1)
==2074==    by 0x10B197: main (main.c:19)*/