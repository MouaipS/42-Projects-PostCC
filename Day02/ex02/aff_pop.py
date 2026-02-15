from ex03.load_csv import load
import matplotlib.pyplot as plt

def preprocess_population(pop_str):
    """
    Preprocesses the population string to convert it into
    a numeric value in standard form.

    Args:
        pop_str (str): Population string with or without
        the 'M' suffix for million.

    Returns:
        float: Numeric population value.
    """

def main():
    """
    Loads population data from a CSV file, processes and
    plots(used for creating line plots)
    the population comparison of three countries.
    """
    dataset = load("population_total.csv")
    if dataset is None:
        print("Failed to load dataset")
        return
    
    countryF = "France"
    countryG = "Germany"
    countryJ = "Japan"
    
    france_data = dataset[dataset['country'] == countryF]
    germany_data = dataset[dataset['country'] == countryG]
    japan_data = dataset[dataset['country'] == countryJ]
    
    if france_data.empty:
        print(f"Error: {countryF} not found in dataset")
        return
    
    if germany_data.empty:
        print(f"Error: {countryG} not found in dataset")
        return
    
    if japan_data.empty:
        print(f"Error: {countryJ} not found in dataset")
        return
    
    years = [str(year) for year in range(1800, 2051)]
    # Filtrer seulement les années qui existent dans le dataset
    available_years = [year for year in years if year in dataset.columns]
    
    france_pop = []
    germany_pop = []
    japan_pop = []
    
    for year in available_years:
        france_value = france_data[year].values[0]
        germany_value = germany_data[year].values[0]
        japan_value = japan_data[year].values[0]
        
        france_pop.append(preprocess_population(france_value))
        germany_pop.append(preprocess_population(germany_value))
        japan_pop.append(preprocess_population(japan_value))
    
    # Convertir les années en entiers pour le graphique
    years_int = [int(year) for year in available_years]
    
        plt.figure(figsize=(10, 6))
    
    # Tracer les trois courbes
    plt.plot(years_int, france_pop, label=countryF, color='blue')
    plt.plot(years_int, germany_pop, label=countryG, color='red')
    plt.plot(years_int, japan_pop, label=countryJ, color='green')
    
    # Ajouter titre et légendes
    plt.title("Population Projections")
    plt.xlabel("Year")
    plt.ylabel("Population")
    
    # Formater l'axe Y pour afficher en millions
    ax = plt.gca()
    ax.yaxis.set_major_formatter(
        plt.FuncFormatter(lambda x, p: f'{int(x/1e6)}M')
    )
    
    # Ajouter la légende
    plt.legend(loc='lower right')
    
    # Ajouter la grille
    plt.grid(True, alpha=0.3)
    
    # Limiter l'axe X de 1800 à 2050
    plt.xlim(1800, 2050)
    
    # Afficher
    plt.tight_layout()
    plt.show()


if __name__ == "__main__":
    main()
    
    
    
    