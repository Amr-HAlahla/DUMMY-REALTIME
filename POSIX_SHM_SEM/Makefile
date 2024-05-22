CC = gcc
CFLAGS = -Wall

all: parent child

parent: parent.c
	@gcc -o parent parent.c

child: child.c
	@gcc -o child child.c

run: parent child
	@echo "Running parent and child processes..."
	@./parent

clean:
	@rm -f parent child