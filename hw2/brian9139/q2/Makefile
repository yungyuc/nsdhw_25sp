PYBIND11_INC := $(shell python3 -m pybind11 --includes 2>/dev/null)

ifeq ($(PYBIND11_INC),)
$(warning "pybind11 not found, installing...")
RUN_PYTHON := pip3 install --user pybind11
endif

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2 -fPIC

all: _vector.so

_vector.so: vector_angle.o bindings.o
	$(CXX) -shared -o $@ $^ $(shell python3-config --ldflags)

vector_angle.o: vector_angle.cpp vector_angle.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

bindings.o: bindings.cpp vector_angle.h
	$(RUN_PYTHON)
	$(CXX) $(CXXFLAGS) $(PYBIND11_INC) -c $< -o $@

clean:
	rm -f *.o _vector.so || true

test:
	python3 -m pytest -v test_vector.py  # ✅ 確保用 python3 執行 pytest

