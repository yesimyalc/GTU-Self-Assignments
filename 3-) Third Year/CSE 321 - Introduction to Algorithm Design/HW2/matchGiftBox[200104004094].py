class test :
    def main(self, args) :
        arrayA = [[0, 6], [1, 12], [2, 4], [3, 3], [4, 11], [5, 56], [6, 21]]
        arrayB = [[0, 4], [1, 6], [2, 12], [3, 3], [4, 56], [5, 21], [6, 11]]
        print("The Gifts:")
        j = 0
        while (j < len(arrayA)) :
            print("(Size:" + str(arrayA[j][1]) + ", Gift:" + str(arrayA[j][0]) + ") ", end ="")   
            j += 1
        print()
        print("The Boxes:")
        j = 0
        while (j < len(arrayB)) :
            print("(Size:" + str(arrayB[j][1]) + ", Box:" + str(arrayB[j][0]) + ") ", end ="")    
            j += 1
        print()
        print()
        arrayB = self.giftBoxSort(arrayB, arrayA, 0)
        arrayA = self.giftBoxSort(arrayA, arrayB, 0)
        arrayC = [[None] * (3) for _ in range(len(arrayA))]
        i = 0
        while (i < len(arrayA)) :
            arrayC[i][0] = arrayA[i][0]
            arrayC[i][1] = arrayB[i][0]
            if (arrayA[i][1] == arrayB[i][1]) : 
                arrayC[i][2] = arrayA[i][1]
            i += 1
        print("Matches")
        i = 0
        while (i < len(arrayC)) :
            print("(Gift no: " + str(arrayC[i][0]) + ", Box no: " + str(arrayC[i][1]) + ", Size: " + str(arrayC[i][2]) + ")")    
            i += 1
            
    def  giftBoxSort(self, array,  pivotArray,  count) :
        if (count == len(pivotArray)) :
            return  array
        else :
            array = self.sort(array, pivotArray[count][1])
            count += 1
            return  self.giftBoxSort(array, pivotArray, count)
            
    def  sort(self, array,  pivot) :
        if (len(array) < 2) :
            return  array
        self.partition(array, 0, len(array) - 1, pivot)
        return  array
        
    def swap(self, table,  pIndex,  cIndex) :
        tempF = table[pIndex][0]
        tempS = table[pIndex][1]
        table[pIndex][0] = table[cIndex][0]
        table[pIndex][1] = table[cIndex][1]
        table[cIndex][0] = tempF
        table[cIndex][1] = tempS
        
    def partition(self, table,  start,  end,  pivot) :
        up = start
        down = end
        while True :
            while (up < down and table[up][1]<pivot) :
                up += 1
            while (table[down][1]>pivot) :
                down -= 1
            if (up < down) : self.swap(table, up, down)
            if((up < down) == False) :
                break


if __name__=="__main__":
    test().main([])