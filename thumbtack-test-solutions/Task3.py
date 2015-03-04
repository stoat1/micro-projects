# 3. Определить, можно ли представить заданные натуральное число как сумму
# кубов 3 натуральных чисел

from math import ceil, pow

def solution(n):
    """sulution(n) --> (a, b, c) such that n == a ** 3 + b ** 3 + c ** 3
or False if no such (a, b, c)"""
    
    # find (a, b, c) such that n == a ** 3 + b ** 3 + c ** 3 and a <= b <= c
    c = 1
    while n >= 1 ** 3 + 1 ** 3 + c ** 3:
        m = n - c ** 3
        
        # find (a, b) such that m == a ** 3 + b ** 3 and a <= b <= c
        a = 1 # set a to min possible value
        b = min(ceil(pow(m - 1 ** 3, 1 / 3)), c) # set b to max possible value
        while a <= b:
            x = a ** 3 + b ** 3
            if m < x:
                b -= 1
            elif m > x:
                a += 1
            else:
                return (a, b, c)
        c += 1
    return False

# testing
if __name__ == '__main__':
    
    from random import Random
    rnd = Random()

    fail = False
    for i in range(100):
        x = sum(rnd.randint(1, 100) ** 3 for i in range(3))
        sol = solution(x)
        if not sol or sol[0] ** 3 + sol[1] ** 3 + sol[2] ** 3 != x:
            print("True-test fails at x =", x)
            fail = True
            break
    if not fail:
        print("True-test passed")

    fail = False
    for i in range(100):
        sol = False
        while sol == False:
            x = rnd.randint(1, 100 * 1000)
            sol = solution(x)
            
        if sol[0] ** 3 + sol[1] ** 3 + sol[2] ** 3 != x:
            print("False-test fails at x = %d" % x)
            fail = True
            break
    if not fail:
        print("False-test passed")
