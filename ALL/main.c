/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhurnik <hhurnik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:02:34 by hhurnik           #+#    #+#             */
/*   Updated: 2025/05/05 16:02:34 by hhurnik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

void	handle_no_input(t_env *env)
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
