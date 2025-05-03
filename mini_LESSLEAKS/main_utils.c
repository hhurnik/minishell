#include "inc/minishell.h"

int	handle_quoted_token(char *input, int *i, t_token **tokens, int *error)
{
	t_quote			current_quote;
	char			*quoted_content;
	t_token_type	quote_type;

	current_quote = get_quote_type(input[*i]);
	(*i)++; // Move past the opening quote
	// read_quoted_content reads *only* the inner content
	quoted_content = read_quoted_content(input, i, current_quote, error);
	if (*error || quoted_content == NULL) // Check error flag OR malloc failure
	{
		// read_quoted_content might set error but return NULL without msg
		if (*error && quoted_content == NULL)
		{
			ft_putstr_fd("Minishell: syntax error: unclosed quote\n",
				STDERR_FILENO);
		}
		else if (quoted_content == NULL)
		{
			// Malloc error likely
			perror("minishell: malloc error in read_quoted_content");
			*error = 1; // Ensure error is set
		}
		// Do not free quoted_content if it's NULL
		// If *error is set but quoted_content is not NULL (shouldn't happen?),
		//	free it.
		if (quoted_content)
			free(quoted_content);
		return (1); // Indicate failure
	}
	// Determine token type based on the quote character used
	quote_type = get_token_type_from_quote(current_quote);
	// Add the token with the correct type and the *inner* content
	add_token(tokens, quoted_content, quote_type);
	free(quoted_content); // Free the temporary content string
	// i is already advanced past the closing quote by read_quoted_content
	return (0); // Indicate success
}

// //moje
// void	free_env(t_env *env)
// {
// 	t_env	*tmp;

// 	while (env)
// 	{
// 		tmp = env->next; // Save next pointer before freeing
// 		free(env->key);
// 		free(env->value);
// 		free(env);
// 		env = tmp;
// 	}
// }

void free_env(t_env *env)
{
    t_env *tmp;
    while (env)
    {
        tmp = env->next;   // 1. Save next pointer
        free(env->key);    // 2. Free key string
        free(env->value);  // 3. Free value string
        free(env);         // 4. Free the node itself
        env = tmp;         // 5. Move to the next node
    }
}

void	init_shell(char **envp, t_env **env)
{
	setup_signals_for_prompt();
	*env = NULL;
	init_env(envp, env);
}

void	handle_eof(int exit_status)
{
	// Don't free env here, it's already freed in cleanup
	// free_env(env);  <- Remove this line
	// Clear readline history
	rl_clear_history();
	// Print a newline for better UX when exiting with Ctrl+D
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	exit(exit_status);
}

void process_input(char *input, t_env **env)
{
    t_resources res = {0}; // Mainly for passing to bi_exit if called
    t_token     *tokens = NULL;
    t_cmd       *cmds = NULL;

    // Keep track of resources mainly for potential cleanup by bi_exit
    res.input = input;
    res.env = *env;
    // Note: We'll free tokens and cmds manually unless bi_exit happens

    tokens = tokenize_input(input);
    if (!tokens)
    {
        free(input); // Free only input on tokenize failure
        return;
    }
    res.tokens = tokens; // Track for bi_exit

    expand_variables(tokens, *env);

    cmds = parse_tokens(tokens);
    if (!cmds)
    {
        free_tokens(tokens); // Free tokens
        free(input);      // Free input
        return;
    }
    res.cmds = cmds; // Track for bi_exit

    // Shift arguments *after* parsing and *before* heredoc/execution
    // Important if expansion results in empty args that affect command name
    shift_empty_args(cmds); // Assuming this handles ["", "cmd"] -> ["cmd"]

    // Handle Heredocs first
    if (handle_all_heredocs(cmds) != 0)
    {
        close_heredoc_fds(cmds); // Close any FDs opened before failure
        free_cmds(cmds);         // Free cmds
        free_tokens(tokens);     // Free tokens
        free(input);          // Free input
        return;
    }

    // Check for empty commands *after* heredocs are processed
    // (e.g., just "> file" or "$EMPTY_VAR")
    if (!cmds->args || !cmds->args[0] || cmds->args[0][0] == '\0') // <<< NEW CHECK
    {
         // Action remains the same: cleanup and return
         close_heredoc_fds(cmds);
         free_cmds(cmds);
         free_tokens(tokens);
         free(input);
         // g_exit_status will be 0 unless handle_all_heredocs or earlier
         // parsing/redirection setup failed and set it to non-zero.
         return; // Nothing to execute
    }


    setup_signals_for_command();

    if (is_builtin(cmds->args[0]) && !cmds->next)
    {
        t_fds fds = {dup(STDIN_FILENO), dup(STDOUT_FILENO)};
        // Pass 'res' so bi_exit can clean up *everything* if called
        handle_parent_builtin(cmds, fds, &res);
        // If handle_parent_builtin called bi_exit, we won't reach the code below.
        // If it didn't exit, fds should have been restored within it.
    }
    else
    {
        execute(cmds, env); // execute handles waiting and sets g_exit_status
    }

    setup_signals_for_prompt();
    close_heredoc_fds(cmds); // Close heredoc FDs *after* execution

    // --- Manually Clean up command-specific resources ---
    // Do NOT call cleanup(&res) here as it frees the environment!
    free_cmds(cmds);
    free_tokens(tokens);
    free(input);
    // 'res' is now potentially dangling if bi_exit wasn't called, but it's local
    // and goes out of scope, so it's okay.
}


