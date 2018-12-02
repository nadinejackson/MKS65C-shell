#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

char ** parse_args( char * line , char  separator) {

  char **ptr = malloc(256 * sizeof(char *));
  //allocating memory for the array of strings

  while(*line == ' ')
    line++;
  int ctr = 0; //the place in the array of string pointers
  while (ptr[ctr++] = strsep(&line, &separator)); //iterate through each split string and add a pointer to it to that index of ptr

  return ptr;
}

void run (char ** parsed)
{
  int * status;
  int f = fork();
  if(!f)
    {
      execvp(parsed[0], parsed);

    }
  else
    waitpid(f, status, 0);
  //printf("%d", f);
  
}
/**
 * main (int, char*)
 * Does everything. Except parse_args. But you knew that. ;) jk. will fix.
 */
int main(int argc, char* argv[])
{
  int *status;
  int p = getpid();
  char * junk = malloc(sizeof *junk);
  char ** programs = malloc(256 * sizeof *programs);
  char * line = malloc(256 * sizeof *line);
  char ** parsed = malloc(256 * sizeof *parsed);
  //char * sc = ';';
  //char * buf = malloc(128 * sizeof *buf);
  while(1)
    {
      printf("%s>> ", getenv("USER"));//, getcwd(buf, sizeof buf));
      fscanf(stdin, "%[^\n]s", line);
      fscanf(stdin, "%c", junk);
      //printf("hiya\n");
      //junk = &';'; //weeeewooooweeeeewoooo houston we have a problem!
      //printf("hiya2\n");
      programs = parse_args(line, ';');
      int i;
      
      for(i = 0; programs[i]; i++)
	{
	  
	  if(!strncmp("cd ", programs[i], 3))//cd
	    chdir(programs[i] + 3);
	  
	  else if(!strncmp("exit", programs[i], 4))//exit
	    return 0;
	  
	  else //everything else
	    {
	      //*junk = ' ';
	      parsed = parse_args(programs[i], ' ');
	      run(parsed);
	      //printf("%s", parsed[0]);
	    }
	}
    }
  return 0;
}
