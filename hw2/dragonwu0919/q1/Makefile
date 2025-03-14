SRC_DIR := .
BUILD_DIR := .

TARGET := $(BUILD_DIR)/line_app
SOURCES := $(SRC_DIR)/main.cpp

.PHONY: all run check clean

all: $(TARGET)

run: all
	./$(TARGET)

check: all
	./$(TARGET) > result.txt

clean:
	rm $(TARGET) $(BUILD_DIR)/main.o $(BUILD_DIR)/line.o result.txt

$(TARGET): $(BUILD_DIR)/main.o $(BUILD_DIR)/line.o
	@mkdir -p $(BUILD_DIR)
	g++ $^ -o $@
	chmod +x $(TARGET)

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp $(SRC_DIR)/line.hpp
	@mkdir -p $(BUILD_DIR)
	g++ -c $< -o $@

$(BUILD_DIR)/line.o: $(SRC_DIR)/line.cpp $(SRC_DIR)/line.hpp
	@mkdir -p $(BUILD_DIR)
	g++ -c $< -o $@

