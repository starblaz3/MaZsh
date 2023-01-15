#include "shell.h"

void print_ls(char **args, int argSize, char *cwd, char *printCwd, char *ogDir)
{
    //set the flags and find directories by adding directories to a array which doesnt start with -
    char *flags = (char *)malloc(40);
    strcpy(flags,"");
    char **dirs = (char **)malloc(sizeof(char *) * 100); //dirs[100][4000]
    for (int i = 0; i < 100; i++)
    {
        dirs[i] = (char *)malloc(4000);
        strcpy(dirs[i], "");
    }
    char **inputDir = (char **)malloc(sizeof(char *) * 100); //inputDir[100][4000]
    for (int i = 0; i < 100; i++)
    {
        inputDir[i] = (char *)malloc(4000);
        strcpy(inputDir[i], "");
    }
    int dirCounter = 0, flagCounter = 0;
    for (int i = 1; i < argSize; i++)
    {
        if (args[i][0] == '-')
        {
            for (int j = 1; j < strlen(args[i]); j++)
            {
                flags[flagCounter] = args[i][j];
                flagCounter++;
            }
        }
        else
        {
            //converting all dirs in args to absolute
            char *tempDir = (char *)malloc(4000);
            char *finalDir = (char *)malloc(4000);
            strcpy(tempDir, "");
            strcpy(finalDir, "");
            char **pieces = (char **)malloc(sizeof(char *) * 100); //pieces[100][4000]
            for (int j = 0; j < 100; j++)
            {
                pieces[j] = (char *)malloc(4000);
                strcpy(pieces[j], "");
            }
            int pieceSize = 0;
            char *token = (char *)malloc(4000);
            strcpy(finalDir, cwd);
            strcpy(inputDir[dirCounter], args[i]);
            if (args[i][0] == '/')
            {
                strcpy(finalDir, args[i]);
            }
            else if (args[i][0] == '~')
            {
                strcpy(finalDir, ogDir);
                strncat(finalDir, args[i] + 1, strlen(args[i]) - 1);
                strcat(finalDir, "\0");
            }
            else
            {
                char * tempTok=(char *)malloc(4000);
                strcpy(tempTok,args[i]);
                token = strtok(tempTok, "/");
                while (token != NULL)
                {
                    strcpy(pieces[pieceSize], token);
                    pieceSize++;
                    token = strtok(NULL, "/");
                }
                for (int j = 0; j < pieceSize; j++)
                {
                    if (strcmp(pieces[j], ".") == 0)
                    {
                        continue;
                    }
                    else if (strcmp(pieces[j], "..") == 0)
                    {
                        for (int k = strlen(finalDir) - 2; k >= 0; k--)
                        {
                            if (finalDir[k] == '/')
                            {
                                if (k == 0)
                                {
                                    strcpy(finalDir, "/");
                                }
                                else
                                {
                                    finalDir[k] = '\0';
                                }
                                break;
                            }
                        }
                    }
                    else
                    {
                        strcat(finalDir, "/");
                        strcat(finalDir, pieces[j]);
                    }
                }
                free(tempTok);
            }            
            strcpy(dirs[dirCounter], finalDir);
            dirCounter++;
            free(tempDir);
            free(finalDir);
            for (int i = 0; i < 100; i++)
                free(pieces[i]);
            free(pieces);
            free(token);
        }
    }
    if (strlen(dirs[0]) == 0)
    {
        dirCounter = 1;
        strcpy(dirs[0], cwd);
        strcpy(inputDir[0], printCwd);
    }
    // first find all the files and dirs for one directory and then remove all files and dirs starting with . and then in for loop of flags for one dir check if flag matches
    
    for (int x = 0; x < dirCounter; x++)
    {
        printf("%s: \n", inputDir[x]);
        char **ogDirList = (char **)malloc(sizeof(char *) * 500); //ogDirList[500][4000]
        for (int i = 0; i < 500; i++)
        {
            ogDirList[i] = (char *)malloc(4000);
            strcpy(ogDirList[i], "");            
        }
        char **printList = (char **)malloc(sizeof(char *) * 500); //printList[500][4000]
        for (int i = 0; i < 500; i++)
        {
            printList[i] = (char *)malloc(4000);
            strcpy(printList[i], "");
        }
        char **outputStatement = (char **)malloc(sizeof(char *) * 500); //outputStatement[500][4000]
        for (int i = 0; i < 500; i++)
        {
            outputStatement[i] = (char *)malloc(4000);
            strcpy(outputStatement[i], "");
        }
        char *flagList = (char *)malloc(20);
        strcpy(flagList, "al");
        int sizePrintList = 0, sizeOgDirList = 0, sizeOutputStatement = 0;
        DIR *directory;
        struct dirent *dirEntry;
        directory = opendir(dirs[x]);
        if (directory)
        {
            while ((dirEntry = readdir(directory)) != NULL)
            {
                strcpy(ogDirList[sizeOgDirList], dirEntry->d_name);                
                sizeOgDirList++;                
            }
            for (int i = 0; i < sizeOgDirList; i++)
            {
                if (ogDirList[i][0] != '.')
                {
                    strcpy(printList[sizePrintList], ogDirList[i]);
                    strcpy(outputStatement[sizeOutputStatement], ogDirList[i]);
                    sizeOutputStatement++;
                    sizePrintList++;
                }
            }
            for (int i = 0; i < strlen(flagList); i++)
            {
                for (int j = 0; j < strlen(flags); j++)
                {
                    if (flags[j] == flagList[i] && flagList[i] == 'a')
                    {
                        for (int k = 0; k < sizeOgDirList; k++)
                        {
                            strcpy(printList[k], ogDirList[k]);
                            strcpy(outputStatement[k], ogDirList[k]);
                            sizePrintList = sizeOgDirList;
                            sizeOutputStatement = sizeOgDirList;
                        }
                    }
                    else if (flags[j] == flagList[i] && flagList[i] == 'l')
                    {
                        for (int k = 0; k < sizePrintList; k++)
                        {
                            char *output = (char *)malloc(4000);
                            output = getL(printList, k, dirs[x]);
                            strcpy(outputStatement[k + 1], output);                            
                            free(output);
                        }
                        sizeOutputStatement = sizePrintList + 1;
                        int hardNodes = 0;
                        char *temp = (char *)malloc(4000);
                        hardNodes = findHardNodes(printList, sizePrintList, dirs[x]);
                        sprintf(temp, "total %d", hardNodes);
                        strcpy(outputStatement[0], temp);
                        free(temp);
                    }
                }
            }
            for (int i = 0; i < sizeOutputStatement; i++)
            {
                printf(" %s\n", outputStatement[i]);
            }
        }
        closedir(directory);
        printf("\n");
        free(flagList);
    }
    free(flags);
    for (int i = 0; i < 100; i++)
        free(dirs[i]);
    free(dirs);
    for (int i = 0; i < 100; i++)
        free(inputDir[i]);
    free(inputDir);
}
int findHardNodes(char **printList, int size, char *dir)
{
    int count = 0;
    for (int i = 0; i < size; i++)
    {
        struct stat filestat;
        char *filepath = (char *)malloc(4000);
        strcpy(filepath, dir);
        strcat(filepath, "/");
        strcat(filepath, printList[i]);
        stat(filepath, &filestat);
        count += filestat.st_blocks;
        free(filepath);
    }
    return count / 2;
}
// TODO: need to check ls with l and 2 year old files
char *getL(char **printList, int k, char *dir)
{
    char *filepath = (char *)malloc(4000);
    char *output = (char *)malloc(4000);
    struct stat fileStat;
    strcpy(filepath, dir);
    strcpy(output, "");
    strcat(filepath, "/");
    strcat(filepath, printList[k]);
    //checking permissions
    if (stat(filepath, &fileStat) < 0)
    {
        // printf("file :%s\n",filepath);
        perror("error with a file in given directory");
    }
    else
    {
        if (S_ISDIR(fileStat.st_mode) != 0)
            strcat(output, "d");
        else
            strcat(output, "-");
        if (fileStat.st_mode & S_IRUSR)
            strcat(output, "r");
        else
            strcat(output, "-");
        if (fileStat.st_mode & S_IWUSR)
            strcat(output, "w");
        else
            strcat(output, "-");
        if (fileStat.st_mode & S_IXUSR)
            strcat(output, "x");
        else
            strcat(output, "-");
        if (fileStat.st_mode & S_IRGRP)
            strcat(output, "r");
        else
            strcat(output, "-");
        if (fileStat.st_mode & S_IWGRP)
            strcat(output, "w");
        else
            strcat(output, "-");
        if (fileStat.st_mode & S_IXGRP)
            strcat(output, "x");
        else
            strcat(output, "-");
        if (fileStat.st_mode & S_IROTH)
            strcat(output, "r");
        else
            strcat(output, "-");
        if (fileStat.st_mode & S_IWOTH)
            strcat(output, "w");
        else
            strcat(output, "-");
        if (fileStat.st_mode & S_IXOTH)
            strcat(output, "x");
        else
            strcat(output, "-");
    }
    strcat(output, "  ");

    // hard links
    char *number = (char *)malloc(20);
    sprintf(number, "%ld", fileStat.st_nlink);
    strcat(output, number);
    strcat(output, " ");

    //owner name and owner group
    struct passwd *pw = getpwuid(fileStat.st_uid);
    struct group *gr = getgrgid(fileStat.st_gid);
    strcat(output, pw->pw_name);
    strcat(output, "\t");
    strcat(output, gr->gr_name);
    strcat(output, "\t");
    //file size
    sprintf(number, "%ld", fileStat.st_size);
    strcat(output, number);
    strcat(output, " ");

    //extract date
    char *fileTime = (char *)malloc(4000);
    char *temp = (char *)malloc(4000);
    memset(temp,'\0',sizeof(temp));
    strcpy(fileTime, ctime(&fileStat.st_mtim.tv_sec));
    int counter = 0;
    for (int i = 4; i < 7; i++)
    {
        temp[counter] = fileTime[i];
        counter++;
    }
    temp[counter] = ' ';
    counter++;
    for (int i = 8; i < 10; i++)
    {
        temp[counter] = fileTime[i];
        counter++;
    }
    temp[counter] = ' ';
    counter++;
    // !!!!!!!!!!!            HAVE TO CHECK CURRENT YEAR AND THEN APPEND THIS FIELD TRY ON ~ IN ZSH     !!!!!!!!!!!!!!!!!!!!!!!!!!
    time_t currentTime;
    time(&currentTime);
    char *ctimeString = (char *)malloc(400);
    char *yearString = (char *)malloc(400);
    strcpy(ctimeString, ctime(&currentTime));
    int tempCounter = 0;
    int thisYear = 1;
    for (int i = 20; i < 24; i++)
    {
        if (ctimeString[i] != fileTime[i])
        {
            thisYear = 0;
            for (int j = 20; j < 24; j++)
            {
                temp[counter] = fileTime[j];
                counter++;
            }
            break;
        }
    }
    if (thisYear == 1)
    {
        for (int i = 11; i < 16; i++)
        {
            temp[counter] = fileTime[i];
            counter++;
        }
    }  
    temp[counter]='\0'  ;
    counter++;
    strcat(output, temp);
    strcat(output, " ");        
    strcat(output, printList[k]);

    free(filepath);
    free(fileTime);
    free(temp);
    free(ctimeString);
    free(yearString);

    return output;
}