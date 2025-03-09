all: main

main: line.o main.o
	g++ -o main line.o main.o

line.o: line.cpp line.hpp
	g++ -c line.cpp

main.o: main.cpp line.hpp
	g++ -c main.cpp

run:
	./main

check: run
	./main > result.txt

clean:
	rm -f *.o main result.txt
