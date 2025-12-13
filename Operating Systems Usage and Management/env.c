/*Implement a C program that processes environment variables as follows:
-displays all environment variables before processing;
-concatenates the values of consecutive environment variables two by two;
-creates or updates environment variables using the concatenated values;
-displays all environment variables after the concatenation process.
*/
#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 

int i; 
extern char **environ;

// Function for displaying environment variables
void print_environment(char **envp)
{
    printf("Environment variables:\n");
    for (i = 0; envp[i] != NULL; i++)
    {
        printf("%d %s\n", i, envp[i]);
    }
    printf("\n");
}

// Function for concatenating environment variable values
void concatenate_environment(char **envp)
{
    int pos, pos1, len1, len2;

    char *aux = NULL, *aux1 = NULL, *aux2 = NULL;

    if (envp[0] == NULL)
        return;

    i = 1;
    while (envp[i] != NULL && envp[i + 1] != NULL)
    {
        len1 = strlen(envp[i]);
        len2 = strlen(envp[i + 1]);

        // Allocate memory for the auxiliary buffer used for concatenation
        aux = (char *)malloc((len1 + 1) * sizeof(char));
        if (aux == NULL)
        {
            perror("malloc");
            exit(1);
        }

        // Determine the position of the first character after '=' in the second variable
        pos = strchr(envp[i + 1], '=') - envp[i + 1] + 1;

        // Copy the first variable into the auxiliary buffer
        memcpy(aux, envp[i], len1);

        // Reallocate memory to include the concatenated part of the second variable
        aux = realloc(aux, len1 + len2 - pos + 1);
        if (aux == NULL)
        {
            perror("realloc");
            exit(1);
        }

        // Copy the part of the second variable value to be concatenated
        memcpy(aux + len1, envp[i + 1] + pos, len2 - pos + 1);

        // Determine the position of '=' in the newly concatenated string
        pos1 = strchr(aux, '=') - aux;

        // Allocate memory to separate the new variable into key and value
        aux1 = (char *)malloc((pos1 + 1) * sizeof(char));
        if (aux1 == NULL)
        {
            perror("malloc");
            exit(1);
        }

        aux2 = (char *)malloc((len1 + len2 - pos + 1 - pos1) * sizeof(char));
        if (aux2 == NULL)
        {
            perror("malloc");
            exit(1);
        }

        // Copy the key into aux1
        memcpy(aux1, aux, pos1);
        aux1[pos1] = '\0';

        // Copy the value into aux2
        memcpy(aux2, aux + pos1 + 1, strlen(aux) - pos1);

        // Set a new environment variable using the computed key and value
        setenv(aux1, aux2, 1);

        // Free allocated memory
        free(aux);
        free(aux1);
        free(aux2);

        aux = NULL;
        aux1 = NULL;
        aux2 = NULL;

        i += 2;
    }
}

int main(int argc, char *argv[])
{
    // Display environment variables before concatenation
    printf("Environment variables before concatenation:\n");
    print_environment(environ);

    // Concatenate environment variable values
    concatenate_environment(environ);

    // Display environment variables after concatenation
    printf("Environment variables after concatenation:\n");
    print_environment(environ);

    return 0;
}
