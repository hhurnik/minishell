/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhurnik <hhurnik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:02:29 by hhurnik           #+#    #+#             */
/*   Updated: 2025/05/05 16:02:30 by hhurnik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

void	finalize_current_word(t_token **tokens, char **current_word,
		int single_quote_used)
{
	t_token_type	type;

	if (*current_word != NULL)
	{
		if (single_quote_used)
			type = T_SINGLE_QUOTED;
		else
			type = T_WORD;
		add_token(tokens, *current_word, type);
		free(*current_word);
		*current_word = NULL;
	}
}

void	process_redirection_token(char first_char, char *input, int *i,
	t_token **tokens)
{
	t_token_type	type;
	char			op_str[3];

	op_str[0] = first_char;
	if (input[*i] == first_char)
	{
		op_str[1] = first_char;
		op_str[2] = '\0';
		(*i)++;
		if (first_char == '>')
			type = T_APPEND;
		else
			type = T_HEREDOC;
	}
	else
	{
		op_str[1] = '\0';
		if (first_char == '>')
			type = T_REDIR_OUT;
		else
			type = T_REDIR_IN;
	}
	add_token(tokens, op_str, type);
}

void	handle_redirection(char *input, int *i, t_token **tokens)
{
	char	first_char;

	first_char = input[*i];
	(*i)++;
	process_redirection_token(first_char, input, i, tokens);
}

t_token	*tokenize_input(char *input)
{
	t_parse_state	state;

	state.tokens = NULL;
	state.current_word = NULL;
	state.i = 0;
	state.error = 0;
	state.single_quote_used = 0;
	state.current_quote = NO_QUOTE;
	parse_input_loop(input, &state);
	if (state.error)
	{
		if (state.current_word)
			free(state.current_word);
		free_tokens(state.tokens);
		return (NULL);
	}
	return (reverse_tokens(state.tokens));
}
