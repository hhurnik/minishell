#include "ms.h"

int ft_strlen(char *str)
{
    int i = 0;

    while (str[i])
        i++;

    return (i);
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