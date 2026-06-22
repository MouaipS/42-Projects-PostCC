# Projet `malloc` — Explication du projet

Réimplémentation de `malloc(3)`, `free(3)` et `realloc(3)` sans utiliser le `malloc` de la libc : on gère soi-même la mémoire avec `mmap(2)` / `munmap(2)`, le tout compilé en bibliothèque partagée `libft_malloc_$HOSTTYPE.so`.

Ce README explique les **notions** du projet, dans l'ordre où on les comprend. Il sert autant de mémo personnel que de support pour la soutenance : chaque section finit sur la formulation à donner à l'oral.

---

## 1. À quoi sert `malloc` ? Le rôle d'intermédiaire

Le système d'exploitation ne distribue la mémoire qu'en **grosses pages** (4096 octets en général), via des appels système qui sont **lents**. Or un programme appelle `malloc` des milliers de fois, souvent pour quelques octets.

L'idée de `malloc` est donc de servir d'**intermédiaire** :

1. Il demande au système un **gros bloc** d'un coup (`mmap`) — rarement.
2. Il le **découpe lui-même** en petits morceaux à chaque appel `malloc`.
3. Au `free`, il **marque** le morceau comme libre pour le réutiliser, sans rien rendre au système.
4. Il ne rend au système (`munmap`) qu'une zone entière redevenue totalement libre.

> **Piège à éviter :** `malloc` ne fait *pas* un appel système à chaque allocation. Tout le projet consiste justement à **limiter les appels à `mmap`/`munmap`** en pré-allouant en gros et en redistribuant en petit.

Précision de vocabulaire : c'est de la **mémoire vive (RAM)**, volatile — rien n'est stocké sur disque.

**À l'oral :** *« malloc pré-alloue de gros blocs avec mmap, puis les redécoupe lui-même pour servir les petites demandes, afin d'éviter un appel système coûteux à chaque allocation. »*

---

## 2. Que fait `free` ? Et quand rend-on la mémoire au système ?

`free` ne rend **pas** la mémoire au système : il **marque le bloc comme libre** pour pouvoir le réutiliser à un prochain `malloc`.

Le `munmap`, lui, n'arrive que dans certains cas précis :

- **Zone TINY/SMALL** : on ne `munmap` **que lorsque TOUS les blocs de la zone sont libres**. Tant qu'un seul bloc est encore occupé, on garde la zone — sinon on rendrait au système une zone contenant encore de la mémoire utilisée, et le programme segfaulterait à son prochain accès.
- **Zone LARGE** : une zone LARGE ne contient qu'une seule allocation. Donc dès qu'on la `free`, elle est forcément 100 % libre → `munmap` immédiat, systématiquement.

> **Piège à éviter :** il n'y a **pas** de « `munmap` final en un coup à la fin du programme ». Ta lib n'a pas de fonction de fin ; quand le programme se termine, c'est le système qui récupère automatiquement toute la mémoire du processus. Ton `munmap` à toi se fait **pendant** la vie du programme, zone par zone.

Optimisation optionnelle (à mentionner, pas obligatoire) : on peut garder volontairement **une** zone TINY et **une** zone SMALL vides en réserve, pour éviter de re-`mmap` immédiatement si une nouvelle petite allocation arrive juste après.

**À l'oral :** *« free marque le bloc comme libre pour réutilisation. Je ne rends la mémoire avec munmap que lorsqu'une zone entière est libre — systématiquement pour les LARGE, et seulement quand tous les blocs sont libres pour les TINY/SMALL. »*

---

## 3. Les trois familles : TINY / SMALL / LARGE

On ne traite pas toutes les demandes de la même façon, selon la taille demandée. Métaphore de l'entrepôt : pour des milliers de petits colis on commande un grand bac partagé ; pour un frigo géant on lui donne sa place rien qu'à lui.

| Famille | Taille demandée | Stratégie |
|---------|-----------------|-----------|
| **TINY** | 1 à `n` octets (ex. ≤ 128) | Zones pré-allouées partagées, ≥ 100 blocs chacune |
| **SMALL** | `n+1` à `m` octets (ex. ≤ 1024) | Zones pré-allouées partagées plus grandes, ≥ 100 blocs |
| **LARGE** | `> m` octets | Chaque allocation a **son propre `mmap`** dédié |

- **TINY et SMALL** → on pré-alloue en gros et on partage, pour amortir l'appel système.
- **LARGE** → rares et énormes : pré-allouer une zone géante « au cas où » serait du gaspillage, donc on demande au coup par coup.

On sépare TINY et SMALL pour ne pas mélanger des tailles trop différentes dans une même zone (ça gaspillerait de la place).

Les seuils `n` et `m` (ex. 128 et 1024) sont **choisis par nous** : n'importe quelles valeurs cohérentes conviennent, du moment qu'on sait les justifier (compromis vitesse / mémoire).

