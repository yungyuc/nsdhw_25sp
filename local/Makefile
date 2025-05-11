CFLAG := -O3 -shared -std=c++11 -fPIC
LFLAG := -shared
C++ := g++
MKLFLAG := -lblas

PYBIND_INCLUDE = $(shell python3 -m pybind11 --includes 2>/dev/null || echo "# missing pybind11")

.PHONY: all test clean push check_env

all:  check_env _matrix.so check_env

check_env:
	@echo "Checking if pybind11 and pytest are installed..."
	@if ! python3 -m pip show pybind11 > /dev/null; then \
		echo "pybind11 is not installed. Installing..."; \
		python3 -m pip install pybind11; \
		echo "Pybind path: $(PYBIND_INCLUDE)";\
	else \
		echo "pybind11 is already installed."; \
	fi
	@if ! python3 -m pip show pytest > /dev/null; then \
		echo "pytest is not installed. Installing..."; \
		python3 -m pip install pytest; \
	else \
		echo "pytest is already installed."; \
	fi

test: all
	python3 -m pytest -xvs test_matrix.py

test_cpp: _matrix.o _matrix_test.o
	$(C++) $^ -o $@ $(MKLFLAG) ;\
	chmod +x $@ ;\
	./$@


clean:
	@echo "doing make clean"
	@if [ -f _matrix.so ]; then rm _matrix.so; fi
	@if [ -f _matrix_mod.o ]; then rm _matrix_mod.o; fi
	@if [ -f _matrix.o ]; then rm _matrix.o ; fi
	@if [ -f _matrix_test.o ]; then rm _matrix_test.o ; fi
	@if [ -f test_cpp ]; then rm test_cpp ; fi
	@if [ -d  __pycache__ ]; then rm -rf __pycache__ ; fi



push:
	git add *; git commit -m "temp"; git push origin

_matrix.so: _matrix.o _matrix_mod.o
	@g++ ${LFLAG}  $^ -o $@ $(MKLFLAG)

_matrix.o: _matrix.cpp _matrix.hpp
	@$(C++) $(CFLAG) $(PYBIND_INCLUDE) $(MKLFLAG) -c $< -o $@ > /dev/null

_matrix_mod.o: _matrix_mod.cpp _matrix.hpp
	@echo "Pybind path: $(PYBIND_INCLUDE)"
	@$(C++) $(CFLAG) $(PYBIND_INCLUDE) -c $< -o $@ 2> /dev/null

_matrix_test.o: _matrix_test.cpp _matrix.hpp
	@$(C++) -c $< -o $@

echo:
	echo "Pybind path: $(PYBIND_INCLUDE)"