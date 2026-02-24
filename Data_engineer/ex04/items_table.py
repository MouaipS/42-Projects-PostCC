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
            "product_id": sqlalchemy.Integer(),
            "category_id": sqlalchemy.BigInteger(),
            "category_code": sqlalchemy.String(),
            "brand": sqlalchemy.String(),
        }
        file.to_sql(table_name, engine, index=False, dtype=define_type)
        
    engine.dispose()
        
if __name__ == "__main__":
    load_csv("/home/adrouin/Documents/Post-CC/Python-Data-Science/Data_engineer/ex04/item/item.csv", "items")