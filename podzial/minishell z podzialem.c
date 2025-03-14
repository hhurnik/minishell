Poniżej przedstawiam dwie wersje podziału projektu – jedna dla modułu parsowania (część 1), a druga dla modułu wykonawczego (część 2). Każda wersja zawiera zarówno pseudokod, jak i przykładowe fragmenty kodu w języku C, które ilustrują najważniejsze zadania w danej części. Dzięki temu każdy z Was może skupić się na swoich zadaniach, a następnie łatwo zintegrować oba moduły.

---

## Część 1: Parsowanie (Parsing)

### 1.1. Pseudokod

#### Główne zadania:
1. **Tokenizacja (Lexer)**
   - Przechodzimy po znaku wejściowej linii.
   - Pomijamy białe znaki.
   - Wykrywamy operatory (np. `|`, `<`, `>`, `>>`, `<<`) oraz słowa.
   - Obsługujemy cudzysłowy, aby zawartość między apostrofami lub cudzysłowami była traktowana jako pojedynczy token.

_Pseudokod dla funkcji tokenize:_
```
function tokenize(line):
    tokens = empty list
    i = 0
    while i < length(line):
        if line[i] jest białym znakiem:
            i++
            continue
        if line[i] jest operatorem:
            if operator ma dwa znaki (np. >> lub <<):
                dodaj token o odpowiednim typie
                i += 2
            else:
                dodaj token o odpowiednim typie
                i++
        else:
            start = i
            in_quotes = false
            while (line[i] istnieje) and (in_quotes lub (nie jest białym znakiem i nie operatorem)):
                if line[i] jest cudzysłowem:
                    zmień stan in_quotes
                i++
            word = substring(line, start, i - start)
            dodaj token typu WORD z wartością word
    return tokens
```

#### Parser
- Pobieramy listę tokenów.
- Tworzymy strukturę polecenia, iterując przez tokeny.
- Gdy natrafimy na token PIPE, tworzymy nowe polecenie w łańcuchu.
- Tokeny typu przekierowania – pobieramy kolejny token jako nazwę pliku.

_Pseudokod dla funkcji build_commands:_
```
function build_commands(tokens):
    cmd = init_cmd()
    current = tokens.head
    while current nie jest EOF:
        if current.type jest WORD:
            add argument do cmd
        else if current.type jest PIPE:
            utwórz nowy cmd i połącz z poprzednim
        else if current.type jest przekierowaniem:
            przejdź do następnego tokena (sprawdź, czy to WORD)
            dodaj przekierowanie do cmd
        current = current.next
    return pierwsze cmd
```

#### Expander (rozwijanie zmiennych)
- Iterujemy przez wszystkie argumenty i przekierowania.
- W pojedynczych cudzysłowach zmienne nie są rozwijane, a w podwójnych – już tak.

_Pseudokod dla funkcji expand_arg:_
```
function expand_arg(arg, shell):
    result = empty string
    i = 0
    while i < length(arg):
        if arg[i] jest pojedynczym cudzysłowem:
            toggle in_single_quotes
        else if arg[i] jest podwójnym cudzysłowem:
            toggle in_double_quotes
        else if arg[i] jest '$' i nie jesteśmy w pojedynczych cudzysłowach:
            if następny znak to '?':
                dodaj wartość exit_status do result
                i++
            else:
                pobierz nazwę zmiennej
                znajdź wartość zmiennej w shell
                dodaj wartość do result
        else:
            dodaj arg[i] do result
        i++
    return result
```

### 1.2. Przykładowy kod w C

#### Struktury danych (header, np. `minishell.h`)
```c
#ifndef MINISHELL_H
# define MINISHELL_H

typedef enum e_token_type {
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIR_IN,
    TOKEN_REDIR_OUT,
    TOKEN_APPEND,
    TOKEN_HEREDOC,
    TOKEN_EOF
} t_token_type;

typedef struct s_token {
    t_token_type type;
    char *value;
    struct s_token *next;
} t_token;

typedef struct s_redir {
    t_token_type type;
    char *file;
    struct s_redir *next;
} t_redir;

typedef struct s_cmd {
    char **args;
    t_redir *redirs;
    struct s_cmd *next;
} t_cmd;

typedef struct s_shell {
    char **env;
    int exit_status;
    t_cmd *cmd;
} t_shell;

#endif
```

