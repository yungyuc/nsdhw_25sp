#!/usr/bin/env python3

import sys
import os
import subprocess
python_bin = os.getenv("PYTHON_BIN","python3")

try:
    subprocess.run([python_bin,"--version"], check=True, stdout=subprocess.DEVNULL, stderr=subprocess.PIPE)
except FileNotFoundError:
    print(f"exec: {python_bin}: not found", file=sys.stderr)
    sys.exit(1)
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
