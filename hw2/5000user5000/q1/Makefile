CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O3

.PHONY: all run check clean

all: main

main: line.o main.o
	${CXX} ${CXXFLAGS} -o main line.o main.o

line.o: line.cpp line.hpp
	${CXX} ${CXXFLAGS} -c line.cpp

main.o: main.cpp line.hpp
	${CXX} ${CXXFLAGS} -c main.cpp

run: main
	./main

check: run
	./main > result.txt

clean:
	rm -f *.o main result.txt