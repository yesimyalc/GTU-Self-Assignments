def findMaxCrossingClusterResult(array, start, middle, end):
    sum1 = 0
    left_sum = -10000
    for i in range(middle, start-1, -1):
        sum1 = sum1 + array[i]
        if (sum1 > left_sum):
            left_sum = sum1
 
    sum2 = 0
    right_sum = -10000
    for i in range(middle + 1, end + 1):
        sum2 = sum2 + array[i]
        if (sum2 > right_sum):
            right_sum = sum2
 
    return max(left_sum + right_sum, left_sum, right_sum)
 
 
def findMaxClusterResult(array, startIndex, endIndex):
    if (startIndex == endIndex):
        return array[startIndex]

    half = (startIndex + endIndex) // 2
 
    return max(findMaxClusterResult(array, startIndex, half),
               findMaxClusterResult(array, half+1, endIndex),
               findMaxCrossingClusterResult(array, startIndex, half, endIndex))
               
profits = [3, -5, 2, 11, -8, 9, -5]
size = len(profits)
res = findMaxClusterResult(profits, 0, size-1)
print("Maximum profit is ", res)