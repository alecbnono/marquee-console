# Command Interpreter

A small command interpreter written in C++17 to practice operating systems concepts (CSOPESY). It reads commands from a prompt, parses them, and responds. The code is split into Model / View / Controller.

## Commands

| Command          | Description                                           |
| ---------------- | ----------------------------------------------------- |
| `set_text <text>` | Saves `<text>` for the marquee and confirms it        |
| `help`           | Lists the available commands                          |
| `exit`           | Quits the program                                     |

Example:

```
> set_text Hello World
Text saved for marquee: Hello World
```

## Requirements

- A C++17 compiler (`g++` / GCC 7 or newer)
- `make` (optional, you can also compile with `g++` directly)

## Linux

Install the tools if needed:

```bash
# Debian/Ubuntu
sudo apt install build-essential
# Arch
sudo pacman -S base-devel
# Fedora
sudo dnf install gcc-c++ make
```

Build and run with make:

```bash
make          # builds build/mo3
make run      # builds and runs
make clean    # removes build/
```

Or with g++ only:

```bash
mkdir -p build
g++ -std=c++17 -Wall -Wextra src/*.cpp -o build/mo3
./build/mo3
```

## Windows

Install GCC through [MSYS2](https://www.msys2.org/) (or any MinGW-w64 distribution), then in the **MSYS2 UCRT64** shell:

```bash
pacman -S mingw-w64-ucrt-x86_64-gcc make
```

Build and run with make from the project root (in the MSYS2 shell, so `mkdir -p` and `rm -rf` in the Makefile work):

```bash
make
./build/mo3.exe
```

Or with g++ only, which also works from PowerShell or Command Prompt once GCC is on your `PATH`:

```bat
mkdir build
g++ -std=c++17 -Wall -Wextra src/*.cpp -o build/mo3.exe
build\mo3.exe
```

If `src/*.cpp` is not expanded by your shell, list the files explicitly:

```bat
g++ -std=c++17 src/main.cpp src/Controller.cpp src/Model.cpp src/View.cpp src/AsciiArt.cpp -o build/mo3.exe
```

## Project layout

```
Makefile
src/
  main.cpp        read-eval loop
  Controller.*    tokenizes and dispatches commands
  Model.*         holds application state (the saved marquee text)
  View.*          all console output
  AsciiArt.*      renders text as figlet-style ASCII art
```
