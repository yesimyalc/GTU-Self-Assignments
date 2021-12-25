
output: main.o Hex.o
	g++ -o output main.o Hex.o 

main.o: main.cpp Hex.h
	g++ -std=c++11 -c main.cpp

Hex.o: Hex.cpp Hex.h
	g++ -std=c++11 -c Hex.cpp

clean: 
	rm *.o output
