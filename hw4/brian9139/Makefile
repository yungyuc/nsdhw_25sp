# Makefile
CXX      = g++
CXXFLAGS = -O2 -std=c++11 -fPIC -DNO_BLAS
LDFLAGS  =

all: matrix libmatrix.so

matrix: matrix.cpp
	$(CXX) $(CXXFLAGS) -o matrix matrix.cpp $(LDFLAGS)

libmatrix.so: matrix.cpp
	$(CXX) $(CXXFLAGS) -shared -o libmatrix.so matrix.cpp $(LDFLAGS)

test: all
	./matrix test

clean:
	rm -f matrix libmatrix.so performance.txt
