
all: hw5

hw5: hw5.o 
	gcc -o hw5 hw5.o -lm -lrt -lpthread -Wall

hw5.o: hw5.c
	gcc -std=gnu99 -Wall -c hw5.c

clean:
	rm *.o

run1: 
	./hw5 -i ./exampleInputs/data1 -j ./exampleInputs/data2 -o output2 -n 4 -m 4

run3:
	./hw5 -i ./exampleInputs/data5 -j ./exampleInputs/data6 -o output2 -n 3 -m 4

run5:
	./hw5 -i ./exampleInputs/data9 -j ./exampleInputs/data10 -o output2 -n 8 -m 8
