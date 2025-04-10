Poniżej znajdziesz rozszerzony i szczegółowo omówiony tutorial dotyczący budowy własnego minishella. Postaram się wytłumaczyć każdy element, abyś mógł zrozumieć, jak podejść do implementacji projektu krok po kroku, zamiast otrzymać gotowy kod, którego nie rozumiesz.

---

## 1. Cel Projektu

**Minishell** to uproszczona wersja interpretera poleceń (shella), podobnego do bash. Twoim zadaniem jest stworzenie programu, który:
- Wyświetla prompt (np. `minishell> `)
- Umożliwia wprowadzanie i przetwarzanie poleceń
- Obsługuje historię poleceń (dzięki readline)
- Uruchamia programy zewnętrzne oraz wykonuje polecenia wbudowane (built-in)
- Obsługuje cudzysłowy, zmienne środowiskowe, przekierowania wejścia/wyjścia oraz potoki
- Reaguje na sygnały (np. Ctrl+C, Ctrl+\)

W kolejnych sekcjach omówimy, jak zaprojektować strukturę projektu, dane oraz logikę działania.

---

## 2. Struktura Katalogów Projektu

Organizacja plików w projekcie pomaga uporządkować kod. Proponowana struktura:

```
minishell/
├── includes/
│   ├── minishell.h        // Główny nagłówek z deklaracjami
│   ├── parser.h           // Deklaracje funkcji parsera
│   └── executor.h         // Deklaracje funkcji wykonawczych
├── src/
│   ├── main.c             // Punkt wejścia programu
│   ├── parser/            // Moduł odpowiedzialny za analizę poleceń
│   │   ├── lexer.c        // Tokenizacja (dzielenie linii na "tokeny")
│   │   ├── parser.c       // Budowanie struktury poleceń z tokenów
│   │   ├── expander.c     // Rozwinięcie zmiennych i obsługa cudzysłowów
│   │   └── quotes.c       // Specjalne traktowanie cudzysłowów
│   ├── executor/          // Moduł wykonawczy
│   │   ├── executor.c     // Główna logika wykonania polecenia
│   │   ├── redirections.c // Obsługa przekierowań wejścia/wyjścia
│   │   ├── pipes.c        // Ustawianie potoków między poleceniami
│   │   └── signals.c      // Obsługa sygnałów
│   ├── builtins/          // Polecenia wbudowane (np. echo, cd, pwd, export, unset, env, exit)
│   │   ├── echo.c
│   │   ├── cd.c
│   │   ├── pwd.c
│   │   ├── export.c
│   │   ├── unset.c
│   │   ├── env.c
│   │   └── exit.c
│   └── utils/             // Funkcje pomocnicze (np. obsługa zmiennych środowiskowych, operacje na łańcuchach znaków)
│       ├── env_utils.c
│       ├── string_utils.c
│       └── error_utils.c
├── libft/                 // Twoja biblioteka (libft)
└── Makefile               // Skrypt budujący projekt
```

Ta struktura dzieli projekt na moduły: parsowanie (analiza składniowa), wykonanie poleceń oraz polecenia wbudowane, co ułatwia organizację pracy i debugowanie.

---

## 3. Projektowanie Struktur Danych

W minishellu kluczowe są struktury reprezentujące:
- **Tokeny** – fragmenty wprowadzonej linii, które są rozpoznawane przez lexer.
- **Polecenia** – reprezentacja pojedynczego polecenia, zawierająca listę argumentów oraz przekierowania.
- **Stan powłoki** – zawiera m.in. zmienne środowiskowe, ostatni status wykonania oraz bieżące polecenie.

### Przykładowe Struktury

