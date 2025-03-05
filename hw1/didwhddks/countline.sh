#!/bin/bash

if [ $# -gt 1 ]; then
    echo "only one argument is allowed"
elif [ $# -lt 1 ]; then
    echo "missing file name"
else
    fname=$1
    if [ ! -f $fname ]; then
        echo "$fname not found"
    else
        echo "$(wc -l < $fname) lines in $fname"
    fi
fi