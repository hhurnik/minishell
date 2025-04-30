#include "inc/minishell.h"

// dodaje tokeny for a single command line, to the beginning of the linked list
void	add_token(t_token **tokens, char *value, t_token_type type)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return ;
	new->value = ft_strdup(value); // MUST duplicate the string
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

void	handle_input_char(char *input, int *i, char **current_word,
		t_token **tokens)
{
	if (ft_isspace(input[*i]))
		skip_space(i, current_word, tokens);
	else if (is_pipe(input[*i]))
		handle_pipe(i, current_word, tokens);
	else if (is_redirect(input[*i]))
		handle_redirect_token(input, i, current_word, tokens);
	else
		append_char_to_word(current_word, input[(*i)++]);
}