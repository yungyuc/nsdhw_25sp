CXX := g++

VENV := venv
PYTHON_BIN := $(VENV)/bin/python
PIP := $(VENV)/bin/pip

OPENBLAS_LIB := -lopenblas

CXXFLAGS := -O3 -Wall -shared -std=c++17 -fPIC -fopenmp

.PHONY: all ensure_openblas set_up_venv test clean

all: ensure_openblas set_up_venv _matrix.so

ensure_openblas:
	@echo "Checking for OpenBLAS library on Linux..."
	@if ldconfig -p | grep -q libopenblas.so; then \
	  echo "OpenBLAS library found."; \
	else \
	  echo "OpenBLAS library not found. Installing OpenBLAS..."; \
	  sudo apt-get update && sudo apt-get install -y libopenblas-dev; \
	fi

set_up_venv:
	if [ ! -d $(VENV) ]; then \
	  python3 -m venv $(VENV) && $(PIP) install pytest pybind11 numpy; \
	fi

_matrix.so: matrix.o mod.o
	$(CXX) $(CXXFLAGS) $(shell $(PYTHON_BIN) -m pybind11 --includes) -I$(shell $(PYTHON_BIN) -c "import numpy; print(numpy.get_include())") matrix.o mod.o -o _matrix.so $(OPENBLAS_LIB)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(shell $(PYTHON_BIN) -m pybind11 --includes) -I$(shell $(PYTHON_BIN) -c "import numpy; print(numpy.get_include())") -c $< -o $@

clean:
	rm -rf $(VENV) *.so __pycache__ *.pyc .pytest_cache *.o