```c
// Typy tokenów (np. słowo, operator potoku, przekierowania)
typedef enum e_token_type {
    TOKEN_WORD,        // Polecenie lub argument
    TOKEN_PIPE,        // |
    TOKEN_REDIR_IN,    // <
    TOKEN_REDIR_OUT,   // >
    TOKEN_APPEND,      // >>
    TOKEN_HEREDOC,     // <<
    TOKEN_AND,         // && (opcjonalnie)
    TOKEN_OR,          // || (opcjonalnie)
    TOKEN_OPEN_PAREN,  // ( (opcjonalnie)
    TOKEN_CLOSE_PAREN, // ) (opcjonalnie)
    TOKEN_EOF          // Koniec wejścia
} t_token_type;

// Struktura tokena (wynik działania leksykalnego)
typedef struct s_token {
    t_token_type type;
    char *value;
    struct s_token *next;
} t_token;

// Struktura przekierowania (np. dla <, >, >>, <<)
typedef struct s_redir {
    t_token_type type;  // Typ przekierowania
    char *file;         // Nazwa pliku lub delimiter dla heredoc
    struct s_redir *next;
} t_redir;

// Struktura polecenia
typedef struct s_cmd {
    char **args;        // Tablica argumentów (pierwszy to nazwa polecenia)
    t_redir *redirs;    // Lista przekierowań
    struct s_cmd *next; // Kolejne polecenie w potoku
} t_cmd;

// Stan powłoki
typedef struct s_shell {
    char **env;         // Zmienne środowiskowe
    int exit_status;    // Status zakończenia ostatniego polecenia
    t_cmd *cmd;         // Aktualne polecenie do wykonania
    int in_pipe;        // Flaga określająca, czy jesteśmy w potoku
    int signal;         // Odebrany sygnał
} t_shell;
```

### Omówienie
- **t_token** – Każdy token zawiera typ oraz wartość (np. nazwa polecenia lub operator). Lista tokenów (powiązana lista) ułatwia kolejny etap przetwarzania.
- **t_cmd** – Reprezentuje pojedyncze polecenie, wraz z listą argumentów i informacjami o przekierowaniach.
- **t_shell** – Przechowuje ogólny stan powłoki, co ułatwia dostęp do zmiennych środowiskowych i zarządzanie wykonaniem poleceń.

---

## 4. Główna Pętla Programu

W pliku `main.c` znajduje się główna pętla powłoki, która:
1. Inicjalizuje powłokę (ustawia zmienne środowiskowe, sygnały itp.)
2. Wyświetla prompt i czeka na wejście od użytkownika
3. Parsuje wprowadzoną linię i buduje strukturę poleceń
4. Wykonuje polecenie
5. Czyści pamięć i przechodzi do kolejnego polecenia

### Przykładowy kod `main.c`

```c
int main(int argc, char **argv, char **envp)
{
    t_shell shell;
    char *line;

    // Inicjalizacja powłoki: kopiujemy środowisko i ustawiamy obsługę sygnałów
    init_shell(&shell, envp);

    // Główna pętla programu
    while (1)
    {
        // Wyświetlenie prompta
        display_prompt();

        // Odczyt wejścia (używając readline)
        line = readline("minishell> ");

        // Obsługa końca wejścia (Ctrl+D)
        if (!line)
            break;

        // Dodanie do historii, jeśli linia nie jest pusta
        if (*line)
            add_history(line);

        // Parsowanie linii do struktury poleceń
        if (parse_line(line, &shell) == 0)
        {
            // Wykonanie polecenia, gdy parsowanie przebiegło pomyślnie
            execute_command(&shell);
        }

        // Zwolnienie pamięci zajmowanej przez linię wejścia
        free(line);
        // Resetujemy stan powłoki przed kolejnym poleceniem
        reset_shell(&shell);
    }

    // Ostateczne sprzątanie i zakończenie programu
    cleanup_shell(&shell);
    return (shell.exit_status);
}
```

