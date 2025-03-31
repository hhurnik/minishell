#include "inc/minishell.h"

/**
 * Dodaje przekierowanie do listy w komendzie.
 */
void add_redir_to_cmd(t_cmd *cmd, t_redir *new_redir) {
    if (!cmd->redirections) {
        cmd->redirections = new_redir;
    } else {
        t_redir *last = cmd->redirections;
        while (last->next) {
            last = last->next;
        }
        last->next = new_redir;
    }
}

t_cmd *cmd_new(void) {
    t_cmd *cmd = malloc(sizeof(t_cmd));
    cmd->args = NULL;
    cmd->redirections = NULL;
    cmd->next = NULL;
    return cmd;
}

t_cmd *cmd_last(t_cmd *cmds) {
    while (cmds && cmds->next) {
        cmds = cmds->next;
    }
    return cmds;
}

void cmd_add_back(t_cmd **cmds, t_cmd *new_cmd) {
    if (!*cmds) {
        *cmds = new_cmd;
    } else {
        t_cmd *last = cmd_last(*cmds);
        last->next = new_cmd;
    }
}


int is_redirection(t_token_type type) {
    return (type == T_REDIR_IN || type == T_REDIR_OUT || type == T_APPEND || type == T_HEREDOC);
}

void add_arg_to_cmd(t_cmd *cmd, char *arg) {
    int i = 0;
    while (cmd->args && cmd->args[i]) {
        i++;
    }
    cmd->args = realloc(cmd->args, (i + 2) * sizeof(char *));
    cmd->args[i] = ft_strdup(arg);
    cmd->args[i + 1] = NULL;
}