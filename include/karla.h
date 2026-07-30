#ifndef KARLA_H
#define KARLA_H

/*
 * karla.h - Shared declarations for the karla shell.
 *
 * Central header included by every translation unit: pulls in the system
 * headers, defines project-wide limits and the Command type, and declares
 * the public interface of each module.
 */
 
/* Must precede all includes: unlocks POSIX functions (strdup, sigaction,
 * setenv, ...) that the standard C headers hide under strict -std modes. */
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

#define MAX_INPUT     1024   /* max bytes per input line   */
#define MAX_ARGS      64     /* max tokens per command     */
#define MAX_HISTORY   50     /* history ring-buffer size   */
#define PROMPT        "karla$ "

/* ANSI colour escapes */
#define CLR_GREEN   "\033[1;32m"
#define CLR_CYAN    "\033[1;36m"
#define CLR_RED     "\033[1;31m"
#define CLR_RESET   "\033[0m"

/* A single parsed command line, produced by the parser and consumed
 * by the executor. Zero-initialised means "no redirection, foreground". */
typedef struct {
    char *argv[MAX_ARGS];   /* NULL-terminated argument vector */
    int   argc;
    char *input_file;       /* target of '<', or NULL          */
    char *output_file;      /* target of '>'/'>>', or NULL     */
    int   append;           /* 1 = '>>' (append), 0 = '>' (truncate) */
    int   background;       /* 1 = trailing '&'                */
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

#endif /* KARLA_H */