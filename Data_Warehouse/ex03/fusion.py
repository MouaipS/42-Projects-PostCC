from sqlalchemy import create_engine, inspect, text
import psycopg2

def merge():
    engine = create_engine("postgresql+psycopg2://adrouin:mysecretpassword@localhost:5432/piscineds")
    inspector = inspect(engine)

    if inspector.has_table("customers") and inspector.has_table("items"):
        print("Let's merge customers and items table")
        with open("fusion.sql", "r") as sql_file:
            script = sql_file.read()
        with engine.connect() as conn:
            conn.execute(text(script))  # from sqlalchemy import text
            conn.commit()
        print("Done! Customers and items are merged.")
    
    engine.dispose()

if __name__ == "__main__":
    merge()