### Wyjaśnienie
- **init_shell**: Funkcja inicjalizująca stan powłoki, kopiująca zmienne środowiskowe i ustawiająca obsługę sygnałów.
- **readline**: Pobiera wejście od użytkownika. Jeśli użytkownik naciśnie Ctrl+D, `readline` zwróci NULL, co przerywa pętlę.
- **parse_line**: Przetwarza wejście użytkownika, dzieląc je na tokeny i budując strukturę poleceń.
- **execute_command**: Wykonuje polecenie lub zestaw poleceń (np. z potokiem).

---

## 5. Moduł Parsowania

Parsowanie dzieli się na kilka etapów:
1. **Analiza leksykalna (tokenizacja)** – dzieli linię wejściową na mniejsze fragmenty (tokeny) i rozpoznaje operatory (|, <, >, itd.) oraz słowa.
2. **Analiza składniowa (parser)** – na podstawie tokenów buduje strukturę polecenia.
3. **Rozwinięcie zmiennych (expander)** – zastępuje zmienne środowiskowe (np. `$PATH`, `$?`) oraz obsługuje cudzysłowy.

### 5.1 Lexer (Tokenizacja)

Funkcja **tokenize** przebiega po znaku po znaku i rozpoznaje:
- Białe znaki (przestrzenie) – są pomijane
- Operatory (np. `|`, `<`, `>`, `>>`, `<<`)
- Słowa – ciągi znaków, które mogą być otoczone cudzysłowami

#### Przykładowy pseudokod

```c
t_token *tokenize(char *line)
{
    t_token *tokens = NULL;
    int i = 0;

    while (line[i])
    {
        // Pomijamy białe znaki
        if (isspace(line[i]))
        {
            i++;
            continue;
        }

        // Obsługa operatorów
        if (line[i] == '|')
            add_token(&tokens, TOKEN_PIPE, "|");
        else if (line[i] == '<' && line[i+1] == '<')
            add_token(&tokens, TOKEN_HEREDOC, "<<");
        else if (line[i] == '>' && line[i+1] == '>')
            add_token(&tokens, TOKEN_APPEND, ">>");
        else if (line[i] == '<')
            add_token(&tokens, TOKEN_REDIR_IN, "<");
        else if (line[i] == '>')
            add_token(&tokens, TOKEN_REDIR_OUT, ">");
        // Możesz dodać obsługę innych operatorów

        // Obsługa tokenów typu "słowo" (w tym z cudzysłowami)
        else
        {
            int start = i;
            int in_quotes = 0;
            char quote_char = 0;

            // Przechodzimy przez ciąg, uwzględniając cudzysłowy
            while (line[i] && (in_quotes || (!isspace(line[i]) && !is_operator(line[i]))))
            {
                // Jeżeli napotkamy cudzysłów, zmieniamy tryb
                if ((line[i] == '\'' || line[i] == '"') && (!in_quotes || line[i] == quote_char))
                {
                    in_quotes = !in_quotes;
                    quote_char = in_quotes ? line[i] : 0;
                }
                i++;
            }

            // Wyodrębniamy słowo
            char *word = ft_substr(line, start, i - start);
            add_token(&tokens, TOKEN_WORD, word);
            free(word);

            // Nie zwiększamy i tutaj, aby ponownie przetworzyć bieżący znak
            continue;
        }

        // Przechodzimy do kolejnego znaku
        i++;
    }

    return tokens;
}
```

**Wyjaśnienie:**
- **add_token:** Funkcja dodająca nowy token do listy.
- **ft_substr:** Funkcja (z biblioteki libft) tworząca podłańcuch.
- Obsługa cudzysłowów gwarantuje, że zawartość między apostrofami lub cudzysłowami zostanie potraktowana jako jeden token.

### 5.2 Parser

Parser przyjmuje listę tokenów i buduje z niej strukturę polecenia (`t_cmd`).
Przykładowy pseudokod:

