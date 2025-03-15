CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

all: main

main: main.o line.o
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

run: main
	./main

check: main
	./main > result.txt

clean:
	rm -f main *.o result.txt
