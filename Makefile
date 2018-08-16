all:
	gcc server.c -o server.out
	gcc client.c -o client.out

run:
	gcc server.c -o server.out
	gcc client.c -o client.out
	./server.out
