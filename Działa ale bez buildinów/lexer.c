#include "inc/minishell.h"


void add_token(t_token **tokens, char *value, t_token_type type) {
    t_token *new = malloc(sizeof(t_token));
    new->value = ft_strdup(value);
    new->type = type;
    new->next = *tokens;
    *tokens = new;
}

void free_tokens(t_token *tokens) {
    while (tokens) {
        t_token *tmp = tokens;
        tokens = tokens->next;
        free(tmp->value);
        free(tmp);
    }
}


char *read_quoted_content(char *input, int *i, t_quote quote, int *error) {
    char quote_char = (quote == SINGLE_QUOTE) ? '\'' : '"';
    int start = *i;
    while (input[*i] && input[*i] != quote_char) {
        (*i)++;
    }
    if (input[*i] != quote_char) {
        *error = 1;
        return NULL;
    }
    char *content = ft_substr(input, start, *i - start);
    (*i)++;
    return content;
}


/**
 * Tokenizuje wejście, rozbijając na części (np. "ls -l > out.txt" → [ls, -l, >, out.txt]).
 * Obsługuje cudzysłowy i specjalne znaki.
 */
t_token *tokenize_input(char *input) {
    t_token *tokens = NULL;
    char *current_word = NULL;
    t_quote current_quote = NO_QUOTE;
    int i = 0;
    int error = 0;

    while (input[i] && !error) {
        if (current_quote == NO_QUOTE) {
            // Pomijaj białe znaki
            if (ft_isspace(input[i])) {
                if (current_word) {
                    add_token(&tokens, current_word, T_WORD);
                    current_word = NULL;
                }
                i++;
                continue;
            }

            // Sprawdź znaki specjalne
            if (input[i] == '|') {
                if (current_word) {
                    add_token(&tokens, current_word, T_WORD);
                    current_word = NULL;
                }
                add_token(&tokens, "|", T_PIPE);
                i++;
            }
            else if (input[i] == '>' || input[i] == '<') {
                if (current_word) {
                    add_token(&tokens, current_word, T_WORD);
                    current_word = NULL;
                }
                handle_redirection(input, &i, &tokens);
            }
            else if (input[i] == '\'' || input[i] == '"') {
                current_quote = (input[i] == '\'') ? SINGLE_QUOTE : DOUBLE_QUOTE;
                i++;
                char *quoted_part = read_quoted_content(input, &i, current_quote, &error);
                if (error) {
                    ft_putstr_fd("minishell: syntax error: unclosed quote\n", STDERR_FILENO);
                    break;
                }
                current_word = ft_strjoin_free(current_word, quoted_part);
                free(quoted_part);
                current_quote = NO_QUOTE;
            }
            else {
                // Dodaj zwykły znak do bieżącego słowa
                current_word = ft_strjoin_char(current_word, input[i]);
                i++;
            }
        } else {
            // Ten blok nie powinien być osiągany (cudzysłowy są obsługiwane wyżej)
            error = 1;
            break;
        }
    }

    // Dodaj ostatnie słowo, jeśli istnieje
    if (current_word) {
        add_token(&tokens, current_word, T_WORD);
    }

    if (error) {
        free_tokens(tokens);
        return NULL;
    }

    return tokens;
}


/**
 * Obsługuje tokeny przekierowań (>, >>, <, <<).
 * - input: wejściowy ciąg znaków.
 * - i: wskaźnik na aktualny indeks.
 * - tokens: lista tokenów, do której dodawany jest nowy token.
 */
void handle_redirection(char *input, int *i, t_token **tokens) {
    char first_char = input[*i];
    (*i)++;
    // Sprawdź, czy to podwójny operator (>> lub <<)
    if (input[*i] == first_char) {
        (*i)++;
        t_token_type type = (first_char == '>') ? T_APPEND : T_HEREDOC;
        add_token(tokens, (char[]){first_char, first_char, '\0'}, type);
    } else { // Pojedynczy operator (> lub <)
        t_token_type type = (first_char == '>') ? T_REDIR_OUT : T_REDIR_IN;
        add_token(tokens, (char[]){first_char, '\0'}, type);
    }
}