```c
int parse_line(char *line, t_shell *shell)
{
    t_token *tokens;

    // Tokenizacja wejścia
    tokens = tokenize(line);
    if (!tokens)
        return (1); // Błąd lub pusta linia

    // Budujemy strukturę poleceń z tokenów
    shell->cmd = build_commands(tokens, shell);

    // Zwolnienie pamięci po tokenach
    free_tokens(tokens);

    // Sprawdzamy, czy parser nie napotkał błędów
    if (!shell->cmd)
        return (1);

    return (0);
}

t_cmd *build_commands(t_token *tokens, t_shell *shell)
{
    t_cmd *cmd = NULL;
    t_token *current = tokens;

    // Tworzymy pierwsze polecenie
    cmd = init_cmd();

    while (current && current->type != TOKEN_EOF)
    {
        if (current->type == TOKEN_WORD)
        {
            // Dodajemy argument do bieżącego polecenia
            add_arg_to_cmd(cmd, current->value);
        }
        else if (current->type == TOKEN_PIPE)
        {
            // Operator potoku – tworzymy nowe polecenie w łańcuchu
            t_cmd *next_cmd = init_cmd();
            cmd->next = next_cmd;
            cmd = next_cmd;
        }
        else if (is_redirection(current->type))
        {
            // Obsługa przekierowań
            current = current->next; // Przechodzimy do tokena z nazwą pliku
            if (!current || current->type != TOKEN_WORD)
            {
                // Błąd składni
                free_commands(cmd);
                return NULL;
            }

            // Dodajemy przekierowanie do polecenia
            add_redirection(cmd, current->type, current->value);
        }

        current = current->next;
    }

    return cmd;
}
```

**Wyjaśnienie:**
- **init_cmd:** Funkcja tworząca nową strukturę `t_cmd`.
- **add_arg_to_cmd:** Funkcja dodająca kolejny argument do listy argumentów polecenia.
- **is_redirection:** Sprawdza, czy dany token reprezentuje przekierowanie.
- Parser iteracyjnie buduje listę poleceń, rozdzielając je operatorami potoku.

### 5.3 Rozwinięcie Zmiennych (Expander)

Funkcja `expand_variables` przegląda wszystkie argumenty i przekierowania, szukając zmiennych środowiskowych (np. `$VAR`, `$?`) i zastępuje je odpowiednimi wartościami.

#### Przykładowy kod

```c
void expand_variables(t_cmd *cmd, t_shell *shell)
{
    int i = 0;

    while (cmd)
    {
        // Rozwijamy zmienne w argumentach
        i = 0;
        while (cmd->args[i])
        {
            cmd->args[i] = expand_arg(cmd->args[i], shell);
            i++;
        }

        // Rozwijamy zmienne w przekierowaniach
        t_redir *redir = cmd->redirs;
        while (redir)
        {
            if (redir->type != TOKEN_HEREDOC) // W heredocu nie rozwijamy delimiter
                redir->file = expand_arg(redir->file, shell);
            redir = redir->next;
        }

        cmd = cmd->next;
    }
}

char *expand_arg(char *arg, t_shell *shell)
{
    // Funkcja rozwijająca zmienne w danym argumencie
    char *result = ft_strdup("");
    int i = 0;
    int in_single_quotes = 0;
    int in_double_quotes = 0;

    while (arg[i])
    {
        if (arg[i] == '\'' && !in_double_quotes)
            in_single_quotes = !in_single_quotes;
        else if (arg[i] == '"' && !in_single_quotes)
            in_double_quotes = !in_double_quotes;
        else if (arg[i] == '$' && !in_single_quotes)
        {
            // Rozwijamy zmienną
            i++;
            if (arg[i] == '?') // Specjalna zmienna $?
            {
                char *status = ft_itoa(shell->exit_status);
                result = ft_strjoin_free(result, status);
                free(status);
                i++;
            }
            else if (ft_isalnum(arg[i]) || arg[i] == '_')
            {
                // Pobieramy nazwę zmiennej
                int start = i;
                while (ft_isalnum(arg[i]) || arg[i] == '_')
                    i++;

                char *var_name = ft_substr(arg, start, i - start);
                char *var_value = get_env(shell, var_name);

                if (var_value)
                    result = ft_strjoin_free(result, var_value);

                free(var_name);
                continue;
            }
        }
        else
        {
            // Zwykły znak – dodajemy do wyniku
            char c[2] = {arg[i], '\0'};
            result = ft_strjoin_free(result, c);
        }

        i++;
    }

    free(arg);
    return result;
}
```

