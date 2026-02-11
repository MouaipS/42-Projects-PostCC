import sys

def main():
	if len(sys.argv) != 3:
		raise AssertionError
	sentence = sys.argv[1]
	nb = int(sys.argv[2])
	longer_status = lambda checker: len(checker) >= nb
	result = [words for words in sentence.split() if longer_status(words)]
	print(result)

if __name__ == "__main__":
	try:
		main()
	except (AssertionError, ValueError):
		print("AssertionError: the arguments are bad")