#include "shell.h"

void pinfo(char ** args, int argSize)
{
    pid_t pid;
    if(argSize>1)
    {
        pid=atoi(args[1]);        
    }
    else
    {
        pid=getpid();
    }
    char * pPath =(char *)malloc(4000);
    char * pstat =(char *)malloc(4000);    
    sprintf(pPath,"/proc/%d/stat",pid);
    FILE* pFD;
    if((pFD=fopen(pPath,"r"))==NULL)
    {
        perror("couldnt access process data:");
    }
    else
    {
        fread(pstat,1,4000,pFD);
        fclose(pFD);

        char ** pieces=(char **)malloc(100*sizeof(char *));
        for(int i=0;i<100;i++)
        {
            pieces[i]=(char *)malloc(4000);
            strcpy(pieces[i],"");
        }
        int pieceSize=0;
        char * token=(char*)malloc(4000);
        token=strtok(pstat," ");
        while(token!=NULL)
        {
            strcpy(pieces[pieceSize],token);
            pieceSize++;
            token=strtok(NULL," ");
        }
        printf("pid -- %d\n",pid);
        char * processStatus=(char*)malloc(4000);
        strcpy(processStatus,"");
        if(pid==getpid())
        {
            strcpy(processStatus,"+");
        }
        printf("Process Status -- %s%s\n",pieces[2],processStatus);
        printf("Memory -- %s\n",pieces[22]);

        sprintf(pPath,"/proc/%d/exe",pid);
        char * ePath=(char *)malloc(400);
        int count=0;
        if((count=readlink(pPath,ePath,350))==-1)
        {
            perror("couldnt find exe path");
        }
        else
        {
            ePath[count]='\0';
            printf("Executable path -- %s\n",ePath);
        }
        for(int i=0;i<100;i++)
            free(pieces[i]);
        free(pieces);
        free(token);
        free(processStatus);
        free(ePath);
    }
    free(pPath);
    free(pstat);
}