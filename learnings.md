# Ledger — Learning Log

Things learned while building this project.

---

## 2025-02-11

### `getenv()`
- Function from `<cstdlib>` that reads environment variables
- Returns `const char*` (C-style string pointer) or `nullptr` if variable doesn't exist
- Used to get `$HOME` directory path: `getenv("HOME")`
- Common pattern: check for `nullptr` before using the result

```cpp
const char* home = getenv("HOME");
if (home == nullptr) {
    // handle missing HOME variable
}
```

---

### String prefixing with `std::`
- In C++, `string` is `std::string` (from `<string>`)
- Need to either:
  - Prefix every use: `std::string path = "...";`
  - Or add `using std::string;` at top of file
- Same applies to `cout`, `cerr`, `endl`, etc.
- Explicit `std::` is more readable in small programs

---

### `std::cerr`
- Standard error stream (from `<iostream>`)
- Like `std::cout` but for error messages
- Outputs to stderr instead of stdout
- Not buffered (appears immediately)
- Allows users to separate normal output from errors

```cpp
std::cerr << "Error: something went wrong" << std::endl;
```

**Why it matters:**
- `ledger status` output goes to stdout → can be piped
- Error messages go to stderr → visible even when piping

---

### `std::filesystem` and `std::ofstream` (detailed)

These are **two separate things** that work together for file operations.

#### `std::filesystem` (from `<filesystem>`)
This is for **working with paths and directories** (NOT file contents).

Think of it as file system navigation and management:
- Creating directories
- Checking if files/folders exist
- Building paths
- Getting file info (size, permissions, etc.)

```cpp
#include <filesystem>
namespace fs = std::filesystem;

fs::path config_dir = "/home/user/.config/ledger";

// Create directory (like `mkdir -p`)
fs::create_directories(config_dir);

// Check if file exists
if (fs::exists(config_dir / "ledger.state")) {
    // file is there
}

// Build paths safely (handles / separators)
fs::path state_file = config_dir / "ledger.state";
```

**Key insight:** `filesystem` doesn't read/write file contents. It just manages paths and directory structure.

---

#### `std::ofstream` (from `<fstream>`)
This is for **writing file contents** (output file stream).

Think of it as opening a file and putting text into it:

```cpp
#include <fstream>

std::ofstream file("/path/to/file.txt");

if (file.is_open()) {
    file << "rank=1" << std::endl;
    file << "xp=0" << std::endl;
    file.close();  // optional, auto-closes when destroyed
}
```

**How it works:**
1. Constructor opens (or creates) the file
2. `<<` operator writes to it (like `cout`)
3. Destructor automatically closes when `file` goes out of scope

**Related streams:**
- `std::ifstream` — **i**nput **f**ile stream (reading)
- `std::ofstream` — **o**utput **f**ile stream (writing)
- `std::fstream` — both reading and writing

---

#### How they work together

```cpp
// 1. Use filesystem to prepare the location
fs::path config_dir = fs::path(home) / ".config" / "ledger";
fs::create_directories(config_dir);  // make sure directory exists

// 2. Use ofstream to write the actual file
fs::path state_file = config_dir / "ledger.state";
std::ofstream file(state_file);
file << "rank=1" << std::endl;
```

**Mental model:**
- `filesystem` = `mkdir`, `ls`, `cd` (structure)
- `ofstream` = `echo "text" > file` (contents)

---

### Argument parsing
- `main(int argc, char* argv[])` receives command-line arguments
- `argc` = argument count (includes program name)
- `argv` = array of C-strings
  - `argv[0]` = program name
  - `argv[1]` = first argument
  - `argv[2]` = second argument, etc.

```cpp
// ledger init
// argc = 2
// argv[0] = "ledger"
// argv[1] = "init"

if (argc < 2) {
    std::cerr << "No command provided" << std::endl;
    return 1;
}

std::string command = argv[1];
```

---

### Placing executables in `/usr/bin` (or `$PATH`)

**How the terminal finds commands:**
- When you type `ledger`, the shell searches directories in `$PATH`
- `$PATH` is a list of directories (like `/usr/bin:/usr/local/bin:...`)
- Shell checks each directory for a file named `ledger`
- If found and executable, it runs it

**Methods:**

1. **Copy to `/usr/bin`** (system-wide, requires sudo)
   ```bash
   sudo cp ledger /usr/bin/ledger
   ```

2. **Copy to `/usr/local/bin`** (better for user-installed tools)
   ```bash
   sudo cp ledger /usr/local/bin/ledger
   ```

3. **Add project directory to `$PATH`** (no copying needed)
   ```bash
   export PATH="$PATH:/path/to/project"
   ```
   Add to `~/.bashrc` to make permanent

4. **Symlink** (best for development)
   ```bash
   sudo ln -s /full/path/to/ledger /usr/local/bin/ledger
   ```
   Changes to source automatically reflect in command

**What makes a file executable:**
- Must have execute permissions: `chmod +x ledger`
- Must be a binary or have a shebang (`#!/bin/bash`, etc.)

**Mental model:**
- Terminal doesn't "know" about commands
- It just searches folders for matching filenames
- `$PATH` tells it which folders to search
- Placing binary in those folders makes it discoverable

---

## Next
Implement `ledger status` — practice reading files with `std::ifstream`