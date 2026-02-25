from sqlalchemy import create_engine, inspect, text
import pandas as pd
import sqlalchemy

def merge():
    engine = create_engine("postgresql+psycopg2://adrouin:mysecretpassword@localhost:5432/piscineds")
    inspector = inspect(engine)
    if inspector.has_table("customers"):
        print("Table customers with no duplicates")
        with engine.begin() as conn:
            conn.execute(text(f"CREATE TEMPORARY TABLE temp_customers AS SELECT DISTINCT * FROM customers;"))
            conn.execute(text(f"TRUNCATE customers;"))
            conn.execute(text(f"INSERT INTO customers SELECT * FROM temp_customers;"))

        print(f"Done! customers have no more duplicates.")
        
    engine.dispose()
if __name__ == "__main__":
    merge()


