def pow(a, n):
    if (n == 0): 
        return 1
    elif (int(n % 2) == 0):
        return (pow(a, n//2) *
               pow(a, n//2))
    else:
        return (a * pow(a, n//2) *
                   pow(a, n//2))

print(pow(3,5))