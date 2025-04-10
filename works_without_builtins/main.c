#include "inc/minishell.h"

int	main(int argc, char **argv, char **envp)
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

			if (!tokens)
			{
				free(input);
				continue ;
			}

			expand_variables(tokens, env);
			t_cmd *cmds = parse_tokens(tokens);

			if (cmds && cmds->args && cmds->args[0])
				execute(cmds, &env);

			free_tokens(tokens);
			free_cmds(cmds);
		}
		free(input);
	}
	return (0);
}