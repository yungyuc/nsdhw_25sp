# Compiler settings
CC = g++
CFLAGS = -Wall -Wextra -std=c++14

# Name of the executable (adjust if needed)
TARGET = sol

# Automatically find all .cpp source files in the directory
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

.PHONY: all run check clean

# Default target: build the executable
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)
	rm -f $(OBJS)

# Compile each .cpp file into an object file
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Run the executable and display its terminal output
run: $(TARGET)
	./$(TARGET)

# Run the executable and redirect the output to result.txt
check: $(TARGET)
	./$(TARGET) > result.txt

# Clean up executable and object files
clean:
	rm -f $(TARGET) $(OBJS) *.txt
