
all: processP processR_i

processP: main.o 
	gcc -o processP main.o -lm

processR_i: processR_i.o
	gcc -o processR_i processR_i.o -lm

main.o: main.c
	gcc -std=gnu99 -c main.c

processR_i.o: processR_i.c
	gcc -std=gnu99 -c processR_i.c

run: 
	./processP -i input.dat -o output.dat
