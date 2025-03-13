PYTHON ?= python3
CXX = c++
PYBIND_INCLUDES = $(shell $(PYTHON) -m pybind11 --includes | tr ' ' '\n' | grep 'pybind11/include')
PYTHON_INCLUDE = $(shell $(PYTHON)-config --includes)
PYTHON_LIBS = $(shell $(PYTHON)-config --ldflags)

CXXFLAGS = -O3 -Wall -shared -std=c++14 -fPIC $(PYBIND_INCLUDES) $(PYTHON_INCLUDE) -undefined dynamic_lookup
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
