#ifndef MS_H
#define MS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

//utils
int ft_strlen(char *str);
char *ft_strdup(const char *s);
int ft_strncmp(char *s1, char *s2, unsigned int n);


// builtins
int bi_cd(char *argv[]);
int bi_echo(char *argv[]);
int bi_env(char *argv[]);
int bi_exit(char *argv[]);
void bi_export(char *envp[]);
int bi_pwd(char *argv[]);

char **copy_env(char *envp[]);
void remove_env_var(char *varname, char **env);
void ft_unset(char **argv, char **env);


#endif