# CC=gcc
# CFLAGS=-I.

# all: writer reader

# writer: writer.c
# 	$(CC) -o writer writer.c $(CFLAGS)

# reader: reader.c
# 	$(CC) -o reader reader.c $(CFLAGS)

# run_writer: writer
# 	./writer

# run_reader: reader
# 	./reader

# clean:
# 	rm -f writer reader

CC=gcc
CFLAGS=-I.

all: parent consumer producer

parent: parent.c
	$(CC) -o parent parent.c $(CFLAGS)

consumer: consumer.c
	$(CC) -o consumer consumer.c $(CFLAGS)

producer: producer.c
	$(CC) -o producer producer.c $(CFLAGS)

run_parent: parent
	./parent $(ARGS)

clean:
	rm -f parent consumer producer