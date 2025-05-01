
// // Dodaje pojedynczy znak do stringa
// char	*ft_strjoin_char(char *s, char c)
// {
// 	char	*new;
// 	int		len;

// 	len = 0;
// 	if (s)
// 		len = ft_strlen(s);
// 	new = malloc(len + 2); // +1 for char, +1 for '\0'
// 	if (!new)
// 		return (NULL);
// 	if (s)
// 		ft_memcpy(new, s, len);
// 	new[len] = c;
// 	new[len + 1] = '\0';
// 	free(s);
// 	return (new);
// }


// char *expand_str(char *str, t_env *env)
// {
//     char *result;
//     int i;

//     result = ft_strdup("");
//     if (!result)
//         return (NULL);
//     i = 0;
//     while (str[i])
//     {
//         if (is_special_var(str, i))
//             i = handle_special_var(&result, i);
//         else if (is_braced_var(str, i))
//             i = handle_braced_var(&result, str, i, env);
//         else if (is_standard_var(str, i))
//             i = handle_standard_var(&result, str, i, env);
//         else
//         {
//             char *old_result = result;
//             result = ft_strjoin_char(old_result, str[i]);
//             if (!result)
//             {
//                 free(old_result);
//                 return (NULL);
//             }
//             i++;
//         }
//     }
//     return (result);
// }


// void expand_variables(t_token *tokens, t_env *env)
// {
//     char *expanded;

//     while (tokens)
//     {
//         if (tokens->type == T_DOUBLE_QUOTED || tokens->type == T_WORD)
//         {
//             expanded = expand_str(tokens->value, env);
//             if (expanded)
//             {
//                 free(tokens->value); // Free the old value
//                 tokens->value = expanded;
//             }
//         }
//         tokens = tokens->next;
//     }
// }



// void process_input(char *input, t_env **env)
// {
//     t_token *tokens = NULL;
//     t_cmd *cmds = NULL;

//     tokens = tokenize_input(input);
//     if (!tokens) {
//         free(input);
//         return;
//     }
    
//     expand_variables(tokens, *env);
    
//     cmds = parse_tokens(tokens);
//     if (!cmds) {
//         free_tokens(tokens);
//         free(input);
//         return;
//     }

//     if (cmds->args && cmds->args[0]) {
//         setup_signals_for_command();
//         execute(cmds, env);
//         setup_signals_for_prompt();
//     }

//     // Always free resources
//     free_cmds(cmds);
//     free_tokens(tokens);
//     free(input);
// }




// int main(int argc, char **argv, char **envp)
// {
//     t_env *env;
//     char *input;
//     t_resources res;

//     (void)argc;
//     (void)argv;
//     init_shell(envp, &env);

//     while (1)
//     {
//         input = readline("minishell$ ");
//         if (!input)
//         {
//             res.env = env;
//             res.cmds = NULL;
//             res.tokens = NULL;
//             res.input = NULL;
//             cleanup(&res); // Pass the t_resources structure
//             handle_eof(env, g_exit_status);
//         }
//         if (*input)
//         {
//             add_history(input);
//             process_input(input, &env);
//         }
//         else
//             free(input);
//     }
//     return (0);
// }

// =6450== 5 bytes in 1 blocks are still reachable in loss record 2 of 66
// ==6450==    at 0x484880F: malloc (vg_replace_malloc.c:446)
// ==6450==    by 0x10BF1D: ft_strjoin_char (utils.c:29)
// ==6450==    by 0x10A452: expand_str (expander.c:248)
// ==6450==    by 0x10A146: expand_variables (expander.c:52)
// ==6450==    by 0x10B485: process_input (main_utils.c:149)
// ==6450==    by 0x10B335: main (main.c:58)