
all: makeFileSystem fileSystemOper

makeFileSystem: main.o 
	gcc -o makeFileSystem main.o -Wall

main.o: main.c
	gcc -std=gnu99 -Wall -c main.c

fileSystemOper: fileSystemOper.o 
	gcc -o fileSystemOper fileSystemOper.o -Wall

fileSystemOper.o: fileSystemOper.c
	gcc -std=gnu99 -Wall -c fileSystemOper.c

clean:
	rm *.o

run: 
	./makeFileSystem 4 400 data.dat

show:
	./fileSystemOper data.dat dumpe2fs

root:
	./fileSystemOper data.dat dir "/"

a:
	./fileSystemOper data.dat dir "/a"

SecondA:
	./fileSystemOper data.dat dir "/a/SecondA"

FirstD:
	./fileSystemOper data.dat dir "/d/FirstD"

D:
	./fileSystemOper data.dat dir "/a/SecondA/D"

c:
	./fileSystemOper data.dat dir "/c"

run1:
	./fileSystemOper data.dat mkdir "/a"

run2:
	./fileSystemOper data.dat mkdir "/b"

run3:
	./fileSystemOper data.dat mkdir "/c"

run4:
	./fileSystemOper data.dat mkdir "/d"

run5:
	./fileSystemOper data.dat mkdir "/a/FirstA"

run6:
	./fileSystemOper data.dat mkdir "/a/SecondA"

run7:
	./fileSystemOper data.dat mkdir "/b/FirstB"

run8:
	./fileSystemOper data.dat mkdir "/b/SecondB"

run9:
	./fileSystemOper data.dat mkdir "/d/FirstD"

run10:
	./fileSystemOper data.dat mkdir "/a/SecondA/A"

run11:
	./fileSystemOper data.dat mkdir "/a/SecondA/B"

run12:
	./fileSystemOper data.dat mkdir "/a/SecondA/C"

run13:
	./fileSystemOper data.dat mkdir "/a/SecondA/D"

run14:
	./fileSystemOper data.dat mkdir "/a/SecondA/E"

run15:
	./fileSystemOper data.dat mkdir "/a/SecondA/F"

run16:
	./fileSystemOper data.dat mkdir "/a/SecondA/G"

run17:
	./fileSystemOper data.dat mkdir "/a/SecondA/H"

run18:
	./fileSystemOper data.dat mkdir "/a/SecondA/J"

run19:
	./fileSystemOper data.dat mkdir "/a/SecondA/K"

run20:
	./fileSystemOper data.dat mkdir "/a/SecondA/L"

run21:
	./fileSystemOper data.dat mkdir "/a/SecondA/M"

run22:
	./fileSystemOper data.dat mkdir "/a/SecondA/N"

run23:
	./fileSystemOper data.dat mkdir "/a/SecondA/O"

run24:
	./fileSystemOper data.dat mkdir "/a/SecondA/P"

run25:
	./fileSystemOper data.dat mkdir "/a/SecondA/R"

run26:
	./fileSystemOper data.dat mkdir "/a/SecondA/S"

run27:
	./fileSystemOper data.dat mkdir "/a/SecondA/T"

run28:
	./fileSystemOper data.dat mkdir "/a/SecondA/U"

run29:
	./fileSystemOper data.dat mkdir "/a/SecondA/V"

run30:
	./fileSystemOper data.dat mkdir "/a/SecondA/Y"

run31:
	./fileSystemOper data.dat mkdir "/a/SecondA/Z"

run32:
	./fileSystemOper data.dat mkdir "/a/SecondA/X"

run33:
	./fileSystemOper data.dat mkdir "/a/SecondA/W"

run34:
	./fileSystemOper data.dat mkdir "/a/SecondA/Q"

run34-1:
	./fileSystemOper data.dat mkdir "/d/FirstD/FFirstD"

run34-2:
	./fileSystemOper data.dat mkdir "/d/FirstD/SSecondD"

run35:
	./fileSystemOper data.dat write "/a/SecondA/D/file1" data100.txt

run36:
	./fileSystemOper data.dat write "/a/SecondA/D/file2" data100.txt

run37:
	./fileSystemOper data.dat write "/a/SecondA/D/file3" data100.txt

run38:
	./fileSystemOper data.dat write "/a/SecondA/D/file4" data100.txt

run39:
	./fileSystemOper data.dat write "/a/SecondA/D/file5" data100.txt

run40:
	./fileSystemOper data.dat write "/a/SecondA/D/file6" data10000.txt

run41:
	./fileSystemOper data.dat write "/a/SecondA/D/file7" data10000.txt

run42:
	./fileSystemOper data.dat write "/a/SecondA/D/file8" data10000.txt

run43:
	./fileSystemOper data.dat write "/a/SecondA/D/file9" data1000000.txt

run44:
	./fileSystemOper data.dat write "/a/SecondA/D/file10" data1000000.txt

run45:
	./fileSystemOper data.dat del "/a/SecondA/D/file10"

run46:
	./fileSystemOper data.dat del "/a/SecondA/D/file4"

run47:
	./fileSystemOper data.dat del "/a/SecondA/D/file1"

run48:
	./fileSystemOper data.dat rmdir "/b/SecondB"

run49:
	./fileSystemOper data.dat rmdir "/a/FirstA"

run50:
	./fileSystemOper data.dat rmdir "/a/SecondA/C"

run51:
	./fileSystemOper data.dat rmdir "/a/SecondA/X"

run52:
	./fileSystemOper data.dat rmdir "/a/SecondA/F"

run53:
	./fileSystemOper data.dat write "/c/fileC1" data100.txt

run54:
	./fileSystemOper data.dat write "/c/fileC2" data10000.txt

run55:
	./fileSystemOper data.dat write "/c/fileC3" data100.txt

run56:
	./fileSystemOper data.dat del "/c/fileC2"

run57:
	./fileSystemOper data.dat write "/d/FirstD/fileD1" data100.txt

run58:
	./fileSystemOper data.dat write "/d/FirstD/fileD2" data100.txt

run59:
	./fileSystemOper data.dat write "/d/FirstD/fileD3" data10000.txt

run60:
	./fileSystemOper data.dat write "/d/FirstD/fileD4" data10000.txt

run61:
	./fileSystemOper data.dat write "/d/FirstD/fileD5" data1000000.txt

run62:
	./fileSystemOper data.dat write "/d/FirstD/fileD6" data100.txt

run63:
	./fileSystemOper data.dat del "/d/FirstD/fileD1"

run64:
	./fileSystemOper data.dat del "/d/FirstD/fileD3"

run65:
	./fileSystemOper data.dat read "/d/FirstD/fileD2" dResult100.txt

run66:
	./fileSystemOper data.dat read "/d/FirstD/fileD5" dResult1000000.txt

run67:
	./fileSystemOper data.dat read "/a/SecondA/D/file9" DAResult1000000.txt

run68:
	./fileSystemOper data.dat read "/a/SecondA/D/file6" DAResult10000.txt

run69:
	./fileSystemOper data.dat write "/a/SecondA/D/File9" data100.txt

run70:
	./fileSystemOper data.dat write "/c/fileC4" data100.txt

run71:
	./fileSystemOper data.dat write "/c/fileC5" data100.txt

run72:
	./fileSystemOper data.dat write "/c/fileC6" data10000.txt

run73:
	./fileSystemOper data.dat del "/c/fileC4"

run74:
	./fileSystemOper data.dat read "/c/fileC6" Last.txt

cmp1:
	cmp data100.txt dResult100.txt

cmp2:
	cmp data1000000.txt DAResult1000000.txt

cmp3:
	cmp data10000.txt Last.txt






