# File Integrity Checker (FIM)

A command-line **File Integrity Checker** written in **C** as a first-semester **Programming Fundamentals (PF)** lab project. The tool is designed to track files and detect unauthorized or unexpected modifications by comparing current file states against a recorded baseline.

## Project Structure

The project is organized into modular C source and header files:

- `main.c` — Entry point coordinating program execution and core workflows.
- `cli.c` — Command-line interface handling user arguments, commands, and console output.
- `baseline.c` — Baseline creation, loading, saving, or management logic.
- `scanner.c` — Directory/file traversal and state inspection logic.
- `fim_types.h` — Shared data structures, constants, and function declarations.
- `.gitignore` — Specifies build artifacts and untracked files to ignore in Git.

## Getting Started

### Prerequisites

- A C compiler (such as `gcc` or `clang`)
- A Unix-like shell or Windows command prompt/PowerShell with your compiler configured in `PATH`

### Building the Project

Compile all source files together into an executable (for example, `fim`):

```bash
gcc -Wall -Wextra -std=c11 main.c cli.c baseline.c scanner.c -o fim
```

*(If your project links external libraries such as OpenSSL for hashing or uses a `Makefile`, adjust the build command accordingly.)*

### Running the Program

Run the compiled binary from your terminal:

```bash
./fim
```

On Windows:

```powershell
.\fim.exe
```

## Overview of How It Works

1. **Baseline Generation (`baseline.c`)**: Captures an initial snapshot of target file metadata/state to serve as a reference point.
2. **Scanning (`scanner.c`)**: Inspects target files or directories to gather their current state.
3. **CLI & Reporting (`cli.c`, `main.c`)**: Parses user commands and displays results or detected changes.

## Course Context

Developed as a first-semester **Programming Fundamentals (PF)** lab project focusing on modular C programming, header/source separation, file/directory handling, and structured data types.

## Notes

- Because this README was generated from the repository file layout, you may want to update the **Usage / CLI Flags** and **Build Flags** sections with the exact commands, hashing algorithm, and baseline file format implemented in your code.