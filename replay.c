#include "shell.h"

void handleReplay(char ** args,int argSize, char *relativeCwd, char *useless, char *ogDir, char *cwd, char *prevDir, int *breakwhile, char *printPrompt)
{
    char ** tempArgs=(char **)malloc(sizeof(char *)*100);
    int commandPos=-1,interval=-1,period=-1;
    int tempArgSize=0;
    for(int i=0;i<100;i++)
    {
        tempArgs[i]=(char *)malloc(4000);
    }
    for(int i=1;i<argSize;i++)
    {
        if(strcmp(args[i],"-command")==0)
        {
            commandPos=i;
            args[i]=NULL;
        }
        else if(strcmp(args[i],"-interval")==0)
        {            
            interval=atoi(args[i+1]);
            args[i]=NULL;
        }
        else if(strcmp(args[i],"-period")==0)
        {
            period=atoi(args[i+1]);
            args[i]=NULL;
        }
    }
    for(int i=commandPos+1;i<argSize;i++)
    {
        if(args[i]==NULL)
        {
            break;
        }
        strcpy(tempArgs[tempArgSize],args[i]);
        tempArgSize++;
    }
    int tempPeriod=period;
    while(period>interval || period==interval)
    {
        sleep(interval);
        handlePipe(tempArgs,tempArgSize, relativeCwd, useless, ogDir, cwd, prevDir, breakwhile, printPrompt);
        period=period-interval;
    }
    if(period>0 && (tempPeriod>interval || tempPeriod==interval))
    {
        sleep(period);
    }
    for(int i=0;i<100;i++)
    {
        free(tempArgs[i]);
    }
    free(tempArgs);
}