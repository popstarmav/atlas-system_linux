# C - ELF: nm/objdump

This project implements functionality similar to the Linux command-line utilities `nm` and `objdump` for analyzing ELF (Executable and Linkable Format) files.

## Overview

This project provides tools for examining the symbol tables and object file information in ELF format executables, object files, and libraries.

## Features

- Symbol table listing (similar to `nm`)
- Object file information display (similar to `objdump`)
- Support for processing multiple files

## Usage

```
./hnm [file...]
```

## Implementation

The project includes functionality to process ELF files and extract relevant information about symbols and sections.

## Files

- `hnm.h`: Header file containing function declarations
  - `process_file()`: Processes an ELF file to extract symbol information

## Building

Compile the project using the provided Makefile:

```bash
make
```
