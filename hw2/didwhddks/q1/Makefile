CXX = g++
CXXFLAGS = -std=c++17

.PHONY: all run check clean

all:
	${CXX} ${CXXFLAGS} main.cpp -o main

run:
	./main

check:
	./main > result.txt

clean:
	rm -f main result.txt