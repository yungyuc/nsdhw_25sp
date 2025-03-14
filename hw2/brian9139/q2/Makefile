PYTHON_INC := $(shell python3 -m pybind11 --includes)
PYTHON_LIB := $(shell python3-config --ldflags)

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2

all: _vector.so

_vector.so: vector_angle.o bindings.o
	$(CXX) -shared -o $@ $^ $(PYTHON_LIB)

vector_angle.o: vector_angle.cpp vector_angle.h
	$(CXX) $(CXXFLAGS) -fPIC -c $< -o $@

bindings.o: bindings.cpp vector_angle.h
	$(CXX) $(CXXFLAGS) -fPIC $(PYTHON_INC) -c $< -o $@

test: _vector.so
	pytest -v test_vector.py

clean:
	rm -f *.o _vector.so


