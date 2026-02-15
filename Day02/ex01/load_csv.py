import pandas as pd

def load(path: str)-> pd.DataFrame:
    try:
        dataset = pd.read_csv(path)
        rows, columns = dataset.shape
        print(f"Loading dataset of dimensions ({rows}, {columns})")
        return dataset
    except FileNotFoundError:
        print(f"Error: File '{path}' not found.")
        return None
        
    except pd.errors.EmptyDataError:
        print("Error: The file is empty.")
        return None
        
    except pd.errors.ParserError:
        print("Error: Unable to parse the file. Bad format.")
        return None
        
    except Exception as e:
        print(f"Error: An unexpected error occurred: {e}")
        return None