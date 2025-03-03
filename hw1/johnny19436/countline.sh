#!/bin/bash
# Check number of arguments
if [ "$#" -lt 1 ]; then
  echo "missing file name"
  exit 0
elif [ "$#" -gt 1 ]; then
  echo "only one argument is allowed"
  exit 0
fi

fname="$1"
if [ -e "$fname" ]; then
  # Count the lines using wc -l and remove any extra spaces/newlines.
  count=$(wc -l < "$fname")
  echo "${count} lines in ${fname}"
else
  echo "${fname} not found"
fi
