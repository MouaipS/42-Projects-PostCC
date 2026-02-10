import math

def NULL_not_found(object: any)-> int:
	if object is None: #None est un objet singleton
		print(f"{object}: None {type(object)}")
	elif isinstance(object, float) and math.isnan(object):
		print(f"Cheese: nan {type(object)}")
	elif object == 0:
		print(f"{object}: 0 {type(object)}")
	elif isinstance(object, type(NULL_not_found)):
		print(f"{object}: {type(object)}")
	elif object is False:
		print(f"{object}: False {type(object)}")
	else:
		print("Type not found")
	return 1