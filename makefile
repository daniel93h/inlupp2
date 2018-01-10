# run = run the program
# gdb = debugg the program
# lager = compile the whole program
# valgrind = run memory leak diagnostics

CC=gcc
FLAGS= -Wall -g
TFLAGS= -pedantic 

clean:
	rm -f *#
	rm -f *.o
	rm -f *~
	rm -f *.out

filecomp: clean
	$(CC) $(FLAGS) lager.c lager_aux.c list.c list_aux.c tree.c tree_aux.c fileman.c 

file: clean
	$(CC) $(FLAGS) lager.c lager_aux.c list.c list_aux.c tree.c tree_aux.c fileman.c 
	./a.out Lagersaldo

run: lager
	./a.out

gdb: filecomp
	gdb ./a.out Lagersaldo
