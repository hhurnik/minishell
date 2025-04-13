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


//przy init_env2 nie ma definitely lost leakow
int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;

	setup_signals();
	//t_env *env = init_env(envp); // stareleakk from ft_strdup, env new, init env
	//malloc in ft_substr, malloc in ft_strdup, env_new, init_env
	t_env *env = NULL; //dodane 13.04
	init_env2(envp, &env); // dodane 13.04

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
	free_env(env); //dodane 13.04, sprawdzam
	return (0);
}