///env | sort | grep -v SHLVL | grep -v ^_




void	shift_empty_args(t_cmd *cmds)
{
	t_cmd	*current_cmd;
	int		i;
	char	*empty_arg_to_free;

	current_cmd = cmds;
	while (current_cmd)
	{
		// Check if args exists, has at least two elements, and args[0] is empty
		if (current_cmd->args && current_cmd->args[0] && current_cmd->args[1]
			&& current_cmd->args[0][0] == '\0')
		{
			i = 0;
			empty_arg_to_free = current_cmd->args[0];
			// Save pointer to free
			// Shift pointers left
			while (current_cmd->args[i + 1])
			{
				current_cmd->args[i] = current_cmd->args[i + 1];
				i++;
			}
			current_cmd->args[i] = NULL; // Null-terminate the shifted array
			free(empty_arg_to_free);     // Free the original empty string arg
		}
		current_cmd = current_cmd->next;
	}
}

void	clean_cmds_tokens_input(t_cmd *cmds, t_token *tokens, char *input)
{
	if (cmds)
		free_cmds(cmds);
	if (tokens)
		free_tokens(tokens);
	if (input)
		free(input);
}

static int	ensure_current_word(char **current_word)
{
	if (*current_word == NULL)
	{
		*current_word = ft_strdup("");
		if (!*current_word)
		{
			perror("minishell: malloc error");
			return (0);
		}
	}
	return (1);
}

void	append_char_to_word(char **word, char c)
{
	char	*old_word;

	if (!ensure_current_word(word))
	{
		return ;
	}
	old_word = *word;
	*word = ft_strjoin_char(old_word, c);
	if (!*word)
	{
		perror("minishell: malloc error");
	}
}

int	handle_quoted_append(char *input, int *i, char **current_word, int *error,
		t_parse_state *state)
{
	t_quote	current_quote;
	char	quote_char;
	int		start;
	int		len;
	char	*content;
	char	*old_word;

	current_quote = get_quote_type(input[*i]);
	quote_char = (current_quote == SINGLE_QUOTE) ? '\'' : '"';
	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != quote_char)
	{
		(*i)++;
	}
	if (input[*i] != quote_char)
	{
		ft_putstr_fd("Minishell: syntax error: unclosed quote\n",
			STDERR_FILENO);
		*error = 1;
		return (1);
	}
	// Ustawiamy flagę, jeśli napotkaliśmy pojedynczy cudzysłów
	if (current_quote == SINGLE_QUOTE)
	{
		state->single_quote_used = 1;
	}
	if (!ensure_current_word(current_word))
	{
		*error = 1;
		return (1);
	}
	len = *i - start;
	if (len > 0)
	{
		content = ft_substr(input, start, len);
		if (!content)
		{
			perror("minishell: malloc error");
			*error = 1;
			return (1);
		}
		old_word = *current_word;
		*current_word = ft_strjoin(old_word, content);
		free(old_word);
		free(content);
		if (!*current_word)
		{
			perror("minishell: malloc error");
			*error = 1;
			return (1);
		}
	}
	(*i)++;
	return (0);
}

