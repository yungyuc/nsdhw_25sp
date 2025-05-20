PYTHON := python3
CXX := g++
UNAME_S := $(shell uname -s)

PYTHON_VERSION = $(shell $(PYTHON) -c "import sys; print('python{}.{}'.format(sys.version_info.major, sys.version_info.minor))")
PYTHON_INCLUDE = $(shell $(PYTHON)-config --includes)
PYTHON_LDFLAGS := $(shell $(PYTHON)-config --ldflags)
NUMPY_INCLUDES := $(shell $(PYTHON) -c "import numpy; print(numpy.get_include())")

# Pybind11 include path
PYBIND11_INCLUDE := $(shell $(PYTHON) -m pybind11 --includes 2>/dev/null | sed 's/-I//')
ifeq ($(PYBIND11_INCLUDE),)
	PYBIND11_INCLUDE := /usr/include/pybind11
endif

# OS-specific BLAS/MKL flags
ifeq ($(UNAME_S),Darwin)
    # For macOS, assume OpenBLAS from Homebrew and add dynamic lookup flag for Python extensions
    BLAS_INC := /opt/homebrew/opt/openblas/include
    BLAS_LIB := -L/opt/homebrew/opt/openblas/lib -lopenblas
    DYNAMIC_LOOKUP := -undefined dynamic_lookup
else ifeq ($(UNAME_S),Linux)
    BLAS_INC := /usr/include/mkl
    BLAS_LIB := -lmkl_rt -lpthread -lm -ldl
    DYNAMIC_LOOKUP :=
else
    $(error "Unsupported OS: $(UNAME_S)")
endif

CXXFLAGS := -O3 -march=native -std=c++14 -fPIC
INCLUDES := -I. -I$(PYBIND11_INCLUDE) -I$(BLAS_INC) -I$(NUMPY_INCLUDES) $(PYTHON_INCLUDE)
LDFLAGS := $(PYTHON_LDFLAGS) $(BLAS_LIB)

SRCS := matrix.cpp mod.cpp
TARGET := _matrix$(shell $(PYTHON)-config --extension-suffix)

.PHONY: all test clean

all: $(TARGET)

# Use BUILD_PYBIND for module definition in mod.cpp
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -DBUILD_PYBIND -shared $(DYNAMIC_LOOKUP) -o $@ $(SRCS) $(INCLUDES) $(LDFLAGS)

test: all
	$(PYTHON) -m pytest --maxfail=1 --disable-warnings -q
	
clean:
	rm -f $(TARGET)
	find . -type f -name '*.so' -delete
	rm -rf __pycache__ .pytest_cache