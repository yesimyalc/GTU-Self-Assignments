def mergeSort(array, array2):
    if len(array) > 1:
        mid = len(array)//2
        left = array[0:mid]
        right = array[mid:len(array)]
        left2 = array2[0:mid]
        right2 = array2[mid:len(array)]
        mergeSort(left, left2)
        mergeSort(right, right2)
        i=0
        j=0
        k=0

        while i < len(left) and j < len(right):
            if left[i] < right[j]:
                array[k] = left[i]
                array2[k] = left2[i]
                i += 1
            else:
                array[k] = right[j]
                array2[k] = right2[j]
                j += 1
            k += 1

        while i < len(left):
            array[k] = left[i]
            array2[k] = left2[i]
            i += 1
            k += 1

        while j < len(right):
            array[k] = right[j]
            array2[k] = right2[j]
            j += 1
            k += 1
            
def calcMaxCourseAmount(startTs, finishTs):
    amount=0
    selectedST=startTs[0]
    selectedFT=finishTs[0]
    amount=amount+1
    mergeSort(finishTs, startTs)
    
    for i in range(1, len(startTs)):
        if(startTs[i]>=selectedFT):
            selectedST=startTs[i]
            selectedFT=finishTs[i]
            amount=amount+1
        
    return amount
    
startTs=[1,3,0,5,8,5]
finishTs=[2,4,6,7,9,9]
print(calcMaxCourseAmount(startTs, finishTs))