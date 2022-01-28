def pow(a, n):
    xPower=1
    for i in range(0, n):
        xPower*=a
    return xPower
    
print(pow(3,5))