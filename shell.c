/* 
Shell program that is able to execute basic system commands
Requires to be run by an UNIX based Operating System
*/

#include "shell.h"


int main() {
    shell_loop();
    return 0;
}


/*
Main loop of the shell
*/
void shell_loop(void) {
    while(TRUE) {
        char *command = get_user_input();
        char **argument_list = parse_line(command);

        /* command checking */
        if(strcmp(argument_list[0], "exit") == SUCCESS) {
            return;
        } else if(strcmp(argument_list[0], "cd") == SUCCESS) {
            change_directory(argument_list);
        } else if(strcmp(argument_list[0], "pwd") == SUCCESS) {
            print_current_directory();
        } else if((strcmp(argument_list[0], "ls") == SUCCESS)
                || (strcmp(argument_list[0], "echo") == SUCCESS)){
            list_directories(argument_list);
        } else {
            printf("Command %s not found\n", argument_list[0]);
        }

        /* memory deallocation */
        __uint8_t number_of_arguments = sizeof(argument_list)/sizeof(char *);
        __uint8_t argument_index;

        /* firstly deallocate the strings */
        for(argument_index = 0; argument_index < number_of_arguments; ++argument_index) {
            free(argument_list[argument_index]);
        }

        /* second, deallocate the pointers */
        free(argument_list);
        free(command);
    }
}


/*
Function that gets input from the user
returns a string
*/
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
    printf("Type your command: ");
    characters = getline(&buffer, &bufsize, stdin);

    /* remove the newline character */
    if (buffer[characters - 1] == '\n') {
        buffer[characters - 1] = '\0';
    }

    return buffer;
}


/*
Function that parses the input from the user
Splits the command and its arguments 
Into different parts and stores them into an array

IN-argument is the command with arguments stored in a string
Returns the array of strings
*/
char **parse_line(char *command) {
    char *token = strtok(command, " ");
    char **argument_list = NULL;
    __uint8_t number_of_arguments = 0;

    while(token != NULL) {
        ++number_of_arguments;

        /* increase memory for the array */
        argument_list = (char **) realloc(argument_list, number_of_arguments * sizeof(char *));
        if(argument_list == NULL) {
            perror("Unable to allocate memory");
            free(token);
            exit(1);
        }

        /* allocate memory for the string token */
        argument_list[number_of_arguments - 1] = (char *) malloc(strlen(token) * sizeof(char));
        if(argument_list[number_of_arguments - 1] == NULL) {
            perror("Unable to allocate memory");
            free(token);
            exit(1);
        }

        /* copy string token into array of arguments */
        strcpy(argument_list[number_of_arguments - 1], token);

        /* get next token */
        token = strtok(NULL, " ");
    }

    ++number_of_arguments;

    /* increase memory for the array to store last NULL value */
    argument_list = (char **) realloc(argument_list, number_of_arguments * sizeof(char *));
    if(argument_list == NULL) {
        perror("Unable to allocate memory");
        free(token);
        exit(1);
    }

    /* last value needs to be null for execvp system command */
    argument_list[number_of_arguments - 1] = NULL; 

    free(token);
    return argument_list;
}


/*
Function that handles the cd command; will change working directory
takes as IN-argument the command and its arguments stored in a vector of strings
*/
void change_directory(char **argument_list) {
    __uint8_t argument_length = MAX_STRING_LENGTH + strlen(argument_list[1]);
    char string[MAX_STRING_LENGTH];
    char *relative_path = (char *) malloc(argument_length * sizeof(char));

    /* creating the relative path */
    strcpy(relative_path, getcwd(string, MAX_STRING_LENGTH));
    strcat(relative_path, "/");
    strcat(relative_path, argument_list[1]);

    if(chdir(argument_list[1]) == SUCCESS) { 
        /* case for absolute path */
        printf("New working directory: ");
        print_current_directory();
    } else if(chdir(relative_path) == SUCCESS) { 
        /* case for relative path */
        printf("New working directory: ");
        print_current_directory();
    } else {
        /* case for unrecognized path */
        printf("%s: No such file or directory\n", argument_list[1]);
    }

    free(relative_path);
}


/*
Function that handles pwd command, printing the currect working directory
*/
void print_current_directory() {
    char string[MAX_STRING_LENGTH];
    printf("%s\n", getcwd(string, MAX_STRING_LENGTH));
}


/*
Function that handles echo and ls commands
takes as IN-argument the command and its arguments stored in a vector of strings
*/
void list_directories(char **argument_list) {
    int return_value_fork = fork();
    if(return_value_fork == 0) {
        /* child process... call system function */
        int status_code = execvp(argument_list[0], argument_list);
        
        if (status_code == -1) {
            perror("System command failed\n");
            exit(1);
        }
    } else if(return_value_fork > 0) {
        /* parent process... continue c program */
        wait(NULL);
    } else {
        /* fork failed... abort mission */
        perror("Fork failed\n");
        exit(1);
    }
}

