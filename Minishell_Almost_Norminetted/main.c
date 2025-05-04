#include "inc/minishell.h"

// nowe, podzielone
static void	handle_no_input(t_env *env)
{
	t_resources	res;

	res.env = env;
	res.cmds = NULL;
	res.tokens = NULL;
	res.input = NULL;
	cleanup(&res);
	handle_eof(g_exit_status);
}

int	main(int argc, char **argv, char **envp)
{
	t_env	*env;
	char	*input;

	(void)argc;
	(void)argv;
	init_shell(envp, &env);
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
			handle_no_input(env);
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
