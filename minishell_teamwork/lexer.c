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
                    free(current_word);
                    current_word = NULL;
                }
                i++;
                continue;
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
           else if (input[i] == '\'' || input[i] == '"') {
                // Finalizuj bieżące słowo przed cudzysłowem
                if (current_word) {
                    add_token(&tokens, current_word, T_WORD);
                    free(current_word);
                    current_word = NULL;
                }

                // Obsługa cudzysłowu
                current_quote = (input[i] == '\'') ? SINGLE_QUOTE : DOUBLE_QUOTE;
                i++;
                char *quoted_part = read_quoted_content(input, &i, current_quote, &error);
                
                if (error) {
                    ft_putstr_fd("Squish: syntax error: unclosed quote\n", STDERR_FILENO);
                    break;
                }

                // Dodaj zawartość cudzysłowu jako specjalny token
                t_token_type quote_type = (current_quote == SINGLE_QUOTE) ? T_SINGLE_QUOTED : T_DOUBLE_QUOTED;
                add_token(&tokens, quoted_part, quote_type);
                free(quoted_part);
                current_quote = NO_QUOTE;
            }
            else {
                current_word = ft_strjoin_char(current_word, input[i]);
                i++;
            }
        }
        else {
            error = 1;
            break;
        }
    }

    if (current_word) {
        add_token(&tokens, current_word, T_WORD);
        free(current_word);
    }

    if (error) {
        free(current_word);
        free_tokens(tokens);
        return NULL;
    }
    return reverse_tokens(tokens);
}

//sprawdza czy nastepny token jest slowem
void handle_redirection(char *input, int *i, t_token **tokens)
{
    char first_char = input[*i];  // Get the current character at this position
    t_token_type type;            // Store the type of redirection

    (*i)++;  // Move to the next character to check for redirection

    // Check if it's a double operator (>> or <<)
    if (input[*i] == first_char)
    {
        (*i)++;
        if (first_char == '>')
            type = T_APPEND;  // >>
        else
            type = T_HEREDOC;  // <<

        // Add the token for the double operator
        add_token(tokens, (char[]){first_char, first_char, '\0'}, type);
    }
    else
    {  // Single operator (> or <)
        if (first_char == '>')
            type = T_REDIR_OUT;  // >
        else
            type = T_REDIR_IN;   // <

        // Add the token for the single operator
        add_token(tokens, (char[]){first_char, '\0'}, type);
    }

    // After adding the redirection token, check the next token
    // The next token must be a filename, so it should be of type T_WORD
    if (*tokens && (*tokens)->next && (*tokens)->next->type != T_WORD)
    {
        // If the next token is not a filename (T_WORD), print an error
        ft_putstr_fd("minishell: syntax error: missing file name after redirection\n", STDERR_FILENO);
        // You can set an error flag here or return to indicate an error
        *tokens = NULL;  // To indicate an error and avoid further processing
        return;
    }
}