void	finalize_current_word(t_token **tokens, char **current_word,
		int single_quote_used)
{
	t_token_type	type;

	if (*current_word != NULL)
	{
		type = single_quote_used ? T_SINGLE_QUOTED : T_WORD;
		add_token(tokens, *current_word, type);
		free(*current_word);
		*current_word = NULL;
	}
}

void	handle_redirection(char *input, int *i, t_token **tokens)
{
	char			first_char;
	t_token_type	type;
	char			op_str[3];

	first_char = input[*i];
	op_str[0] = first_char;
	(*i)++;
	if (input[*i] == first_char)
	{
		op_str[1] = first_char;
		op_str[2] = '\0';
		(*i)++;
		if (first_char == '>')
			type = T_APPEND;
		else
			type = T_HEREDOC;
		add_token(tokens, op_str, type);
	}
	else
	{
		op_str[1] = '\0';
		if (first_char == '>')
			type = T_REDIR_OUT;
		else
			type = T_REDIR_IN;
		add_token(tokens, op_str, type);
	}
}

void	parse_input_loop(char *input, t_parse_state *state)
{
	while (input[state->i] && !(state->error))
	{
		//|| input[state->i - 1] == ' '))
		if (input[state->i] == '#' && (state->i == 0
				|| ft_isspace(input[state->i - 1])))
			// Zatrzymaj przetwarzanie po napotkaniu znaku komentarza
			break ;
		if (ft_isspace(input[state->i]))
		{
			// Finalizuj jakiekolwiek słowo przed pominięciem spacji
			finalize_current_word(&state->tokens, &state->current_word,
				state->single_quote_used);
			state->i++; // Pomijamy spację
		}
		else if (is_pipe(input[state->i]))
		{
			// Finalizuj słowo, dodaj token dla pipe
			finalize_current_word(&state->tokens, &state->current_word,
				state->single_quote_used);
			add_token(&state->tokens, "|", T_PIPE);
			state->i++;
		}
		else if (is_redirect(input[state->i]))
		{
			// Finalizuj słowo przed obsługą operatora przekierowania
			finalize_current_word(&state->tokens, &state->current_word,
				state->single_quote_used);
			handle_redirection(input, &state->i, &state->tokens);
			// Dodaje token przekierowania
		}
		else if (is_quote(input[state->i]))
		{
			// Dodaj zawartość cytatu do aktualnie budowanego słowa
			if (handle_quoted_append(input, &state->i, &state->current_word,
					&state->error, state) != 0)
			{
				// Wystąpił błąd (niezamknięty cudzysłów lub błąd malloc)
				state->error = 1; // Ustawiamy flagę błędu
			}
		}
		else
		{
			// Dodaj zwykły znak do aktualnie budowanego słowa
			append_char_to_word(&state->current_word, input[state->i]);
			if (state->current_word == NULL && errno == ENOMEM)
			{ // Sprawdzamy, czy append_char się nie udał
				state->error = 1;
			}
			state->i++;
		}
	}
	// Po zakończeniu pętli, finalizuj jakiekolwiek pozostałe słowo
	if (!state->error)
	{ // Finalizujemy tylko, jeśli nie wystąpił błąd w trakcie pętli
		finalize_current_word(&state->tokens, &state->current_word,
			state->single_quote_used);
	}
}

t_token	*tokenize_input(char *input)
{
	t_parse_state	state;

	state.tokens = NULL;
	state.current_word = NULL;
	state.i = 0;
	state.error = 0;
	state.single_quote_used = 0; // Inicjalizujemy na 0
	parse_input_loop(input, &state);
	// Finalne słowo obsługiwane *wewnątrz* i *po* parse_input_loop
	if (state.error)
	{
		// Czyszczenie potencjalnie częściowo zbudowanego słowa i tokenów
		if (state.current_word)
			free(state.current_word);
		free_tokens(state.tokens);
		return (NULL);
	}
	return (reverse_tokens(state.tokens));
}
