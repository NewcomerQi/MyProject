main : menu.o node.o student.o main.o
	gcc -o $@ $^
	rm *.o -rf 
%.o : %.c
	gcc -c *.c -Wall
