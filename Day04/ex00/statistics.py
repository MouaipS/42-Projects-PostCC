from typing import Any

def mean(data):
    res = sum(data)/len(data)
    return res

def median(data):
    sorted_data = sorted(data)
    length = len(data)
    mid = length // 2
    if length % 2:
        return (sorted_data[mid-1] + sorted_data[mid])/2
    return sorted_data[mid]
    
def quartile(data):
    sorted_data = sorted(data)
    length = len(data)
    q1 = sorted_data[length//4]
    q3 = sorted_data[(length * 3)//4]
    return [q1, q3]

def std(data):
    m = mean(data)
    variance = sum((items - m) ** 2 for items in data) / len(data)
    return variance ** 0.5
    
def var(data):
    m = mean(data)
    return sum((x - m) ** 2 for x in data) / len(data)
    

def ft_statistics(*args: Any, **kwargs: Any)-> None:
	operations = {
		"mean" : mean,
  		"median" : median,
		"quartile" : quartile,
		"std" : std,
  		"var" : var,
	}

	
	for value in kwargs.values():
		if not args:
			print("ERROR")
		elif value not in operations:
			pass
		else:
			res = operations[value](list(args))
			print(f"{value} : {res}")