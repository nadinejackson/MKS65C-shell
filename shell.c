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
  printf("our_shell$");
  int s;
  int p = getpid();
  while(1)
  {  
      /*
	char * program = malloc(256 * sizeof(char));
	char * program2 = malloc(256 * sizeof(char));
	char * test = malloc(256 * sizeof(char));
	scanf("%s %s %s", program, test, program2);
	printf("hi\n");
	printf("hi, you typed %s %s %s\n", program, test, program2);
	printf("hey\n");
	char ** parsed = malloc(256 * sizeof(char*));
	parsed[0] = program;
	parsed[1] = test;
	parsed[2] = program2;
	parsed[3] = 0;
      */
      char * program = malloc(256 * sizeof(char));
      scanf("%s", program);
      char ** parsed = malloc(256 * sizeof(char*));
      parsed [0] = program;
      
      int f = fork();
      if(getpid() != p)
	{
	  printf("test");
	  execvp(parsed[0], parsed);
	}
      else{
	
	wait(&s);
	printf("our shell$");
	}
}
  return 0;
}
