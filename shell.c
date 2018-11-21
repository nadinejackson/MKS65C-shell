#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

// DELETE if not used :) 
char ** parse_args( char * line ) {

  char **ptr = malloc(8 * sizeof(char *));
  //allocating memory for the array of strings

  int ctr = 0; //the place in the array of string pointers
  while (ptr[ctr++] = strsep(&line, " ")); //iterate through each split string and add a pointer to it to that index of ptr

  return ptr;
}

int main(int argc, char* argv[]) {

  // use this to run command via a.out
  // argv[0] is ./a.out
  // and anything after that is the desired command

  if (argc > 1)
    {
      char **ptr = malloc ( argc * sizeof(char*) );

      int c = 0;
      while ( c < argc-1 ) {
	ptr[c] = argv[c+1];
	c++;
      }
      argv[c] = NULL;
  
      execvp(ptr[0], ptr);
    }
  printf("our_shell$ ");
  int *status;
  int p = getpid();
  char * junk;
  char * program = malloc(256 * sizeof(char));
  char ** parsed = malloc(256 * sizeof(char*));
  while(1)
    {
      fscanf(stdin, "%[^\n]s", program);
      fscanf(stdin, "%c", junk);
      parsed = parse_args(program);

      int f = fork();
      if(getpid() != p)
	{
	  execvp(parsed[0], parsed);
	  
	}
      else{
	waitpid( f, status, 0);
	printf("our shell$ ");
      }
    }
  return 0;
}
