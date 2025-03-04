#!/bin/bash
    fname=$1
    count=$(wc -l < "$fname"| tr -dc '0-9')
    echo "$count lines in $fname"
    exit 0
# vim: set fenc=utf8 ff=unix et sw=2 ts=2 sts=2:
