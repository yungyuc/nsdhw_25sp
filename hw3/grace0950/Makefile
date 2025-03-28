CXX = g++
CXXFLAGS = -O3 -Wall -std=c++11 -fPIC -march=native

# Virtual environment setup
VENV = venv
VENV_PYTHON = $(VENV)/bin/python
VENV_PIP = $(VENV)/bin/pip
VENV_PYTEST = $(VENV)/bin/pytest

# Force using virtual environment
PYTHON = $(VENV_PYTHON)
PYTEST = $(VENV_PYTEST)

# Check if virtual environment needs to be created
VENV_EXISTS = $(wildcard $(VENV)/bin/python)

# Use pybind11 from virtual environment
PYINC = $(shell if [ -f $(VENV_PYTHON) ]; then $(PYTHON) -m pybind11 --includes; fi)

# Fix extension issues
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    LDFLAGS = -shared -undefined dynamic_lookup
    BLAS_LINK = -framework Accelerate
    PYEXT = .so  # macOS uses .so
else ifeq ($(UNAME_S),Linux)
    LDFLAGS = -shared
    PYEXT = .so  # Linux uses .so
    # Linux tries to use OpenBLAS or MKL
    ifneq ($(wildcard /usr/include/mkl.h),)
        CXXFLAGS += -DUSE_MKL
        BLAS_LINK = -lmkl_rt
    else
        BLAS_LINK = -lopenblas
    endif
endif

TARGET = _matrix$(PYEXT)

SOURCES = matrix.cpp

# Absolute path of current directory
CWD := $(shell pwd)

.PHONY: all test benchmark clean venv

# All targets depend on virtual environment
all: venv $(TARGET) benchmark

# Create virtual environment
venv:
ifeq ($(VENV_EXISTS),)
	@echo "Creating virtual environment..."
	python3 -m venv $(VENV)
	$(VENV_PIP) install --upgrade pip
	$(VENV_PIP) install pytest pybind11
else
	@echo "Virtual environment already exists"
endif

$(TARGET): $(SOURCES) | venv
	$(CXX) $(CXXFLAGS) $(PYINC) $(LDFLAGS) -o $(TARGET) $(SOURCES) $(BLAS_LINK)

test: $(TARGET)
	cd $(CWD) && PYTHONPATH="$(CWD)" $(PYTEST) test_matrix.py -v

benchmark: $(TARGET)
	cd $(CWD) && PYTHONPATH="$(CWD)" $(PYTHON) test_matrix.py --benchmark


clean:
	rm -f $(TARGET)
	rm -rf __pycache__
	rm -rf .pytest_cache

distclean: clean
	rm -rf $(VENV)