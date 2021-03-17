from statistics import mean, median
from math import ceil

def read_int(filename):
    with open(filename) as f:
        return [int(x) for x in f]


number_list = read_int('/tmp/lista-arqs2')
number_list.sort()

print("a)")
print("number of elements in lista-arqs: ", len(number_list))

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
print("percentage: ", (count/len(number_list))*100)

print("e)")
list_median = median(number_list)
print("median size of lista-arqs: ", list_median)

print("f)")
count = len([i for i in number_list if i <= list_median]) 
print("files with size lesser or equal than median: ", str(count))
fifty_sum = sum(i for i in number_list if i <=list_median)
print("sum of elements in a list with size lesser or equal than median: ", fifty_sum)
f_b_sum = 0
counter = 0
for num in number_list:
    if(f_b_sum <= 16777216):
        f_b_sum += num
        counter += 1
    else:
        break
print("number of elements that can be summed until 16MB: ", counter)
print("percentage: ", (counter/len(number_list))*100)

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