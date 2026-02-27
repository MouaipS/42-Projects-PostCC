from sqlalchemy import create_engine, inspect, text
import matplotlib.pyplot as plt
import pandas as pd

def building():
    engine = create_engine("postgresql+psycopg2://adrouin:mysecretpassword@localhost:5432/piscineds")
    inspector = inspect(engine)

    if inspector.has_table("customers"):
        # --- Graphique 1 : nombre de customers par fréquence d'achat ---
        with open("building_frequency.sql", "r") as sql_file:
            script = sql_file.read()
        with engine.connect() as conn:
            result = conn.execute(text(script))
            rows = result.fetchall()
            columns = result.keys()
        df_freq = pd.DataFrame(rows, columns=columns)

        plt.figure(figsize=(8, 5))
        plt.bar(df_freq["frequency"], df_freq["num_customers"], color="steelblue", width=0.8)
        plt.xlabel("frequency")
        plt.ylabel("customers")
        plt.tight_layout()
        plt.savefig("building_frequency.png")
        plt.show()

        # --- Graphique 2 : nombre de customers par tranche de dépense totale ---
        with open("building_monetary.sql", "r") as sql_file:
            script = sql_file.read()
        with engine.connect() as conn:
            result = conn.execute(text(script))
            rows = result.fetchall()
            columns = result.keys()
        df_mon = pd.DataFrame(rows, columns=columns)

        plt.figure(figsize=(8, 5))
        plt.bar(df_mon["monetary_range"], df_mon["num_customers"], color="steelblue", width=0.8)
        plt.xlabel("monetary value in ₳")
        plt.ylabel("customers")
        plt.tight_layout()
        plt.savefig("building_monetary.png")
        plt.show()

    engine.dispose()

if __name__ == "__main__":
    building()