import math
def printCluster(array, names):
    start=-1
    end=-1
    result=-1
    sum=-1
    for i in range (len(array)):
        sum=array[i]
        for j in range (i+1, len(array)):
            sum+=array[j]
            if(result<sum):
                start=i
                end=j
                result=sum
    print('The maximum cluster happens in stations: ')
    for k in range (start, end+1):
        print(names[k])
    return

stations=[3, -5, 2, 11, -8, 9, -5]
stationNames=['A', 'B', 'C', 'D', 'E', 'F', 'G']
printCluster(stations, stationNames)