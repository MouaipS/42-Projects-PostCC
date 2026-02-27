from sqlalchemy import create_engine, inspect, text
import matplotlib.pyplot as plt
import pandas as pd

def mustache():
    engine = create_engine("postgresql+psycopg2://adrouin:mysecretpassword@localhost:5432/piscineds")
    inspector = inspect(engine)

    if inspector.has_table("customers"):
        # --- Stats + Box plot 1 : prix de tous les articles achetés ---
        with open("mustache_prices.sql", "r") as sql_file:
            script = sql_file.read()
        with engine.connect() as conn:
            result = conn.execute(text(script))
            rows = result.fetchall()
            columns = result.keys()
        df = pd.DataFrame(rows, columns=columns)

        print(df["price"].describe())

        # Box plot 1 : tous les prix (horizontal, avec outliers)
        plt.figure(figsize=(10, 3))
        plt.boxplot(df["price"], vert=False, flierprops=dict(marker='o', markersize=3))
        plt.xlabel("price")
        plt.tight_layout()
        plt.savefig("mustache_all_prices.png")
        plt.show()

        # Box plot 2 : zoom sans les outliers extrêmes (IQR)
        q1 = df["price"].quantile(0.25)
        q3 = df["price"].quantile(0.75)
        iqr = q3 - q1
        df_filtered = df[(df["price"] >= q1 - 1.5 * iqr) & (df["price"] <= q3 + 1.5 * iqr)]

        plt.figure(figsize=(8, 3))
        plt.boxplot(df_filtered["price"], vert=False,
                    patch_artist=True,
                    boxprops=dict(facecolor="mediumseagreen"),
                    flierprops=dict(marker='o', markersize=3))
        plt.xlabel("price")
        plt.tight_layout()
        plt.savefig("mustache_prices_zoom.png")
        plt.show()

        # --- Box plot 3 : panier moyen par user ---
        with open("mustache_avg_basket.sql", "r") as sql_file:
            script = sql_file.read()
        with engine.connect() as conn:
            result = conn.execute(text(script))
            rows = result.fetchall()
            columns = result.keys()
        df_basket = pd.DataFrame(rows, columns=columns)

        plt.figure(figsize=(8, 3))
        plt.boxplot(df_basket["avg_basket"], vert=False,
                    patch_artist=True,
                    boxprops=dict(facecolor="steelblue"),
                    flierprops=dict(marker='o', markersize=3))
        plt.xlabel("price")
        plt.tight_layout()
        plt.savefig("mustache_avg_basket.png")
        plt.show()

    engine.dispose()

if __name__ == "__main__":
    mustache()