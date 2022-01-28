def mergeSort(array):
    if len(array) > 1:
        mid = len(array)//2
        left = array[0:mid]
        right = array[mid:len(array)]
        mergeSort(left)
        mergeSort(right)
        i=0
        j=0
        k=0

        while i < len(left) and j < len(right):
            if left[i] < right[j]:
                array[k] = left[i]
                i += 1
            else:
                array[k] = right[j]
                j += 1
            k += 1

        while i < len(left):
            array[k] = left[i]
            i += 1
            k += 1

        while j < len(right):
            array[k] = right[j]
            j += 1
            k += 1
            
def findMinMaxRate(ratesArray):
    mergeSort(ratesArray)
    minMaxRatesArray=[]
    minMaxRatesArray.insert(0, ratesArray[0])
    minMaxRatesArray.insert(1, ratesArray[len(ratesArray)-1])
    return minMaxRatesArray
            
rates=[20,10,4,5,23,45,21,78,90,54,93,81,11]
print(findMinMaxRate(rates))