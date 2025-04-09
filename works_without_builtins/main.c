#include "inc/minishell.h"


// int	main(int argc, char **argv, char **envp)
// {
// 	(void)argc; // Unikaj ostrzeżeń o nieużywanych argumentach
// 	(void)argv; // Unikaj ostrzeżeń o nieużywanych argumentach

// 	t_env *env = init_env(envp); // Inicjalizuj środowisko
// 	setup_signals();

// 	while (1)
// 	{
// 		char *input = readline("minishell$ ");
// 		if (!input)
// 		{ // Ctrl+D
// 			ft_putstr_fd("exit\n", STDOUT_FILENO);
// 			exit(0);
// 		}
// 		if (*input)
// 		{
// 			add_history(input);
// 			t_token *tokens = tokenize_input(input);
// 			t_cmd *cmds = parse_tokens(tokens);
// 			expand_variables(tokens, env);
// 			execute(cmds, &env);
// 			// Zwolnij pamięć (tokens, cmds)
// 		}
// 		free(input);
// 	}
// 	return (0);
// }

void print_tokens(t_token *tokens)
{
    printf("\n--- TOKEN DEBUG ---\n");
    int i = 0;
    while (tokens) {
        printf("Token %d: [%s] Type: %d\n", i++, tokens->value, tokens->type);
        tokens = tokens->next;
    }
    printf("--- END TOKENS ---\n\n");
}

int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;

    setup_signals();
    t_env *env = init_env(envp);
    
    while (1)
    {
        char *input = readline("minishell$ ");
        if (!input)
        {
            ft_putstr_fd("exit\n", STDOUT_FILENO);
            exit(0);
        }
        if (*input)
        {
            add_history(input);
            t_token *tokens = tokenize_input(input);
            
            // DEBUG: Print tokens right after tokenization
            //print_tokens(tokens);  // <--- ADD THIS LINE
            
            t_cmd *cmds = parse_tokens(tokens);
            expand_variables(tokens, env);
            
            if (cmds && cmds->args && cmds->args[0])
                execute(cmds, &env);
            // Free tokens and cmds here
        }
        free(input);
    }
    return (0);
}