**Wyjaśnienie:**
- **expand_arg:** Przetwarza ciąg znaków, zwracając nowy łańcuch, w którym zmienne środowiskowe zostały zastąpione ich wartościami.
- Rozpoznaje tryby cudzysłowów – w pojedynczych cudzysłowach zmienne **nie są** rozwijane.
- **ft_strjoin_free:** Funkcja, która łączy dwa łańcuchy, zwalniając przy tym poprzedni wynik, co zapobiega wyciekom pamięci.

---

## 6. Moduł Wykonawczy (Executor)

Moduł wykonawczy odpowiada za:
- Ustawienie przekierowań wejścia/wyjścia (np. przy użyciu `dup2`)
- Ustawienie potoków między poleceniami
- Tworzenie procesów potomnych (fork) i wykonywanie poleceń
- Obsługę poleceń wbudowanych osobno

### 6.1 Wykonanie Pojedynczego Polecenia

Jeśli mamy jedno polecenie (bez potoku) i jest ono wbudowane, wykonujemy je w bieżącym procesie.

#### Przykładowy kod

```c
void execute_command(t_shell *shell)
{
    if (!shell->cmd)
        return;

    // Jeżeli mamy pojedyncze polecenie i jest to polecenie wbudowane
    if (!shell->cmd->next && is_builtin(shell->cmd->args[0]))
    {
        // Ustawiamy przekierowania (zapamiętujemy oryginalne deskryptory)
        int saved_fds[2];
        if (setup_redirections(shell->cmd, saved_fds) == 0)
            shell->exit_status = execute_builtin(shell->cmd, shell);
        restore_fds(saved_fds);
        return;
    }

    // W przypadku poleceń z potokiem – wywołujemy wykonanie potoku
    execute_pipeline(shell);
}
```

### 6.2 Wykonywanie Potoku

Dla wielu poleceń połączonych potokiem wykonujemy następujące kroki:
- Dla każdego polecenia tworzymy potok (pipe)
- Robimy fork, w procesie potomnym ustawiamy odpowiednie deskryptory wejścia/wyjścia
- Wykonujemy polecenie (wbudowane lub zewnętrzne)

#### Przykładowy pseudokod

```c
void execute_pipeline(t_shell *shell)
{
    t_cmd *cmd = shell->cmd;
    int prev_pipe = -1;
    pid_t last_pid;

    while (cmd)
    {
        int pipe_fd[2];

        // Jeśli jest kolejne polecenie, tworzymy potok
        if (cmd->next && pipe(pipe_fd) < 0)
        {
            perror("pipe");
            return;
        }

        pid_t pid = fork();
        if (pid < 0)
        {
            perror("fork");
            if (prev_pipe != -1)
                close(prev_pipe);
            if (cmd->next)
                close_pipe(pipe_fd);
            return;
        }

        if (pid == 0)
        {
            // Proces potomny
            setup_child_process(cmd, prev_pipe, pipe_fd, shell);

            // Jeśli polecenie jest wbudowane, wykonujemy je i kończymy proces potomny
            if (is_builtin(cmd->args[0]))
                exit(execute_builtin(cmd, shell));
            else
                execute_external_command(cmd, shell);
        }
        else
        {
            // Proces macierzysty: zamykamy niepotrzebne deskryptory
            if (prev_pipe != -1)
                close(prev_pipe);

            if (cmd->next)
            {
                close(pipe_fd[1]);
                prev_pipe = pipe_fd[0];
            }

            if (!cmd->next)
                last_pid = pid;
        }

        cmd = cmd->next;
    }

    // Czekamy na zakończenie ostatniego polecenia
    int status;
    waitpid(last_pid, &status, 0);

    // Ustawiamy status wyjścia
    if (WIFEXITED(status))
        shell->exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        shell->exit_status = WTERMSIG(status) + 128;
}
```

