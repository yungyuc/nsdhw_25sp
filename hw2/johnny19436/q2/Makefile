PYTHON ?= python3
CXX = g++
# PYBIND_INCLUDES = $(shell $(PYTHON) -m pybind11 --includes | tr ' ' '\n' | grep 'pybind11/include')
# PYBIND_INCLUDES = $(shell $(PYTHON) -m pybind11 --includes | tr ' ' '\n' | grep 'pybind11/include' 2>/dev/null | sed 's/-I//')
PYTHON_VERSION := $(shell python3 -c "import sys; print('python{}.{}'.format(sys.version_info.major, sys.version_info.minor))")
PYBIND_INCLUDES = $(shell $(PYTHON) -m pybind11 --includes | tr ' ' '\n' | grep 'pybind11/include' 2>/dev/null)
ifeq ($(PYBIND_INCLUDES),)
    PYBIND_INCLUDES = /usr/include/pybind11
endif

PYTHON_INCLUDE = $(shell $(PYTHON)-config --includes)
PYTHON_LIBS = $(shell $(PYTHON)-config --ldflags) -l$(PYTHON_VERSION)

CXXFLAGS = -O3 -Wall -shared -std=c++14 -fPIC $(PYBIND_INCLUDES) $(PYTHON_INCLUDE) 
LDFLAGS = $(PYTHON_LIBS)

EXT_SUFFIX = $(shell $(PYTHON)-config --extension-suffix)
SRC = _vector.cpp
TARGET = _vector$(EXT_SUFFIX)

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $< -o $@

test: all
	env PYTHONPATH=".:$PYTHONPATH" $(PYTHON) -m pytest -v

clean:
	rm -f $(TARGET)
	find . -type f -name '*.so' -delete
	rm -rf __pycache__ .pytest_cache
