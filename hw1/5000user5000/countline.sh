#!/bin/bash

# Ensure use in right way
if [ "$#" -ne 1 ]; then
	echo "Usage: ./countline.sh <filename>" >&2
	exit 1
fi

FILENAME="$1"

# Ensure file exists
if [ ! -f "$FILENAME" ]; then
	echo "Error: File not found: $FILENAME" >&2
	exit 1
fi

# Start to count lines
wc -l < "$FILENAME"
