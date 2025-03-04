#!/usr/bin/env python3

import sys
import os
import subprocess

# Read the environment variable PYTHON_BIN
PYTHON_BIN = os.getenv("PYTHON_BIN")

# Try executing PYTHON_BIN to ensure it exists
try:
    subprocess.run([PYTHON_BIN, "--version"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=True)
except FileNotFoundError:
    # If the specified Python binary is not found, print an error message and exit
    sys.stderr.write(f"exec: {PYTHON_BIN}: not found\n")
    sys.exit(1)
except subprocess.CalledProcessError:
    # If there is an error executing the Python binary, print an error message and exit
    sys.stderr.write(f"exec: {PYTHON_BIN}: error executing\n")
    sys.exit(1)

# Ensure the correct number of arguments are provided
if len(sys.argv) < 2:
    sys.stdout.write("missing file name\n")
    sys.exit(1)
elif len(sys.argv) > 2:
    sys.stdout.write("only one argument is allowed\n")
    sys.exit(1)

# Get the file name from command-line arguments
fname = sys.argv[1]

# Ensure the file exists
if not os.path.exists(fname):
    sys.stdout.write(f"{fname} not found\n")
    sys.exit(1)

# Read the file and count the number of lines
with open(fname, "r") as f:
    lines = f.readlines()

# Output the number of lines in the file
sys.stdout.write(f"{len(lines)} lines in {fname}\n")
