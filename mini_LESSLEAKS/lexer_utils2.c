#include "inc/minishell.h"


int	handle_quote(char *input, int *i, t_token **tokens, int *error)
{
	t_quote current_quote;
	char *quoted;
	t_token_type quote_type;

	current_quote = get_quote_type(input[*i]);
	(*i)++;
	quoted = read_quoted_content(input, i, current_quote, error);
	if (*error)
		return (ft_putstr_fd("Minishell: syntax error: unclosed quote\n",
				STDERR_FILENO), 1);
	quote_type = get_token_type_from_quote(current_quote);
	add_token(tokens, quoted, quote_type);
	free(quoted);
	return (0);
}

char	*read_quoted_content(char *input, int *i, t_quote quote, int *error)
{
	char quote_char = (quote == SINGLE_QUOTE) ? '\'' : '"';
	int start = *i;
	while (input[*i] && input[*i] != quote_char)
		(*i)++;

	if (input[*i] != quote_char)
	{
		*error = 1; // Ustaw błąd jeśli nie znaleziono zamykającego cytatu
		return (NULL);
	}

	char *content = ft_substr(input, start, *i - start);
	(*i)++; // Zwiększ indeks, aby przejść po zamykającym cudzysłowie
	return (content);
}


// //moje
// void	free_tokens(t_token *tokens)
// {
// 	t_token *tmp;

// 	while (tokens)
// 	{
// 		tmp = tokens;
// 		if (tokens->value)
// 			free(tokens->value); // <<< Free the string!
// 		tokens = tokens->next;
// 		free(tmp);
// 	}
// }

void free_tokens(t_token *tokens)
{
    t_token *tmp;

    while (tokens)
    {
        tmp = tokens->next; // Store next *before* freeing current
        if (tokens->value)
            free(tokens->value); // Free the string value
        free(tokens);          // Free the token node
        tokens = tmp;         // Move to the next node
    }
}

// //debugging
// void free_tokens(t_token *tokens)
// {
//     t_token *current = tokens;
//     t_token *next_node;
//     int count = 0; // Debug counter

//     // --- ADDED: Initial check and head pointer ---
//     printf("DEBUG: free_tokens STARTING with head=%p\n", (void*)current);
//     if (!current) {
//         printf("DEBUG: free_tokens received NULL list, doing nothing.\n");
//         return;
//     }
//     // --- END ADDED ---

//     while (current != NULL)
//     {
//         count++;
//         // --- ADDED: Print current node info ---
//         printf("DEBUG: free_tokens loop %d: current node=%p, next=%p, type=%d\n",
//                count, (void*)current, (void*)current->next, current->type);
//         // --- END ADDED ---

//         next_node = current->next; // Store the next pointer *before* freeing anything

//         if (current->value)
//         {
//             // --- ADDED: Print value being freed ---
//             printf("DEBUG: free_tokens loop %d: freeing value=%p ('%s')\n",
//                    count, (void*)current->value, current->value);
//             // --- END ADDED ---
//             free(current->value);
//             current->value = NULL; // Optional: Prevent dangling pointer issues
//         } else {
//             // --- ADDED: Indicate NULL value ---
//              printf("DEBUG: free_tokens loop %d: value pointer was NULL\n", count);
//             // --- END ADDED ---
//         }

//         // --- ADDED: Print node being freed ---
//         printf("DEBUG: free_tokens loop %d: freeing node %p itself\n", count, (void*)current);
//         // --- END ADDED ---
//         free(current);          // Free the token node itself
//         current = next_node;    // Move to the next node using the stored pointer
//     }

//     // --- ADDED: Final count ---
//     printf("DEBUG: free_tokens FINISHED after attempting to free %d nodes\n", count);
//     // --- END ADDED ---
// }
