
all: hw3named hw3unnamed

hw3named: hw3named.o 
	gcc -o hw3named hw3named.o -lm -lrt -lpthread -Wall

hw3named.o: hw3named.c
	gcc -std=gnu99 -c hw3named.c

hw3unnamed: hw3unnamed.o 
	gcc -o hw3unnamed hw3unnamed.o -lm -lrt -lpthread -Wall

hw3unnamed.o: hw3unnamed.c
	gcc -std=gnu99 -c hw3unnamed.c

run1: 
	valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind-out.txt \
	 ./hw3named -i ./exampleInputs/data -n crit1 -e empty1 -f full1 -a chef0 -b chef1 -c chef2 -d chef3 -g chef4 -h chef5 

run2:
	valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind-out.txt \
	 ./hw3named -i ./exampleInputs/data2 -n crit1 -e empty1 -f full1 -a chef0 -b chef1 -c chef2 -d chef3 -g chef4 -h chef5

run3:
	valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind-out.txt \
	./hw3unnamed -i ./exampleInputs/data

run4:
	valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind-out.txt \
	./hw3unnamed -i ./exampleInputs/data2
