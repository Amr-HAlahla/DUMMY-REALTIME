CC=gcc
CFLAGS=-Wall

all: sem_server sem_client1 sem_client2

sem_server: sem_server.c
	$(CC) $(CFLAGS) -o sem_server sem_server.c

sem_client1: sem_client1.c
	$(CC) $(CFLAGS) -o sem_client1 sem_client1.c

sem_client2: sem_client2.c
	$(CC) $(CFLAGS) -o sem_client2 sem_client2.c

run: all
	./sem_server &
	sleep 1
	./sem_client1 &
	./sem_client2 &

clean:
	rm -f sem_server sem_client1 sem_client2