# Ledger Learning Log

Notes recorded while building Ledger.

---

## 2025-02-11

### Environment variables with `getenv()`

`getenv()` from `<cstdlib>` reads environment variables.

It returns:

* `const char*` pointing to the value
* `nullptr` if the variable does not exist

Example:

```cpp
const char* home = getenv("HOME");

if (home == nullptr) {
    // handle missing HOME variable
}
```

Important detail:

`getenv()` returns a C string, so if you need a C++ string you convert it.

```cpp
std::string path = std::string(home) + "/.config";
```

Typical use in CLI programs is to discover paths like `$HOME` or `$XDG_CONFIG_HOME`.

---

### Namespaces and `std::`

Most standard library features live inside the `std` namespace.

Examples:

```cpp
std::string
std::cout
std::cerr
std::filesystem
```

You can either:

```cpp
std::string path = "...";
```

or import specific symbols:

```cpp
using std::string;
```

Explicit `std::` is usually clearer in small utilities because it shows what comes from the standard library.

---

### `std::cout` and `std::cerr`

C++ provides multiple output streams.

`std::cout`
Standard output. Used for normal program output.

`std::cerr`
Standard error. Used for error messages.

Example:

```cpp
std::cout << "Ledger initialized\n";
std::cerr << "Error: cannot open file\n";
```

Reason for separation:

Standard output can be redirected or piped.

Example:

```
ledger status > file.txt
```

Error messages written to `stderr` still appear on the terminal.

---

### Command line arguments

Programs receive command line arguments through:

```cpp
int main(int argc, char* argv[])
```

`argc`
Number of arguments.

`argv`
Array of C strings containing the arguments.

Example command:

```
ledger init
```

Values become:

```
argc = 2
argv[0] = "ledger"
argv[1] = "init"
```

Typical pattern:

```cpp
if (argc < 2) {
    std::cerr << "Missing command\n";
    return 1;
}

std::string command = argv[1];
```

---

### How the shell finds commands

When a command is typed in the terminal the shell searches directories listed in `$PATH`.

Example `$PATH`:

```
/usr/bin:/usr/local/bin:/home/user/.local/bin
```

The shell checks each directory for a file matching the command name.

Ways to make a program available as a command:

Copy binary into `/usr/local/bin`:

```
sudo cp ledger /usr/local/bin/ledger
```

Create a symbolic link:

```
sudo ln -s /full/path/to/ledger /usr/local/bin/ledger
```

Add a project directory to `$PATH`:

```
export PATH="$PATH:/path/to/project"
```

Executable files must have execute permissions:

```
chmod +x ledger
```

---

### Filesystem operations with `std::filesystem`

`std::filesystem` from `<filesystem>` manages directories and paths.

It does not read or write file contents.

Examples:

```cpp
namespace fs = std::filesystem;

fs::path dir = fs::path(home) / ".config" / "ledger";
fs::create_directories(dir);
```

Common functions:

`fs::create_directories(path)`
Creates directories recursively similar to `mkdir -p`.

`fs::exists(path)`
Checks if a file or directory exists.

`fs::path`
Represents filesystem paths.

Paths can be joined safely using `/`:

```cpp
fs::path state_file = dir / "ledger.state";
```

---

### Writing files with `std::ofstream`

`std::ofstream` from `<fstream>` writes to files.

Example:

```cpp
std::ofstream file("data.txt");

if (file) {
    file << "Rank=1\n";
    file << "XP=0\n";
}
```

Behavior:

1. Constructor opens or creates the file.
2. `<<` writes text into it.
3. File closes automatically when the object leaves scope.

Related types:

`std::ifstream`
Input file stream for reading files.

`std::ofstream`
Output file stream for writing files.

`std::fstream`
Supports both reading and writing.

---

### Typical file creation workflow

Filesystem prepares the location and file streams handle the contents.

Example:

```cpp
fs::path dir = fs::path(home) / ".config" / "ledger";
fs::create_directories(dir);

fs::path file = dir / "ledger.state";

std::ofstream out(file);
out << "Rank=1\nXP=0\nFocus=0\n";
```

Mental model:

filesystem manages directory structure
fstream handles file contents

---

## 2025-03-05

### `std::filesystem::exists`

`std::filesystem::exists(path)` checks whether a file or directory exists.

Example:

```cpp
if (std::filesystem::exists(file)) {
    std::cout << "Ledger already initialized\n";
}
```

Common use cases:

* Prevent overwriting files
* Check initialization state
* Validate paths before reading

---

### Reading files with `std::ifstream`

Files are read using `std::ifstream`.

Example:

```cpp
std::ifstream in(file);
```

Check if the file opened successfully:

```cpp
if (!in) {
    std::cerr << "Failed to open file\n";
}
```

Files close automatically when the stream goes out of scope.

---

### Reading files line by line

`std::getline()` reads one line at a time.

```cpp
std::string line;

while (std::getline(in, line)) {
    // process line
}
```

This loop continues until the end of the file.

---

### Parsing key value pairs

Ledger state uses the format:

```
Rank=1
XP=0
Focus=0
```

Each line must be split into key and value.

Locate the separator:

```cpp
size_t pos = line.find('=');
```

If the separator is missing:

```cpp
if (pos == std::string::npos)
    continue;
```

Extract parts of the string:

```cpp
std::string key = line.substr(0, pos);
std::string value_str = line.substr(pos + 1);
```

Convert the value to an integer:

```cpp
int value = std::stoi(value_str);
```

---

### Mapping keys to variables

Example mapping:

```cpp
if (key == "Rank") rank = value;
else if (key == "XP") xp = value;
else if (key == "Focus") focus = value;
```

---

### `size_t`

`size_t` is an unsigned integer type used for sizes and indexes.

Examples:

```cpp
size_t pos = line.find('=');
size_t length = str.length();
```

It is large enough to represent the size of any object in memory and is commonly returned by container functions such as `.size()`.

---

### `std::string::npos`

`std::string::npos` represents an invalid position.

It is returned by string search functions when a match is not found.

Example:

```cpp
size_t pos = line.find('=');

if (pos == std::string::npos) {
    // '=' not found
}
```

Internally it equals `size_t(-1)`.

---

### Tools used for parsing

| Tool                    | Purpose                     |
| ----------------------- | --------------------------- |
| `std::ifstream`         | open files for reading      |
| `std::getline()`        | read file line by line      |
| `std::string::find()`   | locate delimiters           |
| `std::string::substr()` | extract portions of strings |
| `std::stoi()`           | convert string to integer   |

---

### Formatting console output

The `<iomanip>` header provides formatting tools.

Example:

```cpp
#include <iomanip>

std::cout << std::left << std::setw(6) << "Rank" << ": " << rank << "\n";
```

`std::setw()` ensures aligned output columns.

Useful for displaying status tables or structured console output.