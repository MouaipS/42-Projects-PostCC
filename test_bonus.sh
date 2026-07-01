#!/bin/bash

FT_NM="./ft_nm_bonus"
NM="nm"
CORREC="./Correc"
PASS=0
FAIL=0

RED='\033[0;31m'
GRN='\033[0;32m'
YEL='\033[0;33m'
RST='\033[0m'

ok()   { echo -e "${GRN}[OK]${RST}  $*"; PASS=$((PASS + 1)); }
ko()   { echo -e "${RED}[KO]${RST}  $*"; FAIL=$((FAIL + 1)); }
info() { echo -e "${YEL}[--]${RST}  $*"; }

# ── helpers ──────────────────────────────────────────────────────────────────

# Vérifie que ft_nm ne crashe pas (pas de signal) sur un fichier défaillant.
check_no_crash()
{
	local label="$1"; shift
	${FT_NM} "$@" >/dev/null 2>&1
	local code=$?
	if [ $code -ge 128 ]; then
		ko "$label  (crash / signal $((code - 128)))"
	else
		ok "$label  (pas de crash, exit $code)"
	fi
}

# Diff exact entre nm et ft_nm sur un fichier valide.
check_diff()
{
	local label="$1"; shift
	local out_ft out_nm
	out_nm=$(${NM}    "$@" 2>&1)
	out_ft=$(${FT_NM} "$@" 2>&1)
	if [ "$out_ft" = "$out_nm" ]; then
		ok "$label"
	else
		ko "$label"
		diff <(echo "$out_nm") <(echo "$out_ft") \
		     --label "nm" --label "ft_nm_bonus" --unified=3
	fi
}

# ── build ─────────────────────────────────────────────────────────────────────

echo ""
echo "══════════════════════════════════════════"
echo " Compilation"
echo "══════════════════════════════════════════"
if make bonus > /dev/null 2>&1; then
	ok "make bonus"
else
	ko "make bonus"
	echo "Arrêt : la compilation a échoué."
	exit 1
fi

# ── prépare les binaires de test ─────────────────────────────────────────────

TMP=$(mktemp -d)
TEST_FACILE="$TMP/test_facile"
NOT_SO_EASY="$TMP/not_so_easy"
NOT_SO_EASY_32="$TMP/not_so_easy_32"
OBJ_64="$TMP/not_so_easy.o"
OBJ_32="$TMP/not_so_easy_32.o"

cc "$CORREC/main.c"           -o "$TEST_FACILE"    2>/dev/null
cc "$CORREC/not_so_easy_test.c" -o "$NOT_SO_EASY"  2>/dev/null
cc "$CORREC/not_so_easy_test.c" -c -o "$OBJ_64"    2>/dev/null
cc -m32 "$CORREC/not_so_easy_test.c" -o "$NOT_SO_EASY_32" 2>/dev/null
cc -m32 "$CORREC/not_so_easy_test.c" -c -o "$OBJ_32"      2>/dev/null

HAS_32=false
[ -f "$NOT_SO_EASY_32" ] && HAS_32=true

# ── section 1 : fichiers d'erreur ─────────────────────────────────────────────
# La correction : "si le programme quitte de manière inattendue, l'évaluation
#                 s'arrête ici."  → on vérifie uniquement l'absence de crash.

echo ""
echo "══════════════════════════════════════════"
echo " Tests fichiers d'erreur (pas de crash)"
echo "══════════════════════════════════════════"

check_no_crash "header            (fichier trop court)" "$CORREC/header"
check_no_crash "header_copy       (fichier trop court)" "$CORREC/header_copy"
check_no_crash "header_and_prog   (sections manquantes)" "$CORREC/header_and_prog"
check_no_crash "header_and_prog_copy (sections manquantes)" "$CORREC/header_and_prog_copy"

# Ces deux fichiers sont des ELF valides que nm lit sans erreur → diff complet.
echo ""
info "Fichiers corrompus mais lisibles par nm → diff exact"
check_diff     "error_header      (ELF valide)" "$CORREC/error_header"
check_diff     "header_offset_error (ELF valide)" "$CORREC/header_offset_error"

# ── section 2 : easy test ─────────────────────────────────────────────────────

echo ""
echo "══════════════════════════════════════════"
echo " Test facile  (main.c → test_facile)"
echo "══════════════════════════════════════════"

if [ -f "$TEST_FACILE" ]; then
	check_diff "test_facile 64-bit" "$TEST_FACILE"
