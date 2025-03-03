#!/bin/bash

# 從環境變數獲取 Python 執行器，若未指定則預設使用 "python"
PYTHON_CMD=${PYTHON_BIN:-python}

# 使用 exec 來執行 Python，這樣會產生 "exec: python0: not found" 的錯誤
exec $PYTHON_CMD - "$@" << 'EOF'
import sys
import os.path
if len(sys.argv) < 2:
    sys.stdout.write('missing file name\n')
elif len(sys.argv) > 2:
    sys.stdout.write('only one argument is allowed\n')
else:
    fname = sys.argv[1]
    if os.path.exists(fname):
        with open(fname) as fobj:
            lines = fobj.readlines()
        sys.stdout.write('{} lines in {}\n'.format(len(lines), fname))
    else:
        sys.stdout.write('{} not found\n'.format(fname))
EOF
