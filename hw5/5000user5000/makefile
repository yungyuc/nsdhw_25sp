CXX = c++
CXXFLAGS = -O2 -Wall -std=c++17 -fPIC
LDFLAGS = -shared
PYTHON_INCLUDES = $(shell python3 -m pybind11 --includes)
NUMPY_INCLUDE = $(shell python3 -c "import numpy; print(numpy.get_include())")
LIBS = -lm -lblas

TARGET = _matrix$(PYTHON_SOEXT)
PYTHON_SOEXT := $(shell python3-config --extension-suffix)

SRCS = ../mod.cpp matrix.cpp
OBJS = $(SRCS:.cpp=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(PYTHON_INCLUDES) -I$(NUMPY_INCLUDE) -I. -c $< -o $@

clean:
	rm -f *.o _matrix*.so ../*.o
