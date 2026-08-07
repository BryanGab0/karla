#include "karla.h"

static void sigchld_handler(int sig) {
    (void)sig;
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

int main(void) {
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &sa, NULL);

    signal(SIGINT, SIG_IGN);

    printf(CLR_GREEN "Welcome to karla! Type 'help' or 'exit'.\n" CLR_RESET);

    while (1) {
        print_prompt();

        char *line = read_line();
        if (!line)
            break;

        trim_newline(line);

        if (*line == '\0') {
            free(line);
            continue;
        }

        history_add(line);

        Command cmd = {0};
        if (parse_input(line, &cmd) < 0) {
            fprintf(stderr, CLR_RED "karla: parse error\n" CLR_RESET);
            free(line);
            continue;
        }

        if (strcmp(cmd.argv[0], "exit") == 0) {
            free(line);
            break;
        }

        execute_command(&cmd);
        free(line);
    }

    printf(CLR_GREEN "\nBye!\n" CLR_RESET);
    return 0;
}