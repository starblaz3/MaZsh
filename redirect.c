#include "shell.h"

void redirector(char ** args,int argSize, char *relativeCwd, char *useless, char *ogDir, char *cwd, char *prevDir, int *breakwhile, char *printPrompt)
{    
    int inTemp,outTemp,isOut=-1,isIn=-1;
    inTemp= dup(0);
    outTemp= dup(1);
    int iFd,oFd;
    if(inTemp==-1 || outTemp==-1)
    {
        perror("couldnt dup tty stream");
        return;
    }
    for(int i=0;i<argSize;i++)
    {
        if(strcmp(args[i],">")==0)
        {
            isOut=1;
            //TODO: check if cat testin1 > testing2 testing3 should work
            oFd=open(args[i+1],O_WRONLY | O_CREAT | O_TRUNC , 0644 );
            if(oFd==-1)
            {
                perror("couldnt open the given file");
                return;
            }
            if(dup2(oFd,1)==-1)
            {
                perror("couldnt redirect ");
                return;
            }
            args[i]=NULL;
        }
        else if(strcmp(args[i],">>")==0)
        {
            isOut=1;
            oFd=open(args[i+1],O_WRONLY | O_CREAT | O_APPEND, 0644);
            if(oFd==-1)
            {
                perror("couldnt open the given file");
                return;
            }
            if(dup2(oFd,1)==-1)
            {
                perror("couldnt redirect");
                return;
            }
            args[i]=NULL;
        }
        else if(strcmp(args[i],"<")==0)
        {
            isIn=1;
            iFd=open(args[i+1],O_RDONLY);
            if(iFd==-1)
            {
                perror("couldnt open given file");
                return;
            }
            if(dup2(iFd,0)==-1)
            {
                perror("couldnt redirect");
                return;
            }
            args[i]=NULL;
        }        
    }
    for(int i=0;i<argSize;i++)
    {
        if(args[i]==NULL)
        {
            argSize=i;
            break;
        }
    }
    runCommand(args,argSize, relativeCwd, useless, ogDir, cwd, prevDir, breakwhile, printPrompt);    
    if(isIn==1)
    {
        close(iFd);
        if(dup2(inTemp,0)==-1)
        {
            perror("couldnt change input stream");
            return;
        }
    }
    if(isOut==1)
    {
        close(oFd);
        if(dup2(outTemp,1)==-1)
        {
            perror("couldnt change output stream back ");
            return;
        }
    }
}