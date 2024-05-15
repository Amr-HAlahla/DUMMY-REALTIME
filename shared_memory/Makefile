CC=gcc
CFLAGS=-I.

all: shm_client shm_server

shm_client: shm_client.c
	$(CC) -o shm_client shm_client.c $(CFLAGS)

shm_server: shm_server.c
	$(CC) -o shm_server shm_server.c $(CFLAGS)

run: all
	./shm_server &
	./shm_client

clean:
	rm -f shm_client shm_server