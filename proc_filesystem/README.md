# Proc Filesystem

The proc filesystem (procfs) is a special filesystem in Unix-like operating systems that presents information about processes and other system information in a hierarchical file-like structure. It provides an interface to kernel data structures and is mounted at `/proc`.

## Key Features

- **Process Information**: Each running process has a directory under `/proc/[pid]/` containing various files with information about that process.
- **Memory Access**: The `/proc/[pid]/mem` file provides direct access to the process's memory, which is utilized in this project.
- **Virtual Memory Maps**: The `/proc/[pid]/maps` file shows memory mappings for the process, including the heap region.

## Project Usage

This project includes a Python script (`read_write_heap.py`) that demonstrates how to:

1. Access a process's memory through the proc filesystem
2. Find specific strings in the heap memory region
3. Replace those strings with new content

### Security Considerations

- Accessing `/proc/[pid]/mem` requires appropriate permissions (typically root access)
- Modifying a running process's memory can cause instability or crashes if not done carefully
- This technique should be used responsibly and primarily for debugging or educational purposes

## Example Usage

```bash
sudo python3 read_write_heap.py [pid] [search_string] [replace_string]
```

Where:
- `[pid]` is the process ID
- `[search_string]` is the string to find in the heap
- `[replace_string]` is the string to replace it with (must be equal to or shorter than the search string)
