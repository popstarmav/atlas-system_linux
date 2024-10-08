#!/usr/bin/python3

"""
Module for reading and writing to the heap of a running process.
This script finds a string in the heap and replaces it.
"""

import os
import sys

def print_usage():
    print("Usage: read_write_heap.py pid search_string replace_string")
    sys.exit(1)

def get_heap_memory(pid):
    """Get the start and end addresses of the heap memory."""
    try:
        with open(f"/proc/{pid}/maps", "r") as f:
            for line in f:
                if "[heap]" in line:
                    parts = line.split()
                    address_range = parts[0].split('-')
                    start = int(address_range[0], 16)
                    end = int(address_range[1], 16)
                    return start, end
    except FileNotFoundError:
        print(f"Error: Process with PID {pid} does not exist.")
    except Exception:
        print("Error: Unable to access heap memory.")
    return None

def read_heap(pid, start, end):
    """Read the heap memory of the specified process."""
    try:
        with open(f"/proc/{pid}/mem", "rb") as mem_file:
            mem_file.seek(start)
            return mem_file.read(end - start)
    except Exception:
        return None

def write_heap(pid, start, heap_content):
    """Write modified content back to the heap memory of the specified process."""
    try:
        with open(f"/proc/{pid}/mem", "r+b") as mem_file:
            mem_file.seek(start)
            mem_file.write(heap_content)
    except Exception:
        pass

def main():
    if len(sys.argv) != 4:
        print_usage()

    pid = sys.argv[1]
    search_string = sys.argv[2].encode()
    replace_string = sys.argv[3].encode()

    if len(search_string) != len(replace_string):
        print("Error: Strings must be the same length.")
        sys.exit(1)

    if not pid.isdigit():
        print("Error: Invalid PID.")
        sys.exit(1)

    pid = int(pid)

    heap_range = get_heap_memory(pid)
    if not heap_range:
        print("Error: Heap not found.")
        sys.exit(1)

    start, end = heap_range

    heap_content = read_heap(pid, start, end)
    if heap_content is None:
        print("Error: Unable to read heap memory.")
        sys.exit(1)

    index = heap_content.find(search_string)
    if index == -1:
        print("Error: String not found in heap.")
        sys.exit(1)

    mutable_heap = bytearray(heap_content)
    mutable_heap[index:index + len(search_string)] = replace_string
    write_heap(pid, start, bytes(mutable_heap))
    print("SUCCESS!")

if __name__ == "__main__":
    main()

