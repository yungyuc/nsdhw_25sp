#!/usr/bin/env python3

import sys
import os.path
import subprocess
import shutil

# prevent from recursively calling script ifself
if "--no-reexec" not in sys.argv:
    python_bin = os.getenv("PYTHON_BIN")
    if python_bin: # python_bin specified 
        python_path = shutil.which(python_bin)
        if python_path == None:
            sys.stderr.write("exec: {}: not found\n".format(python_bin))
            sys.exit(1)
        else: 
            subprocess.call([python_bin] + sys.argv + ["--no-reexec"])
            sys.exit(0)


# count line
args = [arg for arg in sys.argv if arg != "--no-reexec"]
if len(args) < 2:
    sys.stdout.write('missing file name\n')
elif len(args) > 2:
    sys.stdout.write('only one argument is allowed\n')
else:
    fname = args[1]
    if os.path.exists(fname):
        with open(fname) as fobj:
            lines = fobj.readlines()
        sys.stdout.write('{} lines in {}\n'.format(len(lines), fname))
    else:
        sys.stdout.write('{} not found\n'.format(fname))
        
