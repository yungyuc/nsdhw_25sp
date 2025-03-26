CXX = g++
CXXFLAGS = -O2 -std=c++11 -fPIC

# 如果在 GitHub Actions 環境下（GITHUB_ACTIONS 變數存在），則使用 fallback 模式，不連結 BLAS
ifdef GITHUB_ACTIONS
    CXXFLAGS += -DNO_BLAS
    LDFLAGS =
else
    # 本機環境下，假設你安裝了 OpenBLAS，就用 -lopenblas
    LDFLAGS = -lopenblas
endif

all: matrix libmatrix.so

matrix: matrix.cpp
	$(CXX) $(CXXFLAGS) -o matrix matrix.cpp $(LDFLAGS)

libmatrix.so: matrix.cpp
	$(CXX) $(CXXFLAGS) -shared -o libmatrix.so matrix.cpp $(LDFLAGS)

test: all
	./matrix test

clean:
	rm -f matrix libmatrix.so performance.txt
