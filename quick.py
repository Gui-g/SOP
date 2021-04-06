num_list = [49,11,177,86,5,117,65,25,111,18]

teste = input()
teste = int(teste)

compare_list = list()

for num in num_list:
    compare_list.append(abs(teste - num))

compare_list.sort()
print(compare_list)

num_list.sort()
print(num_list)