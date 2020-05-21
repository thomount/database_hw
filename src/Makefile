all: main


main.o: main.cpp func.h data.h proc.h
	g++ main.cpp -c -o main.o -O2 -std=c++11

func.o: func.cpp func.h
	g++ func.cpp -c -o func.o -O2 -std=c++11

main: main.o func.o
	g++ main.o func.o -o main -O2 -std=c++11

