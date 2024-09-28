#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

char *get_user_input(void) {
    char *buffer;
    size_t bufsize = 32;
    size_t characters;

    /* allocate memory for the string command */
    buffer = (char *)malloc(bufsize * sizeof(char));
    if( buffer == NULL) {
        perror("Unable to allocate buffer");
        exit(1);
    }

    /* Get the command input from the user */
    printf("Type something: ");
    characters = getline(&buffer, &bufsize, stdin);

    /* Remove the newline character */
    if (buffer[characters - 1] == '\n') {
        buffer[characters - 1] = '\0';
    }

    return buffer;
}

void shell_loop(void) {
    while(TRUE) {
        char *command = get_user_input();

        if(strcmp(command, "exit") == 0) {
            return;
        }

        printf("You typed: %s\n", command);
        free(command);
    }
}

int main() {
    shell_loop();
    return 0;
}