La toute première chose que fait `malloc(size)` : regarder `size` et décider TINY, SMALL ou LARGE.

---

## 4. Le header : retrouver les métadonnées au `free`

Quand `free(ptr)` est appelé, on ne reçoit **qu'une adresse**, rien d'autre. Pourtant il faut connaître la taille du bloc, savoir s'il est libre, atteindre ses voisins…

**Astuce :** on stocke ces métadonnées dans un petit en-tête (`t_block`) placé **juste avant** le pointeur rendu à l'utilisateur.

```
adresse réelle du bloc
     │
     ▼
     [ header t_block ][ ........ mémoire utilisable ........ ]
                       ▲
                       └── adresse retournée à l'utilisateur par malloc
```

- Au **`malloc`** : on **avance** depuis le début du bloc pour donner le pointeur utilisateur (après le header).
- Au **`free`** : on **recule** depuis le pointeur utilisateur pour retrouver le header.

```c
t_block *block = (t_block *)((char *)ptr - align(sizeof(t_block)));
```

L'utilisateur ignore complètement l'existence de ce header ; il ne voit que sa mémoire utilisable.

**À l'oral :** *« malloc ne rend pas le début réel du bloc : juste avant le pointeur utilisateur, je cache un header avec les métadonnées. Au free, je recule de la taille du header pour le retrouver. »*

---

## 5. L'alignement (sur 16)

Le processeur exige que les données soient placées à des adresses « rondes » (un `double` à un multiple de 8, etc.). Une donnée mal alignée ralentit fortement l'accès ou fait crasher selon l'architecture.

Comme `malloc` ignore ce que l'utilisateur va stocker, il rend une adresse alignée pour **le type le plus exigeant**. Sur 64 bits, on prend **16 octets** comme alignement universel.

« Aligner une taille » = l'arrondir au **multiple de 16 supérieur ou égal** :

| Demande | Aligné |
|---------|--------|
| `malloc(1)` | 16 |
| `malloc(12)` | 16 |
| `malloc(17)` | 32 |
| `malloc(30)` | 32 |
| `malloc(40)` | 48 |

```c
size_t align(size_t size)
{
    return ((size + 15) & ~(size_t)15);
}
```

On aligne à **deux endroits** : la taille du **header** (pour que la mémoire utilisateur commence alignée) et la taille **demandée** (pour que le bloc suivant commence aligné).

**À l'oral :** *« 16, c'est l'alignement maximal requis par les types scalaires sur architecture 64 bits ; en alignant sur 16, je garantis que n'importe quel type stocké dans la mémoire rendue sera correctement aligné. »*

---

## 6. Le split : ne pas gaspiller au `malloc`

Une zone neuve = un seul gros bloc libre occupant tout l'espace. Si `malloc(20)` consommait toute la zone, on n'y logerait qu'une allocation au lieu de 100.

On **découpe** (split) : un petit morceau sert la demande, et un nouveau bloc libre récupère le reste.

```
Avant :  [ header ][ ............... 4000 octets libres ............... ]

malloc(20) → on coupe :

Après :  [ header ][ 32 — OCCUPÉ ][ header ][ ~3900 — LIBRE ]
```

Le bloc restant reçoit son propre header et reste prêt pour la prochaine allocation. C'est ce qui permet à un seul `mmap` de servir des centaines d'allocations.

**On ne split pas toujours :** si l'excédent est trop petit pour contenir un nouveau header + un minimum utile, le « reste » serait inexploitable. Dans ce cas, on donne le bloc entier tel quel.

```c
if (block->size < size + align(sizeof(t_block)) + 16)
    return; // pas assez de place pour un reste utile → pas de split
```

**Règle :** on split **si** l'excédent est exploitable, **sinon** on laisse le bloc entier.

---

## 7. Le coalescing : recoller au `free`

À force de découper et de libérer, on obtient plein de petits blocs libres côte à côte : c'est la **fragmentation**. Problème : trois blocs libres de 32 octets collés ne peuvent pas servir un `malloc(80)`, alors qu'il y a 96 octets libres au total — juste éparpillés.

L'opération inverse du split : au `free`, on regarde les **voisins immédiats**. S'ils sont libres aussi, on **fusionne** (coalescing) en un seul gros bloc libre.

Les deux mouvements sont complémentaires :

- **`malloc` → split** : couper un grand bloc pour ne pas gaspiller.
- **`free` → coalescing** : recoller les blocs libres voisins contre la fragmentation.

Pour atteindre les voisins, on utilise les champs du header :

- **`next`** → le bloc suivant ; **`prev`** → le bloc précédent. D'où la **liste doublement chaînée** : avec un chaînage simple, on ne pourrait atteindre que le voisin de droite, et il faudrait reparcourir toute la zone pour trouver celui de gauche.
- **`free`** → on ne fusionne un voisin que s'il est marqué libre.
- **`size`** → pour recalculer la taille du bloc fusionné (somme des deux + le header du voisin qui disparaît).

