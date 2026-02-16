from S1E9 import Character

class Baratheon(Character):
    def __init__(self, first_name, is_alive=True):
        super().__init__(first_name, is_alive)
        self.family_name = "Barartheon"
        self.eyes = "brown"
        self.hairs = "black"
    
    def die(self):
        self.is_alive = not self.is_alive

class Lannister(Character):
    def __init__(self, first_name, is_alive=True):
        super().__init__(first_name, is_alive)
        self.family_name = "Lannister"
        self.eyes = "blue"
        self.hairs = "light"
    def die(self):
        self.is_alive = not self.is_alive
        
    @classmethod
    def create_lannister(cls, first_name, is_alive):
        instance = cls(first_name)
        instance.is_alive = is_alive
        return instance
        