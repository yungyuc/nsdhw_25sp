CXX := g++
CXXFLAGS := -std=c++17 -O3 -fopenmp -fPIC -march=native

PYBIND11_INCLUDES := $(shell python3 -m pybind11 --includes)
PYTHON_INCLUDES := $(shell python3-config --includes)
PYTHON_LDFLAGS := $(shell python3-config --ldflags)
NUMPY_INCLUDES := $(shell python3 -c "import numpy; print(numpy.get_include())")
MKL_INC  := /usr/include/mkl
MKL_LIBS := -lmkl_rt

ALL_INCLUDES := $(PYBIND11_INCLUDES) \
                $(PYTHON_INCLUDES)     \
                -I$(NUMPY_INCLUDES)    \
                -I$(MKL_INC)

ALL_LIBS := $(MKL_LIBS) $(PYTHON_LDFLAGS)
SRCS := matrix.cpp mod.cpp
TARGET := _matrix$(shell python3-config --extension-suffix)

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(ALL_INCLUDES) -shared -o $@ $(SRCS) $(ALL_LIBS)

clean:
	rm -f $(TARGET)

test: $(TARGET)
	pytest -q 