#### Funkcja tokenize (lexer.c)
```c
#include "minishell.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

static int is_operator(char c)
{
    return (c == '|' || c == '<' || c == '>');
}

t_token *create_token(t_token_type type, char *value)
{
    t_token *new = malloc(sizeof(t_token));
    if (!new)
        return (NULL);
    new->type = type;
    new->value = strdup(value);
    new->next = NULL;
    return (new);
}

void add_token(t_token **tokens, t_token *new_token)
{
    t_token *tmp;
    if (!*tokens)
        *tokens = new_token;
    else
    {
        tmp = *tokens;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new_token;
    }
}

t_token *tokenize(char *line)
{
    t_token *tokens = NULL;
    int i = 0;
    while (line[i])
    {
        if (isspace(line[i]))
        {
            i++;
            continue;
        }
        if (line[i] == '|')
        {
            add_token(&tokens, create_token(TOKEN_PIPE, "|"));
            i++;
        }
        else if (line[i] == '<' && line[i + 1] == '<')
        {
            add_token(&tokens, create_token(TOKEN_HEREDOC, "<<"));
            i += 2;
        }
        else if (line[i] == '>' && line[i + 1] == '>')
        {
            add_token(&tokens, create_token(TOKEN_APPEND, ">>"));
            i += 2;
        }
        else if (line[i] == '<')
        {
            add_token(&tokens, create_token(TOKEN_REDIR_IN, "<"));
            i++;
        }
        else if (line[i] == '>')
        {
            add_token(&tokens, create_token(TOKEN_REDIR_OUT, ">"));
            i++;
        }
        else
        {
            int start = i;
            int in_quotes = 0;
            char quote_char = '\0';
            while (line[i] && (in_quotes || (!isspace(line[i]) && !is_operator(line[i]))))
            {
                if ((line[i] == '\'' || line[i] == '"') && (!in_quotes || line[i] == quote_char))
                {
                    in_quotes = !in_quotes;
                    quote_char = in_quotes ? line[i] : '\0';
                }
                i++;
            }
            char *word = strndup(line + start, i - start);
            add_token(&tokens, create_token(TOKEN_WORD, word));
            free(word);
        }
    }
    add_token(&tokens, create_token(TOKEN_EOF, ""));
    return tokens;
}
```

#### Parser – budowanie struktury polecenia (parser.c)
```c
#include "minishell.h"
#include <stdlib.h>
#include <stdio.h>

t_cmd *init_cmd(void)
{
    t_cmd *cmd = malloc(sizeof(t_cmd));
    if (!cmd)
        return (NULL);
    cmd->args = NULL;
    cmd->redirs = NULL;
    cmd->next = NULL;
    return cmd;
}

void add_arg_to_cmd(t_cmd *cmd, char *arg)
{
    int count = 0;
    char **new_args;
    while (cmd->args && cmd->args[count])
        count++;
    new_args = malloc(sizeof(char *) * (count + 2));
    for (int i = 0; i < count; i++)
        new_args[i] = cmd->args[i];
    new_args[count] = strdup(arg);
    new_args[count + 1] = NULL;
    free(cmd->args);
    cmd->args = new_args;
}

t_cmd *build_commands(t_token *tokens)
{
    t_cmd *cmd = init_cmd();
    t_cmd *head = cmd;
    t_token *current = tokens;

    while (current && current->type != TOKEN_EOF)
    {
        if (current->type == TOKEN_WORD)
            add_arg_to_cmd(cmd, current->value);
        else if (current->type == TOKEN_PIPE)
        {
            cmd->next = init_cmd();
            cmd = cmd->next;
        }
        // Tutaj można rozszerzyć obsługę przekierowań
        current = current->next;
    }
    return head;
}
```

#### Expander – rozwijanie zmiennych (expander.c)
```c
#include "minishell.h"
#include <stdlib.h>
#include <string.h>

// Przyjmujemy, że funkcja get_env zwraca wartość zmiennej środowiskowej
char *get_env(t_shell *shell, char *var)
{
    // Prosty przykład, należy rozwinąć o pełną obsługę
    for (int i = 0; shell->env[i]; i++)
    {
        if (strncmp(shell->env[i], var, strlen(var)) == 0 &&
            shell->env[i][strlen(var)] == '=')
            return shell->env[i] + strlen(var) + 1;
    }
    return NULL;
}

char *ft_strjoin_free(char *s1, char *s2)
{
    int len = strlen(s1) + strlen(s2);
    char *result = malloc(len + 1);
    if (!result)
        return NULL;
    strcpy(result, s1);
    strcat(result, s2);
    free(s1);
    return result;
}

char *expand_arg(char *arg, t_shell *shell)
{
    char *result = strdup("");
    int i = 0;
    int in_single = 0, in_double = 0;

    while (arg[i])
    {
        if (arg[i] == '\'' && !in_double)
            in_single = !in_single;
        else if (arg[i] == '"' && !in_single)
            in_double = !in_double;
        else if (arg[i] == '$' && !in_single)
        {
            i++;
            if (arg[i] == '?')
            {
                char buffer[16];
                sprintf(buffer, "%d", shell->exit_status);
                result = ft_strjoin_free(result, buffer);
                i++;
                continue;
            }
            else
            {
                int start = i;
                while (arg[i] && (isalnum(arg[i]) || arg[i] == '_'))
                    i++;
                char *var_name = strndup(arg + start, i - start);
                char *var_value = get_env(shell, var_name);
                if (var_value)
                    result = ft_strjoin_free(result, var_value);
                free(var_name);
                continue;
            }
        }
        else
        {
            char tmp[2] = {arg[i], '\0'};
            result = ft_strjoin_free(result, tmp);
        }
        i++;
    }
    free(arg);
    return result;
}
```

