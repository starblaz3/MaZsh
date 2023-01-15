#include "shell.h"

void handlePipe(char **args, int argSize, char *relativeCwd, char *useless, char *ogDir, char *cwd, char *prevDir, int *breakwhile, char *printPrompt)
{
    int pipeCounter = 0;

    for (int i = 0; i < argSize; i++)
    {
        if (strcmp(args[i], "|") == 0)
        {
            pipeCounter++;
        }
    }
    if (pipeCounter == 0)
    {
        redirector(args, argSize, relativeCwd, useless, ogDir, cwd, prevDir, breakwhile, printPrompt);
    }
    else
    {
        int counterArg = 0;         
        int fds[2 * pipeCounter + 1];
        for (int i = 0; i < pipeCounter; i++)
        {
            if (pipe(fds + 2 * i) == -1)
            {
                perror("couldnt make virtual files");
            }
        }
        int pid;
        for (int i = 0; i < pipeCounter + 1; i++)
        {
            char **tempArgs = (char **)malloc(sizeof(char *) * 100);
            int tempArgSize = 0;
            for (int j = 0; j < 100; j++)
            {
                tempArgs[j] = (char *)malloc(4000);
            }
            for (int j = counterArg; j < argSize; j++)
            {
                if (strcmp(args[j], "|") == 0)
                {
                    counterArg = j + 1;
                    break;
                }
                strcpy(tempArgs[tempArgSize], args[j]);
                tempArgSize++;
                counterArg++;
            }
            pid=fork();
            if(pid==0)
            {
                if(i<pipeCounter)
                {
                    dup2(fds[i*2+1],STDOUT_FILENO);
                }
                if(i>0)
                {
                    dup2(fds[i*2-2],STDIN_FILENO);
                }
                for (int j=0;j<2*pipeCounter;j++)
                {
                    close(fds[j]);
                }
                redirector(tempArgs, tempArgSize, relativeCwd, useless, ogDir, cwd, prevDir, breakwhile, printPrompt);
                exit(0);
            }            
            for (int j = 0; j < 100; j++)
            {
                free(tempArgs[j]);
            }
            free(tempArgs);
        }   
        for(int i=0;i<2*pipeCounter;i++)
        {
            close(fds[i]);
        }     
        for(int i=0; i< pipeCounter+1; i++)
        {
            wait(NULL);
        }
    }
}
