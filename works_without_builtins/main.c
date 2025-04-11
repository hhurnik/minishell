#include "inc/minishell.h"

//leaks from readline, add historu -  is it from readline as well??

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;

	setup_signals();
	t_env *env = init_env(envp); //leakk from ft_strdup, env new, init env
	//malloc in ft_substr, malloc in ft_strdup, env_new, init_env

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

			if (!tokens)
			{
				free(input);
				continue ;
			}

			expand_variables(tokens, env);
			t_cmd *cmds = parse_tokens(tokens);

			if (cmds && cmds->args && cmds->args[0])
				execute(cmds, &env); ///problem tutaj, valgrind 989 allocs, 321 frees

			free_tokens(tokens);
			free_cmds(cmds);
		}
		free(input);
	}
	return (0);
}