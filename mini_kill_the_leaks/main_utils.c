#include "inc/minishell.h"

// leakami, pred valgrindem
// void	free_env(t_env *env)
// {
// 	t_env	*tmp;

// 	while (env)
// 	{
// 		tmp = env;
// 		env = env->next;
// 		free(tmp->key);
// 		free(tmp->value);
// 		free(tmp);
// 	}
// }

//po valgrindie
void free_env(t_env *env)
{
    t_env *tmp;
    
    while (env)
    {
        tmp = env->next;  // Save next pointer before freeing
        free(env->key);
        free(env->value);
        free(env);
        env = tmp;
    }
}

void	init_shell(char **envp, t_env **env)
{
	setup_signals_for_prompt();
	*env = NULL;
	init_env(envp, env);
}

// void	handle_eof(t_env *env, int exit_status)
// {
// 	ft_putstr_fd("exit\n", STDOUT_FILENO);
// 	cleanup(env, NULL, NULL, NULL);
// 	exit(exit_status);
// }

//dobre tera
// void handle_eof(t_env *env, int exit_status)
// {
//     t_resources res;

//     res.env = env;
//     res.cmds = NULL;
//     res.tokens = NULL;
//     res.input = NULL;

//     ft_putstr_fd("exit\n", STDOUT_FILENO);
// 	rl_clear_history(); // added 28.04
//     cleanup(&res); // Pass the t_resources structure
//     exit(exit_status);
// }



//diala  exit no leaks!! ale nie /bin/;s
// void handle_eof(t_env *env, int exit_status)
// {
//     ft_putendl_fd("exit", STDERR_FILENO);
//     rl_clear_history(); // Clear Readline history
//     free_env(env);
//     exit(exit_status);
// }

void handle_eof(int exit_status)
{
    // Don't free env here, it's already freed in cleanup
    // free_env(env);  <- Remove this line
    
    // Clear readline history
    rl_clear_history();
    
    // Print a newline for better UX when exiting with Ctrl+D
    ft_putstr_fd("exit\n", STDOUT_FILENO);
    
    exit(exit_status);
}

// void	process_input(char *input, t_env **env)
// {
// 	t_token	*tokens;
// 	t_cmd	*cmds;

// 	tokens = tokenize_input(input);
// 	if (!tokens)
// 	{
// 		free(input); //added valgrind 28.04
// 		return ;
// 	}
// 	expand_variables(tokens, *env);
// 	cmds = parse_tokens(tokens);
// 	if (!cmds)
// 	{
// 		free_tokens(tokens);
// 		free(input); //added valgrind 28.04
// 		return ;
// 	}
// 	if (cmds->args && cmds->args[0])
// 	{
// 		setup_signals_for_command();
// 		execute(cmds, env);
// 		setup_signals_for_prompt();
// 	}
// 	clean_cmds_tokens_input(cmds, tokens, input);
// }

//2 leaki
// void process_input(char *input, t_env **env)
// {
//     t_token *tokens = NULL;
//     t_cmd *cmds = NULL;

//     tokens = tokenize_input(input);
//     if (!tokens) {
//         free(input); // Free input if tokenization fails
//         return;
//     }
//     expand_variables(tokens, *env);
//     cmds = parse_tokens(tokens);
//     if (!cmds) {
//         free_tokens(tokens); // Free tokens if parsing fails
//         free(input);       // Free input if parsing fails
//         return;
//     }

//     if (cmds->args && cmds->args[0]) {
//         setup_signals_for_command();
//         // *** Pass tokens and input to execute ***
//         execute(cmds, env);
//         setup_signals_for_prompt();
//     }

//     // If execute finished normally (didn't call exit), clean up.
//     // If bi_exit was called successfully, it exited, and we won't reach here.
//     // If bi_exit failed validation and returned, we WILL reach here and clean up.
//     // Note: We assume execute itself doesn't free these if exit wasn't called.
//     // If exit was called successfully, these pointers are invalid anyway after cleanup in bi_exit.
//     if (cmds)
// 		free_cmds(cmds);
//     if (tokens)
// 		free_tokens(tokens);
//     if (input)
// 		free(input);
// }

// //2 leaki, ok
// void process_input(char *input, t_env **env)
// {
//     t_token *tokens = NULL;
//     t_cmd *cmds = NULL;

//     tokens = tokenize_input(input);
//     if (!tokens) {
//         free(input);
//         return;
//     }
    
//     expand_variables(tokens, *env);
    
//     cmds = parse_tokens(tokens);
//     if (!cmds) {
//         free_tokens(tokens);
//         free(input);
//         return;
//     }

//     if (cmds->args && cmds->args[0]) 
//     {
//         setup_signals_for_command();
//         execute(cmds, env);
//         setup_signals_for_prompt();
//     }

//     // Always free resources
//     free_cmds(cmds);
//     free_tokens(tokens);
//     free(input);
// }
void process_input(char *input, t_env **env)
{
    t_token *tokens = NULL;
    t_cmd *cmds = NULL;
    t_resources res = {0};  // Initialize resources structure
    
    // Track ALL resources from the beginning
    res.input = input;
    res.env = *env;
    
    tokens = tokenize_input(input);
    if (!tokens) {
        free(input);
        return;
    }
    
    res.tokens = tokens;  // Save tokens in resources
    
    expand_variables(tokens, *env);
    
    cmds = parse_tokens(tokens);
    if (!cmds) {
        free_tokens(tokens);
        free(input);
        return;
    }
    
    res.cmds = cmds;  // Save commands in resources
    
    if (cmds->args && cmds->args[0]) {
        setup_signals_for_command();
        
        // Pass the COMPLETE resources structure
        if (is_builtin(cmds->args[0])) 
        {
            // Create and initialize t_fds structure with the duplicated file descriptors
            t_fds fds;
            fds.stdin = dup(STDIN_FILENO);
            fds.stdout = dup(STDOUT_FILENO);
            
            // Pass the properly initialized t_fds to handle_parent_builtin
            handle_parent_builtin(cmds, fds, &res);
        } else {
            execute(cmds, env);
        }
        
        setup_signals_for_prompt();
    }

    // Only clean up if we didn't call exit
    free_cmds(cmds);
    free_tokens(tokens);
    free(input);
}

void	clean_cmds_tokens_input(t_cmd *cmds, t_token *tokens, char *input)
{
	if (cmds)
		free_cmds(cmds);
	if (tokens)
		free_tokens(tokens);
	if (input)
		free(input);
}
