#include <stdio.h>
#include <pwd.h>
#include <grp.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#ifndef SHELL
#define SHELL
char * changeDir(char *ogDir, char *cwd, char *newPath, char *prevDir);
void print_ls(char **args, int argSize, char *cwd, char *printCwd, char *ogDir);
int findHardNodes(char **printList, int size, char *dir);
char *getL(char **printList, int i, char *dir);
void printEcho(char **args, int argSize);
void printPwd();
void printExecutables(char * commandInput,char * printPrompt);
void sigHandler (int sig);
void removeProcess(pid_t pid);
void pinfo(char ** args, int argSize);
void printRepeat(char ** args,int argSize,int times,char * relativeCwd,char * useless,int * breakwhile,char *cwd,char *ogDir,char *prevDir,char * ogCommand,char * printPrompt);
void showHistory( int count);
void addHistory(char *command);
void printJobs(char ** args,int argSize);
void handleSig(char ** args,int argSize);
void handleBG(char ** args, int argSize);
void sigCtrlZ(int sig);
void addProcess(char * command,pid_t pid);
void ctrlD();
void handleFG(char ** args,int argSize);
void redirector(char ** args,int argSize, char *relativeCwd, char *useless, char *ogDir, char *cwd, char *prevDir, int *breakwhile, char *printPrompt);
void handlePipe(char ** args,int argSize, char *relativeCwd, char *useless, char *ogDir, char *cwd, char *prevDir, int *breakwhile, char *printPrompt);
void runCommand(char ** args,int argSize, char *relativeCwd, char *useless, char *ogDir, char *cwd, char *prevDir, int *breakwhile, char *printPrompt);
void handleReplay(char ** args,int argSize, char *relativeCwd, char *useless, char *ogDir, char *cwd, char *prevDir, int *breakwhile, char *printPrompt);
void sigCtrlZ(int sig);
char *print_path(char *ogDir, char *path);
struct processes
{
    pid_t pid;
    char commands[100];
    int jobId;
};
#endif