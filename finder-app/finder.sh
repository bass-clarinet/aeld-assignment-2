#!/bin/sh
# finder.sh - count files in a directory tree and lines matching a search string
#
# Usage: finder.sh <filesdir> <searchstr>

if [ $# -lt 2 ] || [ -z "$1" ] || [ -z "$2" ]; then
    echo "Error: missing argument(s)."
    echo "Usage: $0 <filesdir> <searchstr>"
    echo "  filesdir  - path to a directory to search"
    echo "  searchstr - text string to search for in the files"
    exit 1
fi

filesdir=$1
searchstr=$2

if [ ! -d "$filesdir" ]; then
    echo "Error: '$filesdir' is not a directory on the filesystem."
    exit 1
fi

# Number of regular files in filesdir and all subdirectories
numfiles=$(find "$filesdir" -type f | wc -l)

# Number of lines containing searchstr (fixed string, not a regex)
nummatches=$(grep -r -F -- "$searchstr" "$filesdir" 2>/dev/null | wc -l)

# Arithmetic expansion strips any padding some wc implementations add
echo "The number of files are $((numfiles)) and the number of matching lines are $((nummatches))"
