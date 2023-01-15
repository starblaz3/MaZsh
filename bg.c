#include "shell.h"

extern struct processes children[100];
extern int childSize;
//TODO: test with stopped and running jobs
void handleBG(char ** args, int argSize)
{
    if(argSize!=2)
    {
        perror("invalid args");
        return;
    }
    else
    {
        int jobId=atoi(args[1]);
        int pos=-1;
        for(int i=0;i<childSize;i++)
        {            
            if(children[i].jobId==jobId)
            {
                pos=i;
                break;
            }
        }
        if(pos==-1)
        {
            perror("invalid job number");
            return;
        }
        else
        {
            kill(children[pos].pid,SIGCONT);
        }
    }
}