CC = gcc
CFLAGS = -Wall

parent: parent.c
	@$(CC) $(CFLAGS) -o parent parent.c

line: line.c
	@$(CC) $(CFLAGS) -o line line.c -lpthread

run : parent line
	@echo "Running the program"
	@./parent

clean:
	@rm -f parent line