def quickSelect(array, k):
    s=partition(array)
    if(s==k-1):
        return array[s]
    elif(s>k-1):
        return quickSelect(array[:s],k)
    else:
        return quickSelect(array[s+1:],k-1-s)
        
def partition(array):
    p=array[0]
    s=0
    for i in range(1,len(array)):
        if(array[i]<p):
            s=s+1
            swap(array, s, i)
        
    swap(array, 0, s)
    return s
    
def swap(list, pos1, pos2):
    list[pos1], list[pos2] = list[pos2], list[pos1]
    return list
    
rates=[20,10,4,5,23,45,21,78,90,54,93,81,11]
print(quickSelect(rates, 5))