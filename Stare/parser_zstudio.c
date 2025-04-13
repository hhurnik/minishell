/nowe poprawkki, 13.04
void handle_redirection_parser2(t_token **tokens, t_cmd *cmd, t_cmd **cmds_head, t_token *all_tokens)
{
	t_redir *new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
	{
		perror("minishell: malloc failed in handle_redirection_parser");
		// Cleanup before exiting due to critical allocation failure
		free_cmds(*cmds_head);      // Free partially built commands
		free_tokens(all_tokens);    // Free the original token list
		exit(EXIT_FAILURE);         // Exit - critical failure
		// return; // Unreachable after exit
	}

	// Store the type of redirection
	new_redir->type = (*tokens)->type;
	new_redir->next = NULL; // Initialize next pointer

	// Move to the next token, which should be the filename
	*tokens = (*tokens)->next;

	// --- Syntax Check: Expecting a filename (T_WORD) ---
	if (!*tokens || (*tokens)->type != T_WORD)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
		if (*tokens) // If there's a token, print its value
			ft_putstr_fd((*tokens)->value, STDERR_FILENO);
		else // Otherwise, the error is at the end of the line
			ft_putstr_fd("newline", STDERR_FILENO);
		ft_putstr_fd("'\n", STDERR_FILENO);

		free(new_redir);            // Free the allocated redirection struct
		// --- Cleanup on Syntax Error ---
		free_cmds(*cmds_head);      // Free partially built commands
		*cmds_head = NULL;          // Mark command list as freed
		free_tokens(all_tokens);    // Free the original token list
		*tokens = NULL;             // *** Signal error state to parse_tokens ***
		//---------------------------------
		g_exit_status = 2;          // Set standard syntax error exit status
		return;                     // Return to indicate parsing failure
	}

	// --- Filename Found: Duplicate it ---
	new_redir->file = ft_strdup((*tokens)->value);
	if (!new_redir->file) // Handle ft_strdup failure
	{
		perror("minishell: strdup failed in handle_redirection_parser");
		free(new_redir);
		// --- Cleanup on Allocation Error ---
		free_cmds(*cmds_head);
		free_tokens(all_tokens);
		exit(EXIT_FAILURE);        // Exit - critical failure
		// return; // Unreachable
	}

	// Add the successfully created redirection to the command
	add_redir_to_cmd(cmd, new_redir);

	// Advance the token pointer past the filename token
	*tokens = (*tokens)->next;
}


/**
 * @brief Parses a list of tokens into a linked list of command structures.
 *
 * Iterates through tokens, creating t_cmd nodes separated by T_PIPE.
 * Populates commands with arguments (T_WORD) and handles redirections
 * by calling handle_redirection_parser2. Performs syntax checks and
 * cleans up allocated memory on error.
 *
 * @param original_tokens The head of the linked list of tokens.
 * @return A pointer to the head of the linked list of t_cmd structures,
 *         or NULL if parsing fails or the input is empty/invalid.
 *         On error, g_exit_status may be set, and relevant memory freed.
 */
