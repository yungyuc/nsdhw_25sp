CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

OBJS = main.o Line.o
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

main.o: main.cpp Line.hpp
Line.o: Line.cpp Line.hpp

run: $(TARGET)
	./$(TARGET)

check: $(TARGET)
	./$(TARGET) > result.txt

clean:
	rm -f $(TARGET) *.o result.txt
