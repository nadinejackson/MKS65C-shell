all: shell.o
	gcc -g shell.o
shell.o: shell.c
	gcc -c -g shell.c
run: a.out
	./a.out $(args)
again: shell.c
	emacs shell.c &
clean: shell.o
	rm shell.o
