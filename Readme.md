# File Integrity Monitor (FIM)

A command-line **File Integrity Monitor** written in **C** as a first-semester **Programming Fundamentals (PF)** lab project. The application recursively scans a target directory, computes **64-bit FNV-1a** hashes for regular files, and compares current file states against a CSV baseline to detect **unchanged**, **modified**, **new**, and **deleted** files.

---

## Features

* **64-bit FNV-1a Hashing (`scanner.c`)**: Reads files in 4 KB chunks (`4096` bytes) in binary mode (`"rb"`) and computes a 16-character hexadecimal FNV-1a hash (`016llx`).
* **Recursive Directory Scanning (`scanner.c`)**: Traverses subdirectories using POSIX `dirent.h` and `sys/stat.h` (`opendir`, `readdir`, `stat`), capturing file paths, hashes, and local timestamps (`YYYY-MM-DD HH:MM:SS`).
* **CSV Baseline Management (`baseline.c`)**: Saves and loads file baselines in comma-separated format (`filepath,hash,timestamp`) up to `1024` files (`MAX_FILES`).
* **Change Detection (`baseline.c`)**: Compares live scans against the baseline and categorizes every file as:
  * `[UNCHANGED]` — File path and hash match the baseline.
  * `[MODIFIED]` — File path exists in the baseline, but its hash has changed.
  * `[NEW]` — File was not present in the baseline.
  * `[DELETED]` — File existed in the baseline, but is missing from the current scan.
* **Interactive Menu & Periodic Auto-Scan (`cli.c`, `main.c`)**: Supports manual on-demand scans as well as a continuous monitoring mode that scans at a fixed interval (default: `5` seconds) and prints real-time alerts.
* **Report Exporting (`baseline.c`)**: Exports scan results to a plain-text report file (`scan_report.txt`).

---

## Project Structure

```text
File-Integrity-Checker/
├── main.c         # Application entry point, baseline auto-load, and menu dispatch loop
├── cli.c          # CLI initialization, formatted table output, and periodic scanner
├── baseline.c     # CSV baseline persistence, baseline comparison, and report export
├── scanner.c      # Recursive directory traversal, timestamps, and 64-bit FNV-1a hashing
├── fim_types.h    # Shared structs (FileRecord, CliState), enums (FileStatus), and constants
└── .gitignore     # Git ignore rules for build artifacts
```

---

## Default Configuration & Limits

Defined in `fim_types.h` and `cli.c`:

| Setting / Constant | Default Value | Description |
| :--- | :--- | :--- |
| `target_dir` | `./monitored_dir` | Default directory monitored on startup |
| `baseline_file` | `baseline.csv` | CSV file storing baseline file paths, hashes, and timestamps |
| `report_file` | `scan_report.txt` | Output file path for exported scan reports |
| `scan_interval_sec` | `5` | Interval (in seconds) between periodic auto-scans |
| `MAX_FILES` | `1024` | Maximum number of tracked file records |
| `MAX_PATH_LEN` | `512` | Maximum character length for file/directory paths |
| `HASH_LEN` | `17` | 16 hex characters + null terminator for 64-bit FNV-1a |

---

## Getting Started

### Prerequisites

* A C compiler supporting C11 or C99 (`gcc` or `clang`)
* Standard C libraries plus `<dirent.h>` and `<sys/stat.h>` (natively available on Linux/macOS, and via MinGW/MSYS2 on Windows)
* No external third-party dependencies are required

### Building the Project

Compile all `.c` files into a single executable:

**Linux / macOS:**
```bash
gcc -Wall -Wextra -std=c11 main.c cli.c baseline.c scanner.c -o fim
```

**Windows (MinGW):**
```bash
gcc -Wall -Wextra -std=c11 main.c cli.c baseline.c scanner.c -o fim.exe
```

### Running the Application

1. Create the default monitored directory (or configure a custom directory from the menu once running):
   ```bash
   mkdir -p monitored_dir
   ```
2. Run the executable:
   ```bash
   ./fim
   ```
   *(On Windows, run `.\fim.exe`)*

---

## CLI Menu Usage

On startup, the program automatically attempts to load an existing `baseline.csv` and presents an interactive 6-option menu:

```text
--- File Integrity Monitor Menu ---
1. Set Target Directory
2. Create/Update Baseline
3. Run Manual Scan
4. Start Periodic Auto-Scan
5. Export Report
6. Exit
Enter choice:
```

1. **Set Target Directory**: Change the directory path to monitor (overrides `./monitored_dir` for the current session).
2. **Create/Update Baseline**: Recursively scans the target directory and writes the snapshot to `baseline.csv`.
3. **Run Manual Scan**: Scans the target directory, compares results against the loaded baseline, and displays the status table (`STATUS`, `FILEPATH`, `HASH`, `TIMESTAMP`).
4. **Start Periodic Auto-Scan**: Continuously scans the target directory every `5` seconds and prints `[ALERT]` messages whenever modified, new, or deleted files are detected. Press `Ctrl+C` to stop.
5. **Export Report**: Writes the latest scan results to `scan_report.txt` in the format:
   ```text
   STATUS: <STATUS> PATH: <FILEPATH> HASH: <HASH> TIME: <TIMESTAMP>
   ```
6. **Exit**: Cleanly exits the application.

---

## Known Limitations

* **Comma-separated paths**: Because `baseline.csv` uses commas (`,`) as delimiters, file or directory paths containing commas may not parse properly when reloading a baseline.
* **Cross-compilation on MSVC**: `scanner.c` relies on `<dirent.h>`, which requires MinGW/GCC or a POSIX compatibility header on Windows rather than stock MSVC.

---

## Course Context

Developed as a first-semester **Programming Fundamentals (PF)** lab project demonstrating modular C architecture, header/source separation, file I/O (`fopen`, `fread`, `fgets`, `fprintf`), recursion, bitwise operations, and structured data management.