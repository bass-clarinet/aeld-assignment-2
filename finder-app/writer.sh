#!/bin/sh
# writer.sh - write a string to a file, creating the path if needed
#
# Usage: writer.sh <writefile> <writestr>

if [ $# -lt 2 ] || [ -z "$1" ] || [ -z "$2" ]; then
    echo "Error: missing argument(s)."
    echo "Usage: $0 <writefile> <writestr>"
    echo "  writefile - full path to the file to create (including filename)"
    echo "  writestr  - text string to write into the file"
    exit 1
fi

writefile=$1
writestr=$2

# Create the directory path if it doesn't exist
writedir=$(dirname "$writefile")
if ! mkdir -p "$writedir"; then
    echo "Error: could not create directory '$writedir'."
    exit 1
fi

# Write the string, overwriting any existing file
if ! printf '%s\n' "$writestr" > "$writefile"; then
    echo "Error: could not create file '$writefile'."
    exit 1
fi
