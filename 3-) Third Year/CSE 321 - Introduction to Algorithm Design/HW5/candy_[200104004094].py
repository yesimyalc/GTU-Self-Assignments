def maxCandyProfit(initialCandyL, candyLs, candyPs):
    K = [[0 for x in range(initialCandyL + 1)] for x in range(len(candyPs) + 1)]
    
    for li in range(len(candyPs) + 1):
        for ci in range(initialCandyL + 1):
            if (li == 0 or ci == 0):
                K[li][ci] = 0
            elif (candyLs[li-1] <= ci):
                K[li][ci] = max(candyPs[li-1]+ K[li-1][ci-candyLs[li-1]],  
                              K[li-1][ci])
            else:
                K[li][ci] = K[li-1][ci]

    return K[len(candyPs)][initialCandyL]

candyPs = [1, 5, 8, 9, 10, 17, 17, 20]
candyLs = [1, 2, 3, 4, 5, 6, 7, 8]
initialCandyL = 8
print (maxCandyProfit(initialCandyL, candyLs, candyPs))