a.out: main.o
	gcc main.o -o a.out

main.o : main.c  
	gcc -c main.c

clean:
	rm -rf main a.out
