# 1. Найти все натуральные числа <= N, делящиеся на каждую из своих цифр

def solution(N):
    """solution(N) generates natural numbers x <= N which is multiple of
each its digits in decimal notation"""
    count = 0
    value = 0
    
    mod3, mod5, mod7, mod9 = 0, 0, 0, 0 # remainders
    decRepr = [] # decimal representaion of value
    digits = [0] * 10 # multiset of digits decRepr contains
    
    while count < N:
        # increment value
        value += 1

        # increment remainders
        mod3 += 1
        if mod3 == 3:
            mod3 = 0

        mod5 += 1
        if mod5 == 5:
            mod5 = 0

        mod9 += 1
        if mod9 == 9:
            mod9 = 0

        mod7 += 1
        if mod7 == 7:
            mod7 = 0

        # increment decimal representation
        # and fix digit multiset
        i = 0
        while i < len(decRepr):
            if decRepr[i] != 9:
                digits[decRepr[i]] -= 1 # remove old digit
                decRepr[i] += 1
                digits[decRepr[i]] += 1 # add new digit
                break
            else:
                decRepr[i] = 0
                digits[9] -= 1 # remove old digit (9)
                digits[0] += 1 # add new digit (0)
                i += 1
        if i == len(decRepr):
            decRepr.append(1)

        # check division by 0
        if digits[0] > 0:
            continue

        # check division by 8, 4, 2
        if digits[8] != 0:
            if value & 0x7 != 0:
                continue
        elif digits[4] != 0:
            if value & 0x3 != 0:
                continue
        elif digits[2] != 0:
            if value & 0x1 != 0:
                continue

        # check division by 6
        if digits[6] != 0:
            if value & 0x1 != 0 or mod3 != 0:
                continue

        # check division by 5
        if digits[5] != 0:
            if mod5 != 0:
                continue

        # check division by 9, 3
        if digits[9] != 0:
            if mod9 != 0:
                continue
        elif digits[3] != 0:
            if mod3 != 0:
                continue

        # check division by 7
        if digits[7] > 0:
            if mod7 != 0:
                continue

        count += 1
        yield value

# testing
if __name__ == '__main__':

    def testSolution(N):
        """This function is not a solution of the task.
    It is used for testing only."""
        value = 0
        count = 0
        while count < N:
            value += 1
            goodValue = True
            for digit in [int(c) for c in str(value)]:
                if digit == 0 or value % digit != 0:
                    goodValue = False
                    break
            if goodValue:
                count += 1
                yield value

    class TestFailed(Exception):
        pass
    
    try:
        for (x, y) in zip(solution(1000), testSolution(1000)):
            if x != y:
                raise TestFailed(x, y)
    except TestFailed as ex:
        print("Test is failed: %d != %d" % ex.args)
    else:
        print("Test passed")
