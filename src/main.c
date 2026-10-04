#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    char *line = NULL;
    size_t len = 0;
    char *args[64];

    while (1) {
        /* Display shell prompt */
        printf("shellforge$ ");
        fflush(stdout);

        /* Ctrl + D */
        if (getline(&line, &len, stdin) == -1)
            break;

        /* Remove newline */
        line[strcspn(line, "\n")] = '\0';

        int i = 0;

        /* Tokenize input */
        char *token = strtok(line, " \t");

        while (token != NULL && i < 63) {
            args[i++] = token;
            token = strtok(NULL, " \t");
        }

        /* NULL terminate argument list */
        args[i] = NULL;

        /* Empty command */
        if (i == 0)
            continue;

        /* Exit shell */
        if (strcmp(args[0], "exit") == 0)
            break;

        /*
         * MILESTONE 5:
         * Built-in cd command
         */
        if (strcmp(args[0], "cd") == 0) {

            /* Check whether path was provided */
            if (args[1] == NULL) {
                fprintf(stderr,
                        "shellforge: missing path parameter\n");
            }
            else {
                /* Change directory in the parent shell */
                if (chdir(args[1]) != 0) {
                    perror("Directory change failed");
                }
            }

            /*
             * Do not fork() for cd.
             * The parent shell itself must change directory.
             */
            continue;
        }

        /*
         * MILESTONE 4:
         * Create child process for external commands
         */
        pid_t pid = fork();

        if (pid == 0) {

            /* Child process */

            execvp(args[0], args);

            /* execvp failed */
            perror("Execution error");
            exit(1);
        }
        else if (pid > 0) {

            /* Parent process */

            waitpid(pid, NULL, 0);
        }
        else {

            /* fork failed */
            perror("Fork creation error");
        }
    }

    free(line);

    return 0;
}
