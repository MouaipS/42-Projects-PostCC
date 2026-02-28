from sqlalchemy import create_engine, inspect, text
import matplotlib.pyplot as plt
import pandas as pd
from sklearn.cluster import KMeans
from sklearn.preprocessing import StandardScaler

def clustering():
    engine = create_engine("postgresql+psycopg2://adrouin:mysecretpassword@localhost:5432/piscineds")
    inspector = inspect(engine)

    if inspector.has_table("customers"):
        with open("clustering_rfm.sql", "r") as sql_file:
            script = sql_file.read()
        with engine.connect() as conn:
            result = conn.execute(text(script))
            rows = result.fetchall()
            columns = result.keys()
        df = pd.DataFrame(rows, columns=columns)

        # Normalisation
        scaler = StandardScaler()
        df_scaled = scaler.fit_transform(df[["recency", "frequency", "monetary"]])

        # KMeans avec 5 clusters (résultat de l'elbow method)
        kmeans = KMeans(n_clusters=5, random_state=42, n_init=10)
        df["cluster"] = kmeans.fit_predict(df_scaled)

        # Assignation des labels selon les caractéristiques de chaque cluster
        cluster_stats = df.groupby("cluster").agg(
            median_recency=("recency", "median"),
            median_frequency=("frequency", "median"),
            median_monetary=("monetary", "median"),
            count=("user_id", "count")
        ).reset_index()

        # On trie par fréquence pour assigner les labels
        cluster_stats = cluster_stats.sort_values("median_frequency")
        labels_map = {}
        sorted_clusters = cluster_stats["cluster"].tolist()
        labels_map[sorted_clusters[0]] = "inactive"
        labels_map[sorted_clusters[1]] = "New customers"
        labels_map[sorted_clusters[2]] = "Loyal customers - Silver"
        labels_map[sorted_clusters[3]] = "Loyal customers - Gold"
        labels_map[sorted_clusters[4]] = "Loyal customers - Platinum"

        df["label"] = df["cluster"].map(labels_map)
        cluster_stats["label"] = cluster_stats["cluster"].map(labels_map)

        # --- Graphique 1 : bar chart horizontal nombre de customers par groupe ---
        colors = ["mediumseagreen", "steelblue", "wheat", "salmon", "mediumpurple"]
        cluster_stats_sorted = cluster_stats.sort_values("count")

        plt.figure(figsize=(10, 5))
        bars = plt.barh(
            cluster_stats_sorted["label"],
            cluster_stats_sorted["count"],
            color=colors[:len(cluster_stats_sorted)]
        )
        for bar, val in zip(bars, cluster_stats_sorted["count"]):
            plt.text(val + 200, bar.get_y() + bar.get_height() / 2,
                     str(val), va="center")
        plt.xlabel("number of customers")
        plt.tight_layout()
        plt.savefig("clustering_bar.png")
        plt.show()

        # --- Graphique 2 : scatter plot Recency vs Frequency (bubble = monetary) ---
        group_summary = df.groupby("label").agg(
            median_recency=("recency", "median"),
            median_frequency=("frequency", "median"),
            avg_monetary=("monetary", "mean")
        ).reset_index()

        plt.figure(figsize=(9, 6))
        bubble_colors = {"inactive": "mediumseagreen", "New customers": "steelblue",
                         "Loyal customers - Silver": "wheat",
                         "Loyal customers - Gold": "salmon",
                         "Loyal customers - Platinum": "mediumpurple"}
        for _, row in group_summary.iterrows():
            plt.scatter(
                row["median_recency"], row["median_frequency"],
                s=row["avg_monetary"] * 2,
                color=bubble_colors.get(row["label"], "gray"),
                alpha=0.7
            )
            plt.annotate(
                f'Average "{row["label"]}": {row["avg_monetary"]:.0f}₳',
                (row["median_recency"], row["median_frequency"]),
                textcoords="offset points", xytext=(10, 5), fontsize=9
            )
        plt.xlabel("Median Recency (month)")
        plt.ylabel("Median Frequency")
        plt.tight_layout()
        plt.savefig("clustering_bubble.png")
        plt.show()

    engine.dispose()

if __name__ == "__main__":
    clustering()