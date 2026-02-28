import pandas as pd
import matplotlib.pyplot as plt
from sklearn.preprocessing import StandardScaler

SEP_X = "Empowered"
SEP_Y = "Strength"

for filename in ["Train_knight.csv", "Test_knight.csv"]:
    df = pd.read_csv(filename)

    # Separate target if present
    has_target = 'knight' in df.columns
    if has_target:
        target = df['knight']
        features = df.drop(columns=['knight'])
    else:
        features = df.copy()

    # Print original data (head)
    print(f"\n--- Original ({filename}) ---")
    print(features.head(3).to_string())

    # Standardize
    scaler = StandardScaler()
    scaled = scaler.fit_transform(features)
    scaled_df = pd.DataFrame(scaled, columns=features.columns)

    print(f"\n--- Standardized ({filename}) ---")
    print(scaled_df.head(3).to_string())

    # Plot
    fig, ax = plt.subplots(figsize=(8, 5))
    if has_target:
        for label, color in [('Jedi', 'blue'), ('Sith', 'red')]:
            mask = target == label
            ax.scatter(scaled_df.loc[mask, SEP_X], scaled_df.loc[mask, SEP_Y],
                       c=color, label=label, alpha=0.5, s=15)
        ax.legend()
    else:
        ax.scatter(scaled_df[SEP_X], scaled_df[SEP_Y], c='green', alpha=0.5, s=15, label='Knight')
        ax.legend()

    ax.set_xlabel(SEP_X)
    ax.set_ylabel(SEP_Y)
    ax.set_title(f"Standardized - {filename}")
    plt.tight_layout()
    plt.show()