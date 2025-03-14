CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O3 -fPIC

VENV := .venv
PYTHON_VENV := $(VENV)/bin/python3
PIP_VENV := $(VENV)/bin/pip

# 自動獲取 Python 和 pybind11 路徑
PYTHON_INCLUDE := $(shell $(PYTHON_VENV) -c "from sysconfig import get_path; print(get_path('include'))")
PYBIND11_INCLUDE := $(shell $(PYTHON_VENV) -m pybind11 --includes 2>/dev/null | sed 's/-I//')

.PHONY: all test clean _setup_venv

all: _setup_venv _vector.so

# 私有目標：設置虛擬環境並安裝依賴
_setup_venv:
	@if [ ! -d "$(VENV)" ]; then \
		echo "Creating virtual environment..."; \
		python3 -m venv $(VENV); \
		$(PIP_VENV) install pybind11 pytest; \
	fi

# 編譯 C++ 模塊
_vector.so: vectormath.cpp _setup_venv
	$(CXX) $(CXXFLAGS) -shared -fPIC \
		-I$(PYTHON_INCLUDE) -I$(PYBIND11_INCLUDE) \
		vectormath.cpp -o _vector.so \
		$(shell $(PYTHON_VENV) -m pybind11 --includes)

# 運行測試
test: _vector.so
	PYTHONPATH=. $(PYTHON_VENV) -m pytest -v test_vectormath.py

# 清理所有生成的文件（包括虛擬環境）
clean:
	rm -f _vector.so
	rm -rf __pycache__ .pytest_cache $(VENV)
