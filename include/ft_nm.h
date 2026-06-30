#ifndef FT_NM_H
# define FT_NM_H

#include "stdio.h"
#include "unistd.h"
#include "fcntl.h"
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <elf.h>       /* Toutes les structures ELF : Elf64_Ehdr, Elf64_Sym, SHT_SYMTAB... */
#include <sys/mman.h>  /* mmap / munmap */
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>


typedef struct s_flags{
	bool 			a; /* --debug-syms   : afficher aussi STT_FILE et STT_SECTION */
	bool 			g; /* --extern-only  : afficher uniquement les symboles globaux */
	bool 			u; /* --undefined-only : afficher uniquement les symboles indéfinis */
	bool 			r; /* --reverse-sort : trier en ordre inverse */
	bool 			p; /* --no-sort      : conserver l'ordre du fichier, sans trier */
} t_flags;

/*
** Métadonnées du ELF Header.
*/
typedef struct s_elf{
	uint16_t		nb_sections;    /* Nombre total de sections (e_shnum) */
	uint16_t		shstrtab_index; /* Index de .shstrtab dans la Section Header Table (e_shstrndx) */
	char			*offset_shstrtab; /* Pointeur vers les données de .shstrtab dans le fichier mappé */
} t_elf;

/*
** Localisation de la table des symboles (.symtab) et de la table des noms (.strtab).
** Ces deux pointeurs pointent directement dans le fichier mappé (data->map),
*/
typedef struct s_symtab{
	void			*symtab;       /* Pointeur vers le début de .symtab dans le fichier mappé */
	size_t			size;          /* Taille totale de .symtab en octets */
	size_t			symbole_size;  /* Taille d'une entrée Elf*_Sym (sh_entsize, ou sizeof si 0) */
	char			*strtab;       /* Pointeur vers .strtab : les noms des symboles en C-strings */
	size_t			strtab_size;   /* Taille de .strtab en octets (pour valider les index st_name) */
} t_symtab;

/*
** Représente un symbole après extraction et prêt à être affiché.
** name pointe dans .strtab (dans le fichier mappé)
** st_info contient le binding : STB_LOCAL, STB_GLOBAL ou STB_WEAK.
*/
typedef struct s_sym{
	const char		*name;      /* Nom du symbole, pointeur dans .strtab */
	char			letter;     /* Lettre nm : T, t, D, d, B, U, W, w, R, A, C, N, ? */
	int				st_info;    /* Binding du symbole (STB_LOCAL / STB_GLOBAL / STB_WEAK) */
	uint64_t		value;      /* Adresse du symbole (uint64_t couvre aussi les valeurs 32-bit) */
	int				has_value;  /* 1 = afficher l'adresse, 0 = afficher des espaces (ex: U, w) */
} t_sym;

/*
** Structure centrale : une instance est créée par fichier traité, sur la pile de process_nm.
** Regroupe tout l'état nécessaire au traitement d'un fichier ELF.
** Les champs *_struct sont alloués sur le tas et libérés en fin de process_nm.
** sym_array est alloué dans symbols64/32 une fois la taille connue.
*/
typedef struct s_data{
	const char		*filename;      /* Chemin du fichier (pour les messages d'erreur) */
	void 			*map;           /* Adresse du fichier mappé en mémoire par mmap */
	struct stat 	buff;           /* Résultat de fstat : contient st_size (taille du fichier) */
	bool			is_64_or_32;    /* true = ELF 64-bit, false = ELF 32-bit */
	t_elf			*header_struct; /* Métadonnées du ELF Header (alloué sur le tas) */
	t_symtab		*symtab_struct; /* Localisation de .symtab et .strtab (alloué sur le tas) */
	t_sym			*sym_array;     /* Tableau de symboles extraits et triés (alloué sur le tas) */
	int				count;          /* Nombre de symboles valides dans sym_array */
	t_flags			flags;          /* Options bonus (-a, -g, -u, -r, -p) */
} t_data;


/* setup.c : ouverture, mmap et validation du fichier ELF */
void ft_setup(t_data *data, const char *filename);
void ft_check_file(t_data *data, const char *filename);

/* process_64.c : traitement complet d'un ELF 64-bit */
void process_64(t_data *data);
void symbols64(t_data *data);
char parse_letter_64(t_data *data, Elf64_Sym *actual_symbol, int count);

/* process_32.c : traitement complet d'un ELF 32-bit */
void process_32(t_data *data);
void symbols32(t_data *data);
char parse_letter_32(t_data *data, Elf32_Sym *actual_symbol, int count);

/* utils.c : tri, affichage, recherche des sections, gestion d'erreur */
void ft_error(const char *filename, const char *error);
void find_tabs_64(t_data *data, Elf64_Shdr *elf_header);
void find_tabs_32(t_data *data, Elf32_Shdr *elf_header);
void parse_symbol(t_data *data);
int  compar_sym(const void *a, const void *b);
void print_list(t_data *data);

/* bonus uniquement */
int  revers_compar_syn(const void *a, const void *b);
void ft_setup_bonus(t_data *data, const char *filename);
void ft_check_file_bonus(t_data *data, const char *filename);

#endif
