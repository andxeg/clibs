CC = gcc
CFLAG = -Wall -g
OBJS = dynamic_array_template.o dynamic_arrays.o lists.o list_template.o file_schema.o iofile.o
MAIN_OBJS = main.o $(OBJS)
TEST_OBJS = test.o $(OBJS)

SRC_DIR = src
INCLUDE_DIR = include
LIST_SRC_DIR = examples/list
LIST_INCLUDE_DIR = examples/list

default: all all_test clean

all: $(MAIN_OBJS)
	$(CC) $(CFLAG) -o test_task $(MAIN_OBJS)

main.o: main.c
	$(CC) $(CFLAG) -c main.c -I ./$(INCLUDE_DIR)/ -I ./$(LIST_INCLUDE_DIR)

dynamic_array_template.o: ./$(SRC_DIR)/dynamic_array_template.c
	$(CC) $(CFLAG) -c ./$(SRC_DIR)/dynamic_array_template.c -I ./$(INCLUDE_DIR)

dynamic_arrays.o: ./$(SRC_DIR)/dynamic_arrays.c
	$(CC) $(CFLAG) -c ./$(SRC_DIR)/dynamic_arrays.c -I ./$(INCLUDE_DIR)

lists.o: ./$(LIST_SRC_DIR)/lists.c
	$(CC) $(CFLAG) -c ./$(LIST_SRC_DIR)/lists.c -I ./$(INCLUDE_DIR) -I ./$(LIST_INCLUDE_DIR)

list_template.o: ./$(LIST_SRC_DIR)/list_template.c
	$(CC) $(CFLAG) -c ./$(LIST_SRC_DIR)/list_template.c -I ./$(INCLUDE_DIR) -I ./$(LIST_INCLUDE_DIR)

file_schema.o: ./$(SRC_DIR)/file_schema.c
	$(CC) $(CFLAG) -c ./$(SRC_DIR)/file_schema.c -I ./$(INCLUDE_DIR) -I ./$(LIST_INCLUDE_DIR)

iofile.o: ./$(SRC_DIR)/iofile.c
	$(CC) $(CFLAG) -c ./$(SRC_DIR)/iofile.c -I ./$(INCLUDE_DIR) -I ./$(LIST_INCLUDE_DIR)

all_test: $(TEST_OBJS) 
	$(CC) $(CFLAG) -o test $(TEST_OBJS)

test.o: test.c
	$(CC) $(CFLAG) -c test.c -I ./$(INCLUDE_DIR)/  -I ./$(LIST_INCLUDE_DIR)


clean:
	rm -rf *.o

clean_all:
	rm -rf *.o test test_task
    
