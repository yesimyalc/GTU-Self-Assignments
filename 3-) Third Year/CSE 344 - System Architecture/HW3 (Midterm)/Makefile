
all: client serverY

serverY: serverY.o 
	gcc -o serverY serverY.o -lm -lpthread -lrt

serverY.o: serverY.c
	gcc -std=gnu99 -c serverY.c

client: client.o 
	gcc -o client client.o -lm

client.o: client.c
	gcc -std=gnu99 -c client.c

run1: 
	./serverY -s /tmp/pathFIFO.fifo -o ./log.dat -p 3 -r 3 -t 20

run2:
	./client -s /tmp/pathFIFO.fifo -o ./exampleInputs/data.csv

run3:
	./client -s /tmp/pathFIFO.fifo -o ./exampleInputs/data2.csv

run4:
	./client -s /tmp/pathFIFO.fifo -o ./exampleInputs/data3.csv

run5:
	./client -s /tmp/pathFIFO.fifo -o ./exampleInputs/data4.csv
