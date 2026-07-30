#include "karla.h"

/*
 * utils.c - Prompt rendering, line input, and small helpers.
 */
 
/* Prints "<basename> $ " for the current directory, falling back to a
 * static prompt if getcwd() fails. fflush is required because the prompt
 * has no trailing newline and would otherwise stay buffered. */
void print_prompt(void) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd))) {
        char *base = strrchr(cwd, '/');
        base = base ? base + 1 : cwd;   /* show last path component only */
        printf(CLR_GREEN "%s" CLR_RESET CLR_CYAN " $ " CLR_RESET, base);
    } else {
        printf(PROMPT);
    }
    fflush(stdout);
}

/* Reads one line from stdin. Returns a heap buffer the caller must free,
 * or NULL on EOF. */
char *read_line(void) {
    char *buf = malloc(MAX_INPUT);
    if (!buf)
        die("malloc");
    if (!fgets(buf, MAX_INPUT, stdin)) {
        free(buf);
        return NULL;
    }
    return buf;
}

/* Strips the trailing newline left by fgets, in place. */
void trim_newline(char *s) {
    size_t n = strlen(s);
    if (n > 0 && s[n - 1] == '\n')
        s[n - 1] = '\0';
}

/* Prints msg + the errno string, then exits. Use for unrecoverable errors. */
void die(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}