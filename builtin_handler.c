
//char *cmd is a pointer to the first character of a string. 
// a pointer to a null-terminated string, like "cd", "echo", "ls" — just the name of the command

int is_builtin(char *cmd)
{
    // !strcmp(cmd, "cd") is true if cmd is "cd".

    if ((!strcmp(cmd, "echo") || !strcmp(cmd, "cd") || !strcmp(cmd, "pwd") ||
    !strcmp(cmd, "export") || !strcmp(cmd, "unset") ||
    !strcmp(cmd, "env") || !strcmp(cmd, "exit")))
        return (1); // jesli to builtin
    else
        return (0);
}

/////krotsa version

// int execute_builtin(char **args)
// {
//     if (!strcmp(args[0], "echo"))
//         return ft_echo(args);
//     else if (!strcmp(args[0], "cd"))
//         return ft_cd(args);
//     else if (!strcmp(args[0], "pwd"))
//         return ft_pwd(args);
//     else if (!strcmp(args[0], "export"))
//         return ft_export(args);
//     else if (!strcmp(args[0], "unset"))
//         return ft_unset(args);
//     else if (!strcmp(args[0], "env"))
//         return ft_env(args);
//     else if (!strcmp(args[0], "exit"))
//         return ft_exit(args);
//     return (0);
// }


// przed tym 
// if (!cmd->args || !cmd->args[0])
//         return 0;   - funkcja nadrzedna od execute_builtin
int execute_builtin(char **args, char **env)
{
    //char **env; //skads to wiać - global? cy prekazana jako argument?
    if (is_builtin == 1)
    {
        if (!strcmp(args[0], "echo"))
            return (bi_echo(args));

        if (!strcmp(args[0], "cd"))
            return (bi_cd(args));

        if (!strcmp(args[0], "pwd"))
            return (bi_pwd(args));

        if (!strcmp(args[0], "env"))
            return (bi_env(args));

        if (!strcmp(args[0], "exit"))
            return (bi_exit(args));

        if (!strcmp(args[0], "unset"))
            return (bi_unset(args, env) ); //environment

        if (!strcmp(args[0], "export"))
            return (bi_export(envp)); //environment
    }
}