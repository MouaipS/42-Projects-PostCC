import glob
import os
from sqlalchemy import create_engine, inspect
import pandas as pd
import sqlalchemy

def load_csv(path, table_name):    
    engine = create_engine("postgresql+psycopg2://adrouin:mysecretpassword@localhost:5432/piscineds")
    inspector = inspect(engine)

    if not inspector.has_table(table_name):
        print(f"New table {table_name}")
        file = pd.read_csv(path)
        define_type = {
            "event_time":   sqlalchemy.DateTime(),
            "event_type":   sqlalchemy.String(255),
            "product_id":   sqlalchemy.Integer(),
            "price":        sqlalchemy.Float(),
            "user_id":      sqlalchemy.BigInteger(),
            "user_session": sqlalchemy.UUID(as_uuid=True),
        }
        file.to_sql(table_name, engine, index=False, dtype=define_type)
    else:
        print(f"Table {table_name} already exists, skipping.")

    engine.dispose()

csv_folder = "./customer"
csv_files = glob.glob(csv_folder + "/*.csv")

for file_path in csv_files:
    table_name = os.path.splitext(os.path.basename(file_path))[0]
    load_csv(file_path, table_name)