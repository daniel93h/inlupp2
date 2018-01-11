run:	clean
	gcc -Wall *.c
	./a.out Lagersaldo

comp:	clean
	gcc -Wall *.c

clean:
	rm -f *~
	rm -f *.out

memtest: comp
	valgrind --leak-check=full ./a.out Lagersaldo
