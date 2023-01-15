
# MaZsh

## About
- MaZsh is a fully functional basic shell written by Manas Yendluri(2019113008), in C for linux operating systems.
- Implemented features such as redirection, piping and shell signal handling from scratch.

## Launch MazSH

- Execute "make && ./mazsh" command which will successfully launch the shell.

## File structure

The main directory has the following files:

- `bg.c` - contains the functions required which perform the bg command
- `cd.c` - contains the functions required which perform the cd command
- `echo.c` - contains the functions required which perform the echo command
- `executables.c` - contains the functions required for managing foreground and background user defined commands
- `fg.c` - contains the functions required which perform the fg command
- `history.c` - contains the functions required which perform the history command
- `jobs.c` - contains the functions required which perform the jobs command
- `ls.c` - contains the functions required which perform the ls command
- `main.c` - is the main file in which the driver while loop executes 
- `pinfo.c` - contains the functions required which perform the pinfo command
- `pipe.c` - contains the functions required for handling piping commands 
- `pwd.c` - contains the functions required which perform the pwd command
- `redirect.c` - contains the functions required which perform the redirect command
- `repeat.c` - contains the functions required which perform the repeat command
- `replay.c` - contains the functions required which perform the replay command
- `runCommand.c` - contains the functions required which executes each individual command inputted into shell
- `shell.h` - contains all the header files and function prototypes from the other external c files 
- `sig.c` - contains the functions required which perform the sig command

## working of shell

This shell assumes the directory from which it was launched as its home directory and relative paths are formatted from this directory.

* the initialization of the main driver while loop of the shell happens in the main.c file which is reponsible for all the user inputs and display prompt of the shell.
* after the input is taken from the user using getline the shell then tokenizes the input and splits the input command using ; as a delimiter and each command is processed sequentially.
* each command seperated by ; is then executed via the wrapper function handlePipes() which handles the pipes in the input command and each command seperated by pipes is then called by redirector() function which is responsible for redirection in the input command after redirection streams are set finally, the runCommand function is called which executes the final command which is devoid of pipes or redirection. This approach is usefull as pipes have higher priority than redirection so that piping is done first before redirection.
* the first arg of each command is compared in a if else loop and if it matches then the respective function for the input command is called via function call which are stored in various external c files which was linked using shell.h.
* shell.h contains all the function prototypes from various command files and global variable declarations.
* make exectutes the makefile which generates a binary "mazsh"(silly shell name).
* the rest of the files with c extension contain functions which correspond to their filename.

## specifications from assignment 2 completed

### specification 1

* assuming special characters wont be inputted 

### specification 2

* assuming spaces and escape sequences need not be handled in echo and cd
* assuming no more than 1 directory will be entered for cd else its considered a error
* assuming strlen(directory) is not greater than 4000

### specification 3

* assuming no of dirs entered in ls command will not be greater than 10 and each size of dir is not greater than 4000

### specification 4

* assuming user defined command exists else treated as an error

### specification 5 

* assuming process data is stored in /proc/pid/stat
* assuming given process exits else treated as error

### specification 6

* background process exists when sigchld signal is returned to parent process

### specification 7

* repeat works for all user defined commands

### bonus

* implemented history and history < num > which are implemented using a global file in /tmp/history.txt and is independant on the session running

## specifications from assignment 3 completed

### specification 1 : redirection
* redirection in shell is basically changing stdout and stdin based on the input command(>,<,>>) and i implement it by copying stdout and stdin before redirection and assigning it the copied value after redirection is finished. Assuming only 1 file to be present on the right side of redirection operator

### specification 2 : pipes
* pipes is handled by initializing a pipe array for virtual file descriptors which will store the output of the prev command in the pipe and this way we can pipe many commands 

### specification 3 : pipes and redirection combined
* since i am using a wrapper function handlePipes() over redirection() over runCommand() the priority is as follows: pipes,redirection,input command. This way pipes is handled before redirection stream changing happens and this way we can combine redirection and pipes using a wrapper function

### specification 4 : user defined command:
* jobs: prints the current running and stopped background processes based on the input flag given
* sig : sends the user inputted signal to the job number mentioned
* fg : sends sigcont to the process with jobID and changed tty stream to foreground.
* bg : sends sigCont signal to the process with given jobID

### specification 4 : signal handling:
* CTRL + Z : pushes the current foreground process to background and sends sigtstp signal to it and add it to the background processes list
* CTRL + C : sends sigint signal to current foreground process and has no effect if no foreground process running in terminal
* CTRL + D : stops reading input and sends sigkill to all the background processes in that session

### bonus : replay:
 * executes given command after every interval for the given time period
