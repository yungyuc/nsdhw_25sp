#!/usr/bin/env python3

import sys
import os
import shutil

def main():
    python_bin = os.getenv("PYTHON_BIN", "python3")
    if not shutil.which(python_bin):
        sys.stderr.write(f"exec: {python_bin}: not found\n")
        sys.exit(1)
    
    if len(sys.argv) < 2:
        sys.stdout.write("missing file name\n")
        sys.exit(1)
    elif len(sys.argv) > 2:
        sys.stdout.write("only one argument is allowed\n")
        sys.exit(1)
    
    fname = sys.argv[1]
    if os.path.exists(fname):
        with open(fname, "r") as fobj:
            lines = fobj.readlines()
        sys.stdout.write(f"{len(lines)}\n")
    else:
        sys.stdout.write(f"{fname} not found\n")
        sys.exit(1)

if __name__ == "__main__":
    main()
