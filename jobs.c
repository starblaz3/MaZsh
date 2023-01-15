#include "shell.h"

extern char globalPrint[4000];
extern struct processes children[100];
extern int childSize;

void printJobs(char **args, int argSize)
{
    //finding the flags
    char *flags = (char *)malloc(100);
    strcpy(flags, "");
    for (int i = 0; i < argSize; i++)
    {
        if (args[i][0] == '-')
        {
            for (int j = 1; j < strlen(args[i]); j++)
            {
                if (args[i][j] == 'r' || args[i][j] == 's')
                {
                    int already = 0;
                    for (int k = 0; k < strlen(flags); k++)
                    {
                        if (flags[k] == args[i][j])
                        {
                            already = 1;
                            break;
                        }
                    }
                    if (already == 0)
                    {
                        strncat(flags, &args[i][j],1);
                    }
                }
            }
        }
    }
    if (strlen(flags) == 0)
    {
        strcpy(flags, "rs");
    }    
    //using the flags to set printlist
    struct processList
    {
        int jobId;
        char processState[200];
        char inputCommand[4000];
        int pid;
    };
    struct processList processInfo[100];
    int processSize = 0;
    for (int i = 0; i < childSize; i++)
    {
        char *fpath = (char *)malloc(4000);
        char *fstat = (char *)malloc(4000);
        sprintf(fpath, "/proc/%d/stat", children[i].pid);
        strcpy(fstat,"");
        FILE *fptr=fopen(fpath, "r");
        if (fptr == NULL)
        {
            perror("couldnt access process data");
            continue;
        }        
        fread(fstat, 1, 1000, fptr);                     
        fclose(fptr);
        int pieceSize = 0;
        char **pieces = (char **)malloc(sizeof(char *) * 100);
        for (int j = 0; j < 100; j++)
        {
            pieces[j] = (char *)malloc(4000);
            strcpy(pieces[j], "");
        }
        char *token = (char *)malloc(4000);        
        token = strtok(fstat, " ");
        while (token != NULL)
        {
            strcpy(pieces[pieceSize], token);
            pieceSize++;
            token = strtok(NULL, " ");
        }                
        if (strcmp(pieces[2], "S") == 0)
        {
            strcpy(processInfo[processSize].processState, "r");
            processInfo[processSize].pid = children[i].pid;
            strcpy(processInfo[processSize].inputCommand, children[i].commands);
            processInfo[processSize].jobId =children[i].jobId;
            processSize++;            
        }
        else if (strcmp(pieces[2], "T") == 0)
        {
            strcpy(processInfo[processSize].processState, "s");
            processInfo[processSize].pid = children[i].pid;
            strcpy(processInfo[processSize].inputCommand, children[i].commands);
            processInfo[processSize].jobId =children[i].jobId;
            processSize++;
        }

        //free statements
        for (int j = 0; j < 100; j++)
        {
            free(pieces[j]);
        }
        free(token);
        free(pieces);
        free(fpath);
        free(fstat);
    }
    //print the process list according to the flag and state
    struct processList printList[100];
    int printSize=0;
    for(int i=0;i<processSize;i++)
    {
        for(int j=0;j<strlen(flags);j++)
        {
            if(processInfo[i].processState[0]==flags[j])
            {
                strcpy(printList[printSize].inputCommand,processInfo[i].inputCommand);
                printList[printSize].jobId=processInfo[i].jobId;
                printList[printSize].pid=processInfo[i].pid;
                strcpy(printList[printSize].processState,processInfo[i].processState);
                printSize++;
            }
        }
    }
    for(int i=0;i<printSize;i++)
    {
        char * output=(char *)malloc(4000);
        char * temp=(char *)malloc(4000);
        if(strcmp(printList[i].processState,"r")==0)        
            strcpy(temp,"Running");        
        else if(strcmp(printList[i].processState,"s")==0)        
            strcpy(temp,"Stopped");    
        sprintf(output,"[%d] %s %s [%d]",printList[i].jobId,temp,printList[i].inputCommand,printList[i].pid);
        printf("%s\n",output);

        free(temp);
        free(output);
    }
    //free statements
    free(flags);
}