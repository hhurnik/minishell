#include "inc/minishell.h"
#include <mcheck.h>

int	main(int argc, char **argv, char **envp)
{
	t_env *env;
	char *input;

	(void)argc;
	(void)argv;
	init_shell(envp, &env);
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
			handle_eof(env, g_exit_status);
		if (*input)
		{
			add_history(input);
			process_input(input, &env);
		}
	}
	return (0);
}