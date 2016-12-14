CC = gcc
FLAGS = -g
all: server client

server: server.c
	$(CC) $(FLAGS) -o server server.c

client: client.c
	$(CC) $(FLAGS) -o client client.c

clean:
	rm server client
	