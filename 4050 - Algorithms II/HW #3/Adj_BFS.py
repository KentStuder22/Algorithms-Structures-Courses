#PSUEDOCODE
#BreadthFirstSearch(A, n, s):
#   visited = []
#   visited[s] = 1
#   
#   queue = []
#   queue_next = [s]
#   number = 0
#   WHILE queue_next is not empty:
#       queue = queue_next
#       queue_next = []
#       print(number)
#       number++
#       FOR i in range(0, len(queue)):
#           temp = queue[i]  
#           print(temp)
#           FOR j in range (i, n):
#               IF A[temp][i] == 1 AND visited[temp] != 1:
#                   queue_next.append(i)
#                   visited[i] = 1     
from calendar import c
import os
import time

def BFS(nNode, source, filename):

    adj_matrix = []

    current_dir = os.path.dirname(os.path.abspath(__file__))
    file_path = os.path.join(current_dir, filename)

    #reading data into 2D array
    with open(file_path, 'r') as file:
        for num_line in file.readlines():
            adj_matrix.append(num_line.split(' '))  

    visited = [0] * nNode
    visited[source] = 1
    next = [source]
    current = []
    number = 0
    while next:
        current = next
        next = []
        print(f"\nL" + str(number), end=': ')
        number += 1
        for i in range(0, len(current)):
            current_index = current[i]
            print(str(current_index + 1), end=', ')
            for j in range(i, nNode):
                if (adj_matrix[current_index][j] == '1' or adj_matrix[current_index][j] == '1\n') and visited[j] == 0:
                    next.append(j)
                    visited[j] = 1
    file.close()

def main():
    
    nNode = int(input("Number of Nodes: "))
    source = int(input("Source Node: "))
    filename = input("Filename: ")

    execution_start = time.time()

    BFS(nNode, source, filename)

    execution_end = time.time()
    final_exec = execution_end - execution_start
    print("\nExecution Time: " + (str(final_exec * 1000)) + "ms")

main()