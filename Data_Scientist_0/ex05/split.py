import pandas as pd
import sys
from sklearn.model_selection import train_test_split

if len(sys.argv) < 2:
    print("Usage: python split.py Train_knight.csv")
    sys.exit(1)

filename = sys.argv[1]
df = pd.read_csv(filename)

# 80% training, 20% validation — classic split ratio
# 80/20 is standard: enough data to train the model while keeping
# a meaningful validation set to evaluate generalization.
train_df, val_df = train_test_split(df, test_size=0.2, random_state=42, shuffle=True)

train_df.to_csv("Training_knight.csv", index=False)
val_df.to_csv("Validation_knight.csv", index=False)

print(f"Split complete:")
print(f"  Training_knight.csv  : {len(train_df)} rows ({len(train_df)/len(df)*100:.0f}%)")
print(f"  Validation_knight.csv: {len(val_df)} rows ({len(val_df)/len(df)*100:.0f}%)")