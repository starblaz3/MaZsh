#include "shell.h"
//TODO: make the while loop more modular
extern int childSize;
extern int globalPid;
extern char globalCommand[4000];
char globalPrint[4000];
char *tokenizer(char *input)
{
    char *finalInput = (char *)malloc(4000);
    strcpy(finalInput, "");
    char *temp = (char *)malloc(4000);
    strcpy(temp, input);
    char *token=(char *)malloc(4000);
    token = strtok(temp, " \t\n");
    while (token != NULL)
    {
        char *tempc = (char *)malloc(4000);
        sprintf(tempc, "%s ", token);
        strcat(finalInput, tempc);
        token = strtok(NULL, " \t\n");
        free(tempc);
    }
    free(temp);
    free(token);
    return finalInput;
}
void sigCtrlZ(int sig)
{    
    printf("in ctrlz\n");
    // kill the process only if a fg process exists       
    if (globalPid != -1)
    {                
        printf("nani\n");
        if (kill(globalPid, SIGTSTP) == -1)
        {
            perror("couldnt send signal to process");
        }
        // setpgid(globalPid,globalPid);
        //since the fg process is sent to bg , need to add to bg list of jobs
        addProcess(globalCommand, globalPid);
        //set the sigchild termination handler for this bg job
        struct sigaction sa;

        sa.sa_handler = &sigHandler;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESTART;
        if (sigaction(SIGCHLD, &sa, NULL) == -1)
        {
            perror("couldnt signal the process");
        }
        // make this -1 because fg process dosent exist anymore
        globalPid = -1;
    }
}
void sigCtrlC(int sig)
{
    // TODO : need to test a few cases where ctrlc fails
    // kill the process only if a fg process exists 
    if (globalPid != -1)
    {
        if (kill(globalPid, SIGINT) == -1)
        {
            perror("couldnt send sigint to process");
        }
        else
        {
            globalPid = -1;
        }
        printf("\n");
        fflush(stdout);
    }
    else
    {
        //just print command prompt and print new line everytime ctrlc is pressed
        printf("\n%s", globalPrint);
        fflush(stdout);
    }
}
char *print_path(char *ogDir, char *path)
{
    int ogLength = strlen(ogDir);
    int pathLength = strlen(path);
    char *finalString = (char *)malloc(4000);
    int i = 0;
    for (i = 0; i < ogLength; i++)
    {
        if (ogDir[i] != path[i])
        {
            return path;
        }
    }
    if (pathLength > ogLength)
    {
        if (path[i] == '/')
        {
            int j = 0;
            finalString[j] = '~';
            j++;
            for (; i < pathLength; i++, j++)
            {
                finalString[j] = path[i];
            }
            finalString[j] = '\0';
        }
        else
        {
            return path;
        }
    }
    else
    {
        strcpy(finalString, "~");
    }
    return finalString;
}
int main()
{
    struct sigaction sa;
    sa.sa_handler = &sigCtrlZ;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGTSTP, &sa, NULL) == -1)
    {
        perror("coldnt handle sigtstp signal");
    }

    struct sigaction saCtrlC;
    saCtrlC.sa_handler = &sigCtrlC;
    sigemptyset(&saCtrlC.sa_mask);
    saCtrlC.sa_flags = SA_RESTART;
    if (sigaction(SIGINT, &saCtrlC, NULL) == -1)
    {
        perror("coldnt handle sigINT signal");
    }

    char *ogDir = (char *)malloc(4000);
    char *prevDir = (char *)malloc(4000);
    strcpy(ogDir, "");
    strcpy(prevDir, "");
    FILE *fptr;
    fptr = fopen("/tmp/history.txt", "r+");
    if (fptr == NULL)
        fptr = fopen("/tmp/history.txt", "w+");
    fclose(fptr);

    int counter = 0, breakwhile = 0;
    if (!getcwd(ogDir, 4000))
    {
        perror("couldnt get current dir");
        return 0;
    }
    strcpy(prevDir, ogDir);
    while (1)
    {
        struct passwd *pwd;
        struct utsname sys_name;
        uid_t uid;
        size_t len = 0;
        char *printPrompt = (char *)malloc(4000);
        uid = geteuid();
        pwd = getpwuid(uid);
        if (!pwd)
        {
            perror("couldnt get user");
            return 0;
        }
        if (uname(&sys_name) < 0)
        {
            perror("couldnt get system name");
            return 0;
        }
        char *cwd = (char *)malloc(4000);
        char *relativeCwd = (char *)malloc(sizeof(char) * 4000);
        getcwd(cwd, 4000);
        strcpy(relativeCwd, cwd);
        char *useless = print_path(ogDir, cwd);
        strcpy(cwd, useless);
        printf("<%s@%s:%s> ", pwd->pw_name, sys_name.nodename, cwd);        
        sprintf(printPrompt, "<%s@%s:%s> ", pwd->pw_name, sys_name.nodename, cwd);
        strcpy(globalPrint, printPrompt);
        char *input = NULL;
        char *inputCommands = (char *)malloc(4000);
        len = 0;
        if (getline(&input, &len, stdin) == -1)
        {
            ctrlD();
            break;
        }        
        strcpy(inputCommands, tokenizer(input));
        addHistory(inputCommands);
        char **commands = (char **)malloc(sizeof(char *) * 10); //commands[10][4000]
        for (int i = 0; i < 10; i++)
        {
            commands[i] = (char *)malloc(4000);
            strcpy(commands[i], "");
        }
        int commandSize = 0;
        char *token = (char *)malloc(4000);
        token = strtok(inputCommands, ";");
        counter = 0;
        while (token != NULL)
        {
            strcpy(commands[counter], token);
            counter++;
            token = strtok(NULL, ";");
        }
        commandSize = counter;
        for (int i = 0; i < commandSize; i++)
        {
            char **args = (char **)malloc(sizeof(char *) * 10); //args[10][4000]
            for (int j = 0; j < 10; j++)
            {
                args[j] = (char *)malloc(4000);
                strcpy(args[j], "");
            }
            int argSize = 0;
            char *token = strtok(commands[i], " ");
            int counter = 0;
            while (token != NULL)
            {
                strcpy(args[counter], token);
                counter++;
                token = strtok(NULL, " ");
            }
            argSize = counter;

            handlePipe(args,argSize, relativeCwd, useless, ogDir, cwd, prevDir, &breakwhile, printPrompt);
            
            for (int j = 0; j < 10; j++)
                free(args[j]);
            free(args);            
        }     
        printf("global:%d\n",globalPid);   
        free(printPrompt);
        free(cwd);
        free(relativeCwd);
        free(inputCommands);
        free(token);
        for (int i = 0; i < 10; i++)
        {
            free(commands[i]);
        }
        free(commands);
        if (breakwhile == 1)
        {
            break;
        }
    }
    free(ogDir);
    free(prevDir);
    return 0;
}
