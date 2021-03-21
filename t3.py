from statistics import mean, median
from math import ceil

def read_int(filename):
    with open(filename) as f:
        return [int(x) for x in f]


number_list = read_int('/tmp/lista-arqs2')
number_list.sort()

print("a)")
total_files = len(number_list)
print("number of elements in lista-arqs: ", total_files)

print("b)")
print("biggest file size in lista-arqs: ", number_list[-1])

print("c)")
print("file with size = 0: ", number_list.count(0))
print("percentage = ", (number_list.count(0)/len(number_list))*100)

print("d)")
list_mean = mean(number_list)
print("mean size of lista-arqs: ", list_mean)
count = len([i for i in number_list if i <= list_mean]) 
print("files with size lesser or equal than mean: ", str(count))
print("percentage: ", (count/total_files)*100)

print("e)")
list_median = median(number_list)
print("median size of lista-arqs: ", list_median)

print("f)")
count = len([i for i in number_list if i <= list_median]) 
print("files with size lesser or equal than median: ", str(count))
sum_fifty_per = sum(i for i in number_list if i <=list_median)
print("sum of elements in a list with size lesser or equal than median: ", sum_fifty_per)
sum_f_b = 0
counter = 0
for num in number_list:
    if(sum_f_b <= 16777216):
        sum_f_b += num
        counter += 1
    else:
        break
print("number of elements that can be summed until 16MB: ", counter)
print("percentage: ", (counter/total_files)*100)

print("g)")
block_size = 4096
file_count_g = 0
block_total = 0
file_count_level0 = 0
file_count_level1 = 0
file_count_level2 = 0
file_count_level3 = 0
for num in number_list:
    block_num = ceil(num / block_size)
    block_total += block_num
    file_count_g += 1
    if(block_total == 12):
        file_count_level0 = file_count_g
    if(block_total == 12+1094):
        file_count_level1 = file_count_g - file_count_level0
    else:
        file_count_level2 = file_count_g - (file_count_level1 + file_count_level0)
#print(block_total)

    #Arquivos esgotam antes da indireção tripla
print("arquivos nos 12 ponteiros diretos: " + str(file_count_level0) + " | " + str((file_count_level0/total_files)*100) + 
"\narquivos em indireção simples: " + str(file_count_level1) + " | " + str((file_count_level1/total_files)*100))
print("arquivos em indireção dupla: " + str(file_count_level2) + " | " + str((file_count_level2/total_files)*100) + 
"\narquivos em indireção tripla: " + str(file_count_level3) + " | " + str((file_count_level3/total_files)*100)) 

print("h)")
block_size = 4096
sum_h = 0
for num in number_list:
    block_num = ceil(num / block_size)
    sum_h += block_num*block_size - num
total_size = sum(number_list)
print("total size of files: ", total_size)
print("wasted: ", sum_h)
print("percentage: ", (sum_h/total_size)*100)