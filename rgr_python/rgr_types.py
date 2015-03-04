# rgr types

class RGRError(Exception):
    pass

class DestructDegree:
    FULL = 1
    STRONG = 2
    MIDDLE = 3
    WEAK = 4
    LIGHT = 5
    iterable = (FULL, STRONG, MIDDLE, WEAK, LIGHT)

class BldType:
    IND = "I"
    ADM = "A"
    iterable = (IND, ADM)

class Building:
    def __init__(self, bld_type, dist, area, density):
        if bld_type not in BldType.iterable :
            raise TypeError("building type must be in " + str(BuildingType.iterable))
        self.bld_type = bld_type
        self.dist = dist
        self.area = area
        self.density = density
