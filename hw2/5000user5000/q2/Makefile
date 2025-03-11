CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O3 -fPIC 

# 確保 Python 頭文件正確
PYTHON_VERSION := $(shell python3 -c "import sys; print('python{}.{}'.format(sys.version_info.major, sys.version_info.minor))")
PYTHON_INCLUDE := $(shell python3 -c "from sysconfig import get_path; print(get_path('include'))")

# 檢查 pybind11 是否可用，否則使用默認路徑
PYBIND11_INCLUDE := $(shell python3 -m pybind11 --includes 2>/dev/null | sed 's/-I//')
ifeq ($(PYBIND11_INCLUDE),)
    PYBIND11_INCLUDE := /usr/include/pybind11
endif

.PHONY: all test clean

all: _vector.so

_vector.so: vector_angle.o pybind_wrapper.o
	$(CXX) -shared -o _vector.so vector_angle.o pybind_wrapper.o -fPIC -I$(PYTHON_INCLUDE) -I$(PYBIND11_INCLUDE) -l$(PYTHON_VERSION)

vector_angle.o: vector_angle.cpp vector_angle.hpp
	$(CXX) $(CXXFLAGS) -c vector_angle.cpp -I$(PYTHON_INCLUDE) -I$(PYBIND11_INCLUDE)

pybind_wrapper.o: pybind_wrapper.cpp
	$(CXX) $(CXXFLAGS) -c pybind_wrapper.cpp -I$(PYTHON_INCLUDE) -I$(PYBIND11_INCLUDE)

test:
	PYTHONPATH=".:$PYTHONPATH" pytest -v test_vector_angle.py

clean:
	rm -f *.o _vector.so
	rm -rf __pycache__ .pytest_cache
