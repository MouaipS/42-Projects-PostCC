from load_csv import load
import matplotlib.pyplot as plt
import numpy as np


def main():
    """
    Load life expectancy and GDP data, then display a scatter plot
    showing the correlation between GDP per capita and life expectancy
    for the year 1900 with a trend line.
    """
    try:
        # Charger les deux datasets
        life_expectancy = load("life_expectancy_years.csv")
        gdp_data = load("income_per_person_gdppercapita_ppp_inflation_adjusted.csv")
        
        if life_expectancy is None or gdp_data is None:
            print("Failed to load one or both datasets")
            return
        
        # Vérifier que la colonne 1900 existe
        year = "1900"
        if year not in life_expectancy.columns or year not in gdp_data.columns:
            print(f"Year {year} not found in datasets")
            return
        
        # Créer des listes pour stocker les valeurs valides
        gdp_values = []
        life_exp_values = []
        
        # Parcourir tous les pays dans life_expectancy
        for idx, row in life_expectancy.iterrows():
            country = row['country']
            life_exp = row[year]
            
            # Chercher le même pays dans gdp_data
            gdp_row = gdp_data[gdp_data['country'] == country]
            
            if not gdp_row.empty:
                gdp = gdp_row[year].values[0]
                
                # Vérifier que les deux valeurs sont valides (pas NaN)
                try:
                    life_exp_float = float(life_exp)
                    gdp_float = float(gdp)
                    
                    # Ajouter seulement si les deux valeurs sont valides
                    if not (life_exp_float != life_exp_float or gdp_float != gdp_float):
                        gdp_values.append(gdp_float)
                        life_exp_values.append(life_exp_float)
                except (ValueError, TypeError):
                    continue
        
        print(f"Number of countries with data: {len(gdp_values)}")
        
        # Créer le scatter plot
        plt.figure(figsize=(12, 7))
        
        # Scatter plot avec des points bleus
        plt.scatter(gdp_values, life_exp_values, 
                   alpha=0.6,
                   s=60,
                   color='#1f77b4',
                   edgecolors='white',
                   linewidth=0.5)
        
        # Ajouter une ligne de tendance (régression logarithmique)
        if len(gdp_values) > 2:
            # Calculer la régression sur l'échelle log
            log_gdp = np.log10(gdp_values)
            z = np.polyfit(log_gdp, life_exp_values, 1)
            p = np.poly1d(z)
            
            # Créer des points pour la ligne de tendance
            gdp_sorted = np.logspace(np.log10(min(gdp_values)), 
                                     np.log10(max(gdp_values)), 
                                     100)
            life_trend = p(np.log10(gdp_sorted))
            
            # Tracer la ligne de tendance
            plt.plot(gdp_sorted, life_trend, 
                    'r--', 
                    alpha=0.7, 
                    linewidth=2,
                    label='Trend line')
            
            # Calculer le coefficient de corrélation
            correlation = np.corrcoef(log_gdp, life_exp_values)[0, 1]
            print(f"Correlation coefficient: {correlation:.3f}")
        
        # Ajouter titre et légendes
        plt.title(year, fontsize=18, fontweight='bold', pad=20)
        plt.xlabel("Gross domestic product", fontsize=13)
        plt.ylabel("Life Expectancy", fontsize=13)
        
        # Utiliser une échelle logarithmique pour l'axe X (GDP)
        plt.xscale('log')
        
        # Ajouter la grille
        plt.grid(True, alpha=0.3, linestyle='--')
        
        # Ajouter la légende si on a une ligne de tendance
        if len(gdp_values) > 2:
            plt.legend(loc='lower right', fontsize=10)
        
        # Afficher
        plt.tight_layout()
        plt.show()
        
        # Afficher la question du sujet
        print("\n" + "="*60)
        print("Do you see a correlation between life span and")
        print("gross domestic product?")
        print("="*60)
        print("\nAnswer: YES! The scatter plot shows a clear positive")
        print("correlation. Countries with higher GDP per capita tend")
        print("to have higher life expectancy.")
        
    except Exception as e:
        print(f"Error: {e}")
        import traceback
        traceback.print_exc()


if __name__ == "__main__":
    main()