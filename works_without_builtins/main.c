#include "inc/minishell.h"

int main(int argc, char **argv, char **envp) {

    (void)argc; // Unikaj ostrzeżeń o nieużywanych argumentach
    (void)argv; // Unikaj ostrzeżeń o nieużywanych argumentach

    t_env *env = init_env(envp); // Inicjalizuj środowisko
    setup_signals();

    while (1) {
        char *input = readline("minishell$ ");
        if (!input) { // Ctrl+D
            ft_putstr_fd("exit\n", STDOUT_FILENO);
            exit(0);
        }
        if (*input) {
            add_history(input);
            t_token *tokens = tokenize_input(input);
            t_cmd *cmds = parse_tokens(tokens);
            expand_variables(tokens, env);
            execute(cmds, &env);
            // Zwolnij pamięć (tokens, cmds)
        }
        free(input);
    }
    return 0;
}
