def countString(string, startL, endL):
    count=0
    for i in range (len(string)-1):
        if(string[i]==startL):
            for j in range (i+1, len(string)):
                if(string[j]==endL):
                    count+=1
                    
    return count
res=countString("TXZXXJZWX", 'X', 'Z')
print(res)