main:main.o add.o sub.o mul.o div.o
	gcc main.o add.o sub.o mul.o div.o -o main
main.o:main.c algs.h
	gcc -c main.c -Wall -g
add.o:add.c algs.h
	gcc -c add.c -Wall -g
sub.o:sub.c algs.h
	gcc -c sub.c -Wall -g
mul.o:mul.c algs.h
	gcc -c mul.c -Wall -g
div.o:div.c algs.h
	gcc -c div.c -Wall -g

.PHONY:clean rebuild
clean:
	rm -f main main.o add.o sub.o mul.o div.o
rebuild:
	clean main
