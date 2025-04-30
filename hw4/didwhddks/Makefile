CXX := g++

VENV := venv
PYTHON_BIN := $(VENV)/bin/python
PIP := $(VENV)/bin/pip

OPENBLAS_LIB := -lopenblas

CXXFLAGS := -O3 -Wall -shared -std=c++11 -fPIC -fopenmp

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
	  python3 -m venv $(VENV) && $(PIP) install pytest pybind11; \
	fi

_matrix.so: matrix.cpp
	$(CXX) $(CXXFLAGS) $(shell $(PYTHON_BIN) -m pybind11 --includes) matrix.cpp -o _matrix.so $(OPENBLAS_LIB)

test: ensure_openblas set_up_venv _matrix.so test_matrix.py
	$(PYTHON_BIN) -m pytest test_matrix.py

test_performance: ensure_openblas set_up_venv _matrix.so test_performance.py
	$(PYTHON_BIN) test_performance.py

clean:
	rm -rf $(VENV) *.so __pycache__ *.pyc .pytest_cache