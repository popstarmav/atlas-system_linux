#!/bin/bash

# Check if exactly one argument (PID) is given
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <pid>"
    exit 1
fi

# Send SIGQUIT to the specified PID
kill -SIGQUIT "$1"

# Check if kill command was successful
if [ "$?" -eq 0 ]; then
    exit 0
else
    echo "Failed to send SIGQUIT to process $1"
    exit 1
fi

