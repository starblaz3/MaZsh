#include "shell.h"

void printRepeat(char **args, int argSize, int times, char *relativeCwd, char *useless, int *breakwhile, char *cwd, char *ogDir, char *prevDir, char *ogCommand, char *printPrompt)
{

    for (int i = 0; i < times; i++)
    {
        handlePipe(args,argSize, relativeCwd, useless, ogDir, cwd, prevDir, breakwhile, printPrompt);        
    }
}