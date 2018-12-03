#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
char ** parse_args( char * line , char  separator) {

    char **ptr = malloc(256 * sizeof(char *));
    //allocating memory for the array of strings

    while(*line == ' ') //removing spaces from beginning
        line++;
  
    while(!strncmp(line + strlen(line) - 1, " ", 1)) //removing spaces from end
        *(line + strlen(line) - 1) = '\0';
  
    int ctr = 0; //the place in the array of string pointers
    while (ptr[ctr++] = strsep(&line, &separator)); //iterate through each split string and add a pointer to it to that index of ptr

    return ptr;
}

static void sighandler(int signo) //sigint no longer ends the program
{
    printf("\nType exit if you wish to exit the shell, otherwise, type as usual.\n");
}

void run (char ** parsed)
{
    int * status; //to hold status
    int f = fork(); //returns pid of child to parent, null to child
    if(!f) //child will become program
        execvp(parsed[0], parsed);
    else
        waitpid(f, status, 0); //parent will wait ffor child
}

void myPipe (char * input)
{

    char ** parsed = parse_args(input, '|');

    char ** left = parse_args(parsed[0], ' ');
    char ** right = parse_args(parsed[1], ' ');
    

    int fds[2];
    pipe( fds );
    char line[100];
    int f = fork();
    if (f==0) {
        close( fds[0] );
        int backup_stdout = dup(STDOUT_FILENO);
        dup2(fds[1], STDOUT_FILENO);
        run(left);
        dup2(backup_stdout, STDOUT_FILENO);
    }
    else {
        close( fds[1] );
        int backup_stdin = dup(STDIN_FILENO);
        dup2(fds[0], STDIN_FILENO);
        run(right);
        dup2(backup_stdin, STDIN_FILENO);
    }

}

void redirect(char * input)
{
    char ** parts = parse_args(input, '>');
    int fdnew = open(parts[1], O_CREAT | O_WRONLY, 0664);
    char ** ptr = malloc(256 * sizeof(*ptr));
    ptr = parse_args(parts[0], ' ');
    int backup_stdout = dup(STDOUT_FILENO);
    dup2(fdnew, STDOUT_FILENO);
    run(ptr);
    dup2(backup_stdout, STDOUT_FILENO);
}

int main(int argc, char* argv[])
{
    signal(SIGINT, sighandler);
    char * junk = malloc(sizeof *junk); //holds the newline so fscanf works
    char ** programs = malloc(256 * sizeof *programs); //will hold things separated by semicolons
    char * line = malloc(256 * sizeof *line); //will hold input
    char ** parsed = malloc(256 * sizeof *parsed); //will be input to execvp
    int i; //for the for loop

    while(1) //forever
    {
        printf("%s>> ", getenv("USER"));//, getcwd(buf, sizeof buf));
        fscanf(stdin, "%[^\n]s", line); //scans line of user input to newline
        fscanf(stdin, "%c", junk); //holds the newline so input can continue

        programs = parse_args(line, ';'); //separate each statement by semicolons and parse
        for(i = 0; programs[i]; i++) //for each semicolon-seaparated command
        {
            
            if(!strncmp("cd ", programs[i], 3))//cd
                chdir(programs[i] + 3);
	  
            else if(!strncmp("exit", programs[i], 4))//exit
                return 0;
	  
            else //everything else
            {
                if ( strchr(programs[i], '|') ) {
                    myPipe(programs[i]);
                }
                
                else if (strchr(programs[i], '>')) {
                    redirect(programs[i]);
                }

                else {
                    parsed = parse_args(programs[i], ' '); //separate by spaces to input into execvp
                    run(parsed); //fork and exec
                }
                line = malloc(256 * sizeof *line);
            }
        }
    }
    return 0;
}
