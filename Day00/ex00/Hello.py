ft_list = ["Hello", "tata!"]
ft_tuple = ("Hello", "toto!")
ft_set = {"Hello", "tutu!"}
ft_dict = {"Hello" : "titi!"}

ft_list[1] = "World!"

tuple_tmp = list(ft_tuple)
tuple_tmp[1] = "France!"
ft_tuple = tuple(tuple_tmp)

ft_set.remove("tutu!")
ft_set.add("Angouleme!")

ft_dict["Hello"] = "42Angouleme!"


print(ft_list)
print(ft_tuple)
print(ft_set)
print(ft_dict)