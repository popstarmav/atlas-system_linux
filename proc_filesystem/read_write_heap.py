#!/usr/bin/python3
"""Finds & overwrites a string in a process' mem file"""

from sys import argv, exit

USAGE = "USAGE: read_write_heap.py pid search_string replace_string"

def parse_maps_file(pid):
    """Parses /proc/PID/maps file for heap info."""
    try:
        with open(f"/proc/{pid}/maps", "r") as file:
            for line in file:
                if "[heap]" in line:
                    heap_start, heap_stop = [int(x, 16) for x in line.split()[0].split("-")]
                    return heap_start, heap_stop
    except Exception:
        exit(1)
    exit(1)

def update_mem_file(pid, search_string, replace_string, heap_start, heap_stop):
    """Finds search_string in /proc/PID/mem and writes replace_string."""
    try:
        with open(f"/proc/{pid}/mem", "r+b") as mem_file:
            mem_file.seek(heap_start)
            data = mem_file.read(heap_stop - heap_start)
            string_offset = data.find(search_string.encode())

            if string_offset != -1:
                mem_file.seek(heap_start + string_offset)

                # Handle the different cases for string replacement
                if len(replace_string) > len(search_string):
                    # Case: Replacement string is longer than the original
                    return False  # Can't replace with a longer string

                # Create the modified heap content
                mutable_heap = bytearray(data)

                # Replace the string and fill the remaining bytes with null
                mutable_heap[string_offset:string_offset + len(replace_string)] = replace_string.encode()
                mutable_heap[string_offset + len(replace_string):string_offset + len(search_string)] = b'\x00' * (len(search_string) - len(replace_string))

                mem_file.write(mutable_heap)
                return True
    except Exception:
        return False
    return False

if __name__ == "__main__":
    if len(argv) != 4:
        print(USAGE)
        exit(1)

    pid = int(argv[1])
    search_string = argv[2]
    replace_string = argv[3]

    # Validate lengths for specific replacement cases
    if len(search_string) < len(replace_string):
        print(USAGE)
        exit(1)

    heap_start, heap_stop = parse_maps_file(pid)
    if update_mem_file(pid, search_string, replace_string, heap_start, heap_stop):
        print("SUCCESS!")
    else:
        exit(1)