**Wyjaśnienie:**
- **setup_child_process:** Ustawia przekierowania oraz potok w procesie potomnym.
- **execute_external_command:** Szuka ścieżki do zewnętrznego polecenia (np. przy użyciu zmiennej PATH) i wywołuje `execve`.

### 6.3 Obsługa Przekierowań

Przekierowania wejścia i wyjścia ustawiamy, zapisując oryginalne deskryptory, aby później móc je przywrócić.

#### Przykładowy kod

```c
int setup_redirections(t_cmd *cmd, int saved_fds[2])
{
    t_redir *redir = cmd->redirs;

    // Zapamiętujemy oryginalne deskryptory stdin i stdout
    saved_fds[0] = dup(STDIN_FILENO);
    saved_fds[1] = dup(STDOUT_FILENO);

    while (redir)
    {
        if (redir->type == TOKEN_REDIR_IN)
        {
            int fd = open(redir->file, O_RDONLY);
            if (fd < 0)
            {
                perror(redir->file);
                return (1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        else if (redir->type == TOKEN_REDIR_OUT)
        {
            int fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0)
            {
                perror(redir->file);
                return (1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        else if (redir->type == TOKEN_APPEND)
        {
            int fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0)
            {
                perror(redir->file);
                return (1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        else if (redir->type == TOKEN_HEREDOC)
        {
            // Obsługa heredocu
            int heredoc_pipe[2];
            if (pipe(heredoc_pipe) < 0)
            {
                perror("pipe");
                return (1);
            }

            // Odczytujemy zawartość heredocu
            handle_heredoc(heredoc_pipe, redir->file);

            // Przekierowujemy wejście z potoku heredocu
            dup2(heredoc_pipe[0], STDIN_FILENO);
            close(heredoc_pipe[0]);
            close(heredoc_pipe[1]);
        }

        redir = redir->next;
    }

    return (0);
}

void restore_fds(int saved_fds[2])
{
    // Przywracamy oryginalne deskryptory stdin i stdout
    dup2(saved_fds[0], STDIN_FILENO);
    dup2(saved_fds[1], STDOUT_FILENO);
    close(saved_fds[0]);
    close(saved_fds[1]);
}
```

**Wyjaśnienie:**
- **dup2:** Kopiuje deskryptor pliku, co pozwala przekierować standardowe wejście lub wyjście.
- **handle_heredoc:** Funkcja odczytuje dane do momentu napotkania określonego delimiteru, zapisując je do potoku.

---

## 7. Polecenia Wbudowane (Builtins)

Niektóre polecenia (np. `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`) muszą być obsłużone bezpośrednio przez minishell, ponieważ zmieniają stan powłoki.
Przykład implementacji `echo`:

```c
int builtin_echo(t_cmd *cmd)
{
    int i = 1;
    int n_flag = 0;

    // Sprawdzamy, czy jest opcja -n (nie dodawać nowej linii)
    if (cmd->args[i] && ft_strcmp(cmd->args[i], "-n") == 0)
    {
        n_flag = 1;
        i++;
    }

    // Wypisujemy kolejne argumenty
    while (cmd->args[i])
    {
        ft_putstr_fd(cmd->args[i], STDOUT_FILENO);
        if (cmd->args[i + 1])
            ft_putchar_fd(' ', STDOUT_FILENO);
        i++;
    }

    // Jeżeli nie ma flagi -n, dodajemy znak nowej linii
    if (!n_flag)
        ft_putchar_fd('\n', STDOUT_FILENO);

    return (0);
}
```

