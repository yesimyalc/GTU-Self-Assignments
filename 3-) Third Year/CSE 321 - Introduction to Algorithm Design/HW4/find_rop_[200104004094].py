def findRop(array):
    if len(array) <=1:
        return 0
    if len(array) > 1:
        mid = len(array)//2
        left = array[0:mid]
        right = array[mid:len(array)]
        counter=findRop(left)+findRop(right)
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
                for m in range(i, len(left)):
                    counter=counter+1
            k += 1

        while i < len(left):
            array[k] = left[i]
            i += 1
            k += 1

        while j < len(right):
            array[k] = right[j]
            j += 1
            k += 1
            
        return counter
            
rates=[20,10,4,5,23,45,21,78,90,54,93,81,11]
print(findRop(rates))