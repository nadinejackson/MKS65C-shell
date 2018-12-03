# MKS65C-shell

## Implementation

* Parses arguments to commands
* Parses multiple commands on one line with `;`
* Simple redirection using `>` and `<`
* Simple pipes using `|`

## Attempted, but unsuccessful

* Displaying the current working directory besides the prompt

## Bugs

* Potential problem with exit producing an infinite print

## Function headers

```char ** parse_args( char * line , char  separator);```
* Takes a command string (`char *`) and separates the command with the `char` separator
* Returns a `char **` (pointer to a `char *`) of the separated commands, without the separator

```static void sighandler(int signo);```
* Signal handler, catches and intercepts signals
    
```void run (char ** parsed);```
* Takes a parsed command (`char **`), forks a child, and runs the command in the child process, while the parent process waits for the child process to finish

```void myPipe (char * input);```
* Takes a command string (`char *`) with the pipe character
* Creates an unnamed pipe, redirects `STDOUT` to the write end and `STDIN` to the read end
* Runs the left command, and `STDOUT` result is piped to the `STDIN` of the right command

```void redirect(char * input);```
* Takes a command string (`char *`) with a redirection character
* Redirects the `STDOUT` and `STDIN` to the read/write ends of the appropriate programs per the standards of the specified redirection character

```int main(int argc, char* argv[]);```
* Main driver function for the shell
* Returns `0` at the conclusion of the program