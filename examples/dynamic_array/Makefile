CFLAG = -Wall -g
CC = gcc
OBJS = main.o dynamic_array.o

default: all clean

all: $(OBJS)
	$(CC) $(CFLAG) -o test_dyn_array $(OBJS)

main.o: main.c
	$(CC) $(CFLAG) -c main.c

dynamic_array.o: dynamic_array.c
	$(CC) $(CFLAG) -c dynamic_array.c

clean:
	rm -rf *.o	


