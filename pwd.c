#include "shell.h"

void printPwd()
{
    char *pwd = (char *)malloc(4000);
    getcwd(pwd, 4000);
    printf("%s\n", pwd);
    free(pwd);
}