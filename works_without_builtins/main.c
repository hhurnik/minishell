#include "inc/minishell.h"
 #include <mcheck.h>

//leaks from readline, add historu -  is it from readline as well??

void free_env(t_env *env)
{
    t_env *tmp;
    
    while (env)
    {
        tmp = env;
        env = env->next;
        free(tmp->key);
        free(tmp->value);
        free(tmp);
    }
}


//przy init_env2 nie ma definitely lost leakow - stare, dziala, ponizej proba
// int	main(int argc, char **argv, char **envp)
// {
// 	(void)argc;
// 	(void)argv;

// 	setup_signals();
// 	//t_env *env = init_env(envp); // stareleakk from ft_strdup, env new, init env
// 	//malloc in ft_substr, malloc in ft_strdup, env_new, init_env
// 	t_env *env = NULL; //dodane 13.04
// 	init_env2(envp, &env); // dodane 13.04

// 	while (1)
// 	{
// 		char *input = readline("minishell$ ");
// 		if (!input)
// 		{
// 			ft_putstr_fd("exit\n", STDOUT_FILENO);
// 			exit(0);
// 		}
// 		if (*input)
// 		{
// 			add_history(input);
// 			t_token *tokens = tokenize_input(input);

// 			if (!tokens)
// 			{
// 				free(input);
// 				continue ;
// 			}

// 			expand_variables(tokens, env);
// 			t_cmd *cmds = parse_tokens(tokens);

// 			if (cmds && cmds->args && cmds->args[0])
// 				execute(cmds, &env); ///problem tutaj, valgrind 989 allocs, 321 frees

// 			free_tokens(tokens);
// 			free_cmds(cmds);
// 		}
// 		free(input);
// 	}
// 	free_env(env); //dodane 13.04, sprawdzam
// 	return (0);
// }

/////koniec stare

// proba 13.04
int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;
    int exit_status_on_eof = 0; // Store exit status if needed

    setup_signals();
    t_env *env = NULL;
    init_env2(envp, &env);

    while (1)
    {
        char *input = readline("minishell$ ");
        if (!input) // Handle Ctrl+D (EOF)
        {
            ft_putstr_fd("exit\n", STDOUT_FILENO);
            exit_status_on_eof = g_exit_status; // Save last status
            cleanup(env, NULL, NULL, NULL); // Clean up environment
            exit(exit_status_on_eof);         // Exit gracefully
        }

        t_token *tokens = NULL;
        t_cmd *cmds = NULL;

        if (*input)
        {
            add_history(input);
            tokens = tokenize_input(input);

            if (!tokens)
            {
                free(input); // Input freed here if tokenization failed
                input = NULL;
                continue;
            }

            expand_variables(tokens, env);
            cmds = parse_tokens(tokens); // Potential leak if parse fails mid-way

            // Check if parsing failed and returned NULL but tokens existed
            if (!cmds && tokens) {
                 // Free tokens if parsing failed to produce commands
                // free_tokens(tokens); // Already freed in parse_tokens error path? Double check.
                // tokens = NULL;       // Or handle error from parse_tokens better
            }
             else if (cmds && cmds->args && cmds->args[0])
            {
                // Pass input and tokens to execute/builtins if they need it for cleanup
                execute(cmds, &env);
            }
        }

        // Free structures for this iteration *before* the next readline
        if (cmds) {
            free_cmds(cmds);
            cmds = NULL;
        }
        if (tokens) {
            free_tokens(tokens);
            tokens = NULL;
        }
        if (input) {
            free(input);
            input = NULL;
        }
    }

    // This part is now effectively unreachable due to exit() calls
    // cleanup(env, NULL, NULL, NULL); // Call cleanup here if loop could break differently
    // return (g_exit_status); // Return last status
}