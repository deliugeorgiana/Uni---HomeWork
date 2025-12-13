/*Implement a shell with the following features:
-accepts the internal command exit;
-accepts external commands with an arbitrary number of arguments;
-accepts the execution of scripts; a script may contain a list of utility commands and may accept arguments from the command line.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 100
#define MAX_ARGUMENTS 10

void execute_command(char *args[]) 
{
    pid_t pid = fork(); 
    if (pid < 0) 
    {
        perror("Fork failed");
    }
else 
    if (pid == 0) 
    {
        if (execvp(args[0], args) == -1)
        {
            perror("The execution failed");
        }
        exit(EXIT_FAILURE);
    } 
else 
    {
        wait(NULL);
    }
}

void run_script(const char *filename, char *script_args[])
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) 
    {
        perror("The file cannot be opened\n");
        exit(EXIT_FAILURE);
    }

 char line[MAX_COMMAND_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) 
    {
        line[strcspn(line, "\n")] = 0;

        char *args[MAX_ARGUMENTS];
        char *token = strtok(line, " ");
        int arg_nr = 0;

        while (token != NULL && arg_nr <= MAX_ARGUMENTS - 1) 
        {
            args[arg_nr++] = token;
            token = strtok(NULL, " ");
        }

       
        for (int i = 0; script_args[i] != NULL && arg_nr <= MAX_ARGUMENTS - 1; i++) 
        {
            args[arg_nr++] = script_args[i];
        }

        args[arg_nr] = NULL;

        execute_command(args);
    }

    fclose(file);
}

int main() {
    char command[MAX_COMMAND_LENGTH];

    while (1) 
    {
        printf("Prompt> ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0;

        if (strcmp(command, "exit") == 0) 
        {
            printf("Exit from shell\n");
            break;
        } 
else
        if (strstr(command, ".sh") != NULL && strchr(command, ' ') == NULL)
        {
           
            char *script_args[] = { NULL }; 
            run_script(command, script_args);
        } 
else 
        {
            char *args[MAX_ARGUMENTS];
            char *token = strtok(command, " ");
            int arg_nr = 0;

            while (token != NULL && arg_nr < MAX_ARGUMENTS - 1) 
            {
                args[arg_nr++] = token;
                token = strtok(NULL, " ");
            }
            args[arg_nr] = NULL;

            execute_command(args);
        }
    }

    return 0;
}
