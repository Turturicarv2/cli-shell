/* INCLUDES */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/* MACROS */
#define TRUE 1
#define FALSE 0
#define SUCCESS 0
#define MAX_STRING_LENGTH 100

/* FUNCTION DECLARATIONS*/
void shell_loop(void);
char *get_user_input(void);
char **parse_line(char *command);
void change_directory(char **command_arguments);
void print_current_directory();
void list_directories(char **argument_list);
