import pandas as pd
import matplotlib.pyplot as plt

# ─── Chargement des données ───────────────────────────────────────────────────
train = pd.read_csv("Train_knight.csv")
test  = pd.read_csv("Test_knight.csv")

features = [col for col in train.columns if col != 'knight']

N_COLS = 5
N_ROWS = (len(features) + N_COLS - 1) // N_COLS  # 6 lignes pour 30 features

# ─── Graphique 1 : Test_knight.csv (pas de target) ───────────────────────────
fig, axes = plt.subplots(N_ROWS, N_COLS, figsize=(20, N_ROWS * 3))
axes = axes.flatten()

for i, col in enumerate(features):
    axes[i].hist(test[col], bins=30, color='green', alpha=0.7)
    axes[i].set_title(col, fontsize=8)
    axes[i].tick_params(labelsize=6)

# Cacher les subplots vides
for j in range(len(features), len(axes)):
    axes[j].set_visible(False)

fig.suptitle("Test_knight.csv — Feature Distributions", fontsize=14, y=1.01)
plt.tight_layout()
plt.show()

# ─── Graphique 2 : Train_knight.csv (Jedi vs Sith) ───────────────────────────
fig, axes = plt.subplots(N_ROWS, N_COLS, figsize=(20, N_ROWS * 3))
axes = axes.flatten()

for i, col in enumerate(features):
    ax = axes[i]
    for label, color in [('Jedi', 'blue'), ('Sith', 'red')]:
        mask = train['knight'] == label
        ax.hist(train.loc[mask, col], bins=30, alpha=0.5, label=label, color=color)
    ax.set_title(col, fontsize=8)
    ax.tick_params(labelsize=6)
    ax.legend(fontsize=5)

for j in range(len(features), len(axes)):
    axes[j].set_visible(False)

fig.suptitle("Train_knight.csv — Jedi vs Sith", fontsize=14, y=1.01)
plt.tight_layout()
plt.show()