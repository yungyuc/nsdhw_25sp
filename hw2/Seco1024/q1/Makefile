CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2

SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)
EXEC = main

.PHONY: all run check clean

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp 
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean: 
	rm -f $(OBJ) $(EXEC)

run: $(EXEC)
	./$(EXEC)

check: $(EXEC)
	./$(EXEC) > result.txt