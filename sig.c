#include "shell.h"
//TODO: fix bug when u give a typo in userdefined command and try doing sigkill after that it kinda bugs out
extern struct processes children[100];
extern int childSize;

void handleSig(char **args, int argSize)
{
    if (argSize != 3)
    {
        perror("too many args");
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
            int sig = atoi(args[2]);
            if (sig < 32 && sig > 0)
            {
                kill(children[pos].pid, sig);
            }
        }
        else
        {
            perror("couldnt find job, invalid job id");
        }
    }
}