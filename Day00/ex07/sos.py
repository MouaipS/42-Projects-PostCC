import sys

MORSE_DICT = { 'A':'.-', 'B':'-...',
                    'C':'-.-.', 'D':'-..', 'E':'.',
                    'F':'..-.', 'G':'--.', 'H':'....',
                    'I':'..', 'J':'.---', 'K':'-.-',
                    'L':'.-..', 'M':'--', 'N':'-.',
                    'O':'---', 'P':'.--.', 'Q':'--.-',
                    'R':'.-.', 'S':'...', 'T':'-',
                    'U':'..-', 'V':'...-', 'W':'.--',
                    'X':'-..-', 'Y':'-.--', 'Z':'--..',
                    '1':'.----', '2':'..---', '3':'...--',
                    '4':'....-', '5':'.....', '6':'-....',
                    '7':'--...', '8':'---..', '9':'----.',
                    '0':'-----', ',':'--..--', '.':'.-.-.-',
                    '?':'..--..', '/':'-..-.', '-':'-....-',
                    '(':'-.--.', ')':'-.--.-'}

def main():
	if len(sys.argv) != 2:
		raise AssertionError
	message = sys.argv[1].upper()
	index = ''
	for charac in message:
		if charac.isspace():
			index += ' '
		elif charac in MORSE_DICT:
			index += MORSE_DICT[charac]
		else:
			raise AssertionError
	print (index)

if __name__ == "__main__":
	try:
		main()
	except AssertionError:
		print("AssertionError: the arguments are bad")