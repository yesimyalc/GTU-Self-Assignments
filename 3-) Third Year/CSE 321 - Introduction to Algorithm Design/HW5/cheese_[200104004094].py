def mergeSort(array, array2, array3):
    if len(array) > 1:
        mid = len(array)//2
        left = array[0:mid]
        right = array[mid:len(array)]
        left2 = array2[0:mid]
        right2 = array2[mid:len(array)]
        left3 = array3[0:mid]
        right3 = array3[mid:len(array)]
        mergeSort(left, left2, left3)
        mergeSort(right, right2, right3)
        i=0
        j=0
        k=0

        while i < len(left) and j < len(right):
            if left[i] < right[j]:
                array[k] = left[i]
                array2[k] = left2[i]
                array3[k] = left3[i]
                i += 1
            else:
                array[k] = right[j]
                array2[k] = right2[j]
                array3[k] = right3[j]
                j += 1
            k += 1

        while i < len(left):
            array[k] = left[i]
            array2[k] = left2[i]
            array3[k] = left3[i]
            i += 1
            k += 1

        while j < len(right):
            array[k] = right[j]
            array2[k] = right2[j]
            array3[k] = right3[j]
            j += 1
            k += 1
            
def calcMaxBoxPrice(cheesePs, cheeseWs, capacity):
    rates=[]
    price=0
    for i in range(0, len(cheesePs)):
        rates.insert(i, cheesePs[i]/cheeseWs[i])
    mergeSort(rates, cheesePs, cheeseWs)
    rates.reverse()
    cheesePs.reverse()
    cheeseWs.reverse()
    
    for i in range(0, len(rates)):
        if(capacity-cheeseWs[i]==0):
            price=price+cheesePs[i]
            capacity=0
            break
        elif(capacity-cheeseWs[i]>0):
            price=price+cheesePs[i]
            capacity=capacity-cheeseWs[i]
        else:
            price=price+(capacity*cheesePs[i]/cheeseWs[i])
            capacity=0
            break
        
    return price
    
cheesePs=[60, 40, 100, 120]
cheeseWs=[10, 40, 20, 30]
capacity=50
print(calcMaxBoxPrice(cheesePs, cheeseWs, capacity))