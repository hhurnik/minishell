int main(void)
{
    char *input;
    t_cmd *cmd_list;

    while (1)
    {
        input = readline("minishell$ ");
        if (!input)
            break;
        if (*input)
            add_history(input);

        // zzakladam ze parser zwraca linked t_cmd list
        cmd_list = parse_input(input); // <- finalne parsowanie

        if (!cmd_list)
        {
            free(input);
            continue;
        }

        if (!cmd_list->next && is_builtin(cmd_list->args[0]))
            execute_builtin(cmd_list->args);
        else
            execute_pipeline(cmd_list);

        free_cmd_list(cmd_list);
        free(input);
    }
    return 0;
}
