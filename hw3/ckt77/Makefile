CXX = g++
CXXFLAGS = -std=c++11 -O3 -Wall -fPIC
LDFLAGS = -shared

# 检测系统是否支持 OpenMP
OPENMP_CHECK := $(shell $(CXX) -fopenmp -dM -E - < /dev/null 2>/dev/null | grep -q "_OPENMP" && echo "yes" || echo "no")
ifeq ($(OPENMP_CHECK), yes)
    CXXFLAGS += -fopenmp
    LDFLAGS += -fopenmp
endif

# 使用 python3-config 查找 Python 头文件路径
PYTHON_INCLUDE := $(shell python3 -c "import sysconfig; print(sysconfig.get_path('include'))" 2>/dev/null || echo "/usr/include/python3")
PYTHON_CONFIG := $(shell which python3-config 2>/dev/null)

ifneq ($(PYTHON_CONFIG),)
    PYBIND_INCLUDES := $(shell python3 -m pybind11 --includes 2>/dev/null || python3-config --includes)
else
    PYBIND_INCLUDES := -I$(PYTHON_INCLUDE) -I/usr/include/python3.8 -I/usr/local/include/pybind11
endif

# 检查 MKL 库路径
MKL_PATHS := /usr/include/mkl /opt/intel/mkl/include /usr/local/include/mkl
MKL_CHECK := $(shell for p in $(MKL_PATHS); do if [ -d $$p ]; then echo $$p; break; fi; done)
ifneq ($(MKL_CHECK),)
    MKL_INCLUDE := -I$(MKL_CHECK)
    CXXFLAGS += -DHAS_MKL
else
    MKL_INCLUDE :=
    $(warning MKL include path not found, using alternative implementation)
endif

# MKL 库
MKL_LIBS = -lmkl_rt -lpthread -lm -ldl
ifneq ($(OPENMP_CHECK), yes)
    MKL_LIBS := $(filter-out -fopenmp,$(MKL_LIBS))
endif

PYTHON = python3
PYTEST = python3 -m pytest

all: _matrix.so performance.txt

matrix.o: matrix.cpp matrix.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

matrix_mul.o: matrix_mul.cpp matrix.hpp
	$(CXX) $(CXXFLAGS) $(MKL_INCLUDE) -c $< -o $@

matrix_mod.o: matrix_mod.cpp matrix.hpp
	$(CXX) $(CXXFLAGS) $(PYBIND_INCLUDES) -c $< -o $@

_matrix.so: matrix.o matrix_mul.o matrix_mod.o
	$(CXX) $(LDFLAGS) $^ -o $@ $(MKL_LIBS)

performance.txt: _matrix.so benchmark.py
	$(PYTHON) benchmark.py

test: _matrix.so
	$(PYTEST) -xvs test_matrix.py

clean:
	rm -f *.o *.so performance.txt
	rm -rf __pycache__ .pytest_cache

.PHONY: all test clean
