import pandas as pd

# ─── Chargement des données ───────────────────────────────────────────────────
df = pd.read_csv("Train_knight.csv")

# Encoder la colonne cible en numérique : Jedi=0, Sith=1
df['knight'] = df['knight'].map({'Jedi': 0, 'Sith': 1})

# ─── Corrélation absolue avec la target, triée par ordre décroissant ─────────
corr = df.corr()['knight'].abs().sort_values(ascending=False)

print(corr.to_string())