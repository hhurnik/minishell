/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhurnik <hhurnik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:01:40 by hhurnik           #+#    #+#             */
/*   Updated: 2025/05/05 16:01:42 by hhurnik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

char	*read_quoted_content(char *input, int *i, t_quote quote, int *error)
{
	char	quote_char;
	int		start;
	char	*content;

	if (quote == SINGLE_QUOTE)
		quote_char = '\'';
	else
		quote_char = '\"';
	start = *i;
	while (input[*i] && input[*i] != quote_char)
		(*i)++;
	if (input[*i] != quote_char)
	{
		*error = 1;
		return (NULL);
	}
	content = ft_substr(input, start, *i - start);
	(*i)++;
	return (content);
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens->next;
		if (tokens->value)
			free(tokens->value);
		free(tokens);
		tokens = tmp;
	}
}
