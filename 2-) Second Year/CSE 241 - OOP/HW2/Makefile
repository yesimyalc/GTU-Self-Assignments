
output: main.o helper.o
	g++ -o output main.o helper.o 

main.o: main.cpp helper.h
	g++ -std=c++11 -c main.cpp

helper.o: helper.cpp helper.h
	g++ -std=c++11 -c helper.cpp

clean: 
	rm *.o output
