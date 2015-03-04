# utilities


def print_list(xs):
    """Display list element per line"""
    for x in xs:
        print(x)

def interpolate(point_1, point_2, x):
    """Return y(x) of line which contain point_1 and point_2"""
    (x1, y1) = point_1
    (x2, y2) = point_2
    if x1 == x2 :
        if y1 != y2 :
            raise ArithmeticError("conflicting points")
        else :
            if x == x1 :
                return y1
            else :
                raise ArithmeticError("cannot interpolate by equals points")
    else :
        return y1 * (x - x2) / (x1 - x2) + y2 * (x - x1) / (x2 - x1)

def inverse_dict(src):
    dst = {}
    for (k, v) in src.items():
        if v in dst.keys():
            raise KeyError("dict is not inversable")
        else:
            dst[v] = k
    return dst
