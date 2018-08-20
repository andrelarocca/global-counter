all:
	gcc -pthread server.c -o server
	gcc -pthread client.c -o client

run:
	gcc -pthread server.c -o server
	gcc -pthread client.c -o client
	./server
