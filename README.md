# karla

A small, POSIX-style command shell written in C.

`karla` reads commands from a prompt, parses them, and runs them by forking
child processes — the same `fork`/`exec`/`wait` model that real Unix shells
are built on. It supports I/O redirection, background jobs, and a handful of
built-in commands.

## Features

- Runs external programs found on `$PATH`
- I/O redirection: `<`, `>`, and `>>`
- Background execution with `&` (children are reaped automatically)
- Command history (fixed-size ring buffer)
- Built-in commands: `cd`, `pwd`, `echo`, `export`, `history`, `help`, `exit`
- `Ctrl-C` interrupts the running command without killing the shell
- `Ctrl-D` exits cleanly

## Build

Requires `gcc` (or any C11 compiler) and `make`.

```sh
make
```

The binary is produced at `./karla`. Object files land in `build/`.

```sh
make clean   # remove build artifacts
make re      # clean rebuild
```

## Usage

```sh
./karla
```

```
karla$ pwd
/home/user/projects
karla$ ls -la > listing.txt
karla$ echo appended line >> listing.txt
karla$ sort < listing.txt
karla$ sleep 5 &
[bg] pid 4213
karla$ history
    1  pwd
    2  ls -la > listing.txt
    ...
karla$ exit
```

## Project structure

```
karla/
├── src/          # implementation
│   ├── main.c        # REPL loop and signal setup
│   ├── parser.c      # line → Command
│   ├── executor.c    # fork/exec/wait, redirection
│   ├── builtins.c    # in-shell commands
│   ├── history.c     # ring-buffer history
│   └── utils.c       # prompt, input, helpers
├── include/
│   └── karla.h       # shared declarations
├── Makefile
├── README.md
├── LICENSE
└── .gitignore
```