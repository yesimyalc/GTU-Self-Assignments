
output: AbstractHex.o HexVector.o HexArray1D.o cell.o globalFunctions.o main.o HexVectorGame.o HexArray1DGame.o HexAdapterGame.o
	g++ -o mainHexGame AbstractHex.o HexVector.o HexArray1D.o cell.o globalFunctions.o main.o
	g++ -o HexVectorGame AbstractHex.o HexVector.o cell.o HexVectorGame.o
	g++ -o HexArray1DGame AbstractHex.o HexArray1D.o cell.o HexArray1DGame.o
	g++ -o HexAdapterGame AbstractHex.o cell.o HexAdapterGame.o

cell.o: cell.cpp cell.h
	g++ -std=c++11 -c cell.cpp

AbstractHex.o: AbstractHex.cpp cell.h exceptions.h AbstractHex.h
	g++ -std=c++11 -c AbstractHex.cpp

HexVector.o: HexVector.cpp cell.h exceptions.h AbstractHex.h HexVector.h
	g++ -std=c++11 -c HexVector.cpp

HexArray1D.o: HexArray1D.cpp cell.h exceptions.h AbstractHex.h HexArray1D.h
	g++ -std=c++11 -c HexArray1D.cpp

globalFunctions.o: globalFunctions.cpp cell.h exceptions.h AbstractHex.h HexVector.h HexArray1D.h HexAdapter.h globalFunctions.h
	g++ -std=c++11 -c globalFunctions.cpp

main.o: main.cpp cell.h exceptions.h AbstractHex.h HexVector.h HexArray1D.h HexAdapter.h globalFunctions.h
	g++ -std=c++11 -c main.cpp

HexVectorGame.o: HexVectorGame.cpp cell.h exceptions.h AbstractHex.h HexVector.h
	g++ -std=c++11 -c HexVectorGame.cpp

HexArray1DGame.o: HexArray1DGame.cpp cell.h exceptions.h AbstractHex.h HexArray1D.h
	g++ -std=c++11 -c HexArray1DGame.cpp

HexAdapterGame.o: HexAdapterGame.cpp cell.h exceptions.h AbstractHex.h HexAdapter.h
	g++ -std=c++11 -c HexAdapterGame.cpp

clean: 
	rm -f *.o 
