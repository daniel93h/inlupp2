# run = run the program
# gdb = debugg the program
# lager = compile the whole program
# valgrind = run memory leak diagnostics

CC=gcc
FLAGS= -Wall -g
TFLAGS= -pedantic 


run: lager
	./a.out

gdb: lager
	gdb ./a.out

lager: lager.o
	$(CC) $(FLAGS) lager.o lager_aux.o tree_aux.o list.o tree.o list_aux.o  -o ./a.out

valgrind: lager
	valgrind --leak-check=yes ./lager

gdb tester: tester.o
	$(CC) $(FLAGS) tester.o list.o tree.o list_aux.o tree_aux.o -o tester -lcunit
	gdb ./a.out

tester: tester.o
	$(CC) $(FLAGS) tester.o list.o tree.o list_aux.o tree_aux.o -o tester -lcunit
	./tester

tester.o: tester.c lager_aux.o list.o tree.o list_aux.o tree_aux.o 
	$(CC) $(FLAGS) -c tester.c

lager.o: lager.c lager_aux.o list.o tree.o list_aux.o tree_aux.o
	$(CC) $(FLAGS) -c lager.c

lager_aux.o: lager_aux.c lager_aux.h
	$(CC) $(FLAGS) -c lager_aux.c

list.o: list.c list.h
	$(CC) $(FLAGS) -c list.c 

list_aux.o: list_aux.c list_aux.h
	$(CC) $(FLAGS) -c list_aux.c



tree.o: tree.c tree.h tree_aux.o
	$(CC) $(FLAGS) -c tree.c

tree_aux.o: tree_aux.c tree_aux.h
	$(CC) $(FLAGS) -c tree_aux.c

clean:
	rm -f *.o
	rm -f *~

# Compile stack_test to an executable
stack_test:
	gcc -ggdb --coverage -Wall -pedantic stack.c stack_test.c -o stack_test -lcunit

# Run the tests (dependency: compilation)
test:	stack_test
	./stack_test

# Print test coverage (dependency: run tests and thus also compilation)
coverage: test
	gcov --all-blocks --branch-probabilities --branch-counts --display-progress stack.c
	@echo NOTE: You can open stack.c.gcov in e.g. emacs for more details. 

# Remove temporary files 
clean_test:
	rm -f stack_test stack.gcda stack.gcno stack_test.gcda stack_test.gcno stack.c.gcov
