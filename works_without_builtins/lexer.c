#include "inc/minishell.h"

//dodaje tokeny for a single command line, to the beginning of the linked list
// void	add_token(t_token **tokens, char *value, t_token_type type)
// {
// 	t_token *new = malloc(sizeof(t_token));
// 	new->value = ft_strdup(value);
// 	new->type = type;
// 	new->next = *tokens;
// 	*tokens = new;
// }

void add_token(t_token **tokens, char *value, t_token_type type)
{
    t_token *new = malloc(sizeof(t_token));
    if (!new) return;
    
    new->value = ft_strdup(value);  // MUST duplicate the string
    if (!new->value) {
        free(new);
        return;
    }
    
    new->type = type;
    new->next = *tokens;
    *tokens = new;
}

t_token *reverse_tokens(t_token *tokens)
{
    t_token *prev = NULL;
    t_token *current = tokens;
    t_token *next = NULL;
    
    while (current) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    return prev;
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
//to read if its after ' or ", as long as its not ' or " at the end
//error if it doesnt end with ' or "
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

/**
 * Tokenizuje wejście, rozbijając na części (np. "ls -l > out.txt" → [ls, -l, >,
	out.txt]).
 * Obsługuje cudzysłowy i specjalne znaki.
 */
//the word gets built char by char, for example "echo" - "e", then "ec"..
//it only adds a token if a whitespace or a special character "| > <"
//is encountered after it
// t_token	*tokenize_input(char *input)
// {
// 	t_token *tokens = NULL; //pointer to a  list of tokens
// 	char *current_word = NULL; //a buffer to build the current word being parsed
// 	t_quote current_quote = NO_QUOTE; //a state variable to track whether were inside "" or not
// 	int i = 0;
// 	int error = 0; //flag for a syntax error

// 	//process the input string until te end of the string input or until an error
// 	while (input[i] && !error)
// 	{
// 		if (current_quote == NO_QUOTE) //if were not inside the quotes ""
// 		{
// 			// Pomijaj białe znaki
// 			if (ft_isspace(input[i]))
// 			{
// 				if (current_word) //if we have a word, add it to tokens
// 				{
// 					add_token(&tokens, current_word, T_WORD);
// 					current_word = NULL;
// 				}
// 				i++;
// 				continue ;
// 			}
// 			// for(i = 0; tokens[i].value; i++)
// 			// 	printf("%s", tokens[i].value);
// 			// Sprawdź znaki specjalne
// 			if (input[i] == '|')
// 			{
// 				if (current_word)
// 				{
// 					add_token(&tokens, current_word, T_WORD);
// 					current_word = NULL;
// 				}
// 				add_token(&tokens, "|", T_PIPE);
// 				i++;
// 			}
// 			else if (input[i] == '>' || input[i] == '<')
// 			{
// 				if (current_word)
// 				{
// 					add_token(&tokens, current_word, T_WORD);
// 					current_word = NULL;
// 				}
// 				handle_redirection(input, &i, &tokens);
// 			}
// 			// the \ is needed here to tell the compiler im refering to the actual ' character
// 			//and not using it to define a character literal
// 			// ----is it a single ' or "
// 			else if (input[i] == '\'' || input[i] == '"')
// 			{
// 				if (input[i] == '\'') 
// 					current_quote = SINGLE_QUOTE;
// 				else 
// 					current_quote = DOUBLE_QUOTE;
// 				//current_quote = (input[i] == '\'') ? SINGLE_QUOTE : DOUBLE_QUOTE;
// 				i++;
// 				char *quoted_part = read_quoted_content(input, &i,
// 						current_quote, &error);
// 				if (error)
// 				{
// 					ft_putstr_fd("minishell: syntax error: unclosed quote\n",
// 						STDERR_FILENO);
// 					break ;
// 				}
// 				current_word = ft_strjoin_free(current_word, quoted_part);
// 				free(quoted_part);
// 				current_quote = NO_QUOTE;
// 			}
// 			else
// 			{
// 				// Dodaj zwykły znak do bieżącego słowa
// 				current_word = ft_strjoin_char(current_word, input[i]);
// 				i++;
// 			}
// 		}
// 		else
// 		{
// 			// Ten blok nie powinien być osiągany (cudzysłowy są obsługiwane wyżej)
// 			error = 1;
// 			break ;
// 		}
// 	}

// 	// Dodaj ostatnie słowo, jeśli istnieje
// 	if (current_word)
// 	{
// 		add_token(&tokens, current_word, T_WORD);
// 	}

// 	if (error)
// 	{
// 		free_tokens(tokens);
// 		return (NULL);
// 	}

// 	return reverse_tokens(tokens);
// }



t_token *tokenize_input(char *input)
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
                    free(current_word);  // Free after adding to tokens
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
                    free(current_word);  // Free after adding to tokens
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
                    free(current_word);  // Free after adding to tokens
                    current_word = NULL;
                }
                handle_redirection(input, &i, &tokens);
            }
            else if (input[i] == '\'' || input[i] == '"')
            {
                current_quote = (input[i] == '\'') ? SINGLE_QUOTE : DOUBLE_QUOTE;
                i++;
                char *quoted_part = read_quoted_content(input, &i, current_quote, &error);
                if (error)
                {
                    free(current_word);  // Free if error occurs
                    ft_putstr_fd("minishell: syntax error: unclosed quote\n", STDERR_FILENO);
                    break;
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
            break;
        }
    }

    if (current_word)
    {
        add_token(&tokens, current_word, T_WORD);
        free(current_word);  // Free the final word
    }

    if (error)
    {
        free(current_word);  // Cleanup if error
        free_tokens(tokens);
        return NULL;
    }

    tokens = reverse_tokens(tokens);
    return tokens;
}

