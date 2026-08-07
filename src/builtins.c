#include "karla.h"

static const char *BUILTINS[] = {
    "cd", "pwd", "help", "history", "echo", "export", NULL
};

int is_builtin(const char *name) {
    for (int i = 0; BUILTINS[i]; i++)
        if (strcmp(name, BUILTINS[i]) == 0)
            return 1;
    return 0;
}

static int builtin_cd(Command *cmd) {
    const char *dir = cmd->argc > 1 ? cmd->argv[1] : getenv("HOME");
    if (!dir) {
        fprintf(stderr, "cd: HOME not set\n");
        return 1;
    }
    if (chdir(dir) < 0) {
        perror(cmd->argv[1]);
        return 1;
    }
    return 0;
}

static int builtin_pwd(void) {
    char buf[1024];
    if (!getcwd(buf, sizeof(buf))) {
        perror("pwd");
        return 1;
    }
    printf("%s\n", buf);
    return 0;
}

static int builtin_help(void) {
    printf(CLR_CYAN
           "karla - built-in commands\n"
           "────────────────────────────────────────\n" CLR_RESET);
    printf("  cd [dir]          change directory (default: HOME)\n");
    printf("  pwd               print working directory\n");
    printf("  echo [args...]    print arguments\n");
    printf("  export VAR=VALUE  set environment variable\n");
    printf("  history           show command history\n");
    printf("  help              show this message\n");
    printf("  exit              quit karla\n");
    printf("\n");
    printf("Redirection: cmd < in > out >> append\n");
    printf("Background:  cmd &\n");
    return 0;
}

static int builtin_echo(Command *cmd) {
    for (int i = 1; i < cmd->argc; i++) {
        printf("%s", cmd->argv[i]);
        if (i + 1 < cmd->argc)
            printf(" ");
    }
    printf("\n");
    return 0;
}

static int builtin_export(Command *cmd) {
    if (cmd->argc < 2) {
        fprintf(stderr, "export: usage: export VAR=VALUE\n");
        return 1;
    }
    char *eq = strchr(cmd->argv[1], '=');
    if (!eq) {
        fprintf(stderr, "export: expected VAR=VALUE format\n");
        return 1;
    }
    *eq = '\0';
    if (setenv(cmd->argv[1], eq + 1, 1) < 0) {
        perror("setenv");
        *eq = '=';
        return 1;
    }
    *eq = '=';
    return 0;
}

int run_builtin(Command *cmd) {
    if (strcmp(cmd->argv[0], "cd")      == 0) return builtin_cd(cmd);
    if (strcmp(cmd->argv[0], "pwd")     == 0) return builtin_pwd();
    if (strcmp(cmd->argv[0], "help")    == 0) return builtin_help();
    if (strcmp(cmd->argv[0], "echo")    == 0) return builtin_echo(cmd);
    if (strcmp(cmd->argv[0], "export")  == 0) return builtin_export(cmd);
    if (strcmp(cmd->argv[0], "history") == 0) {
        history_print();
        return 0;
    }
    return 0;
}