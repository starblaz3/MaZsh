#include "shell.h"
void runCommand(char ** args,int argSize, char *relativeCwd, char *useless, char *ogDir, char *cwd, char *prevDir, int *breakwhile, char *printPrompt)
{    
    char *ogCommand = (char *)malloc(4000);    
    strcpy(ogCommand,"");
    for(int i=0;i<argSize;i++)
    {
        strcat(ogCommand,args[i]);
        strcat(ogCommand," ");
    }
    getcwd(relativeCwd, 4000);
    useless = print_path(ogDir, relativeCwd);
    strcpy(cwd, useless);   
    if (strcmp(args[0], "cd") == 0)
    {
        if (argSize > 2)
        {
            perror(" too many arguments");
            return;
        }
        else
        {
            if (argSize < 2)
            {
                char *tempChar = (char *)malloc(4000);
                tempChar = changeDir(ogDir, relativeCwd, ogDir, prevDir);
                strcpy(prevDir, relativeCwd);
                strcpy(relativeCwd, tempChar);
                free(tempChar);
            }
            else
            {
                char *tempChar = (char *)malloc(4000);
                tempChar = changeDir(ogDir, relativeCwd, args[1], prevDir);
                strcpy(prevDir, relativeCwd);
                strcpy(relativeCwd, tempChar);
                free(tempChar);
            }
        }
    }
    else if (strcmp(args[0], "echo") == 0)
    {
        printEcho(args, argSize);
    }
    else if (strcmp(args[0], "pwd") == 0)
    {
        printPwd();
    }
    else if (strcmp(args[0], "ls") == 0)
    {
        print_ls(args, argSize, relativeCwd, cwd, ogDir);
    }
    else if (strcmp(args[0], "exit") == 0)
    {
        *breakwhile = 1;
    }
    else if (strcmp(args[0], "pinfo") == 0)
    {
        pinfo(args, argSize);
    }
    else if (strcmp(args[0], "repeat") == 0)
    {
        if (argSize < 3)
        {
            printf("too less arguments\n");
        }
        else
        {
            char **tempArgs = (char **)malloc(100 * sizeof(char *));
            int tempArgsCounter = 0;
            for (int j = 0; j < 100; j++)
            {
                tempArgs[j] = (char *)malloc(4000);
                strcpy(tempArgs[j], "");
            }
            for (int j = 2; j < argSize; j++)
            {
                strcpy(tempArgs[tempArgsCounter], args[j]);
                tempArgsCounter++;
            }
            int times = atoi(args[1]);
            printRepeat(tempArgs, tempArgsCounter, times, relativeCwd,useless,breakwhile, cwd, ogDir, prevDir, ogCommand, printPrompt);
            for (int j = 0; j < 100; j++)
                free(tempArgs[j]);
            free(tempArgs);
        }
    }
    else if (strcmp(args[0], "history") == 0)
    {
        if (argSize > 1)
            showHistory(atoi(args[1]));
        else
        {
            showHistory(-1);
        }
    }
    else if (strcmp(args[0], "jobs") == 0)
    {
        printJobs(args, argSize);
    }
    else if (strcmp(args[0], "sig") == 0)
    {
        handleSig(args, argSize);
    }
    else if (strcmp(args[0], "bg") == 0)
    {
        handleBG(args, argSize);
    }
    else if (strcmp(args[0], "fg") == 0)
    {
        handleFG(args, argSize);
    }
    else if(strcmp(args[0],"replay")==0)
    {
        handleReplay(args,argSize, relativeCwd, useless, ogDir, cwd, prevDir, breakwhile, printPrompt);
    }
    else
    {           
        printExecutables(ogCommand, printPrompt);
    }
    free(ogCommand);
    
}