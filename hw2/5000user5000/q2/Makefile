PYTHON_VERSION := $(shell python3 -c "import sys; print('python{}.{}'.format(sys.version_info.major, sys.version_info.minor))")
PYTHON_INCLUDE := $(shell python3 -c "from sysconfig import get_path; print(get_path('include'))")
PYBIND11_INCLUDE := $(shell python3 -m pybind11 --includes | sed 's/-I//')

all: _vector.so

_vector.so: vector_angle.o pybind_wrapper.o
	g++ -shared -o _vector.so vector_angle.o pybind_wrapper.o -fPIC -I$(PYTHON_INCLUDE) -I$(PYBIND11_INCLUDE) -l$(PYTHON_VERSION)

vector_angle.o: vector_angle.cpp vector_angle.hpp
	g++ -c -fPIC vector_angle.cpp -I$(PYTHON_INCLUDE) -I$(PYBIND11_INCLUDE)

pybind_wrapper.o: pybind_wrapper.cpp
	g++ -c -fPIC pybind_wrapper.cpp -I$(PYTHON_INCLUDE) -I$(PYBIND11_INCLUDE)

test:
	PYTHONPATH=".:$PYTHONPATH" pytest -v test_vector_angle.py

clean:
	rm -f *.o _vector.so