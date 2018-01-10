gprof: clean
	gcc -Wall -pg list.c list_aux.c
	./a.out
	gprof -b ./a.out gmon.out > lists.txt

clean:
	rm -f *.out
	rm -f *~
