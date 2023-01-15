#include "shell.h"

char * changeDir(char *ogDir, char *cwd, char *newPathx, char *prevDir)
{ 
    char *newPath=(char *)malloc(4000);
    strcpy(newPath,newPathx);
    if (strlen(newPath) == 0)
    {
        int ret;
        ret = chdir(ogDir);
        if (ret < 0)
            perror("couldnt change into given dir:(");
    }
    else
    {        
        char **dirs = (char **)malloc(sizeof(char *) * 40); //dirs[40][400]
        for (int i = 0; i < 40; i++)
        {
            dirs[i] = (char *)malloc(400);
            strcpy(dirs[i], "");
        }
        char *finalDir = (char *)malloc(4000);        
        strcpy(finalDir, cwd);
        int noOfDirs = 0;
        if (newPath[0] == '/')
        {            
            strcpy(finalDir, newPath);
        }
        else if(newPath[0] == '~')
        {
            strcpy(finalDir, ogDir);
            strncat(finalDir,newPath+1,strlen(newPath)-1);
            finalDir[strlen(finalDir)]='\0';            
        }
        else if(newPath[0]=='-')
        {
            strcpy(finalDir, prevDir);
        }
        else
        {
            char *token = (char *)malloc(4000);
            token = strtok(newPath, "/");
            int counter = 0;
            while (token != NULL)
            {
                strcpy(dirs[counter], token);
                counter++;
                token = strtok(NULL, "/");
            }
            noOfDirs = counter;
            counter = 0;
            for (int i = 0; i < noOfDirs; i++)
            {                  
                if (strcmp(dirs[i], ".") == 0)
                    continue;
                else if (strcmp(dirs[i], "..") == 0)
                {
                    for (int j = strlen(finalDir) - 2; j >= 0; j--)
                    {
                        if (finalDir[j] == '/')
                        {
                            counter = j;
                            break;
                        }
                    }
                    if (counter == 0)
                        strcpy(finalDir, "/");
                    else
                        finalDir[counter] = '\0';
                    counter = 0;
                }                                
                else
                {                    
                    strcat(finalDir, "/");
                    strcat(finalDir, dirs[i]);
                }                
            }
            free(token); 
        }        
        int ret = chdir(finalDir);                       
        if (ret < 0)
        {
            perror("failed operation invalid dir entered");                    
        }
        for(int i=0;i<40;i++)
            free(dirs[i]);
        free(dirs);              
        return finalDir;
    }
    free(newPath);
}