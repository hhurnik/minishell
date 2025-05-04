#include "inc/minishell.h"

void	process_input(char *input, t_env **env)
{
	t_resources	res;
	t_token		*tokens;
	t_cmd		*cmds;

	tokens = NULL;
	cmds = NULL;
	res.input = NULL;
	res.tokens = NULL;
	res.cmds = NULL;
	res.env = NULL;
	res.input = input;
	res.env = *env;
	if (!initialize_and_tokenize(input, &tokens))
		return ;
	res.tokens = tokens;
	if (!prepare_commands(tokens, &cmds, &res))
	{
		free_tokens(tokens);
		free(input);
		return ;
	}
	res.cmds = cmds;
	execute_commands(cmds, env, &res);
}

bool	initialize_and_tokenize(char *input, t_token **tokens)
{
	*tokens = tokenize_input(input);
	if (!*tokens)
	{
		free(input);
		return (false);
	}
	return (true);
}

bool	prepare_commands(t_token *tokens, t_cmd **cmds, t_resources *res)
{
	expand_variables(tokens, res->env);
	*cmds = parse_tokens(tokens);
	if (!*cmds)
		return (false);
	shift_empty_args(*cmds);
	if (handle_all_heredocs(*cmds) != 0)
	{
		close_heredoc_fds(*cmds);
		free_cmds(*cmds);
		return (false);
	}
	if (!(*cmds)->args || !(*cmds)->args[0] || (*cmds)->args[0][0] == '\0')
	{
		close_heredoc_fds(*cmds);
		free_cmds(*cmds);
		return (false);
	}
	return (true);
}

void	execute_commands(t_cmd *cmds, t_env **env, t_resources *res)
{
	t_fds	fds;

	setup_signals_for_command();
	if (is_builtin(cmds->args[0]) && !cmds->next)
	{
		fds.stdin = dup(STDIN_FILENO);
		fds.stdout = dup(STDOUT_FILENO);
		handle_parent_builtin(cmds, fds, res);
	}
	else
		execute(cmds, env);
	setup_signals_for_prompt();
	close_heredoc_fds(cmds);
	free_cmds(cmds);
	free_tokens(res->tokens);
	free(res->input);
}
