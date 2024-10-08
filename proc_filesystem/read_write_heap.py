#!/usr/bin/python3
"""Finds & overwrites a string in a process' memory file"""

import sys

USAGE = "USAGE: read_write_heap.py pid search_string replace_string"


def get_heap_memory(pid):
    """Parses /proc/PID/maps file for heap info"""
    heap_start = heap_stop = None
    try:
        with open(f"/proc/{pid}/maps", "r") as file:
            for line in file:
                if "[heap]" in line:
                    heap_start, heap_stop = [int(x, 16) for x in line.split()[0].split("-")]
                    print(f"[*] Heap starts at {heap_start:02X}")
                    return heap_start, heap_stop
    except FileNotFoundError:
        print(f"[ERROR] Process with PID {pid} does not exist.")
        sys.exit(1)
    except Exception as e:
        print(f"[ERROR] Could not read heap memory: {e}")
        sys.exit(1)

    print("[ERROR] Heap address not found.")
    sys.exit(1)


def replace_string_in_memory(pid, search_string, replace_string, heap_start, heap_stop):
    """Finds search_string in /proc/PID/mem and writes replace_string"""
    try:
        with open(f"/proc/{pid}/mem", "r+b") as mem_file:
            mem_file.seek(heap_start)
            data = mem_file.read(heap_stop - heap_start)
            print(f"[*] Read {heap_stop - heap_start} bytes")
            string_offset = data.find(search_string.encode())

            if string_offset != -1:
                print(f"[*] String found at {heap_start + string_offset:02X}")
                mem_file.seek(heap_start + string_offset)
                written = mem_file.write(replace_string.encode() + b'\x00')
                print(f"[*] {written} bytes written!")
            else:
                print(f"[ERROR] String '{search_string}' not found in heap.")
                sys.exit(1)
    except Exception as e:
        print(f"[ERROR] Could not update memory: {e}")
        sys.exit(1)


def main():
    if len(sys.argv) != 4:
        print(USAGE)
        sys.exit(1)

    pid = sys.argv[1]
    search_string = sys.argv[2]
    replace_string = sys.argv[3]

    if not pid.isdigit():
        print("[ERROR] PID must be a number.")
        sys.exit(1)

    if len(search_string) != len(replace_string):
        print("[ERROR] Strings must be the same length.")
        sys.exit(1)

    pid = int(pid)
    heap_start, heap_stop = get_heap_memory(pid)
    replace_string_in_memory(pid, search_string, replace_string, heap_start, heap_stop)


if __name__ == "__main__":
    main()

