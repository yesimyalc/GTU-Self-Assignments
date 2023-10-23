
output: Timer.o main.o TimerEvent.o
	g++ -std=c++14 -o hw1 Timer.o main.o TimerEvent.o -pthread

Timer.o: Timer.cpp ITimer.h TimerEvent.h
	g++ -std=c++14 -c Timer.cpp -pthread

TimerEvent.o: TimerEvent.cpp TimerEvent.h
	g++ -std=c++14 -c TimerEvent.cpp -pthread

main.o: main.cpp Timer.h
	g++ -std=c++14 -c main.cpp -pthread

clean: 
	rm -f *.o hw1

run: 
	./hw1