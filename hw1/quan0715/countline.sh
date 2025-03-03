#!/bin/bash

#
# This script try to reimplement the countline.py script.
#

numberOfArgs=$#
fileName=$1

if [ $numberOfArgs -eq 0 ]; then
    echo "missing file name"
    exit 1
fi

if [ $numberOfArgs -gt 1 ]; then
    echo "only one argument is allowed"
    exit 1
fi

if [ ! -f $fileName ]; then
    echo "$fileName not found"
    exit 1
fi

numberOfLines=$(wc -l <"$fileName" | tr -d ' ')
echo "$numberOfLines lines in $fileName"
