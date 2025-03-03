#!/bin/bash
exec ${PYTHON_BIN} -c "import sys; print(open(sys.argv[1], 'r').read().count('\n'))" "$@"