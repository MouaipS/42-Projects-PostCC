import pandas as pd
import psycopg2
import glob
import os

csv_folder = "./customer"
csv_files = glob.glob(csv_folder+"/*.csv")
for file_path in csv_files:
	table_name = "staging_" + os.path.splitext(os.path.basename(file_path))[0]
	data_frame = pd.read_csv(file_path)
	data_frame.iloc[:, 0] = pd.to_datetime(data_frame.iloc[:, 0])

	conn = psycopg2.connect(
	    dbname="piscineds",
	    user="adrouin",
	    password="mysecretpassword",
	    host="postgres",
	    port=5432
	)
	cursor = conn.cursor()
	
	cursor.execute(f"""
	CREATE TABLE IF NOT EXISTS {table_name} (
	    event_time TIMESTAMP,
	    event_type TEXT,
	    product_id INTEGER,
	    price FLOAT,
	    user_id BIGINT,
	    user_session UUID
	)
	""")
	conn.commit()
	
	with open(file_path, "r") as f:
	    cursor.copy_expert(f"COPY {table_name} FROM STDIN WITH CSV HEADER", f)
	conn.commit()
	
	final_table = "final_" + os.path.splitext(os.path.basename(file_path))[0]
	
	# Créer table finale si elle n’existe pas
	cursor.execute(f"""
	CREATE TABLE IF NOT EXISTS {final_table} (
	    event_time TIMESTAMP,
	    event_type TEXT,
	    product_id INTEGER,
	    price FLOAT,
	    user_id BIGINT,
	    user_session UUID
	)
	""")
	conn.commit()
	
	# Copier les données filtrées depuis staging
	cursor.execute(f"""
	INSERT INTO {final_table} (event_time, event_type, product_id, price, user_id, user_session)
	SELECT *
	FROM {table_name}
	WHERE price > 0
	""")
	conn.commit()
	print("Données filtrées copiées vers la table finale :", final_table)
	
	cursor.close()
	conn.close()
	print("Connexion fermée pour ce fichier\n")
