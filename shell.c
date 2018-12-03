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
  /* Takes a command string (`char *`) and separates the command with the `char` separator
   * Returns a `char **` (pointer to a `char *`) of the separated commands, without the separator*/

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
//* Signal handler, catches and intercepts signals
{
  printf("\nType exit if you wish to exit the shell, otherwise, type as usual.\n");
}

void run (char ** parsed)
//Takes a parsed command (`char **`), forks a child, and runs the command in the child process, while the parent process waits for the child process to finish
{
  int * status; //to hold status
  int f = fork(); //returns pid of child to parent, null to child
  if(!f) //child will become program
    execvp(parsed[0], parsed);
  else
    waitpid(f, status, 0); //parent will wait ffor child
}

int myPipe (char * input)
{
  /*Takes a command string (`char *`) with the pipe character
   * Creates an unnamed pipe, redirects `STDOUT` to the write end and `STDIN` to the read end
   * Runs the left command, and `STDOUT` result is piped to the `STDIN` of the right command
   */
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
/* Takes a command string (`char *`) with a redirection character
 * Redirects the `STDOUT` and `STDIN` to the read/write ends of the appropriate programs per the standards of the specified redirection character
 */
{
  int type;
  char ** parts = malloc(2 * sizeof(*parts));
  parts[0] = malloc(256 * sizeof(char));
  parts[1] = malloc(256 * sizeof(char));
  int backup;
  int fdnew;
  /*
    if(strstr(input, ">>"))
    {
    strncpy(parts[0], input, strstr(input, ">>") - input - 1);
    strcpy(parts[1], strstr(input, ">>") + 2);
    parse_args(parts[1], ' ');
    type = 0;
    fdnew = open(parts[1], O_CREAT | O_APPEND, 0664);
    }
    else if(strstr(input, "2>"))
    {
    strncpy(parts[0], input, strstr(input, "2>") - input - 1);
    strcpy(parts[1], strstr(input, "2>") + 2);
    parse_args(parts[1], ' ');
    type = 1;
    fdnew = open(parts[1], O_CREAT | O_WRONLY, 0664);
    }
  */
  if(strchr(input, '<'))
    {
      
      parts = parse_args(input, '<');
      //parse_args(parts[1], ' ');
      fdnew = open(parts[1], O_RDONLY, 0664);
      type = 2;
      
    }
 
  else
    {
      parts = parse_args(input, '>');
      parse_args(parts[1], ' ');
      fdnew = open(parts[1], O_CREAT | O_WRONLY, 0664);
      type = 3;
    }
  if (type == 3 ||type == 0)
    {
      backup = dup(STDOUT_FILENO);
      dup2(fdnew, STDOUT_FILENO);
    }
  /*
    else if (type == 1)
    {
    backup = dup(STDERR_FILENO);
    dup2(fdnew, STDERR_FILENO);
    }
  */
  else
    {
      backup = dup(STDIN_FILENO);
      dup2(fdnew, STDIN_FILENO);
    }
  
  if (strchr(parts[0], '|') )
    {
      myPipe(parts[0]);
    }
      
  else
    {
      char ** ptr = malloc(256 * sizeof(*ptr));
      ptr = parse_args(parts[0], ' ');
      run(ptr);
    }
  if (type == 0 || type == 3)
    dup2(backup, STDOUT_FILENO);
  /*
    else if(type == 1)
    dup2(backup, STDERR_FILENO);*/
  else
    dup2(backup, STDIN_FILENO);
}

int main(int argc, char* argv[])
{
  /* Main driver function for the shell
   * Returns `0` at the conclusion of the program
   */
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
                
	      else if (strchr(programs[i], '>')||strchr(programs[i], '<')) {
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
