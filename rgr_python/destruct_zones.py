# calculating destriction zones

import util
from util import interpolate

import rgr_types
from rgr_types import DestructDegree, BldType

import tables

def getDestructionZones(mode, building_type, mass):
    """Return destruction zone radius"""

    # check type of building_type
    if building_type not in BldType.iterable:
        raise TypeError("building_type must be in " + str(BldType.iterable))

    # search mass in the table

    # select concrete table
    try:
        table = tables.tab9_14[mode]
    except KeyError:
        raise RGRError("no data for mode " + str(mode))

    masses = sorted(table.keys())
    if len(masses) == 0 :
        raise RGRError("table for mode " + str(mode) + " is empty")
    lower = masses[0]
    upper = masses[-1]
    if lower > mass or upper < mass :
        raise RGRError("table does not contain data for mass " + str(mass))
    for m in masses :
        if m <= mass :
            lower = max(m, lower)
        if m >= mass :
            upper = min(m, upper)

    result = {}
    for zone in DestructDegree.iterable:
        column = 2 * (zone - 1)
        if building_type == BldType.ADM :
            column += 1
        result[zone] = interpolate((lower, table[lower][column]), (upper, table[upper][column]), mass)     
    
    return result
