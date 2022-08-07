
all: headerImplementations.o binaryTree.o client server servant

headerImplementations.o: headerImplementations.c
	gcc -std=gnu99 -c headerImplementations.c -Wall

binaryTree.o: binaryTree.c
	gcc -std=gnu99 -c binaryTree.c -Wall

client: client.o 
	gcc -o client client.o headerImplementations.o binaryTree.o -lrt -lpthread -Wall

client.o: client.c
	gcc -std=gnu99 -c client.c -Wall

server: server.o 
	gcc -o server server.o headerImplementations.o binaryTree.o -lrt -lpthread -Wall

server.o: server.c
	gcc -std=gnu99 -c server.c -Wall

servant: servant.o 
	gcc -o servant servant.o headerImplementations.o binaryTree.o -lrt -lpthread -Wall

servant.o: servant.c
	gcc -std=gnu99 -c servant.c -Wall

clean:
	rm *.o client server servant


