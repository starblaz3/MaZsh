#include "shell.h"

extern struct processes children[100];
extern int childSize;
extern int globalPid;
extern char globalCommand[4000];

void handleFG(char **args, int argSize)
{
    if (argSize != 2)
    {
        printf("invalid args\n");
    }
    else
    {
        int pos = -1;
        for (int i = 0; i < childSize; i++)
        {
            if (children[i].jobId == atoi(args[1]))
            {
                pos = i;
                break;
            }
        }
        if (pos != -1)
        {
            int status;            
            int tempPid = children[pos].pid;  
            char * temper=(char *)malloc(4000)                    ;
            strcpy(temper,children[pos].commands);
            signal(SIGTTOU, SIG_IGN);
            signal(SIGTTIN, SIG_IGN);            
            tcsetpgrp(STDIN_FILENO, getpgid(children[pos].pid));                               
            kill(children[pos].pid, SIGCONT);            
            globalPid = children[pos].pid;
            strcpy(globalCommand,children[pos].commands);              
            removeProcess(children[pos].pid);
            if(waitpid(tempPid, &status, WUNTRACED)>0)      
            {
                if(WIFSTOPPED(status))
                {
                    addProcess(temper,tempPid);
                }
            }
            tcsetpgrp(STDIN_FILENO, getpgid(0));
            signal(SIGTTOU, SIG_DFL);
            signal(SIGTTIN, SIG_DFL);            
        }        
    }
}