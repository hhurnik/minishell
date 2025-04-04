#include "inc/minishell.h"

t_env *env_new(char *key, char *value) {
    t_env *node = malloc(sizeof(t_env));
    node->key = ft_strdup(key);
    node->value = ft_strdup(value);
    node->next = NULL;
    return node;
}

void env_add_back(t_env **env, t_env *new_node) {
    if (!*env) {
        *env = new_node;
    } else {
        t_env *last = *env;
        while (last->next) {
            last = last->next;
        }
        last->next = new_node;
    }
}

t_env   *init_env(char **envp) {
    t_env *env = NULL;
    while (*envp) {
        char *eq = ft_strchr(*envp, '=');
        char *key = ft_substr(*envp, 0, eq - *envp);
        char *value = ft_strdup(eq + 1);
        env_add_back(&env, env_new(key, value));
        envp++;
    }
    return env;
}

char *get_env_value(char *key, t_env *env) {
    while (env) {
        if (ft_strcmp(env->key, key) == 0)      
            return env->value;
        env = env->next;
    }
    return NULL;
}

void update_pwd_env(t_env **env) {
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));
    env_add_back(env, env_new("PWD", ft_strdup(cwd)));
}