**À l'oral :** *« je limite la fragmentation avec deux mécanismes : split au malloc, fusion des voisins libres au free. La liste doublement chaînée me donne next et prev en accès direct. »*

---

## 8. La validation du pointeur : ne jamais segfault

Le sujet l'exige : aucune fonction ne doit segfault. Or `free` retrouve le header en reculant aveuglément depuis `ptr`. Si on l'appelle avec une adresse au hasard, un pointeur de pile ou un pointeur déjà libéré, on lit des octets qui **ne sont pas** un vrai header → en suivant un faux `next`, c'est le crash.

**On ne peut donc jamais faire confiance au `ptr` reçu.** La source de vérité, c'est le global `g_zones`, qui contient **toutes** les zones et **tous** les blocs qu'on a alloués.

Avant de toucher quoi que ce soit, on vérifie que `ptr` correspond bien à l'adresse utilisateur d'un de nos blocs réels :

```c
int is_valid_ptr(void *ptr)
{
    t_zone  *zone = g_zones;
    t_block *block;

    while (zone)
    {
        block = zone->blocks;
        while (block)
        {
            void *user = (char *)block + align(sizeof(t_block));
            if (user == ptr)
                return (1);
            block = block->next;
        }
        zone = zone->next;
    }
    return (0);
}
```

```c
void free(void *ptr)
{
    if (!ptr || !is_valid_ptr(ptr))
        return; // NULL ou adresse inconnue → on ne fait RIEN
    ...
}
```

Ce garde-fou règle d'un coup les trois cas dangereux :

- **`free(NULL)`** → bloqué par `!ptr` (et exigé par le sujet).
- **adresse inventée / pointeur de pile** → jamais trouvée dans `g_zones` → on ne fait rien.
- **double free** → détectable en vérifiant si le bloc est déjà marqué `free`.

Honnêteté technique : ce parcours est en O(n), donc lent. Un allocateur de production fait mieux. Mais ici **la sécurité prime sur la vitesse** — c'est défendable.

**À l'oral :** *« je ne touche jamais à un pointeur sans avoir vérifié qu'il appartient à une de mes zones ; ma source de vérité, c'est g_zones, qui contient l'intégralité de mes allocations. »*

---

## 9. Le coût du header (overhead)

Le header n'est pas gratuit : il occupe de la place physique dans la zone, **en plus** de la donnée. Pour servir `malloc(size)`, on consomme :

```
align(sizeof(t_block))  +  align(size)
   header                   mémoire utilisateur
```

Exemple avec un header aligné de 32 octets :

| Demande | Donnée alignée | Header | Consommé en zone |
|---------|----------------|--------|------------------|
| `malloc(16)` | 16 | 32 | **48** |
| `malloc(40)` | 48 | 32 | **72** |

> **Attention au calcul :** pour `malloc(40)`, ce n'est pas 40 + 32, mais `align(40)` + 32 = 48 + 32 = 72. La donnée passe d'abord par l'alignement.

Conséquences à connaître :

1. Le header est un **coût par allocation**. Pour de toutes petites `malloc`, il peut peser plus lourd que la donnée elle-même — inhérent à ce design.
2. C'est aussi pour ça que `malloc(0)` mérite réflexion : même pour 0 octet, il faudrait un header complet. D'où le choix simple de renvoyer `NULL`.
3. Dans `get_zone_size`, on compte toujours `header + data` par bloc, sinon les zones seraient trop petites pour loger réellement 100 blocs.

---

## Récapitulatif des notions

| Notion | Idée clé |
|--------|----------|
| Rôle d'intermédiaire | Pré-allouer en gros, redistribuer en petit, limiter les appels système |
| `free` / `munmap` | Marquer libre ; rendre au système seulement une zone entièrement libre |
| TINY / SMALL / LARGE | Trois stratégies de rangement selon la taille |
| Header | Métadonnées cachées avant le pointeur, retrouvées en reculant |
| Alignement (16) | Arrondir au multiple de 16 pour que tout type tienne |
| Split | Découper un grand bloc au malloc pour ne pas gaspiller |
| Coalescing | Recoller les voisins libres au free contre la fragmentation |
| Validation du pointeur | Vérifier via `g_zones` avant de toucher quoi que ce soit |
| Overhead du header | Le header s'ajoute toujours à la donnée |

---

## Sujets restants à préparer

- **Makefile** : `$HOSTTYPE`, la `.so`, le lien symbolique, pourquoi `-fPIC` et `-shared`.
- **`realloc`** : logique et cas limites (`realloc(NULL, n)` ≡ `malloc`, `realloc(ptr, 0)` ≡ `free`).
- **`show_alloc_mem`** : tri par adresses croissantes, affichage hexadécimal sans `printf` (uniquement `write`).
- **Bonus** : thread-safety avec mutex pthread, et pourquoi un seul global supplémentaire suffit.