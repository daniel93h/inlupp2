run:	clean
	gcc -Wall -g -v *.c
	./a.out Lagersaldo

comp:	clean
	gcc -Wall *.c

clean:
	rm -f *~
	rm -f *.out

memtest: comp
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./a.out Lagersaldo
