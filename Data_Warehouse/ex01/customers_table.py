from sqlalchemy import create_engine, inspect, text
import pandas as pd
import sqlalchemy

def merge():
    engine = create_engine("postgresql+psycopg2://adrouin:mysecretpassword@localhost:5432/piscineds")
    inspector = inspect(engine)
    if not inspector.has_table("customers"):
        print("New table merged customers")
        all_tables = inspector.get_table_names()
        valid_table = []
        for table_name in all_tables:
            if len(table_name) == 13 and table_name.startswith("data_202"):
                valid_table.append(table_name)
        if not valid_table:
            print("No valid table available")
            return
        command = "\nUNION ALL\n".join([f"SELECT * FROM {table}" for table in valid_table])
        with engine.begin() as conn:
            conn.execute(text(f"CREATE TABLE customers AS ({command})"))

        print(f"Done! customers created from {len(valid_table)} tables.")
        
    engine.dispose()
if __name__ == "__main__":
    merge()