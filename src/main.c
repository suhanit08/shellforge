#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(void) {
char *line = NULL;
size_t len = 0;
ssize_t nread;
while (1) {
printf("shellforge$ ");
fflush(stdout); // Force prompt to print immediately without buffer delays
nread = getline(&line, &len, stdin);
if (nread == -1) { // Detects Ctrl-D (End of File)
printf("\nExiting cleanly...\n");
break;
}
// Strip the trailing newline character caused by pressing Enter
if (nread > 0 && line[nread - 1] == '\n') {
line[nread - 1] = '\0';
}
// Exit trap condition
if (strcmp(line, "exit") == 0) {
break;
}
if (strlen(line) > 0) {
printf("You typed: %s\n", line);
}
}
free(line); // Prevent memory leaks
return 0;
}
