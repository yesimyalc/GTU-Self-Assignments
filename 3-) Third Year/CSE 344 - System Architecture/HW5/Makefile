
all: hw4

hw4: hw4.o 
	gcc -o hw4 hw4.o -lm -lrt -lpthread -Wall

hw4.o: hw4.c
	gcc -std=gnu99 -Wall -c hw4.c

clean:
	rm *.o

run1: 
	./hw4 -C 5 -N 5 -F ./exampleInputs/data

run2:
	./hw4 -C 10 -N 3 -F ./exampleInputs/data2

run3:
	./hw4 -C 10 -N 10 -F ./exampleInputs/data3

run4:
	./hw4 -C 8 -N 50 -F ./exampleInputs/data4

run5:
	./hw4 -C 10 -N 200 -F ./exampleInputs/data5
