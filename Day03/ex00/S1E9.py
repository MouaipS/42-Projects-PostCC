from abc import ABC, abstractmethod

class Character(ABC):
    """
	Docstring pour Character
	Abstract class
	args = first_name = name of the character
	set is_alive flag to True by default

	method = die(change is_alive)
	"""
    
    @abstractmethod
    def __init__(self, first_name, is_alive = True):
        self.first_name = first_name
        self.is_alive = is_alive
    
    def die(self):
        """
		Docstring pour die
		change is_alive flag
		"""
        pass
            

class Stark(Character):
    """
	Docstring pour Stark
	class inherit form Character ABC
	args = first_name = name of the character
	set is_alive flag to True by default(in ABC)

	method = die(change is_alive)
	"""
    def __init__(self, first_name, is_alive=True):
        """
		Docstring pour __init__
		
		:param first_name: name of the charactere
		"""
        self.first_name = first_name
        self.is_alive = is_alive
    
    def die(self):
        """
		Docstring pour die
		
		change is_alive flag
		"""
        if self.is_alive is True:
            self.is_alive = False
        else:
            self.is_alive = True