// t_token *tokenize_input(char *input)
// {
//     // printf("\n=== START TOKENIZATION ===\n");
//     // printf("Input string: \"%s\"\n", input);
    
//     t_token *tokens = NULL;
//     char *current_word = NULL;
// 	//char *current_word = malloc(sizeof(char) * 100);

//     t_quote current_quote = NO_QUOTE;
//     int i = 0;
//     int error = 0;

//     while (input[i] && !error)
//     {
//         // printf("\nProcessing char %d ('%c'):\n", i, input[i]);
//         // printf("Current state: word=%s, quote=%d\n", 
//         //        current_word ? current_word : "NULL", 
//         //        current_quote);

//         if (current_quote == NO_QUOTE)
//         {
//             if (ft_isspace(input[i]))
//             {
//                 printf("Whitespace encountered\n");
//                 if (current_word)
//                 {
//                     // printf("Adding word token: \"%s\"\n", current_word);
//                     add_token(&tokens, current_word, T_WORD);
//                     current_word = NULL;
//                 }
//                 i++;
//                 continue;
//             }

//             // if (input[i] == '|')
//             // {
//             //     printf("Pipe character found\n");
//             //     if (current_word)
//             //     {
//             //         printf("Adding word token before pipe: \"%s\"\n", current_word);
//             //         add_token(&tokens, current_word, T_WORD);
//             //         current_word = NULL;
//             //     }
//             //     add_token(&tokens, "|", T_PIPE);
//             //     i++;
//             // }
//             // else if (input[i] == '>' || input[i] == '<')
//             // {
//             //     printf("Redirection character found: '%c'\n", input[i]);
//             //     if (current_word)
//             //     {
//             //         printf("Adding word token before redir: \"%s\"\n", current_word);
//             //         add_token(&tokens, current_word, T_WORD);
//             //         current_word = NULL;
//             //     }
//             //     handle_redirection(input, &i, &tokens);
//             // }
//             // else if (input[i] == '\'' || input[i] == '"')
//             // {
//             //     printf("Quote found: '%c'\n", input[i]);
//             //     current_quote = (input[i] == '\'') ? SINGLE_QUOTE : DOUBLE_QUOTE;
//             //     i++;
//             //     printf("Reading quoted content starting at pos %d\n", i);
//             //     char *quoted_part = read_quoted_content(input, &i, current_quote, &error);
                
//             //     if (error)
//             //     {
//             //         printf("ERROR: Unclosed quote\n");
//             //         ft_putstr_fd("minishell: syntax error: unclosed quote\n", STDERR_FILENO);
//             //         break;
//             //     }
                
//             //     printf("Quoted content: \"%s\" (length %zu)\n", quoted_part, strlen(quoted_part));
//             //     current_word = ft_strjoin_free(current_word, quoted_part);
//             //     printf("Combined word: \"%s\"\n", current_word);
//             //     free(quoted_part);
//             //     current_quote = NO_QUOTE;
//             // }
//             else
//             {
//                 // printf("Regular character, adding to word\n");
//                 current_word = ft_strjoin_char(current_word, input[i]);
//                 //printf("Current word after add: \"%s\"\n", current_word);
//                 i++;
//             }
//         }
//         else
//         {
//             //printf("ERROR: Unexpected state - in quotes but quote handler not active\n");
//             error = 1;
//             break;
//         }
//     }

//     if (current_word)
//     {
//         //printf("Adding final word token: \"%s\"\n", current_word);
//         add_token(&tokens, current_word, T_WORD);
//     }

//     if (error)
//     {
//         printf("ERROR DETECTED - Cleaning tokens\n");
//         free_tokens(tokens);
//         return (NULL);
//     }

//     // printf("\n=== TOKENIZATION RESULTS ===\n");
//     // printf("Raw tokens (before reversal):\n");
//     t_token *tmp = tokens;
//     while (tmp)
//     {
//         //printf("Token: \"%s\" (type %d)\n", tmp->value, tmp->type);
//         tmp = tmp->next;
//     }

//     tokens = reverse_tokens(tokens);
    
//     //printf("\nFinal tokens (after reversal):\n");
//     tmp = tokens;
//     while (tmp)
//     {
//         //printf("Token: \"%s\" (type %d)\n", tmp->value, tmp->type);
//         tmp = tmp->next;
//     }
//     //printf("=== END TOKENIZATION ===\n\n");

//     return tokens;
// }

/**
 * Obsługuje tokeny przekierowań (>, >>, <, <<).
 * - input: wejściowy ciąg znaków.
 * - i: wskaźnik na aktualny indeks.
 * - tokens: lista tokenów, do której dodawany jest nowy token.
 */
void	handle_redirection(char *input, int *i, t_token **tokens)
{
	char first_char = input[*i]; //get the current character at this position
	t_token_type type; //store the type of redirection

	(*i)++;
	// Sprawdź, czy to podwójny operator (>> lub <<)
	//(if the next character is the same as the first one)
	if (input[*i] == first_char)
	{
		(*i)++;
		//t_token_type type = (first_char == '>') ? T_APPEND : T_HEREDOC;
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
		//t_token_type type = (first_char == '>') ? T_REDIR_OUT : T_REDIR_IN;
		add_token(tokens, (char[]){first_char, '\0'}, type);
	}
}
