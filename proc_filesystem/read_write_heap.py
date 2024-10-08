#!/usr/bin/python3

"""
Module for reading and writing to the heap of a running process.
This script finds a string in the heap and replaces it.
"""

import sys
import os
import re

def print_usage():
    """Print usage and exit."""
    print("Usage: read_write_heap.py pid search_string replace_string")
    sys.exit(1)

def get_heap_memory(pid):
    """Get heap start and end addresses from /proc/<pid>/maps."""
    try:
        with open(f"/proc/{pid}/maps", "r") as maps_file:
            for line in maps_file:
                if "[heap]" in line:  # Find heap section
                    match = re.match(r"([0-9a-f]+)-([0-9a-f]+)", line)
                    if match:
                        return int(match.group(1), 16), int(match.group(2), 16)
    except FileNotFoundError:
        print(f"Error: PID {pid} not found.")
        sys.exit(1)
    return None

def read_heap(pid, start, end):
    """Read heap memory content from /proc/<pid>/mem."""
    try:
        with open(f"/proc/{pid}/mem", "rb") as mem_file:
            mem_file.seek(start)  # Move to heap start
            return bytearray(mem_file.read(end - start))  # Read heap
    except PermissionError:
        print("Error: Permission denied. Run as root or use sudo.")
        sys.exit(1)

def write_heap(pid, start, heap_content):
    """Write modified heap content back to memory."""
    try:
        with open(f"/proc/{pid}/mem", "r+b") as mem_file:
            mem_file.seek(start)  # Move to heap start
            mem_file.write(heap_content)  # Write modified heap
    except PermissionError:
        print("Error: Permission denied. Run as root or use sudo.")
        sys.exit(1)

def main():
    if len(sys.argv) != 4:
        print_usage()

    pid = sys.argv[1]
    search_string = sys.argv[2].encode()  # Convert to bytes
    replace_string = sys.argv[3].encode()  # Convert to bytes

    if len(search_string) != len(replace_string):
        print("Error: Strings must be the same length.")
        sys.exit(1)

    if not pid.isdigit():
        print("Error: Invalid PID.")
        sys.exit(1)

    pid = int(pid)
    heap_range = get_heap_memory(pid)
    if not heap_range:
        print(f"Error: Heap not found for PID {pid}.")
        sys.exit(1)

    start, end = heap_range
    heap_content = read_heap(pid, start, end)

    index = heap_content.find(search_string)
    if index == -1:
        print(f"String '{search_string.decode()}' not found in heap.")
    else:
        heap_content[index:index + len(search_string)] = replace_string
        write_heap(pid, start, heap_content)
        print("SUCCESS!")  # Print only the success message

if __name__ == "__main__":
    main()

