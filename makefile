all: shell.o
	gcc shell.o
shell.o: shell.c
	gcc -c shell.c
run: a.out
	./a.out $(args)
again: shell.c
	emacs shell.c &
clean: shell.o
	rm shell.o
