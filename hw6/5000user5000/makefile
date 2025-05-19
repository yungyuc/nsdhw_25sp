# Compiler and flags
CXX := g++
CXXFLAGS := -O2 -march=native -std=c++17 -fPIC

# Python executable and its include/ldflags
PYTHON := python3
PYTHON_VERSION := $(shell $(PYTHON) -c "import sys; print('python{}.{}'.format(sys.version_info.major, sys.version_info.minor))")
PYTHON_INCLUDE := $(shell $(PYTHON) -c "from sysconfig import get_path; print(get_path('include'))")
PYTHON_LDFLAGS := $(shell $(PYTHON)-config --ldflags)

# Pybind11 include path (with fallback)
PYBIND11_INCLUDE := $(shell $(PYTHON) -m pybind11 --includes 2>/dev/null | sed 's/-I//')
ifeq ($(PYBIND11_INCLUDE),)
	PYBIND11_INCLUDE := /usr/include/pybind11
endif

# NumPy include path
NUMPY_INCLUDE := $(shell $(PYTHON) -c "import numpy; print(numpy.get_include())")

# MKL settings (optional fallback to BLAS if MKL not available)
MKL_INCLUDE := /usr/include/mkl
MKL_LIBS := -lmkl_rt -lpthread -lm -ldl   # Intel MKL runtime libraries

# Aggregate include paths
INCLUDES := -I$(PYTHON_INCLUDE) -I$(PYBIND11_INCLUDE) -I$(NUMPY_INCLUDE) -I$(MKL_INCLUDE) -I.

# Link flags: Python + MKL
LDFLAGS := -shared $(PYTHON_LDFLAGS) $(MKL_LIBS)

# Extension suffix for Python module
TARGET := _matrix$(shell $(PYTHON)-config --extension-suffix)

# Object files
OBJ1 := mod.o
OBJ2 := matrix.o

.PHONY: all clean

all: $(TARGET)

# Compile the provided wrapper (mod.cpp) into mod.o
# mod.cpp is located in the parent directory (hw5/)
mod.o: mod.cpp matrix.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c mod.cpp

# Compile your implementation
matrix.o: matrix.cpp matrix.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c matrix.cpp

# Link into a Python extension
$(TARGET): $(OBJ1) $(OBJ2)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(OBJ1) $(OBJ2) $(TARGET)