else
	ko "test_facile : compilation échouée"
fi

# ── section 3 : not so easy ───────────────────────────────────────────────────

echo ""
echo "══════════════════════════════════════════"
echo " Test moins facile  (not_so_easy_test.c)"
echo "══════════════════════════════════════════"

if [ -f "$NOT_SO_EASY" ]; then
	check_diff "not_so_easy 64-bit" "$NOT_SO_EASY"
else
	ko "not_so_easy 64-bit : compilation échouée"
fi

if $HAS_32; then
	check_diff "not_so_easy 32-bit" "$NOT_SO_EASY_32"
else
	info "not_so_easy 32-bit : gcc-multilib absent, test ignoré"
fi

# ── section 4 : arguments multiples ──────────────────────────────────────────

echo ""
echo "══════════════════════════════════════════"
echo " Arguments multiples"
echo "══════════════════════════════════════════"

if [ -f "$TEST_FACILE" ] && [ -f "$NOT_SO_EASY" ]; then
	check_diff "multi : test_facile + not_so_easy" \
	           "$TEST_FACILE" "$NOT_SO_EASY"
	check_diff "multi : error_header + not_so_easy" \
	           "$CORREC/error_header" "$NOT_SO_EASY"
fi

# ── section 5 : fichiers objet .o ─────────────────────────────────────────────

echo ""
echo "══════════════════════════════════════════"
echo " Fichiers objet (.o)"
echo "══════════════════════════════════════════"

if [ -f "$OBJ_64" ]; then
	check_diff "objet 64-bit (.o)" "$OBJ_64"
else
	ko "objet 64-bit : compilation échouée"
fi

if $HAS_32 && [ -f "$OBJ_32" ]; then
	check_diff "objet 32-bit (.o)" "$OBJ_32"
else
	info "objet 32-bit : gcc-multilib absent, test ignoré"
fi

# ── section 6 : bibliothèque dynamique ───────────────────────────────────────

echo ""
echo "══════════════════════════════════════════"
echo " Bibliothèque dynamique (.so)"
echo "══════════════════════════════════════════"

# La correc précise "symbol order can be arbitrary" → vérifier uniquement pas de crash.
SOLIB=""
for f in $(find /usr/lib/x86_64-linux-gnu -maxdepth 1 -name "*.so*" -type f 2>/dev/null); do
	if ${NM} "$f" 2>/dev/null | grep -q .; then
		SOLIB="$f"
		break
	fi
done

if [ -n "$SOLIB" ]; then
	check_no_crash "$(basename "$SOLIB")  (pas de crash)" "$SOLIB"
else
	info ".so : aucune bibliothèque avec symboles trouvée, test ignoré"
fi

# ── section 7 : flags bonus ──────────────────────────────────────────────────

echo ""
echo "══════════════════════════════════════════"
echo " Flags bonus (-a -g -u -r -p)"
echo "══════════════════════════════════════════"

TARGET="${FT_NM}"

check_diff "aucun flag"              "$TARGET"
check_diff "-p  (pas de tri)"    -p  "$TARGET"
check_diff "-r  (tri inversé)"   -r  "$TARGET"
check_diff "-g  (extern only)"   -g  "$TARGET"
check_diff "-u  (undefined only)" -u "$TARGET"
check_diff "-a  (debug syms)"    -a  "$TARGET"
check_diff "-gr"                 -g -r "$TARGET"
check_diff "-gu"                 -g -u "$TARGET"
check_diff "-ur"                 -u -r "$TARGET"
check_diff "-pr"                 -p -r "$TARGET"
check_diff "-ar"                 -a -r "$TARGET"
check_diff "-ap"                 -a -p "$TARGET"
check_diff "--no-sort"         --no-sort         "$TARGET"
check_diff "--reverse-sort"    --reverse-sort    "$TARGET"
check_diff "--extern-only"     --extern-only     "$TARGET"
check_diff "--undefined-only"  --undefined-only  "$TARGET"
check_diff "--debug-syms"      --debug-syms      "$TARGET"

# ── résumé ────────────────────────────────────────────────────────────────────

echo ""
echo "══════════════════════════════════════════"
printf " Résultat : ${GRN}%d OK${RST} / %d tests  (${RED}%d échoué(s)${RST})\n" \
       "$PASS" "$((PASS + FAIL))" "$FAIL"
echo "══════════════════════════════════════════"

rm -rf "$TMP"
