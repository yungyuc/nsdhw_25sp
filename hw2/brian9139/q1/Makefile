CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2

TARGET = line_program
SRCS = main.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

check:
	./$(TARGET) > result.txt


diff:
	diff result.txt golden.txt

clean:
	rm -f $(TARGET) $(OBJS) result.txt
