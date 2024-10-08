#!/usr/bin/python3
"""Finds & overwrites a string in a process' mem file"""

from sys import argv

USAGE = "USAGE: read_write_heap.py pid search_string replace_string"

def parse_maps_file(pid):
    """Parses /proc/PID/maps file for heap info"""
    heap_start = heap_stop = None
    with open(f"/proc/{pid}/maps", "r") as file:
        for line in file:
            if "[heap]" in line:
                heap_start, heap_stop = [int(x, 16) for x in line.split(" ")[0].split("-")]
    if heap_start is None or heap_stop is None:
        exit(1)
    return heap_start, heap_stop

def update_mem_file(pid, search_string, replace_string, heap_start, heap_stop):
    """Finds search_string in /proc/PID/mem and writes replace_string"""
    with open(f"/proc/{pid}/mem", "r+b") as f:
        f.seek(heap_start)
        data = f.read(heap_stop - heap_start)
        string_offset = data.find(search_string.encode())
        
        if string_offset > -1:
            if len(search_string) == len(replace_string):
                # Normal replace
                f.seek(heap_start + string_offset)
                f.write(replace_string.encode())
                return "SUCCESS!"  # Normal replace
            
            elif len(search_string) > len(replace_string):
                # Short replace
                f.seek(heap_start + string_offset)
                f.write(replace_string.encode())
                return "SUCCESS! (Short replace)"  # Short replace

            elif len(search_string) < len(replace_string):
                # Replace by shortest string
                f.seek(heap_start + string_offset)
                f.write(replace_string.encode())
                return "SUCCESS! (Replaced by longer string)"  # Replace by longest string
        else:
            # Not found case
            return "String not found."

    # If replace string is empty
    if not replace_string:
        return "SUCCESS! (Replaced by empty string)"  # Replace by empty string

if __name__ == "__main__":
    if len(argv) != 4:
        print(USAGE)
        exit(1)

    pid = int(argv[1])
    search_string = argv[2]
    replace_string = argv[3]

    heap_start, heap_stop = parse_maps_file(pid)
    result = update_mem_file(pid, search_string, replace_string, heap_start, heap_stop)
    
    print(result)

