#!/bin/bash
exec ${PYTHON_BIN} -c "import sys; print(len(open(sys.argv[1]).readlines()))" "$@"