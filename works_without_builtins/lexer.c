#include "inc/minishell.h"


// dodaje tokeny for a single command line, to the beginning of the linked list
void	add_token(t_token **tokens, char *value, t_token_type type)
{
	t_token *new = malloc(sizeof(t_token));
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
	t_token *prev = NULL;
	t_token *current = tokens;
	t_token *next = NULL;

	while (current)
	{
		next = current->next;
		current->next = prev;
		prev = current;
		current = next;
	}
	return (prev);
}

void	free_tokens(t_token *tokens)
{
	while (tokens)
	{
		t_token *tmp = tokens;
		tokens = tokens->next;
		free(tmp->value);
		free(tmp);
	}
}
// to read if its after ' or ", as long as its not ' or " at the end
// error if it doesnt end with ' or "
char	*read_quoted_content(char *input, int *i, t_quote quote, int *error)
{
	char quote_char = (quote == SINGLE_QUOTE) ? '\'' : '"';
	int start = *i;
	while (input[*i] && input[*i] != quote_char)
	{
		(*i)++;
	}
	if (input[*i] != quote_char)
	{
		*error = 1;
		return (NULL);
	}
	char *content = ft_substr(input, start, *i - start);
	(*i)++;
	return (content);
}

t_token	*tokenize_input(char *input)
{
	t_token *tokens = NULL;
	char *current_word = NULL;
	t_quote current_quote = NO_QUOTE;
	int i = 0;
	int error = 0;

	while (input[i] && !error)
	{
		if (current_quote == NO_QUOTE)
		{
			if (ft_isspace(input[i]))
			{
				if (current_word)
				{
					add_token(&tokens, current_word, T_WORD);
					free(current_word); // Free after adding to tokens
					current_word = NULL;
				}
				i++;
				continue ;
			}

			if (input[i] == '|')
			{
				if (current_word)
				{
					add_token(&tokens, current_word, T_WORD);
					free(current_word); // Free after adding to tokens
					current_word = NULL;
				}
				add_token(&tokens, "|", T_PIPE);
				i++;
			}
			else if (input[i] == '>' || input[i] == '<')
			{
				if (current_word)
				{
					add_token(&tokens, current_word, T_WORD);
					free(current_word); // Free after adding to tokens
					current_word = NULL;
				}
				handle_redirection(input, &i, &tokens);
			}
			else if (input[i] == '\'' || input[i] == '"')
			{
				current_quote = (input[i] == '\'') ? SINGLE_QUOTE : DOUBLE_QUOTE;
				i++;
				char *quoted_part = read_quoted_content(input, &i,
						current_quote, &error);
				if (error)
				{
					free(current_word); // Free if error occurs
					ft_putstr_fd("minishell: syntax error: unclosed quote\n",
						STDERR_FILENO);
					break ;
				}
				current_word = ft_strjoin_free(current_word, quoted_part);
				free(quoted_part);
				current_quote = NO_QUOTE;
			}
			else
			{
				current_word = ft_strjoin_char(current_word, input[i]);
				i++;
			}
		}
		else
		{
			error = 1;
			break ;
		}
	}

	if (current_word)
	{
		add_token(&tokens, current_word, T_WORD);
		free(current_word); // Free the final word
	}

	if (error)
	{
		free(current_word); // Cleanup if error
		free_tokens(tokens);
		return (NULL);
	}

	tokens = reverse_tokens(tokens);
	return (tokens);
}

void	handle_redirection(char *input, int *i, t_token **tokens)
{
	char first_char = input[*i]; // get the current character at this position
	t_token_type type;           // store the type of redirection

	(*i)++;
	// Sprawdź, czy to podwójny operator (>> lub <<)
	//(if the next character is the same as the first one)
	if (input[*i] == first_char)
	{
		(*i)++;
		// t_token_type type = (first_char == '>') ? T_APPEND : T_HEREDOC;
		if (first_char == '>')
			type = T_APPEND; // >>
		else
			type = T_HEREDOC; // <<

		add_token(tokens, (char[]){first_char, first_char, '\0'}, type);
	}
	else
	{ // Pojedynczy operator (> lub <)
		if (first_char == '>')
			type = T_REDIR_OUT;
		else
			type = T_REDIR_IN;
		// t_token_type type = (first_char == '>') ? T_REDIR_OUT : T_REDIR_IN;
		add_token(tokens, (char[]){first_char, '\0'}, type);
	}
}
