SHELL = /bin/bash

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -shared -fPIC `python3 -m pybind11 --includes` `python3-config --includes`
LDFLAGS = `python3-config --ldflags` 

SRC = _vector.cpp
TARGET = _vector.so  
TEST = test_vector.py

all: $(TARGET)

install-deps:
	python3 -m pip install --user pybind11 pytest

$(TARGET): install-deps $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

.PHONY: test clean

test:
	python3 -m pytest

clean:
	rm -f $(TARGET)
