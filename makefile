CC=gcc
CFLAGS = -Wall -ansi -pedantic



all:  clean instructions.o symbols.o dataseg.o first_pass.o main.c 
	$(CC) $(CFLAGS) main.c -o main *.o

first_pass.o: support.o parsing.o instructions.o symbols.o dataseg.o first_pass.c first_pass.h
	$(CC) $(CFLAGS) -c first_pass.c

parsing.o: support.o instructions.o parsing.c parsing.h
	$(CC) $(CFLAGS) -c parsing.c

instructions.o: support.o instructions.c instructions.h
	$(CC) $(CFLAGS) -c instructions.c
   
symbols.o: support.o symbols.c symbols.h
	$(CC) $(CFLAGS) -c symbols.c

support.o: support.c support.h errors.h
	$(CC) $(CFLAGS) -c support.c

dataseg.o: dataseg.c dataseg.h
	$(CC) $(CFLAGS) -c dataseg.c

clean: 
	rm -f *.o
