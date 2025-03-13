CXX = g++
CXXFLAGS = -O3 -Wall -shared -std=c++11 -fPIC
PYTHON_INCLUDE = $(shell python3 -c "import sysconfig; print(sysconfig.get_paths()['include'])")
PYBIND11_INCLUDE = $(shell python3 -c "import pybind11; print(pybind11.get_include())")

all: _vector.so

_vector.so: vectormath.cpp
	$(CXX) $(CXXFLAGS) -I$(PYTHON_INCLUDE) -I$(PYBIND11_INCLUDE) vectormath.cpp -o _vector.so

test: _vector.so
	python3 -m pytest -v test_vectormath.py

clean:
	rm -f *.so *.o
	rm -rf __pycache__
	rm -rf .pytest_cache

.PHONY: all test clean

