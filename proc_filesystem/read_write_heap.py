#!/usr/bin/env python3

import os
import sys
import re

def usage_error():
    print("Usage: read_write_heap.py pid search_string replace_string")
    sys.exit(1)

def is_ascii(s):
    return all(ord(c) < 128 for c in s)

def get_heap_range(pid):
    try:
        with open(f"/proc/{pid}/maps", "r") as maps_file:
            for line in maps_file:
                if "[heap]" in line:
                    match = re.match(r"([0-9a-fA-F]+)-([0-9a-fA-F]+)", line)
                    if match:
                        heap_start = int(match.group(1), 16)
                        heap_end = int(match.group(2), 16)
                        return heap_start, heap_end
    except FileNotFoundError:
        print(f"Error: Process {pid} not found.")
        sys.exit(1)
    
    print("Error: Heap region not found.")
    sys.exit(1)

def read_memory(pid, address, length):
    try:
        with open(f"/proc/{pid}/mem", "rb") as mem_file:
            mem_file.seek(address)
            return mem_file.read(length)
    except (FileNotFoundError, OSError):
        print(f"Error: Failed to read memory of process {pid}.")
        sys.exit(1)

def write_memory(pid, address, data):
    try:
        with open(f"/proc/{pid}/mem", "r+b") as mem_file:
            mem_file.seek(address)
            mem_file.write(data)
    except (FileNotFoundError, OSError):
        print(f"Error: Failed to write memory of process {pid}.")
        sys.exit(1)

def search_and_replace_in_heap(pid, search_str, replace_str):
    if len(search_str) != len(replace_str):
        print("Error: Search and replace strings must be of equal length.")
        sys.exit(1)

    # Get heap range
    heap_start, heap_end = get_heap_range(pid)
    heap_size = heap_end - heap_start
    print(f"Heap starts at: 0x{heap_start:x}, ends at: 0x{heap_end:x}, size: {heap_size} bytes")

    # Read the heap memory
    heap_data = read_memory(pid, heap_start, heap_size)

    # Search for the string
    offset = heap_data.find(search_str.encode('ascii'))
    if offset == -1:
        print(f"String '{search_str}' not found in heap.")
        return

    found_address = heap_start + offset
    print(f"Found string at address: 0x{found_address:x}")

    # Replace the string
    modified_data = heap_data[:offset] + replace_str.encode('ascii') + heap_data[offset + len(replace_str):]
    
    # Write the modified memory back
    write_memory(pid, heap_start, modified_data)
    print(f"Replaced '{search_str}' with '{replace_str}' at address: 0x{found_address:x}")

def main():
    # Check arguments
    if len(sys.argv) != 4:
        usage_error()

    pid = sys.argv[1]
    search_str = sys.argv[2]
    replace_str = sys.argv[3]

    # Validate inputs
    if not pid.isdigit():
        print(f"Error: Invalid PID '{pid}'.")
        usage_error()
    
    if not is_ascii(search_str) or not is_ascii(replace_str):
        print("Error: Strings must be ASCII.")
        usage_error()

    # Perform the search and replace
    search_and_replace_in_heap(int(pid), search_str, replace_str)

if __name__ == "__main__":
    main()

