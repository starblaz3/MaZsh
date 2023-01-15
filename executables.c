#include "shell.h"

extern char globalPrint[4000];
struct processes children[100];
int childSize = 0;
int globalPid = -1;
char globalCommand[4000];
void ctrlD()
{
    //killing all the bg jobs from shell
    for (int i = 0; i < childSize; i++)
    {
        if (kill(children[i].pid, SIGKILL) == -1)
        {
            perror("couldnt kill bg process");
        }
    }
    return;
}
void removeProcess(pid_t pid)
{
    int pos = -1;
    for (int i = 0; i < childSize; i++)
    {
        if (children[i].pid == pid)
        {
            pos = i;
            break;
        }
    }
    if (pos != -1)
    {
        for (int i = pos; i < childSize - 1; i++)
        {
            children[i].pid = children[i + 1].pid;
            strcpy(children[i].commands, children[i + 1].commands);
            children[i].jobId = children[i + 1].jobId;
        }
        children[childSize - 1].pid = -1;
        strcpy(children[childSize - 1].commands, "");
        children[childSize - 1].jobId = -1;
        childSize = childSize - 1;
    }
}
void addProcess(char *command, pid_t pid)
{
    int pos = childSize;
    //finding command bigger than input command
    for (int i = 0; i < childSize; i++)
    {
        if (strcmp(command, children[i].commands) < 0)
        {
            pos = i;
            break;
        }
    }
    //moving all commands to the next post
    for (int i = childSize - 1; i >= pos; i--)
    {
        strcpy(children[i + 1].commands, children[i].commands);
        children[i + 1].pid = children[i].pid;
        children[i + 1].jobId = children[i].jobId;
    }
    childSize++;
    strcpy(children[pos].commands, command);
    children[pos].pid = pid;
    children[pos].jobId = -1;
    for (int i = 1; i < childSize + 1; i++)
    {
        int ispresent = 0;
        for (int j = 0; j < childSize; j++)
        {
            if (children[j].jobId == i)
            {
                ispresent = 1;
                break;
            }
        }
        if (ispresent == 0)
        {
            children[pos].jobId = i;
        }
    }
}
void sigHandler(int sig)
{
    pid_t pid;
    int status;
    char *printStatment = (char *)malloc(4000);
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        status = WIFEXITED(status);
        for (int i = 0; i < childSize; i++)
        {
            if (children[i].pid == pid)
            {
                sprintf(printStatment, "\n%s with pid %d exited %s", children[i].commands, (int)pid, status != 0 ? "normally" : "abnormally");
                printf("%s\n", printStatment);
                //need to remove child from struct array
                removeProcess(pid);
                break;
            }
        }
        write(1, globalPrint, strlen(globalPrint));
        fflush(stdout);
    }
    free(printStatment);
}

void printExecutables(char *commandInput, char *printPrompt)
{
    // childSize = 0;
    strcpy(globalPrint, printPrompt);
    int andPos[20];
    int andCounter = 0;
    int counter = 0;
    for (int i = 0; i < strlen(commandInput); i++)
    {
        if (commandInput[i] == '&')
        {
            andPos[andCounter] = i;
            andCounter++;
        }
    }
    // if & exists in the argument
    int leftoverFlag = 0;
    char *leftover = (char *)malloc(4000);
    if (andCounter > 0)
    {
        int andCommandSize = 0;
        char **andCommands = (char **)malloc(40 * sizeof(char *));
        for (int i = 0; i < 40; i++)
        {
            andCommands[i] = (char *)malloc(4000);
            strcpy(andCommands[i], "");
        }
        char *token = (char *)malloc(4000);
        token = strtok(commandInput, "&");
        while (token != NULL)
        {
            strcpy(andCommands[andCommandSize], token);
            andCommandSize++;
            token = strtok(NULL, "&");
        }
        if (andCounter < andCommandSize)
        {
            strcpy(leftover, andCommands[andCommandSize - 1]);
            strcpy(andCommands[andCommandSize - 1], "");
            andCommandSize--;
            leftoverFlag = 1;
        }
        for (int i = 0; i < andCommandSize; i++)
        {
            char *command = (char *)malloc(4000);
            strcpy(command, andCommands[i]);
            char *args[40];
            for (int j = 0; j < 40; j++)
            {
                args[j] = (char *)malloc(4000);
                strcpy(args[j], "");
            }
            int argSize = 0;
            token = strtok(command, " ");
            while (token != NULL)
            {
                strcpy(args[argSize], token);
                argSize++;
                token = strtok(NULL, " ");
            }
            int pid;
            pid = fork();
            if (pid < 0)
            {
                perror("couldnt fork");
            }
            else if (pid == 0)
            {
                //child process
                int cid = getpid();
                args[argSize] = NULL;
                struct sigaction sa;
                sa.sa_handler = &sigCtrlZ;
                sigemptyset(&sa.sa_mask);
                sa.sa_flags = SA_RESTART;
                if (sigaction(SIGTSTP, &sa, NULL) == -1)
                {
                    perror("coldnt handle sigtstp signal");
                }
                // setpgid(0, 0);
                if (execvp(args[0], args) == -1)
                {
                    perror("couldnt execute given command");
                    fflush(stderr);
                    return;
                }
            }
            else
            {
                printf("%d\n", pid);
                //add bg process here in a sorted array
                addProcess(andCommands[i], pid);
                struct sigaction sa;

                sa.sa_handler = &sigHandler;
                sigemptyset(&sa.sa_mask);
                sa.sa_flags = SA_RESTART;
                if (sigaction(SIGCHLD, &sa, NULL) == -1)
                {
                    perror("couldnt signal the process");
                }
            }
            free(command);
            for (int j = 0; j < 40; j++)
                free(args[j]);
        }
        free(token);
        for (int i = 0; i < 40; i++)
            free(andCommands[i]);
        free(andCommands);
    }
    if (leftoverFlag == 1 || andCounter == 0)
    {
        // if no & just a foreground process
        char *args[40];
        for (int i = 0; i < 40; i++)
        {
            args[i] = (char *)malloc(4000);
            strcpy(args[i], "");
        }
        int argsSize = 0;
        char *token = (char *)malloc(4000);
        if (leftoverFlag == 1)
        {
            strcpy(globalCommand, leftover);
            token = strtok(leftover, " ");
        }
        else
        {
            strcpy(globalCommand, commandInput);
            token = strtok(commandInput, " ");
        }
        if (token != NULL)
        {
            while (token != NULL)
            {
                strcpy(args[argsSize], token);
                argsSize++;
                token = strtok(NULL, " ");
            }
            int pid;
            pid = fork();
            if (pid < 0)
            {
                perror("couldnt fork");
            }
            else if (pid == 0)
            {
                //child process
                // last pointer should be null
                args[argsSize] = NULL;
                if (execvp(args[0], args) == -1)
                {
                    perror("couldnt execute given command");
                }
            }
            else
            {
                // tcsetpgrp(STDIN_FILENO,pid);
                globalPid = pid;
                int status;
                waitpid(-1, &status, WUNTRACED);
                globalPid = -1;
            }
        }
        free(token);
        for (int j = 0; j < 40; j++)
            free(args[j]);
    }
    free(leftover);
}