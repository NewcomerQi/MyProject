smtp_pop : base64.o parsemail.o mailrecv.o net.o central.o devicecontrol.o
	gcc base64.o parsemail.o mailrecv.o net.o central.o devicecontrol.o -o smtp_pop -lsqlite3 -pthread
	rm *.o -rf
%.o : %.c
	gcc -g -Wall -c *.c 
