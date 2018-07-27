CFLAG = -Wall -g
CC = gcc
OBJS = main.o all_possible_print.o all_possible_sums.o print_array_template.o sum_as_template.o

default: all clean

all: $(OBJS)
	$(CC) $(CFLAG) -o test_template $(OBJS)

main.o: main.c
	$(CC) $(CFLAG) -c main.c

all_possible_print.o: all_possible_print.c
	$(CC) $(CFLAG) -c all_possible_print.c

all_possible_sums.o: all_possible_sums.c
	$(CC) $(CFLAG) -c all_possible_sums.c

print_array_template.o: print_array_template.c
	$(CC) $(CFLAG) -c print_array_template.c

sum_as_template.o: sum_as_template.c
	$(CC) $(CFLAG) -c sum_as_template.c

clean:
	rm -rf *.o	


