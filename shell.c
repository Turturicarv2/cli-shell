#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

int main() {
    shell_loop();
    return 0;
}

void shell_loop(void) {
    while(TRUE) {
        char *command = get_user_input();
        char **command_arguments = parse_line(command);

        /* command checking */
        if(strcmp(command_arguments[0], "exit") == SUCCESS) {
            return;
        } else if(strcmp(command_arguments[0], "cd") == SUCCESS) {
            change_directory(command_arguments);
        } else if(strcmp(command_arguments[0], "pwd") == SUCCESS) {
            print_current_directory();
        } else {
            printf("Command %s not found\n", command_arguments[0]);
        }

        /* memory deallocation */
        __uint8_t number_of_arguments = sizeof(command_arguments)/sizeof(char *);
        __uint8_t argument_index;

        /* firstly deallocate the strings */
        for(argument_index = 0; argument_index < number_of_arguments; ++argument_index) {
            free(command_arguments[argument_index]);
        }

        /* second, deallocate the pointers */
        free(command_arguments);
        free(command);
    }
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

    /* get the command input from the user */
    printf("Type something: ");
    characters = getline(&buffer, &bufsize, stdin);

    /* remove the newline character */
    if (buffer[characters - 1] == '\n') {
        buffer[characters - 1] = '\0';
    }

    return buffer;
}

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

void change_directory(char **command_arguments) {
    __uint8_t argument_length = MAX_STRING_LENGTH + strlen(command_arguments[1]);
    char string[MAX_STRING_LENGTH];
    char *relative_path = (char *) malloc(argument_length * sizeof(char));

    /* creating the relative path */
    strcpy(relative_path, getcwd(string, MAX_STRING_LENGTH));
    strcat(relative_path, "/");
    strcat(relative_path, command_arguments[1]);

    if(chdir(command_arguments[1]) == SUCCESS) { 
        /* case for absolute path */
        printf("New working directory: ");
        print_current_directory();
    } else if(chdir(relative_path) == SUCCESS) { 
        /* case for relative path */
        printf("New working directory: ");
        print_current_directory();
    } else {
        /* case for unrecognized path */
        printf("%s: No such file or directory\n", command_arguments[1]);
    }

    free(relative_path);
}

void print_current_directory() {
    char string[MAX_STRING_LENGTH];
    printf("%s\n", getcwd(string, MAX_STRING_LENGTH));
}


