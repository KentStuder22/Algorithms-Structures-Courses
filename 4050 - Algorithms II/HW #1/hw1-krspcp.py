import time
import os

def Binary_Search(low, high, array, query , counter):
    #check if our low pointer is not greater then the high
    if(low <= high):
        
        middle = (low + high) // 2

        if(array[middle] == query):
            return middle
        elif(array[middle] < query):
            if(counter < 4):
                print("Low Pointer: " + str(low))
                print("High Pointer: " + str(high))
            return Binary_Search(middle + 1, high, array, query, counter + 1)
        else:
            if(counter < 4):
                print("Low Pointer: " + str(low))
                print("High Pointer: " + str(high))
            return Binary_Search(low, middle - 1, array, query, counter + 1)
    else:
        return -1

def main():
    execution_start = time.time()

    file_name = input("Filename: ")
    query_num = int(input("Query: "))

    current_dir = os.path.dirname(os.path.abspath(__file__))
    file_path = os.path.join(current_dir, file_name)

    with open(file_path, 'r') as file:
        numbers = file.read()

    #list comp to break the string into array of ints
    array = [int(number) for number in numbers.split()]

    low = 0
    high = len(array)
    counter = 0

    algo_start = time.time()
    bsearch_res = Binary_Search(low, high, array, query_num, counter)
    algo_end = time.time()

    execution_end = time.time()
    final_exec = execution_end - execution_start
    final_algo = algo_end - algo_start

    print("Index: " + str(bsearch_res))
    print("Execution Time: " + str(final_exec))
    print("Algorithm Time: " + str(final_algo))
    

main()