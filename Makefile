all:
	gcc server.c -o server.out

run:
	gcc server.c -o server.out
	./server.out
