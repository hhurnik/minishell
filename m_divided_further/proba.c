
// #include "inc/minishell.h"

// int main(int argc, char **argv, char **envp)
// {
// 	t_env	*env;
//     int		exit_status_on_eof;
// 	char	*input;
// 	t_token	*tokens;
// 	t_cmd	*cmds;
	
//     (void)argc;
//     (void)argv;
// 	exit_status_on_eof = 0; // Store exit status if needed
//     setup_signals_for_prompt(); // Set up signal handling for the prompt
//     env = NULL;
//     init_env(envp, &env);

//     while (1)
//     {
//         input = readline("minishell$ ");
//         if (!input) // Handle Ctrl+D (EOF)
//         {
//             ft_putstr_fd("exit\n", STDOUT_FILENO);
//             exit_status_on_eof = g_exit_status; // Save last status
//             cleanup(env, NULL, NULL, NULL); // Clean up environment
//             exit(exit_status_on_eof);         // Exit gracefully
//         }

//         tokens = NULL;
//         cmds = NULL;

//         if (*input)
//         {
//             add_history(input);
//             tokens = tokenize_input(input);

//             if (!tokens)
//             {
//                 free(input); // Input freed here if tokenization failed
//                 input = NULL;
//                 continue;
//             }

//             expand_variables(tokens, env);
//             cmds = parse_tokens(tokens); // Potential leak if parse fails mid-way

//             // Check if parsing failed and returned NULL but tokens existed
//             if (!cmds && tokens) 
// 			{
// 				free_tokens(tokens);
//                 free(input);
//                 continue ;
//                  // Free tokens if parsing failed to produce commands
//                 // free_tokens(tokens); // Already freed in parse_tokens error path? Double check.
//                 // tokens = NULL;       // Or handle error from parse_tokens better
//             }
//             else if (cmds && cmds->args && cmds->args[0])
//             {
//                 setup_signals_for_command(); // Set up signal handling for command execution
//                 execute(cmds, &env);
//                 setup_signals_for_prompt(); // Restore signal handling for the prompt
//             }
//         }

//         // Free structures for this iteration *before* the next readline
//         if (cmds) {
//             free_cmds(cmds);
//             cmds = NULL;
//         }
//         if (tokens) {
//             free_tokens(tokens);
//             tokens = NULL;
//         }
//         if (input) {
//             free(input);
//             input = NULL;
//         }
//     }

//     // This part is now effectively unreachable due to exit() calls
//     // cleanup(env, NULL, NULL, NULL); // Call cleanup here if loop could break differently
//     // return (g_exit_status); // Return last status
// }

