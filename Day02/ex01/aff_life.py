from ex03.load_csv import load
import matplotlib.pyplot as plt


def main():
    """
    Load life expectancy data and display it for France.
    """
    try:
        dataset = load("life_expectancy_years.csv")
        
        if dataset is None:
            print("Failed to load dataset")
            return
                
        country = "France"
        country_data = dataset[dataset['country'] == country]
        #Compare chaque valeur de la colonne 'country' avec la variable `country` (ex: "France")
		#Retourne une Series de booléens (True/False)
		#dataset[...]` utilise cette serie de booléens comme un "masque"
		#Garde seulement les lignes où la valeur est `True`
		#Résultat : un DataFrame avec seulement la ligne de la France
        
        if country_data.empty:
            print(f"Error: {country} not found in dataset")
            return
        
        year_columns = [col for col in dataset.columns if col != 'country']
        #Prend toutes les colonnes sauf celle de la France
        #col pour creer une nouvelle liste avec les colonnes filtrees
        
        years = [int(year) for year in year_columns] # transformation de string a int
        life_expectancy = country_data[year_columns].values[0] #les valeurs sont dans un numpy et on recupere la premiere ligne
        
        # Create the plot
        plt.figure(figsize=(10, 6))
        
        # Plot the data
        plt.plot(years, life_expectancy, color='blue')
        
        # Add title and labels
        plt.title(f"{country} Life expectancy Projections")
        plt.xlabel("Year")
        plt.ylabel("Life expectancy")
        
        # Add grid for better readability
        plt.grid(True, alpha=0.3)
        
        # Show the plot
        plt.tight_layout()
        plt.show()
        
    except Exception as e:
        print(f"Error: {e}")


if __name__ == "__main__":
    main()

