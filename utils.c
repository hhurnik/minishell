#include "ms.h"

int ft_strlen(const char *str)
{
    int i = 0;

    while (str[i])
        i++;

    return (i);
}

int is_integer(char *str)
{
    int i = 0;

    if (str[i] == '-' || str[i] == '+')
        i++;

    while (str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return (0);
        i++;
    }

    return (1);
}

int ft_atoi(const char *str) 
{
    int result = 0;
    int sign = 1;
    
    // Handle optional leading '+' or '-'
    if (*str == '-') 
    {
        sign = -1;
        str++;
    } 
    else if (*str == '+') 
    {
        str++;
    }
    
    // Convert string to integer
    while (*str >= '0' && *str <= '9') {
        result = result * 10 + (*str - '0');
        str++;
    }
    
    return result * sign;
}

char *ft_strdup(const char *s)
{
    char *str;
    size_t i;

    i = 0;

    if (!s)
        return (NULL);

    str = (char *)malloc(sizeof(*s) * (ft_strlen(s) + 1));
    if (!str)
        return (NULL);
    
    while (s[i] != '\0')
    {
        str[i] = s[i];
        i++;
    }

    str[i] = '\0';
    return (str);
}

int ft_strncmp(char *s1, char *s2, unsigned int n)
{
    unsigned int i;

    i = 0;

    while (i < n && s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i])
        i++;
    
    if (i == n)
        return (0);

    return (s1[i] - s2[i]);
}



//bedziemy miec w mainie
char **copy_env(char *envp[])
{
    int i = 0;
    char **env;

    while (envp[i])
        i++;

    env = malloc(sizeof(char *) * (i + 1));
    if (!env)
        return (NULL);


    
    while (envp[i]) 
    {
        env[i] = ft_strdup(envp[i]);
        i++;
    }

    env[i] = NULL;
    return (env);
}