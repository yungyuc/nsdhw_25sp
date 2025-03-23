CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

OBJS = main.o line.o
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

main.o: main.cpp line.hpp
line.o: line.cpp line.hpp

run: $(TARGET)
	./$(TARGET)

check: $(TARGET)
	./$(TARGET) > result.txt

clean:
	rm -f $(TARGET) *.o result.txt
