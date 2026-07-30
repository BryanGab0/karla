#include "karla.h"

/*
 * executor.c - Runs commands via fork/exec, plus I/O redirection.
 */
 
/* Applies the redirections recorded in cmd by pointing stdin/stdout at the
 * requested files. Called in the child, so exit() on error is safe. */
void setup_redirections(Command *cmd) {
    if (cmd->input_file) {
        int fd = open(cmd->input_file, O_RDONLY);
        if (fd < 0) {
            perror(cmd->input_file);
            exit(EXIT_FAILURE);
        }
        dup2(fd, STDIN_FILENO);     /* fd 0 now reads from the file */
        close(fd);
    }

    if (cmd->output_file) {
        int flags = O_WRONLY | O_CREAT | (cmd->append ? O_APPEND : O_TRUNC);
        int fd = open(cmd->output_file, flags, 0644);
        if (fd < 0) {
            perror(cmd->output_file);
            exit(EXIT_FAILURE);
        }
        dup2(fd, STDOUT_FILENO);    /* fd 1 now writes to the file */
        close(fd);
    }
}

int execute_command(Command *cmd) {
    /* Builtins run in the shell itself (no fork). */
    if (is_builtin(cmd->argv[0]))
        return run_builtin(cmd);

    pid_t pid = fork();

    if(pid < 0) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {
        /* Child */
        signal(SIGINT, SIG_DFL);    /* undo the shell's SIGINT ignore */
        setup_redirections(cmd);
        execvp(cmd->argv[0], cmd->argv);

        /* Only reached if execvp failed (e.g. command not found). */
        fprintf(stderr, CLR_RED "karla: %s: %s\n" CLR_RESET, cmd->argv[0], strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* Parent */
    if (cmd->background) {
        printf("[bg] pid %d\n", pid);   /* don't wait; reaped by SIGCHLD */
    } else {
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
            fprintf(stderr, CLR_RED "[exited with code %d]\n" CLR_RESET, WEXITSTATUS(status));
    }

    return 0;
}