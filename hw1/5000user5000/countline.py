#!/usr/bin/env python3

import sys
import os.path

# Get env var
python_bin = os.getenv("PYTHON_BIN")


if python_bin is None:
    print("Error: Python_BIN is not set.",file=sys.stderr)
    sys.exit(1)

if python_bin != "python2" and python_bin != "python3":
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
        sys.stdout.write('{}\n'.format(len(lines)))
    else:
        sys.stdout.write('{} not found\n'.format(fname))
