# use system python
PYTHON := python3

# module name
MODULE_NAME := vector

# system type
UNAME_S := $(shell uname -s)

# compiler
CXX := g++
CXXFLAGS := -O3 -Wall -std=c++11 -fPIC

# auto get python and pybind11 compile parameters
PYBIND_INCLUDES := $(shell $(PYTHON) -c "import pybind11; print('-I' + pybind11.get_include() + ' -I' + pybind11.get_include(True))" 2>/dev/null || echo "-I/usr/include/python3.9 -I/usr/include/python3.9")
PYTHON_INCLUDES := $(shell $(PYTHON) -c "import sysconfig; print('-I' + sysconfig.get_config_var('INCLUDEPY'))" 2>/dev/null || echo "")

# different compile and link parameters based on system
PYTHON_VERSION := $(shell $(PYTHON) -c "import sys; print('{}.{}'.format(sys.version_info.major, sys.version_info.minor))" 2>/dev/null || echo "3.9")
PYTHON_LIBDIR := $(shell $(PYTHON) -c "import sysconfig; print(sysconfig.get_config_var('LIBDIR'))" 2>/dev/null || echo "/usr/lib")
LDFLAGS := -L$(PYTHON_LIBDIR) -lpython$(PYTHON_VERSION)

# 完整的編譯參數
CXXFLAGS += $(PYTHON_INCLUDES) $(PYBIND_INCLUDES)

TARGET := _vector.so
SOURCES := vector.cpp

.PHONY: all test run check clean

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -shared $^ -o $@ $(LDFLAGS)

test: $(TARGET)
	PYTHONPATH=".:" $(PYTHON) -m pytest -v

clean:
	rm -rf $(TARGET) __pycache__ .pytest_cache *.o *.so