def findMaxProfit(array):
    totalMax =array[0]
    currentMax = array[0]
    
    for n in range(1,len(array)):
        currentMax = max(array[n], currentMax + array[n])
        totalMax = max(totalMax,currentMax)
        
    return totalMax

array = [3, -5, 2, 11, -8, 9, -5]
print(findMaxProfit(array))