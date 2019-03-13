CC=gcc
CFLAGS = -Wall -ansi -pedantic



all:  clean instructions.o symbols.o dataseg.o first_pass.o errors.o second_pass.o main.c 
	$(CC) $(CFLAGS) main.c -o main *.o

second_pass.o: memory_words.o support.o parsing.o instructions.o symbols.o codeseg.o errors.o first_pass.o second_pass.c second_pass.h
	$(CC) $(CFLAGS) -c second_pass.c

first_pass.o: support.o parsing.o instructions.o symbols.o dataseg.o errors.o first_pass.c first_pass.h
	$(CC) $(CFLAGS) -c first_pass.c

errors.o: errors.c errors.h 
	$(CC) $(CFLAGS) -c errors.c

memoey_words.o: memory_words.c memory_words.h
	$(CC) $(CFLAGS) -c memory_words.c

parsing.o: support.o instructions.o parsing.c parsing.h
	$(CC) $(CFLAGS) -c parsing.c

instructions.o: support.o instructions.c instructions.h
	$(CC) $(CFLAGS) -c instructions.c
   
symbols.o: support.o symbols.c symbols.h
	$(CC) $(CFLAGS) -c symbols.c

support.o: support.c support.h errors.h
	$(CC) $(CFLAGS) -c support.c

codeseg.o: codeseg.c codeseg.h
	$(CC) $(CFLAGS) -c codeseg.c

dataseg.o: dataseg.c dataseg.h
	$(CC) $(CFLAGS) -c dataseg.c

clean: 
	rm -f *.o
