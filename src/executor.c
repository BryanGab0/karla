#include "karla.h"

void setup_redirections(Command *cmd) {
    if (cmd->input_file) {
        int fd = open(cmd->input_file, O_RDONLY);
        if (fd < 0) {
            perror(cmd->input_file);
            exit(EXIT_FAILURE);
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }

    if (cmd->output_file) {
        int flags = O_WRONLY | O_CREAT | (cmd->append ? O_APPEND : O_TRUNC);
        int fd = open(cmd->output_file, flags, 0644);
        if (fd < 0) {
            perror(cmd->output_file);
            exit(EXIT_FAILURE);
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
}

int execute_command(Command *cmd) {
    if (is_builtin(cmd->argv[0]))
        return run_builtin(cmd);

    pid_t pid = fork();

    if(pid < 0) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {
        /* Child */
        signal(SIGINT, SIG_DFL);
        setup_redirections(cmd);
        execvp(cmd->argv[0], cmd->argv);

        fprintf(stderr, CLR_RED "karla: %s: %s\n" CLR_RESET, cmd->argv[0], strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* Parent */
    if (cmd->background) {
        printf("[bg] pid %d\n", pid);
    } else {
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
            fprintf(stderr, CLR_RED "[exited with code %d]\n" CLR_RESET, WEXITSTATUS(status));
    }

    return 0;
}