---

## Część 2: Wykonanie (Execution)

### 2.1. Pseudokod

#### Główne zadania:
1. **Główna pętla wykonania**
   - Wywołanie funkcji parsującej, a następnie przekazanie struktury poleceń do modułu wykonawczego.
2. **Wykonywanie poleceń prostych**
   - Jeżeli polecenie jest pojedyncze i wbudowane, wykonaj je w bieżącym procesie.
3. **Wykonywanie poleceń z potokami**
   - Iteracja przez listę poleceń.
   - Dla każdego polecenia utworzenie potoku, forki oraz ustawienie deskryptorów.
4. **Obsługa przekierowań**
   - Przed wykonaniem polecenia zapamiętujemy oryginalne deskryptory.
   - Ustawiamy przekierowania (wejście/wyjście) za pomocą `dup2`.
   - Po wykonaniu przywracamy oryginalne deskryptory.
5. **Obsługa poleceń wbudowanych i zewnętrznych**
   - Rozróżniamy polecenia wbudowane (wykonywane wewnątrz minishella) oraz zewnętrzne (przy użyciu `execve`).
6. **Obsługa sygnałów**
   - Ustawienie funkcji obsługi sygnałów (np. SIGINT dla Ctrl+C) przy użyciu `sigaction`.

_Pseudokod dla funkcji execute_pipeline:_
```
function execute_pipeline(shell):
    cmd = shell.cmd
    prev_pipe = -1
    while cmd istnieje:
        if cmd ma następne polecenie:
            utwórz pipe_fd
        fork() -> pid
        if (pid == 0): // proces potomny
            if (prev_pipe != -1):
                dup2(prev_pipe, STDIN)
                zamknij(prev_pipe)
            if (cmd ma następne):
                dup2(pipe_fd[1], STDOUT)
                zamknij(pipe_fd[0]) i pipe_fd[1]
            if (polecenie jest wbudowane):
                wykonaj built-in i exit
            else:
                znajdź ścieżkę do polecenia
                execve(ścieżka, args, env)
        else: // proces macierzysty
            jeśli (prev_pipe != -1) zamknij(prev_pipe)
            jeśli (cmd ma następne):
                prev_pipe = pipe_fd[0]
            zapamiętaj pid ostatniego polecenia
        cmd = cmd.next
    czekaj na ostatni proces potomny
```

#### Przekierowania
```
function setup_redirections(cmd, saved_fds):
    saved_fds[0] = dup(STDIN)
    saved_fds[1] = dup(STDOUT)
    for każdy redir w cmd.redirs:
        if redir.type == REDIR_IN:
            otwórz plik do odczytu, dup2(fd, STDIN)
        else if redir.type == REDIR_OUT:
            otwórz plik do zapisu/trunc, dup2(fd, STDOUT)
        else if redir.type == APPEND:
            otwórz plik do zapisu/append, dup2(fd, STDOUT)
    return success

function restore_fds(saved_fds):
    dup2(saved_fds[0], STDIN)
    dup2(saved_fds[1], STDOUT)
    zamknij saved_fds[0] i saved_fds[1]
```

### 2.2. Przykładowy kod w C

#### Główna pętla wykonania (main.c – fragment związany z execution)
```c
#include "minishell.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

void execute_command(t_shell *shell)
{
    if (!shell->cmd)
        return;

    // Jeśli jest tylko jedno polecenie i jest wbudowane
    if (!shell->cmd->next && is_builtin(shell->cmd->args[0]))
    {
        int saved_fds[2];
        if (setup_redirections(shell->cmd, saved_fds) == 0)
            shell->exit_status = execute_builtin(shell->cmd, shell);
        restore_fds(saved_fds);
        return;
    }

    // W przeciwnym razie wykonujemy potok
    execute_pipeline(shell);
}
```

