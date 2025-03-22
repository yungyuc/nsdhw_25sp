# Makefile for q1 - Line class compilation

# 編譯器設定
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11

# 目標檔
TARGET = line

# 來源檔
SRCS = line.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)

# 默認目標
all: $(TARGET)

# 連結規則
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# 編譯規則
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 運行程序
run: $(TARGET)
	./$(TARGET)

# 檢查輸出
check: $(TARGET)
	./$(TARGET) > result.txt

# 清理規則
clean:
	rm -f $(OBJS) $(TARGET) result.txt

.PHONY: all clean run check