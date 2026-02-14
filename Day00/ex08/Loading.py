import os
from tqdm import tqdm

def ft_tqdm(lst: range) -> None:
	term_size = os.get_terminal_size().columns - 50
	print(term_size)
	lst_size = len(lst)
	nb_step = term_size / lst_size

	i = 0
	for items in lst:
		i+=1
		already_filled = int(term_size * i/lst_size)
		empty = term_size - already_filled
		percent = int(100 * i / lst_size)
		bar = '[' + '=' * already_filled + '>' + ' ' * (empty - 1) + ']'
		print(f'\r{percent:3}%|{bar}| {i}/{lst_size}', end='', flush=True)
		yield items
	print()