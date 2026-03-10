
MouaipS/nm
main

GITHUB

Fait moi un ReadMe fr et en

11:43 AM

Check available skills
Check available skills
Je vais créer un README bilingue (FR/EN) en Markdown.


Created a file, read a file
Created a file, read a file
README bilingue FR/EN généré ! Il couvre description, fonctionnalités, prérequis, compilation, utilisation, le tableau des lettres de type de symbole et la structure du projet — en français puis en anglais.

Readme
Document · MD 





Claude is AI and can make mistakes. Please double-check responses.
Readme · MD
Copy

# ft_nm

> 🇫🇷 Français | 🇬🇧 English

---

## 🇫🇷 Français

### Description

`ft_nm` est une réimplémentation partielle de la commande Unix `nm`, écrite en C. Elle analyse des fichiers ELF (Executable and Linkable Format) 32 et 64 bits et affiche leurs tables de symboles, en indiquant pour chaque symbole son adresse mémoire, son type (sous forme de lettre) et son nom — le tout trié par ordre alphabétique.

### Fonctionnalités

- Lecture et analyse de fichiers ELF 32 bits et 64 bits
- Affichage des symboles avec leur adresse et leur type
- Tri alphabétique des symboles (insensible à la casse, les underscores sont ignorés)
- Prise en charge des types de symboles : `T`, `D`, `B`, `R`, `U`, `A`, `C`, `W`, `w`, `N`, etc.
- Traitement de plusieurs fichiers en argument
- Par défaut, analyse `a.out` si aucun fichier n'est fourni

### Prérequis

- Compilateur `cc` (GCC ou Clang)
- Système Linux (utilise `<elf.h>`, `mmap`, etc.)
- `make`

### Compilation

```bash
make
```

Autres règles disponibles :

```bash
make clean    # Supprime les fichiers objets
make fclean   # Supprime les objets et le binaire
make re       # Recompile entièrement
```

### Utilisation

```bash
./ft_nm  [fichier2 ...]
```

**Exemples :**

```bash
./ft_nm a.out
./ft_nm /bin/ls /usr/bin/gcc
./ft_nm mon_programme
```

### Lettres de type de symbole

| Lettre | Signification |
|--------|---------------|
| `T` / `t` | Section de code (text) — global / local |
| `D` / `d` | Section de données initialisées — global / local |
| `B` / `b` | Section BSS (données non initialisées) — global / local |
| `R` / `r` | Section en lecture seule — global / local |
| `U` | Symbole indéfini (externe) |
| `A` | Adresse absolue |
| `C` | Symbole commun |
| `W` / `w` | Symbole faible (weak) — défini / indéfini |
| `N` / `n` | Symbole sans section allouée |
| `?` | Type inconnu |

### Structure du projet

```
ft_nm/
├── Makefile
├── include/
│   └── ft_nm.h          # En-têtes et structures
└── src/
    ├── main.c            # Point d'entrée
    ├── setup.c           # Ouverture et validation du fichier
    ├── process_64.c      # Traitement ELF 64 bits
    ├── process_32.c      # Traitement ELF 32 bits
    └── utils.c           # Tri, affichage, utilitaires
```

---

## 🇬🇧 English

### Description

`ft_nm` is a partial reimplementation of the Unix `nm` command, written in C. It parses 32-bit and 64-bit ELF (Executable and Linkable Format) files and displays their symbol tables, showing for each symbol its memory address, type (as a letter), and name — sorted alphabetically.

### Features

- Parsing of 32-bit and 64-bit ELF files
- Display of symbols with their address and type
- Alphabetical sorting (case-insensitive, leading underscores ignored)
- Support for symbol types: `T`, `D`, `B`, `R`, `U`, `A`, `C`, `W`, `w`, `N`, etc.
- Multiple file arguments supported
- Defaults to `a.out` if no file is provided

### Requirements

- `cc` compiler (GCC or Clang)
- Linux system (uses `<elf.h>`, `mmap`, etc.)
- `make`

### Building

```bash
make
```

Other available rules:

```bash
make clean    # Remove object files
make fclean   # Remove objects and binary
make re       # Full rebuild
```

### Usage

```bash
./ft_nm  [file2 ...]
```

**Examples:**

```bash
./ft_nm a.out
./ft_nm /bin/ls /usr/bin/gcc
./ft_nm my_program
```

### Symbol Type Letters

| Letter | Meaning |
|--------|---------|
| `T` / `t` | Code (text) section — global / local |
| `D` / `d` | Initialized data section — global / local |
| `B` / `b` | BSS section (uninitialized data) — global / local |
| `R` / `r` | Read-only section — global / local |
| `U` | Undefined symbol (external) |
| `A` | Absolute address |
| `C` | Common symbol |
| `W` / `w` | Weak symbol — defined / undefined |
| `N` / `n` | Symbol with no allocated section |
| `?` | Unknown type |

### Project Structure

```
ft_nm/
├── Makefile
├── include/
│   └── ft_nm.h          # Headers and structures
└── src/
    ├── main.c            # Entry point
    ├── setup.c           # File opening and validation
    ├── process_64.c      # 64-bit ELF processing
    ├── process_32.c      # 32-bit ELF processing
    └── utils.c           # Sorting, display, utilities
```

---

*Projet réalisé dans le cadre de l'école 42 / Project made as part of 42 school.*


