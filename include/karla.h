#ifndef KARLA_H
#define KARLA_H

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>

#define MAX_INPUT     1024
#define MAX_ARGS      64
#define MAX_HISTORY   50
#define PROMPT        "karla$ "

#define CLR_GREEN   "\033[1;32m"
#define CLR_CYAN    "\033[1;36m"
#define CLR_RED     "\033[1;31m"
#define CLR_RESET   "\033[0m"

typedef struct {
    char *argv[MAX_ARGS];
    int   argc;
    char *input_file;
    char *output_file;
    int   append;
    int   background;
} Command;

/* parser.c */
int parse_input(char *line, Command *cmd);

/* executor.c */
int  execute_command(Command *cmd);
void setup_redirections(Command *cmd);

/* builtins.c */
int is_builtin(const char *name);
int run_builtin(Command *cmd);

/* history.c */
void history_add(const char *line);
void history_print(void);
int  history_count(void);

/* utils.c */
void  print_prompt(void);
char *read_line(void);
void  trim_newline(char *s);
void  die(const char *msg);

#endif