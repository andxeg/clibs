CC = gcc
CFLAG = -Wall -g
OBJS = main.o lists.o list_template.o dynamic_array_template.o dynamic_arrays.o

SRC_DIR = src
INCLUDE_DIR = include

default: all clean

all: $(OBJS)
	$(CC) $(CFLAG) -o test_list $(OBJS) -I ../../$(INCLUDE_DIR)

main.o: main.c
	$(CC) $(CFLAG) -c main.c -I ../../$(INCLUDE_DIR)

lists.o: lists.c 
	$(CC) $(CFLAG) -c lists.c -I ../../$(INCLUDE_DIR)

list_template.o: list_template.c
	$(CC) $(CFLAG) -c list_template.c

dynamic_array_template.o: ../../$(SRC_DIR)/dynamic_array_template.c
	$(CC) $(CFLAG) -c ../../$(SRC_DIR)/dynamic_array_template.c -I ../../$(INCLUDE_DIR)

dynamic_arrays.o: ../../$(SRC_DIR)/dynamic_arrays.c
	$(CC) $(CFLAG) -c ../../$(SRC_DIR)/dynamic_arrays.c -I ../../$(INCLUDE_DIR)

clean:
	rm -rf *.o
    
