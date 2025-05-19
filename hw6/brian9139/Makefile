CXX      := g++
PY_INCL  := $(shell python3-config --includes)
NP_INCL  := -I$(shell python3 -c "import numpy; print(numpy.get_include())")
CXXFLAGS := -O3 -std=c++17 -fPIC $(PY_INCL) $(NP_INCL)
LDFLAGS  := -shared
EXTSUF   := $(shell python3-config --extension-suffix)
TARGET   := _matrix$(EXTSUF)

SRCS     := matrix.cpp matrix_py.cpp
OBJS     := $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@

%.o: %.cpp matrix.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)
