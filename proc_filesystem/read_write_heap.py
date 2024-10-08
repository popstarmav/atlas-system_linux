#!/usr/bin/env python3

import os
import sys
import struct

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
                    start = int(parts[0], 16)
                    end = int(parts[1], 16)
                    return start, end
    except FileNotFoundError:
        print(f"Process with PID {pid} does not exist.")
    return None

def read_heap(pid, start, end):
    """Read the heap memory of the specified process."""
    with open(f"/proc/{pid}/mem", "rb") as mem_file:
        mem_file.seek(start)
        return mem_file.read(end - start)

def write_heap(pid, start, heap_content):
    """Write modified content back to the heap memory of the specified process."""
    with open(f"/proc/{pid}/mem", "r+b") as mem_file:
        mem_file.seek(start)
        mem_file.write(heap_content)

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
        print(f"Error: Heap not found for PID {pid}.")
        sys.exit(1)

    start, end = heap_range
    heap_content = read_heap(pid, start, end)

    index = heap_content.find(search_string)
    if index == -1:
        print(f"String '{search_string.decode()}' not found in heap.")
    else:
        print("String found, replacing...")  # Debugging print
        heap_content[index:index + len(search_string)] = replace_string
        write_heap(pid, start, heap_content)
        print("SUCCESS!")  # Print only the success message
        return  # Ensure we exit after printing

    print("Exiting main function")  # Debugging exit point

if __name__ == "__main__":
    main()

