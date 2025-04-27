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

void	handle_eof(t_env *env, int exit_status)
{
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	cleanup(env, NULL, NULL, NULL);
	exit(exit_status);
}

void	process_input(char *input, t_env **env)
{
	t_token	*tokens;
	t_cmd	*cmds;

	tokens = tokenize_input(input);
	if (!tokens)
		return ;
	expand_variables(tokens, *env);
	cmds = parse_tokens(tokens);
	if (!cmds)
	{
		free_tokens(tokens);
		return ;
	}
	if (cmds->args && cmds->args[0])
	{
		setup_signals_for_command();
		execute(cmds, env);
		setup_signals_for_prompt();
	}
	clean_cmds_tokens_input(cmds, tokens, input);
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
