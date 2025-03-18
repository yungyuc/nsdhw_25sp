CXX := g++

VENV := venv
PYTHON_BIN := $(VENV)/bin/python3
PIP := $(VENV)/bin/pip

CXXFLAGS := -O3 -Wall -shared -std=c++11 -fPIC

.PHONY: all set_up_venv test clean

all: set_up_venv _vector.so

set_up_venv:
	if [ ! -d $(VENV) ]; then python3 -m venv $(VENV) && $(PIP) install pytest pybind11; fi

_vector.so: bindings.cpp calc_angle.hpp
	$(CXX) $(CXXFLAGS) $(shell $(PYTHON_BIN) -m pybind11 --includes) bindings.cpp -o _vector.so

test: _vector.so unit_test.py
	$(PYTHON_BIN) -m pytest unit_test.py

clean:
	rm -rf $(VENV) *.so __pycache__ *.pyc .pytest_cache