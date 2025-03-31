#include "inc/minishell.h"

// Sprawdza, czy znak jest białym znakiem
int ft_isspace(char c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v');
}

// Łączy dwa stringi, zwalniając s1
char *ft_strjoin_free(char *s1, char *s2) {
    char *result = ft_strjoin(s1, s2);
    free(s1);
    return result;
}

// Dodaje pojedynczy znak do stringa
char *ft_strjoin_char(char *s1, char c) {
    char str[2] = {c, '\0'};
    return ft_strjoin_free(s1, str);
}

int ft_strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}