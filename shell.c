#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

char ** parse_args( char * line ) {

  char **ptr = malloc(8 * sizeof(char *));
  //allocating memory for the array of strings

  int ctr = 0; //the place in the array of string pointers
  while (ptr[ctr++] = strsep(&line, " ")); //iterate through each split string and add a pointer to it to that index of ptr

  return ptr;
}

int main(int argc, char* argv[]) {
  //char line[100] = "ls -l --all --color";
  //char line2[128] = "ls -l -a";
  char ** args = parse_args(argv[0]); //splitting into ls, -l, --all, --color
  //printf("%s\n", args[0]); //printing each argument 
  //printf("%s\n", args[1]);
  //printf("%s\n", args[2]);
  //printf("%p\n", args);
  execvp(args[0], args); //executing ls
  
  
  return 0;
}
