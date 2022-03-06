CC = gcc -std=c17

CFLAGS = -c -Wall -Werror

LOG_LIB = ../My_Lib/My_Lib.a

all: Stack

Stack: main.o Stack.o
	$(CC) Objects/main.o Objects/Stack.o $(LOG_LIB) -o Stack.out

main.o: main.c
	$(CC) $(CFLAGS) main.c -o Objects/main.o

Stack.o: Stack.c
	$(CC) $(CFLAGS) Stack.c -o Objects/Stack.o

run:
	./Stack.out

clean:
	rm -rf Objects/main.o Objects/Stack.o
	rm Stack.out
	rm *.log
