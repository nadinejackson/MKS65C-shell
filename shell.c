#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

char ** parse_args( char * line , char * separator) {

  char **ptr = malloc(256 * sizeof(char *));
  //allocating memory for the array of strings

  while(*line == ' ')
    line++;
  int ctr = 0; //the place in the array of string pointers
  while (ptr[ctr++] = strsep(&line, separator)); //iterate through each split string and add a pointer to it to that index of ptr

  return ptr;
}

int main(int argc, char* argv[])
{
  int *status;
  int p = getpid();
  char * junk;
  char ** programs = malloc(256 * sizeof *programs);
  char * program = malloc(256 * sizeof *program);
  char ** parsed = malloc(256 * sizeof *parsed);
  while(1)
    {
      printf("our_shell$ ");
      fscanf(stdin, "%[^\n]s", program);
      fscanf(stdin, "%c", junk);
      *junk = ';';
      programs = parse_args(program, junk);
      int i;
      
      for(i = 0; programs[i]; i++)
	{
	  if(!strncmp("cd ", programs[i], 3))
	    {
	      chdir(programs[i] + 3);
	    }
	  else if(!strncmp("exit", programs[i], 4))
	    return 0;
	  else
	    {
	      *junk = ' ';
	      parsed = parse_args(programs[i], junk);
	      
	      int f = fork();
	      if(getpid() != p)
		execvp(parsed[0], parsed);
	      else
		waitpid( f, status, 0);
	      //free(parsed);
	    }
	}
    }
  return 0;
}
