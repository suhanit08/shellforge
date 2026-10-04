#define _GNU_SOURCE

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
        /* Display shell prompt */
        printf("shellforge$ ");
        fflush(stdout);

        /* Ctrl + D exits the shell */
        if (getline(&line, &len, stdin) == -1) {
            break;
        }

        /* Remove newline character */
        line[strcspn(line, "\n")] = '\0';

        /* Parse the command */
        parse_command(line, &cmd);

        /* If no command was entered */
        if (cmd.count == 0) {
            continue;
        }

        /* Exit command */
        if (strcmp(cmd.args[0], "exit") == 0) {
            break;
        }

        /* Create child process */
        pid_t pid = fork();

        if (pid == 0) {
            /* CHILD PROCESS */

            /*
             * Execute the command.
             *
             * Example:
             * ls -l
             *
             * cmd.args[0] = "ls"
             * cmd.args[1] = "-l"
             * cmd.args[2] = NULL
             */
            execvp(cmd.args[0], cmd.args);

            /* Reached only if execvp() fails */
            perror("Command execution error");
            exit(1);
        }
        else if (pid > 0) {
            /* PARENT PROCESS */

            /* Wait for child to finish */
            waitpid(pid, NULL, 0);
        }
        else {
            /* fork() failed */
            perror("Fork creation error");
        }
    }

    /* Free dynamically allocated input buffer */
    free(line);

    return 0;
}
         
