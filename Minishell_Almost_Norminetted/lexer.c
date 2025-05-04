#include "inc/minishell.h"

// dodaje tokeny for a single command line, to the beginning of the linked list
void	add_token(t_token **tokens, char *value, t_token_type type)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return ;
	new->value = ft_strdup(value);
	if (!new->value)
	{
		free(new);
		return ;
	}
	new->type = type;
	new->next = *tokens;
	*tokens = new;
}

t_token	*reverse_tokens(t_token *tokens)
{
	t_token	*prev;
	t_token	*current;
	t_token	*next;

	prev = NULL;
	current = tokens;
	next = NULL;
	while (current)
	{
		next = current->next;
		current->next = prev;
		prev = current;
		current = next;
	}
	return (prev);
}
