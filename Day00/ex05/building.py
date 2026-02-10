import sys

def print_info(user_info):

	punctuation = "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"
	upper = 0
	lower = punct = digit = 0
	print(f"The text contains {len(user_info)} characters:")
	for c in user_info:
		if c.isupper():
			upper +=1
		elif c.islower():
			lower +=1
		elif c.isdigit():
			digit +=1
		elif c in punctuation:
			punct +=1

	print(f"{upper} uppercase(s)")
	print(f"{lower} lowercase(s)")
	print(f"{user_info.count(' ')} space(s)")
	print(f"{punct} punctuations marks")
	print(f"{digit} digit(s)")

def main():
	try:
		if len(sys.argv) == 1:
			print("What is the text to count ?")
			user_input = input()
		elif len(sys.argv) != 2:
			raise IndexError("Expected only one argument")
		else:
			user_input = sys.argv[1]
	except IndexError:
		print("AssertionError: more than one argument is provided")
		return
	print_info(user_input)

if __name__ == "__main__":
	main()