CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O3 -fPIC

PYTHON_VERSION := $(shell python3 -c "import sys; print('python{}.{}'.format(sys.version_info.major, sys.version_info.minor))")
PYTHON_INCLUDE := $(shell python3 -c "from sysconfig import get_path; print(get_path('include'))")

PYBIND11_INCLUDE := $(shell python3 -m pybind11 --includes 2>/dev/null | sed 's/-I//')
ifeq ($(PYBIND11_INCLUDE),)
    PYBIND11_INCLUDE := /usr/include/pybind11
endif

.PHONY: all test clean install check_deps

check_deps:
	@command -v python3 >/dev/null 2>&1 || { echo >&2 "Python3 is not installed. Aborting."; exit 1; }
	@python3 -c "import pybind11" >/dev/null 2>&1 || { echo >&2 "pybind11 is not installed. Aborting."; exit 1; }
	@python3 -c "import pytest" >/dev/null 2>&1 || { echo >&2 "pytest is not installed. Aborting."; exit 1; }

all: check_deps _vector.so

_vector.so: vectormath.o
	$(CXX) -shared -o _vector.so vectormath.o -fPIC -I$(PYTHON_INCLUDE) -I$(PYBIND11_INCLUDE) -l$(PYTHON_VERSION)

vectormath.o: vectormath.cpp
	$(CXX) $(CXXFLAGS) -c vectormath.cpp -I$(PYTHON_INCLUDE) -I$(PYBIND11_INCLUDE)

test: all
	PYTHONPATH=".:$(PYTHONPATH)" python3 -m pytest -v test_vectormath.py

clean:
	rm -f *.o _vector.so
	rm -rf __pycache__ .pytest_cache

install:
	python3 -m pip install --upgrade pip
	python3 -m pip install pybind11 pytest
