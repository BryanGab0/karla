#include "karla.h"

/*
 * parser.c - Turns a raw input line into a Command.
 *
 * Tokenises on whitespace, then interprets each token as a redirection
 * (< > >>), a background marker (&), or a plain argument.
 *
 * Note: strtok() mutates `line` in place, so the caller must be done with
 * the raw line (e.g. history) before calling this.
 */
int parse_input(char *line, Command *cmd) {
    memset(cmd, 0, sizeof(*cmd));

    char *tokens[MAX_ARGS];
    int   ntok = 0;

    /* Split on spaces/tabs. Stop one short of MAX_ARGS to leave room
     * for the terminating NULL. */
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
            cmd->input_file = tokens[++i];      /* consume the filename */

        } else if (strcmp(tokens[i], ">") == 0) {
            if (i + 1 >= ntok) {
                fprintf(stderr, "karla: expected filename after '>'\n");
                return -1;
            }
            cmd->output_file = tokens[++i];
            cmd->append = 0;                    /* truncate */

        } else if (strcmp(tokens[i], ">>") == 0) {
            if (i + 1 >= ntok) {
                fprintf(stderr, "karla: expected filename after '>>'\n");
                return -1;
            }
            cmd->output_file = tokens[++i];
            cmd->append = 1;                    /* append */

        } else if (strcmp(tokens[i], "&") == 0) {
            cmd->background = 1;                 /* no operand to consume */

        } else {
            cmd->argv[argc++] = tokens[i];       /* plain argument */
        }
    }
    cmd->argv[argc] = NULL;                      /* required by execvp */
    cmd->argc = argc;

    return (argc > 0) ? 0 : -1;                  /* -1 if only operators */
}