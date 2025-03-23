CXX = g++
CXXFLAGS = -O3 -Wall -std=c++11 -fPIC

# 虛擬環境設置
VENV = venv
VENV_PYTHON = $(VENV)/bin/python
VENV_PIP = $(VENV)/bin/pip
VENV_PYTEST = $(VENV)/bin/pytest

# 強制使用虛擬環境
PYTHON = $(VENV_PYTHON)
PYTEST = $(VENV_PYTEST)

# 檢查虛擬環境是否需要創建
VENV_EXISTS = $(wildcard $(VENV)/bin/python)

PYINC = $(shell if [ -f $(VENV_PYTHON) ]; then $(PYTHON) -m pybind11 --includes; fi)

# 特別為macOS設置
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    LDFLAGS = -shared -undefined dynamic_lookup
else
    LDFLAGS = -shared
endif

TARGET = _vector.so
SOURCES = vector.cpp binding.cpp

.PHONY: all test clean venv

# 所有目標都依賴虛擬環境
all: venv $(TARGET)

# 創建虛擬環境
venv:
ifeq ($(VENV_EXISTS),)
	@echo "正在創建虛擬環境..."
	python3 -m venv $(VENV)
	$(VENV_PIP) install --upgrade pip
	$(VENV_PIP) install pytest pybind11
else
	@echo "虛擬環境已存在"
endif

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(PYINC) $(SOURCES) -o $(TARGET) $(LDFLAGS)

# 測試前確保虛擬環境存在
test: venv $(TARGET)
	PYTHONPATH=".:${PYTHONPATH}" $(PYTHON) -m pytest -v test_vector.py

clean:
	rm -f $(TARGET)
	rm -rf __pycache__
	rm -rf .pytest_cache
	rm -rf $(VENV)
