#!/bin/bash

# Check if the correct number of arguments is provided
if [ $# -lt 1 ]; then
    echo "missing file name"  # Print error message if no file name is given
    exit 1
elif [ $# -gt 1 ]; then
    echo "only one argument is allowed"  # Print error if more than one argument is provided
    exit 1
fi

# Store the file name argument
fname="$1"

# Check if the file exists
if [ -f "$fname" ]; then
    # Count the number of lines in the file using wc -l
    line_count=$(wc -l < "$fname")
    echo "$line_count lines in $fname"  # Print the line count with file name
else
    echo "$fname not found"  # Print error message if file does not exist
    exit 1
fi
