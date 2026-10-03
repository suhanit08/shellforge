#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

typedef struct {
    char *args[64];
    int count;
} Command;

void parse_command(char *line, Command *cmd) {
    cmd->count = 0;

    char *token = strtok(line, " \t");

    while (token != NULL && cmd->count < 63) {
        cmd->args[cmd->count] = token;
        cmd->count++;

        token = strtok(NULL, " \t");
    }

    cmd->args[cmd->count] = NULL;
}

int main(void) {
    char *line = NULL;
    size_t len = 0;
    Command cmd;

    while (1) {
        printf("shellforge$ ");
        fflush(stdout);

        if (getline(&line, &len, stdin) == -1)
            break;

        line[strcspn(line, "\n")] = '\0';

        parse_command(line, &cmd);

        if (cmd.count == 0)
            continue;

        if (strcmp(cmd.args[0], "exit") == 0)
            break;

        /* Create child process */
        pid_t pid = fork();

        if (pid == 0) {
            /* Child process */

            execvp(cmd.args[0], cmd.args);

            /* Only reached if execvp fails */
            perror("Command execution error");
            exit(1);
        }
        else if (pid > 0) {
            /* Parent process waits for child */
            waitpid(pid, NULL, 0);
        }
        else {
            /* fork() failed */
            perror("Fork creation error");
        }
    }

    free(line);

    return 0;
}
