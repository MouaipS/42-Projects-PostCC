from sqlalchemy import create_engine, inspect, text
import matplotlib.pyplot as plt
import pandas as pd

def merge():
    engine = create_engine("postgresql+psycopg2://adrouin:mysecretpassword@localhost:5432/piscineds")
    inspector = inspect(engine)

    if inspector.has_table("customers"):
        with open("pie.sql", "r") as sql_file:
            script = sql_file.read()

        with engine.connect() as conn:
            result = conn.execute(text(script))
            rows = result.fetchall()  #on recupere chaque ligne de nos donnees
            columns = result.keys()	#on recup aussi les colonnes(fetchall perd cette donnee, on a que le contenu)

        df = pd.DataFrame(rows, columns=columns)

        plt.figure(figsize=(8, 8))
        plt.pie(
            df["total"],
            labels=df["event_type"],
            autopct="%1.1f%%",
            startangle=140
        )
        plt.title("Distribution des événements sur le site")
        plt.tight_layout()
        plt.savefig("pie_chart.png")
        plt.show()

    engine.dispose()

if __name__ == "__main__":
    merge()