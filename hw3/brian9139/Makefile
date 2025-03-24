CXX = g++
CXXFLAGS = -O2 -std=c++11 -fPIC
LDFLAGS = -lopenblas

all: matrix libmatrix.so

matrix: matrix.cpp
	$(CXX) $(CXXFLAGS) -o matrix matrix.cpp $(LDFLAGS)

libmatrix.so: matrix.cpp
	$(CXX) $(CXXFLAGS) -shared -o libmatrix.so matrix.cpp $(LDFLAGS)

# 修改 test 目標，讓它依賴 all (這樣 libmatrix.so 也會被建置)
test: all
	./matrix test

clean:
	rm -f matrix libmatrix.so performance.txt