#### Funkcja execute_pipeline (executor.c)
```c
#include "minishell.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

void setup_child_process(t_cmd *cmd, int prev_pipe, int pipe_fd[2])
{
    // Ustawienie przekierowań zdefiniowanych w cmd
    if (cmd->redirs)
        apply_redirections(cmd->redirs);

    // Jeśli istnieje poprzedni potok, ustawiamy STDIN
    if (prev_pipe != -1)
    {
        dup2(prev_pipe, STDIN_FILENO);
        close(prev_pipe);
    }

    // Jeśli istnieje następne polecenie, ustawiamy STDOUT na pipe_fd[1]
    if (cmd->next)
    {
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[0]);
        close(pipe_fd[1]);
    }
}

void execute_pipeline(t_shell *shell)
{
    t_cmd *cmd = shell->cmd;
    int prev_pipe = -1;
    pid_t last_pid;
    int pipe_fd[2];

    while (cmd)
    {
        if (cmd->next)
        {
            if (pipe(pipe_fd) < 0)
            {
                perror("pipe");
                return;
            }
        }

        pid_t pid = fork();
        if (pid < 0)
        {
            perror("fork");
            return;
        }

        if (pid == 0)
        {
            // Proces potomny
            setup_child_process(cmd, prev_pipe, pipe_fd);
            if (is_builtin(cmd->args[0]))
                exit(execute_builtin(cmd, shell));
            else
                execute_external_command(cmd, shell);
        }
        else
        {
            // Proces macierzysty
            if (prev_pipe != -1)
                close(prev_pipe);

            if (cmd->next)
            {
                close(pipe_fd[1]);
                prev_pipe = pipe_fd[0];
            }
            else
            {
                last_pid = pid;
            }
        }
        cmd = cmd->next;
    }

    int status;
    waitpid(last_pid, &status, 0);
    if (WIFEXITED(status))
        shell->exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        shell->exit_status = WTERMSIG(status) + 128;
}
```

#### Funkcje przekierowań (redirections.c)
```c
#include "minishell.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int setup_redirections(t_cmd *cmd, int saved_fds[2])
{
    t_redir *redir = cmd->redirs;

    saved_fds[0] = dup(STDIN_FILENO);
    saved_fds[1] = dup(STDOUT_FILENO);

    while (redir)
    {
        int fd;
        if (redir->type == TOKEN_REDIR_IN)
        {
            fd = open(redir->file, O_RDONLY);
            if (fd < 0)
            {
                perror(redir->file);
                return 1;
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        else if (redir->type == TOKEN_REDIR_OUT)
        {
            fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0)
            {
                perror(redir->file);
                return 1;
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        else if (redir->type == TOKEN_APPEND)
        {
            fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0)
            {
                perror(redir->file);
                return 1;
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        // Obsługa heredocu może być rozszerzona według potrzeb
        redir = redir->next;
    }

    return 0;
}

void restore_fds(int saved_fds[2])
{
    dup2(saved_fds[0], STDIN_FILENO);
    dup2(saved_fds[1], STDOUT_FILENO);
    close(saved_fds[0]);
    close(saved_fds[1]);
}
```

#### Wyszukiwanie i wykonanie poleceń zewnętrznych (external.c)
```c
#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

char *find_command_path(char *cmd, t_shell *shell)
{
    // Przykładowa implementacja: przeszukujemy PATH
    // Tutaj należy zaimplementować poprawne wyszukiwanie ścieżki
    return strdup(cmd); // Dla uproszczenia
}

void execute_external_command(t_cmd *cmd, t_shell *shell)
{
    char *path = find_command_path(cmd->args[0], shell);
    if (!path)
    {
        fprintf(stderr, "minishell: command not found: %s\n", cmd->args[0]);
        exit(127);
    }
    execve(path, cmd->args, shell->env);
    perror("execve");
    free(path);
    exit(126);
}
```

#### Obsługa sygnałów (signals.c)
```c
#include "minishell.h"
#include <signal.h>
#include <stdio.h>
#include <readline/readline.h>

void handle_sigint(int signum)
{
    (void)signum;
    write(STDOUT_FILENO, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void setup_signals(void)
{
    struct sigaction sa;
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    // Można dodać obsługę innych sygnałów, np. SIGQUIT
}
```

---

## Podsumowanie

**Część 1: Parsowanie** obejmuje:
- Definicję struktur danych (token, cmd, redir).
- Implementację funkcji tokenizacji (`tokenize`), budowania struktury poleceń (`build_commands`) i rozwijania zmiennych (`expand_arg`).

**Część 2: Wykonanie** obejmuje:
- Główną pętlę wykonania, rozróżnienie poleceń wbudowanych i zewnętrznych.
- Obsługę potoków (funkcja `execute_pipeline`), przekierowań (`setup_redirections`/`restore_fds`), oraz wykonanie poleceń zewnętrznych za pomocą `execve`.
- Obsługę sygnałów.

Obie części są zaprojektowane tak, aby ich interfejs (np. struktura `t_cmd`) był wspólny. Dzięki temu osoba odpowiedzialna za parsowanie tworzy strukturę, którą druga osoba wykorzystuje do wykonania poleceń. Po integracji obu modułów uzyskacie kompletny projekt minishella.
