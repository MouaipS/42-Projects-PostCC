import sys
import argparse

def whatis():
	try:
		if len(sys.argv) == 1:
			return
		number = int(sys.argv[1])
		if len(sys.argv) != 2:
			raise IndexError("Expected only one arguments")
	except IndexError:
		print("AssertionError: more than one argument is provided")
		return
	except ValueError:
		print("AssertionError: argument is not an integer")
		return
	if number % 2 == 0:
		print("I'm Even")
	else:
		print("I'm Odd")

if __name__ == "__main__":
	whatis()
