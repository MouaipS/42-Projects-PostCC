from sqlalchemy import create_engine, inspect
import pandas as pd
import sqlalchemy

def load_csv(path, table_name):    
    engine = create_engine("postgresql+psycopg2://adrouin:mysecretpassword@localhost:5432/piscineds")
    inspector = inspect(engine)
    table_name = table_name.replace(".csv", "")
    if not inspector.has_table(table_name):
        print(f"New table {table_name}")
        file = pd.read_csv(path)
        define_type = {
            "event_time": sqlalchemy.DateTime(),
            "event_type": sqlalchemy.String(255),
            "product_id": sqlalchemy.Integer(),
            "price": sqlalchemy.Float(),
            "user_id": sqlalchemy.BigInteger(),
            "user_session": sqlalchemy.UUID(as_uuid=True),
        }
        file.to_sql(table_name, engine, index=False, dtype=define_type)
        
    engine.dispose()
        
if __name__ == "__main__":
    load_csv("/home/adrouin/Documents/Post-CC/Python-Data-Science/Data_engineer/ex02/customer/data_2022_dec.csv", "data_2022_dec.csv")