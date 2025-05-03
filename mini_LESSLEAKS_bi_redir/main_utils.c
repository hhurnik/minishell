#include "inc/minishell.h"

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
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

void	handle_eof(int exit_status)
{
	rl_clear_history();
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	exit(exit_status);
}

static int	ensure_current_word(char **current_word)
{
	if (*current_word == NULL)
	{
		*current_word = ft_strdup("");
		if (!*current_word)
		{
			perror("minishell: malloc error");
			return (0);
		}
	}
	return (1);
}

void	append_char_to_word(char **word, char c)
{
	char	*old_word;

	if (!ensure_current_word(word))
	{
		return ;
	}
	old_word = *word;
	*word = ft_strjoin_char(old_word, c);
	if (!*word)
	{
		perror("minishell: malloc error");
	}
}
