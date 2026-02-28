import pandas as pd
import matplotlib.pyplot as plt

# ─── Chargement des données ───────────────────────────────────────────────────
train = pd.read_csv("Train_knight.csv")
test  = pd.read_csv("Test_knight.csv")

# Features très corrélées avec knight (ex01) → clusters séparés visuellement
SEP_X = "Empowered"
SEP_Y = "Strength"

# Features peu corrélées avec knight (ex01) → clusters mélangés
MIX_X = "Push"
MIX_Y = "Midi-chlorien"

fig, axes = plt.subplots(2, 2, figsize=(14, 10))

# ── Train séparé ─────────────────────────────────────────────────────────────
ax = axes[0][0]
for label, color in [('Jedi', 'blue'), ('Sith', 'red')]:
    mask = train['knight'] == label
    ax.scatter(train.loc[mask, SEP_X], train.loc[mask, SEP_Y],
               c=color, label=label, alpha=0.5, s=15)
ax.set_xlabel(SEP_X)
ax.set_ylabel(SEP_Y)
ax.set_title("Train_knight.csv — Séparé")
ax.legend()

# ── Train mélangé ────────────────────────────────────────────────────────────
ax = axes[0][1]
for label, color in [('Jedi', 'blue'), ('Sith', 'red')]:
    mask = train['knight'] == label
    ax.scatter(train.loc[mask, MIX_X], train.loc[mask, MIX_Y],
               c=color, label=label, alpha=0.5, s=15)
ax.set_xlabel(MIX_X)
ax.set_ylabel(MIX_Y)
ax.set_title("Train_knight.csv — Mélangé")
ax.legend()

# ── Test séparé ──────────────────────────────────────────────────────────────
ax = axes[1][0]
ax.scatter(test[SEP_X], test[SEP_Y],
           c='green', alpha=0.5, s=15, label='Knight')
ax.set_xlabel(SEP_X)
ax.set_ylabel(SEP_Y)
ax.set_title("Test_knight.csv — Séparé")
ax.legend()

# ── Test mélangé ─────────────────────────────────────────────────────────────
ax = axes[1][1]
ax.scatter(test[MIX_X], test[MIX_Y],
           c='green', alpha=0.5, s=15, label='Knight')
ax.set_xlabel(MIX_X)
ax.set_ylabel(MIX_Y)
ax.set_title("Test_knight.csv — Mélangé")
ax.legend()

plt.tight_layout()
plt.show()