#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

char **parse_line(char *command) {
    char *token = strtok(command, " ");
    char **array_of_arguments = NULL;
    __uint8_t number_of_arguments = 0;

    while(token != NULL) {
        ++number_of_arguments;

        /* increase memory for the array */
        array_of_arguments = (char **) realloc(array_of_arguments, number_of_arguments * sizeof(char *));
        if(array_of_arguments == NULL) {
            perror("Unable to allocate memory");
            free(token);
            exit(1);
        }

        /* allocate memory for the string token */
        array_of_arguments[number_of_arguments - 1] = (char *) malloc(strlen(token) * sizeof(char));
        if(array_of_arguments[number_of_arguments - 1] == NULL) {
            perror("Unable to allocate memory");
            free(token);
            exit(1);
        }

        /* copy string token into array of arguments */
        strcpy(array_of_arguments[number_of_arguments - 1], token);

        /* get next token */
        token = strtok(NULL, " ");
    }

    free(token);
    return array_of_arguments;
}

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

        char **command_arguments = parse_line(command);
        
        //do what you have to do with command arguments

        free(command_arguments);
        free(command);
    }
}

int main() {
    shell_loop();
    return 0;
}
