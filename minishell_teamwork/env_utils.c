#include "inc/minishell.h"

t_env *env_new_no_dup(char *key, char *value) 
{
    t_env *new = malloc(sizeof(t_env));
    if (!new) return NULL;
    new->key = key;     // Takes ownership
    new->value = value; // Takes ownership
    new->next = NULL;
    return new;
}

//dodaje new node to the end of a linked list of t_env nodes
void	env_add_back(t_env **env, t_env *new_node)
{
	//if empty list, zrob nowy node
	if (!*env)
	{
		*env = new_node;
	}
	else
	{
		t_env *last = *env;
		//dopoki ten node ma nastepny node, keep going (last idzie a do final node)
		while (last->next)
		{
			last = last->next;
		}
		last->next = new_node;
	}
}
//stare init_env - dziala ale z leakami -- 55 blocks
t_env	*init_env(char **envp)
{
	t_env *env = NULL;
	while (*envp)
	{
		char *eq = ft_strchr(*envp, '='); //zwraca pointer to the first occurence of =
		char *key = ft_substr(*envp, 0, eq - *envp); //przed "="
		char *value = ft_strdup(eq + 1); //zwraca wszystko po "=", points to char after =
		env_add_back(&env, env_new_no_dup(key, value));
		free(key); //dodane 13.04, sprawdzam
		envp++;
	}
	return (env);
}


void init_env2(char **envp, t_env **env)
{
    int i = 0;
    while (envp[i])
    {
        char *eq = ft_strchr(envp[i], '=');
        if (eq)
        {
            int key_len = eq - envp[i];
            char *key = ft_substr(envp[i], 0, key_len);     // malloc key
            if (!key) { /* handle error */ continue; }
            char *value = ft_strdup(eq + 1);                // malloc value
            if (!value) { free(key); /* handle error */ continue; }

            // Create new node, env_new should NOT strdup again if we pass ownership
            t_env *new_node = malloc(sizeof(t_env));
            if (!new_node) {
                free(key);
                free(value);
                // Handle major allocation failure - maybe free existing env and exit?
                perror("minishell: malloc");
                free_env_list(*env); // Free what we have so far
                exit(EXIT_FAILURE);
            }
            new_node->key = key;     // Assign pointer directly
            new_node->value = value; // Assign pointer directly
            new_node->next = NULL;

            env_add_back(env, new_node);
        }
        i++;
    }
}

//dodane 13.04, sprawdzam
void free_env_list(t_env *env)
{
    t_env *current = env;
    while (current)
    {
        t_env *next = current->next;
        free(current->key);
        free(current->value);
        free(current);
        current = next;
    }
}


//szuka danego key w env i wraca value
//tu char *key - ten ktorego szukam
char	*get_env_value(char *key, t_env *env)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		//if the current key doesnt match, move to the next noe
		env = env->next;
	}
	return (NULL);
}

//PATH_MAX - stala definiujaca max possible length of a path on a system (4096 on Linux)
//creates a "PWD" env variable with that path
//adds it t the end of the list
//pwd - absolute path of your curent directory
//potrzebne zeby minishell wiedzial gdzie jest w kazdym momencie
void	update_pwd_env(t_env **env)
{
	char cwd[PATH_MAX];
	getcwd(cwd, sizeof(cwd));
	env_add_back(env, env_new_no_dup("PWD", ft_strdup(cwd)));
}

//pwd (print wdir) - prints your current directory
//cd (change dir) - changes your working directory to your home directory, does not print by default

// cd / cd ~ wstawi HOME
