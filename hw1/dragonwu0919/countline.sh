#!/bin/bash

if [ $# -lt 1 ]; then
    echo 'missing file name\n'
    exit
elif [ $# -gt 1 ]; then
    echo 'only one argument is allowed\n'
    exit
fi

fname=$1

if [ -e $fname ]; then
    line_count=$(wc -l < "$fname")
    echo "$line_count lines in $fname"
else
    echo "$fname not found"
fi