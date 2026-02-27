from sqlalchemy import create_engine, inspect, text
import matplotlib.pyplot as plt
import pandas as pd
from sklearn.cluster import KMeans
from sklearn.preprocessing import StandardScaler

def elbow():
    engine = create_engine("postgresql+psycopg2://adrouin:mysecretpassword@localhost:5432/piscineds")
    inspector = inspect(engine)

    if inspector.has_table("customers"):
        with open("elbow_rfm.sql", "r") as sql_file:
            script = sql_file.read()
        with engine.connect() as conn:
            result = conn.execute(text(script))
            rows = result.fetchall()
            columns = result.keys()
        df = pd.DataFrame(rows, columns=columns)

        # Normalisation des données avant clustering
        scaler = StandardScaler()
        df_scaled = scaler.fit_transform(df[["recency", "frequency", "monetary"]])

        # Elbow Method : on teste de 1 à 10 clusters
        inertias = []
        k_range = range(1, 11)
        for k in k_range:
            kmeans = KMeans(n_clusters=k, random_state=42, n_init=10)
            kmeans.fit(df_scaled)
            inertias.append(kmeans.inertia_)

        plt.figure(figsize=(8, 5))
        plt.plot(k_range, inertias, color="steelblue")
        plt.xlabel("Number of clusters")
        plt.title("The Elbow Method")
        plt.tight_layout()
        plt.savefig("elbow.png")
        plt.show()

    engine.dispose()

if __name__ == "__main__":
    elbow()