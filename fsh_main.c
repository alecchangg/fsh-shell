#include "fsh_builtins.h"

#define FSH_RL_BUFSIZE 1024
#define FSH_TOK_BUFSIZE 64
#define FSH_TOK_DELIM " \t\r\n\a"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define BOLD_RESET "\x1B[0m"
#define BOLD "\x1B[1m"

int fsh_launch(char **args) {
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("fsh");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0) {
        perror("fsh");
    }
    else {
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}

int fsh_execute(char **args) {
    if (args[0] == NULL) {
        return 1;
    }

    for (int i = 0; i < fsh_num_builtins(); ++i) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }

    return fsh_launch(args);
}

char **fsh_split_line(char *line) {
    int position = 0;
    int bufsize = FSH_TOK_BUFSIZE;
    char **tokens = malloc(bufsize * sizeof(char));
    char *token;

    if (!token) {
        fprintf(stderr, "fsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, FSH_TOK_DELIM);
    while (token != NULL) {
        tokens[position] = token;
        ++position;

        if (position >= bufsize) {
            bufsize += FSH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "fsh: allocation error\n");
                exit(EXIT_FAILURE);
            }         
        }

        token = strtok(NULL, FSH_TOK_DELIM);
    }

    tokens[position] = NULL;
    return tokens;
}

char *fsh_read_line(void) {
    char *line = NULL;
    ssize_t bufsize = 0;

    if (getline(&line, &bufsize, stdin) == -1) {
        if (feof(stdin)) {
            exit(EXIT_SUCCESS);
        }
        else {
            perror("readline");
            exit(EXIT_FAILURE);
        }
    }

    return line;
}

void fsh_cwd() {
    char buf[PATH_MAX];
    if (getcwd(buf, sizeof(buf)) != NULL) {
        printf(ANSI_COLOR_BLUE "%s%s%s" ANSI_COLOR_RESET, BOLD, buf, BOLD_RESET);
    }
    else {
        perror("fsh_cwd");
    }
}

void fsh_loop(void) {
    char *line;
    char **args;
    int status;

    do {
        fsh_cwd();
        printf("> ");
        line = fsh_read_line();
        args = fsh_split_line(line);
        status = fsh_execute(args);

        free(line);
        free(args);
    } while( status );
}

int main(int argc, char **argv) {
    fsh_loop();
}