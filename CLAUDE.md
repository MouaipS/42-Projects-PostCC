# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

`ft_nm` is a 42 school reimplementation of the Unix `nm` command in C. It parses 32-bit and 64-bit ELF files via `mmap` and prints their symbol tables sorted alphabetically. Linux-only (`<elf.h>`, `mmap`).

## Build commands

```bash
make              # Build ft_nm (mandatory)
make bonus        # Build ft_nm_bonus
make re           # Full rebuild
make clean        # Remove object files
make fclean       # Remove objects and binaries
```

Compiler flags: `cc -g -Wall -Wextra -Werror -I include`

## Testing

No test suite — test manually against system `nm`:

```bash
./ft_nm /bin/ls
nm /bin/ls
```

## Architecture

### Data flow (mandatory)

```
main() → process_nm(filename)
  → ft_setup()        # open + mmap the file into data.map
  → ft_check_file()   # validate ELF magic + class (32 vs 64)
  → process_64/32()   # locate section headers, call find_tabs_64/32()
    → find_tabs_64/32()  # scan sections for SHT_SYMTAB → fills t_symtab
    → symbols64/32()     # build t_sym array, call parse_letter_64/32()
    → qsort(compar_sym)  # sort: skip leading '_', strcasecmp, then strcmp
    → print_list()       # write address (16 or 8 hex chars), letter, name
```

### Central struct: `t_data` (`include/ft_nm.h`)

All state is passed via a single `t_data` on the stack per file processed:
- `map` / `buff` — mmap pointer and `fstat` result
- `is_64_or_32` — true = 64-bit ELF
- `header_struct` (`t_elf`) — heap-allocated; holds `nb_sections`, `shstrtab_index`, `offset_shstrtab`
- `symtab_struct` (`t_symtab`) — heap-allocated; raw pointers into `map` for symtab and strtab
- `sym_array` (`t_sym[]`) — heap-allocated array built from all symtab entries
- `flags` (`t_flags`) — bonus only; populated by `parse_flags()` before file args

### Symbol type (`parse_letter_64/32`)

Determined by inspecting `st_shndx`, `st_type`, `st_bind`, and the section name via `shstrtab`. Standard `nm` letters: `T/t`, `D/d`, `B/b`, `R/r`, `U`, `A`, `C`, `W/w`, `N/n`, `?`.

### Bonus vs mandatory

`src_bonus/` mirrors `src/` but:
- `main_bonus.c` calls `parse_flags()` first, then passes `t_data *` into `process_nm`
- `setup_bonus.c` / `utils_bonus.c` contain the bonus variants of setup and flag-aware printing
- Both builds call the same `process_64`/`process_32` functions (defined in `src/`)

### Memory management

`header_struct` and `symtab_struct` are `malloc`'d at the start of each `process_nm` call and freed (along with `sym_array` and `munmap`) at the end. Errors exit immediately after freeing whatever has been allocated so far.


### Subject 

You have to recode the nm (with no options)
To complete this project, you will have to use a Linux OS. You have to work with ELF binaries. You have to handle
x86_32 and x86_64 binaries, object files, and shared libraries (.so).
Since this project relies on symbol table inspection, you are expected to test
your program on non-stripped binaries (the recommended VM environment).
Use the file command to view details about a file. You can use the binaries located
in your system (/usr/bin/, /usr/lib/...).
man nm
• This project will be corrected by humans only. You’re allowed to organize and name
your files as you see fit, but you must follow the following rules.
• The executable must be named ft_nm
• You must code in C and use a Makefile.
• Your Makefile must compile the project and contain the usual rules.
• If you choose to do so, you may use your own library for ft_nm. Also submit
your libft folder including its own Makefile at the root of your repository. Your
Makefile will have to compile the library, and then compile your project.