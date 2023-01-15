#include "shell.h"

void addHistory(char *command)
{
    //copying from file to buffer
    FILE *fptr;
    fptr=fopen("/tmp/history.txt","r");
    char * historyFile=(char *)malloc(10000);
    strcpy(historyFile,"");
    fseek(fptr,0,SEEK_END);
    int length=ftell(fptr);
    fseek(fptr,0,SEEK_SET);    
    fread(historyFile,length, 1,fptr);
    historyFile[length++]='\0';    
    fclose(fptr);

    //splitting the buffer into individual commands
    char **oldCommands = (char **)malloc(40 * sizeof(char *));
    for (int i = 0; i < 40; i++)
    {
        oldCommands[i] = (char *)malloc(4000);
        strcpy(oldCommands[i],"");
    }
    int oldCommandSize = 0;
    char *token = (char *)malloc(4000);
    token = strtok(historyFile, "\n");
    while (token != NULL)
    {
        strcpy(oldCommands[oldCommandSize], token);
        oldCommandSize++;
        token = strtok(NULL, "\n");        
    }    
    strcpy(oldCommands[oldCommandSize], command);
    oldCommandSize++;
    if (oldCommandSize > 20)
    {        
        int sizer = oldCommandSize - 20;
        for (int i = sizer, j = 0; i < oldCommandSize; i++, j++)
        {
            strcpy(oldCommands[j], oldCommands[i]);
        }
        oldCommandSize = 20;
    }        
    fptr=fopen("/tmp/history.txt","w");
    fseek(fptr,0,SEEK_SET);
    for(int i=0;i<oldCommandSize;i++)
    {
        char * tempc=(char *)malloc(4000);
        sprintf(tempc,"%s\n",oldCommands[i]);
        fwrite(tempc,1,strlen(tempc),fptr);        
        free(tempc);
    }
    fwrite("\0",1,1,fptr);
    fclose(fptr);  

    free(historyFile);
    for(int i=0;i<40;i++)
        free(oldCommands[i]);
    free(oldCommands);
    free(token);
}
void showHistory(int count)
{
    if (count > 20)
    {
        count = 20;
    }
    else if (count < 0)
    {
        count = 10;
    }
    FILE *fptr;
    fptr = fopen("/tmp/history.txt", "r");
    if (fptr != NULL)
    {
        char *historyFile = (char *)malloc(10000);
        strcpy(historyFile,"");
        fseek(fptr, 0, SEEK_END);
        int length = ftell(fptr);
        fseek(fptr, 0, SEEK_SET);
        fread(historyFile, 1, length, fptr);
        historyFile[length++]='\0';  
        fclose(fptr);   

        char **oldCommands = (char **)malloc(40 * sizeof(char *));
        for (int i = 0; i < 40; i++)
        {
            oldCommands[i] = (char *)malloc(4000);
            strcpy(oldCommands[i], "");
        }
        int oldCommandSize = 0;
        char *token = (char *)malloc(4000);
        token = strtok(historyFile, "\n");
        while (token != NULL)
        {
            strcpy(oldCommands[oldCommandSize], token);
            oldCommandSize++;
            token = strtok(NULL, "\n");
        }
        int sizer;
        if (count < oldCommandSize)
        {
            sizer = oldCommandSize - count;
        }
        else
        {
            sizer = 0;
        }
        for (int i = sizer; i < oldCommandSize; i++)
        {
            printf("%s\n", oldCommands[i]);
        }

        free(historyFile);
        for (int i = 0; i < 40; i++)
            free(oldCommands[i]);
        free(oldCommands);
        free(token);
    }
}