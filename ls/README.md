# hls - Custom ls Implementation

## Overview
`hls` is a custom implementation of the Unix/Linux `ls` command that lists directory contents. This project demonstrates how to build a simplified version of this essential command-line utility while adding some custom functionality.

## Features
- List files and directories in the current or specified location
- Support for single-line directory listing with the `-1` option
- Implements custom sorting functionality for directory contents
- Clean and readable code structure with separate header file

## Files in this Repository
- `hls.c` - Main implementation file
- `hls.h` - Header file with function declarations and data structures
- `test` - Test directory for verifying functionality

## Usage
```
./hls [options] [directory]
```

### Supported Options
- `-1`: Display one file per line (single-line mode)
- Additional sorting options as implemented in the code

## Building the Project
To compile the program:
```
gcc -o hls hls.c
```

## Implementation Details
The implementation focuses on:
- Proper directory traversal using system calls
- Sorting directory entries according to specified criteria
- Formatting output based on user options
- Error handling for various edge cases

## Recent Updates
- Added sorting functionality for directory listings
- Implemented the `-1` option for single-line directory listing
- Improved code structure and documentation

## Future Enhancements
- Add support for more standard `ls` options
- Implement colorized output
- Add detailed file information display (similar to `ls -l`)
