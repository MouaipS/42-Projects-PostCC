def slice_me(family: list, start: int, end: int)-> list:
	"""
    Slices a 2D array and prints shape information.
    -family: 2D list (list of lists)
    -start: Starting index for slicing
    -end: Ending index for slicing
    
    Returns:
        Sliced 2D list
    
    Raises:
        TypeError: If family is not a list or contains non-list elements
        ValueError: If family is empty or lists have different sizes
    """
	if not isinstance(family, list):
		raise TypeError("Family must be a list")
	if len(family) == 0:
		raise ValueError("Family can't be empty")
	for items in family:
		if not isinstance(items, list):
			raise TypeError("Elements in family must be a list")
		if len(items) != len(family[0]):
			raise ValueError("Lists must have the same length")
	
	rows = len(family)
	colomns = len(family[0])
	print(f"My shape is : ({rows}, {colomns})")

	slice = family[start:end]
	rows = len(slice)
	if rows > 0:
		colomns = len(slice[0])
	print(f"My new shape is : ({rows}, {colomns})")
	return slice
	
