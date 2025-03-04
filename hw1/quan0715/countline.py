#!/usr/bin/env python3
import sys
import os.path
# check PYTHON_BIN environment variable
python_bin = os.getenv('PYTHON_BIN', 'python3')

# print("PYTHON_BIN: ", python_bin)
if python_bin not in ['python3', 'python2']:
    sys.stderr.write(f"exec: {python_bin}: not found\n")
    sys.exit(1)

if len(sys.argv) < 2:
    sys.stdout.write('missing file name\n')
    sys.exit(1)
elif len(sys.argv) > 2:
    sys.stdout.write('only one argument is allowed\n')
    sys.exit(1)
else:
    fname = sys.argv[1]
    if os.path.exists(fname):
        with open(fname) as fobj:
            lines = fobj.readlines()
        sys.stdout.write('{0} lines in {1}\n'.format(len(lines), fname))
    else:
        sys.stdout.write('{0} not found\n'.format(fname))
        sys.exit(1)
