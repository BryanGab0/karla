#include "karla.h"

void print_prompt(void) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd))) {
        char *base = strrchr(cwd, '/');
        base = base ? base + 1 : cwd;
        printf(CLR_GREEN "%s" CLR_RESET CLR_CYAN " $ " CLR_RESET, base);
    } else {
        printf(PROMPT);
    }
    fflush(stdout);
}

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

void trim_newline(char *s) {
    size_t n = strlen(s);
    if (n > 0 && s[n - 1] == '\n')
        s[n - 1] = '\0';
}

void die(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}