t_cmd *parse_tokens(t_token *original_tokens)
{
	t_cmd   *cmds = NULL;               // Head of the command list
	t_cmd   *current_cmd = NULL;        // Command currently being built
	// Use a separate pointer to iterate, keep original_tokens for cleanup
	t_token *current_token_ptr = original_tokens;

	// Handle empty input immediately
	if (!current_token_ptr)
		return (NULL);

	while (current_token_ptr)
	{
		// --- Create New Command Node if Needed ---
		// Condition: Start of parsing OR encountered a pipe token.
		if (!current_cmd || current_token_ptr->type == T_PIPE)
		{
			// Check for pipe at the very beginning (invalid syntax)
            if (!current_cmd && current_token_ptr->type == T_PIPE) {
                 ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", STDERR_FILENO);
                 // No commands allocated yet, just free tokens
                 free_tokens(original_tokens);
                 g_exit_status = 2;
                 return (NULL);
            }

			// If it's a pipe, advance past it before creating the new command node
			if (current_token_ptr->type == T_PIPE) {
				current_token_ptr = current_token_ptr->next;
				// Check for pipe at the end of input (invalid syntax)
				if (!current_token_ptr) {
					ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", STDERR_FILENO);
					free_cmds(cmds); // Free commands built so far
					free_tokens(original_tokens);
					g_exit_status = 2;
					return (NULL);
				}
				// Check for consecutive pipes `||` (invalid syntax)
                if (current_token_ptr->type == T_PIPE) {
                    ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", STDERR_FILENO);
                    free_cmds(cmds);
                    free_tokens(original_tokens);
                    g_exit_status = 2;
                    return (NULL);
                }
			}

			// Create the new command node
			t_cmd *new_cmd_node = cmd_new();
			if (!new_cmd_node) { // Handle cmd_new allocation failure
				perror("minishell: malloc failed in parse_tokens (cmd_new)");
				free_cmds(cmds); // Free commands built so far
				free_tokens(original_tokens);
				exit(EXIT_FAILURE); // Critical failure
				// return (NULL); // Unreachable
			}
			cmd_add_back(&cmds, new_cmd_node); // Add to the list
			current_cmd = cmd_last(cmds);      // Update pointer to the latest command

			// If we advanced past a pipe and hit the end, we are done after creating the node.
			if (!current_token_ptr) break;

			// No need to 'continue' here unless the *only* thing was a pipe advance.
			// The loop will naturally process the token pointed to by current_token_ptr.
		}

		// --- Process Current Token ---
		if (is_redirection(current_token_ptr->type))
		{
			// Pass address of iterator, current cmd, address of cmd list head, and original token list head
			handle_redirection_parser2(&current_token_ptr, current_cmd, &cmds, original_tokens);
			// Check if handle_redirection_parser signaled an error by setting ptr to NULL
			if (current_token_ptr == NULL) {
				// Cleanup was already handled inside handle_redirection_parser2
				// cmds was also freed inside, so return NULL directly.
				return (NULL);
			}
			// No need to advance current_token_ptr here, handle_redirection_parser2 does it.
		}
		else if (current_token_ptr->type == T_WORD)
		{
			char *arg_copy = ft_strdup(current_token_ptr->value);
			if (!arg_copy)
			{
				perror("minishell: strdup failed in parse_tokens (arg_copy)");
				free_cmds(cmds);
				free_tokens(original_tokens); // Free original list on error
				exit(EXIT_FAILURE); // Critical failure
				// return (NULL); // Unreachable
			}
			// add_arg_to_cmd handles its own realloc/free internally for the args array
			add_arg_to_cmd(current_cmd, arg_copy);
			current_token_ptr = current_token_ptr->next; // Advance ONLY for T_WORD
		}
		else // Should not happen if lexer is correct (Pipe is handled above)
		{
			ft_putstr_fd("minishell: unexpected token type in parser: ", STDERR_FILENO);
			ft_putstr_fd(current_token_ptr->value, STDERR_FILENO); // Print problematic token
            ft_putstr_fd("\n", STDERR_FILENO);
			free_cmds(cmds);
			free_tokens(original_tokens);
			g_exit_status = 2; // Treat as syntax error
			return (NULL);
		}
	} // End while loop

	// --- Final Syntax/Validity Checks ---
	// Case: Input was only whitespace (lexer might produce no tokens, handled at start)
	// Case: Command list built, but the last command has no arguments and no redirections (e.g., "echo | ")
	if (cmds && current_cmd && !current_cmd->args && !current_cmd->redirections) {
		// This often happens with a trailing pipe, which is handled earlier,
		// but could happen with just redirections followed by nothing. Check if this is an error.
		// Bash allows `> file | wc` but might error on `cmd | > file` if file needs word.
		// Let's consider an empty command segment after a pipe as an error for simplicity,
        // unless the previous check for pipe at end already caught it.
        // This check might be redundant depending on earlier pipe checks.
	}
	// Check if the first command itself is empty (e.g. just "> file")
    if (!cmds || (!cmds->args && !cmds->redirections)) {
        // It's possible to have only redirections, which might be valid depending on shell rules.
        // However, if there are no commands *at all* after parsing valid input,
        // it might indicate an issue or just empty effective input.
        // If `cmds` is NULL here, it means the loop didn't even create one node.
        // If `cmds` exists but first node is empty, maybe cleanup?
        // Bash allows `> out` which results in an empty file. Let's allow this for now.
        // So, only return NULL if cmds is actually NULL.
        if (!cmds) {
            // free_tokens(original_tokens); // Tokens should be freed by main loop
            return NULL;
        }
    }


	// print_parsed_commands(cmds); // Optional: Debugging
	return (cmds); // Return the head of the successfully parsed command list
}
