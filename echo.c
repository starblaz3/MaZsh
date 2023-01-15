#include "shell.h"

void printEcho(char **args, int argSize)
{
    for (int j = 1; j < argSize; j++)
    {
        printf("%s ", args[j]);
    }
    printf("\n");
}