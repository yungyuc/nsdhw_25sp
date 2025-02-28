#!/bin/bash
#
# This script counts the number of lines in a file.
#
# Setup
export PYTHON_BIN

# Actions
wc -l $1 | tr -dc '0-9'