**Wyjaśnienie:**
- **ft_putstr_fd, ft_putchar_fd:** Funkcje z libft do wypisywania na zadany deskryptor.
- Funkcja zwraca 0, co oznacza powodzenie wykonania.

Pozostałe polecenia (cd, pwd, export, unset, env, exit) implementujesz w podobny sposób, dbając o modyfikację stanu powłoki (np. zmiana bieżącego katalogu w `cd`).

---

## 8. Obsługa Sygnałów

Minishell powinien odpowiednio reagować na sygnały, takie jak:
- **SIGINT (Ctrl+C):** Powinien przerwać bieżące polecenie i wyświetlić nowy prompt.
- **SIGQUIT (Ctrl+\):** Zwykle ignorowany.

#### Przykładowy kod obsługi sygnałów

```c
void setup_signals(void)
{
    // Ustawiamy obsługę sygnałów
    struct sigaction sa;

    // Inicjalizujemy strukturę
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    // Ustawienie obsługi Ctrl+C (SIGINT)
    sa.sa_handler = handle_sigint;
    sigaction(SIGINT, &sa, NULL);

    // Ignorowanie sygnału SIGQUIT (Ctrl+\)
    sa.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa, NULL);
}

void handle_sigint(int signum)
{
    (void)signum;

    // Wyświetlenie nowej linii i prompta po naciśnięciu Ctrl+C
    ft_putchar_fd('\n', STDOUT_FILENO);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}
```

**Wyjaśnienie:**
- **sigaction:** Umożliwia zdefiniowanie własnej funkcji obsługi sygnału.
- **rl_replace_line, rl_redisplay:** Funkcje biblioteki readline, pozwalające na odświeżenie prompta.

---

## 9. Plan Pracy i Podział Zadań

Aby skutecznie stworzyć minishell, warto podzielić pracę (jeśli pracujesz zespołowo) lub uporządkować etapy:
1. **Inicjalizacja projektu**
   - Utworzenie struktury katalogów, Makefile, nagłówków.
2. **Implementacja pętli głównej**
   - Wyświetlanie prompta, odczyt wejścia, proste wykonanie poleceń.
3. **Moduł parsowania**
   - Implementacja lexera, parsera i rozwinięcia zmiennych.
4. **Moduł wykonawczy**
   - Ustawienie przekierowań, obsługa potoków, forki, uruchamianie poleceń.
5. **Implementacja poleceń wbudowanych**
   - echo, cd, pwd, export, unset, env, exit.
6. **Obsługa sygnałów i testowanie**
   - Konfiguracja sygnałów, testowanie edge-case’ów, walidacja działania.
7. **Dodanie funkcji dodatkowych (bonus)**
   - Operatory &&, ||, grupowanie poleceń itp.

---

## 10. Podsumowanie

Powyższy tutorial krok po kroku przedstawia, jak samodzielnie zbudować minishell. Ważne jest:
- Zrozumienie przepływu programu: od odczytania wejścia, przez parsowanie, aż do wykonania poleceń.
- Podział kodu na moduły: parser, executor, builtins, obsługa sygnałów.
- Dokładne zarządzanie pamięcią i deskryptorami plików, aby uniknąć wycieków.

Podejmując się implementacji, zacznij od stworzenia szkieletu programu (pętla główna) i stopniowo dodawaj kolejne funkcjonalności, testując każdy etap. Dzięki temu nauczysz się nie tylko samej implementacji, ale również dobrych praktyk w programowaniu systemowym w języku C.

Powodzenia przy budowie Twojego minishella! Jeśli masz dodatkowe pytania lub napotkasz problemy, możesz wrócić po dalszą pomoc lub wyjaśnienia.
