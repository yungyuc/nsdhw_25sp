SRC_DIR := .
BUILD_DIR := .
TEST_DIR := .

VENV := .env
PIP_VENV := $(VENV)/bin/pip
PYTHON_VENV := $(VENV)/bin/python3


MODULE_NAME := angle
SUFFIX := cpython-312-x86_64-linux-gnu.so

TARGET := $(BUILD_DIR)/_vector.so
SOURCES := $(SRC_DIR)/${MODULE_NAME}.cpp

.PHONY: all test clean setup_venv

all: setup_venv $(TARGET)

setup_venv:
	@if [ ! -d "$(VENV)" ]; then \
		echo "Creating virtual environment..."; \
		python3 -m venv $(VENV); \
		$(PIP_VENV) install pybind11 pytest; \
	fi

test: all
	@PYTHONPATH=. $(PYTHON_VENV) -m pytest -v test_angle.py

clean:
	rm $(TARGET)
	rm -rf __pycache__ .pytest_cache $(VENV)

$(TARGET): $(SRC_DIR)/${MODULE_NAME}.cpp 
	c++ -O3 -Wall -shared -std=c++11 -fPIC $$($(PYTHON_VENV) -m pybind11 --includes) ${SRC_DIR}/${MODULE_NAME}.cpp -o _vector.so
