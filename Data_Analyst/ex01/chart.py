from sqlalchemy import create_engine, inspect, text
import matplotlib.pyplot as plt
import pandas as pd

def charts():
    engine = create_engine("postgresql+psycopg2://adrouin:mysecretpassword@localhost:5432/piscineds")
    inspector = inspect(engine)

    if inspector.has_table("customers"):
        # --- Graphique 1 : Nombre de customers par jour (line chart) ---
        with open("chart_customers_per_day.sql", "r") as sql_file:
            script = sql_file.read()
        with engine.connect() as conn:
            result = conn.execute(text(script))
            rows = result.fetchall()
            columns = result.keys()
        df1 = pd.DataFrame(rows, columns=columns)
        df1["day"] = pd.to_datetime(df1["day"])

        plt.figure(figsize=(10, 5))
        plt.plot(df1["day"], df1["num_customers"], color="steelblue")
        plt.xlabel("")
        plt.ylabel("Number of customers")
        plt.tight_layout()
        plt.savefig("chart_customers_per_day.png")
        plt.show()

        # --- Graphique 2 : Total des ventes par mois (bar chart) ---
        with open("chart_sales_per_month.sql", "r") as sql_file:
            script = sql_file.read()
        with engine.connect() as conn:
            result = conn.execute(text(script))
            rows = result.fetchall()
            columns = result.keys()
        df2 = pd.DataFrame(rows, columns=columns)

        plt.figure(figsize=(8, 5))
        plt.bar(df2["month"], df2["total_sales"], color="steelblue", width=0.5)
        plt.xlabel("month")
        plt.ylabel("total sales in million of ₳")
        plt.tight_layout()
        plt.savefig("chart_sales_per_month.png")
        plt.show()

        # --- Graphique 3 : Dépense moyenne par customer par jour (area chart) ---
        with open("chart_avg_spend_per_day.sql", "r") as sql_file:
            script = sql_file.read()
        with engine.connect() as conn:
            result = conn.execute(text(script))
            rows = result.fetchall()
            columns = result.keys()
        df3 = pd.DataFrame(rows, columns=columns)
        df3["day"] = pd.to_datetime(df3["day"])

        plt.figure(figsize=(10, 5))
        plt.fill_between(df3["day"], df3["avg_spend"], color="steelblue", alpha=0.4)
        plt.plot(df3["day"], df3["avg_spend"], color="steelblue", linewidth=0.8)
        plt.xlabel("")
        plt.ylabel("average spend/customers in ₳")
        plt.tight_layout()
        plt.savefig("chart_avg_spend_per_day.png")
        plt.show()

    engine.dispose()

if __name__ == "__main__":
    charts()