def give_bmi(height: list[int | float], weight: list[int |float])->list [int | float]:
	"""
	Calculate BMI values from lists of heights and weights.
    -height: List of heights in meters
    -weight: List of weights in kilograms
        
    Returns:
        List of BMI values (weight / height^2)
        
    Raises:
        TypeError: If inputs are not lists or contain non-numeric values
        ValueError: If lists have different lengths or contain invalid values
	"""

	if len(height) != len(weight):
		raise ValueError("Height and weight must have the same length !")
	elif len(height) == 0 or len(weight) == 0:
		raise ValueError("List cannot be empty !")
	bmi_list = [];
	for h, w in zip(height, weight): #permet d'iterer en meme temps dans les deux listes ( w et h sont les valeurs de chaque liste)
		if not isinstance(h, (int, float)) or not isinstance(w, (int, float)):
			raise TypeError("All values must be integers or floats")
		if h <=0 or w <= 0:
			raise ValueError("Value must be positive value")
		bmi = w / (h ** 2) #opérateur exponentiel **
		bmi_list.append(bmi)
	return bmi_list

def apply_limit(bmi: list[int | float], limit: int)-> list [bool]:
	"""
    Check if BMI values are above a given limit.
    -bmi: List of BMI values
    -limit: Threshold value to compare against
        
    Returns:
        List of booleans (True if BMI > limit, False otherwise)
        
    Raises:
        TypeError: If bmi is not a list, contains non-numeric values, or limit is not int
        ValueError: If list is empty
    """
	if len(bmi) <= 0:
		raise ValueError("List cannot be emty")
	elif not isinstance(limit, int):
		raise TypeError("Need integer for limit")
	status = [];
	for res in bmi:
		if not isinstance(res, (int, float)):
			raise TypeError("Need integer or float in list")
		status.append(res > limit)
	return status