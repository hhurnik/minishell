#include "inc/minishell.h"

//tworzy node z danym key i value
t_env	*env_new(char *key, char *value)
{
	t_env *node = malloc(sizeof(t_env));
	node->key = ft_strdup(key);
	node->value = ft_strdup(value);
	node->next = NULL;
	return (node);
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

t_env	*init_env(char **envp)
{
	t_env *env = NULL;
	while (*envp)
	{
		char *eq = ft_strchr(*envp, '='); //zwraca pointer to the first occurence of =
		char *key = ft_substr(*envp, 0, eq - *envp); //przed "="
		char *value = ft_strdup(eq + 1); //zwraca wszystko po "=", points to char after =
		env_add_back(&env, env_new(key, value));
		envp++;
	}
	return (env);
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
	env_add_back(env, env_new("PWD", ft_strdup(cwd)));
}

//pwd (print wdir) - prints your current directory
//cd (change dir) - changes your working directory to your home directory, does not print by default

// cd / cd ~ wstawi HOME
