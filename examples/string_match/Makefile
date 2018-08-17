CC = gcc
CFLAG = -Wall -g
OBJS = main.o 

default: all clean

all: $(OBJS)
	$(CC) $(CFLAG) -o string_match $(OBJS)

main.o: main.c
	$(CC) $(CFLAG) -c main.c

clean:
	rm -rf *.o

clean_all:
	rm -rf *.o string_match
    
