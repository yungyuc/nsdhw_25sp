CXX := g++
CXXFLAGS := -O3 -march=native -std=c++11 -fPIC
PYTHON := python3

PYTHON_VERSION := $(shell $(PYTHON) -c "import sys; print('python{}.{}'.format(sys.version_info.major, sys.version_info.minor))")
PYTHON_INCLUDE := $(shell $(PYTHON) -c "from sysconfig import get_path; print(get_path('include'))")
PYTHON_LDFLAGS := $(shell $(PYTHON)-config --ldflags)

# Pybind11 include path
PYBIND11_INCLUDE := $(shell $(PYTHON) -m pybind11 --includes 2>/dev/null | sed 's/-I//')
ifeq ($(PYBIND11_INCLUDE),)
	PYBIND11_INCLUDE := /usr/include/pybind11
endif

# === MKL 設定 ===
MKL_INC := /usr/include/mkl
MKL_LIB := -lmkl_rt -lpthread -lm -ldl
INCLUDES := -I$(PYBIND11_INCLUDE) -I$(MKL_INC) -I$(PYTHON_INCLUDE)
LDFLAGS := $(PYTHON_LDFLAGS) $(MKL_LIB)

# Output .so name for import _matrix
SO_NAME := _matrix$(shell $(PYTHON)-config --extension-suffix)

all: matrix_bench $(SO_NAME)

matrix_bench: matrix.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(INCLUDES) $(LDFLAGS)

$(SO_NAME): matrix.cpp
	$(CXX) $(CXXFLAGS) -DBUILD_PYBIND -shared -o $@ $< $(INCLUDES) $(LDFLAGS)

# pytest will automatically load _matrix*.so
test: $(SO_NAME)
	$(PYTHON) -m pytest --maxfail=1 --disable-warnings -q

clean:
	rm -f matrix_bench _matrix*.so performance.txt