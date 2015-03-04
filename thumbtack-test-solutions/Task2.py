# 2. Найти первые N натуральных чисел, делителями которых являются
# только 2, 3, 5

# Видимо, в задании имелись в виду числа, ПРОСТЫМИ делителями которых
# являются только 2, 3, 5, иначе задача бы не имела решение, так как
# любое число делится на 1 (а также на самого себя и на произведение своих
# простых делителей)

def integerSolutions(p):
    """integerSolutins(p) --> count of integer solutions (x, y, z) of equation
x * log(2) + y * log(3) + z * log(5) <= log(p)"""
    from math import floor, log

    if p <= 0:
        return 0
    
    log2, log3, log5, logP = log(2), log(3), log(5), log(p)
    x, y, z = 0, 0, 0
    count = 0

    # count integer solutions (x, y, z) of equation
    # x * log2 + y * log3 + z * log5 <= logP
    while logP - z * log5 >= 0:
        y = 0
        
        # count integer solutions (x, y) of equation
        # x * log2 + y * log3 <= logP - z * log5
        while logP - z * log5 - y * log3 >= 0:
            
            # add count of integer solution x of equation
            # x >= (logP - z * log5 - y * log3) / log2
            count += floor((logP - z * log5 - y * log3) / log2) + 1
            # FIXME: recalculation of same expressions
            y += 1
        z += 1
    return count

def solveEquation(f, y, precision):
    """solveEquation(f, y) --> x such that f(x) == y
Note: function f(x) must be increasing"""
    
    a, b = 0, 1
    # invariant: a <= b
    
    while f(b) < y:
        b <<= 1

    mid = (b + a) / 2
    while abs(f(mid) - y) > precision:
        if f(mid) >= y:
            b = mid
        elif f(mid) <= y:
            a = mid
            
        mid = (b + a) / 2
    return mid

def solution(N):
    """solution(N) generates first N numbers x such that
x == (2 ** a) * (3 ** b) * (5 ** c) for some natural a, b, c"""
    maxVal = solveEquation(lambda x: integerSolutions(x), N, 0.5)
    assert integerSolutions(maxVal) == N
    
    # enumerate (a, b, c) such that (2 ** a) * (3 ** b) * (5 ** c) <= maxVal
    c = 0
    while 5 ** c <= maxVal:
        b = 0
        # FIXME: recalculations, readable but not efficient
        while (3 ** b) * (5 ** c) <= maxVal:
            a = 0
            while (2 ** a) * (3 ** b) * (5 ** c) <= maxVal:
                yield (2 ** a) * (3 ** b) * (5 ** c)
                a += 1
            b += 1
        c += 1    

# testing
if __name__ == '__main__':
    from math import ceil, sqrt

    class TestFailed(Exception):
        pass

    try:
        N = 10 * 1000
        count = 0
        for x in solution(N):
            # find divisors
            y = x
            while y % 2 == 0:
                y //= 2
            while y % 3 == 0:
                y //= 3
            while y % 5 == 0:
                y //= 5
            if y != 1:
                raise TestFailed("x = %d, y = %y" % (x, y ))
            count += 1
        if count != N:
            raise TestFailed("count = %d", count)
    except TestFailed as ex:
        print("Test fails:", ex)
    else:
        print("Test passed")
        
