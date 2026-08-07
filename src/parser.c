#include "karla.h"

int parse_input(char *line, Command *cmd) {
    memset(cmd, 0, sizeof(*cmd));

    char *tokens[MAX_ARGS];
    int   ntok = 0;

    char *tok = strtok(line, " \t");
    while (tok && ntok < MAX_ARGS - 1) {
        tokens[ntok++] = tok;
        tok = strtok(NULL, " \t");
    }
    tokens[ntok] = NULL;

    if (ntok == 0)
        return -1;

    int argc = 0;
    for(int i = 0; i < ntok; i++) {
        if (strcmp(tokens[i], "<") == 0) {
            if (i + 1 >= ntok) {
                fprintf(stderr, "karla: expected filename after '<'\n");
                return -1;
            }
            cmd->input_file = tokens[++i];

        } else if (strcmp(tokens[i], ">") == 0) {
            if (i + 1 >= ntok) {
                fprintf(stderr, "karla: expected filename after '>'\n");
                return -1;
            }
            cmd->output_file = tokens[++i];
            cmd->append = 0;

        } else if (strcmp(tokens[i], ">>") == 0) {
            if (i + 1 >= ntok) {
                fprintf(stderr, "karla: expected filename after '>>'\n");
                return -1;
            }
            cmd->output_file = tokens[++i];
            cmd->append = 1;

        } else if (strcmp(tokens[i], "&") == 0) {
            cmd->background = 1;

        } else {
            cmd->argv[argc++] = tokens[i];
        }
    }
    cmd->argv[argc] = NULL;
    cmd->argc = argc;

    return (argc > 0) ? 0 : -1;
}