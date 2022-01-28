def cutting(wireSize):
    if(wireSize==1):
        return 0
    else:
        if(wireSize%2==0):
            return cutting(wireSize//2)+1
        else:
            return cutting(wireSize//2+1)+1
        
res=cutting(100)
print(res)