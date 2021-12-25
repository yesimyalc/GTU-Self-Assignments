def calcPoly(constants, x):
    result=0
    xPower=1
    for constant in reversed(constants):
        result+=constant*xPower
        xPower*=x
    return result

myConstants=[1,2,4,5,3]
myX=2
res=calcPoly(myConstants, myX)
print(res)