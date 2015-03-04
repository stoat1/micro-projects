# people defeat zones

import util
from util import interpolate

import rgr_types
from rgr_types import BldType, RGRError

import tables

def getDefeatProbs():
    """Return pople defeat probabilities"""
    return tables.defeatProbs


def getDefeatZones(mode, mass):
    """Return people defeat zones radiuses"""
    try:
        table = tables.tab15_20[mode]
    except KeyError:
        raise RGRError("no data for mode " + str(mode))

    if len(table) == 0 :
        raise RGRError("empty data for mode " + str(mode))

    masses = sorted(table.keys())
    lower = masses[0]
    upper = masses[-1]

    if lower > mass or upper < mass :
        raise RGRError("no data for mass " + str(mass))

    for m in masses:
        if m <= mass :
            lower = max(m, lower)
        if m >= mass :
            upper = min(m, upper)

    zones = []
    for i in range(len(tables.defeatProbs)):
        prob = tables.defeatProbs[i]
        lowerPoint = (lower, table[lower][i])
        upperPoint = (upper, table[upper][i])
        radius = interpolate(lowerPoint, upperPoint, mass)
        zones.append(radius)

    return zones

def getIndoorSurvivalProb(destruct_degree, bld_type):
    """Return probability of surviving people indoor"""
    try:
        survivalProb = tables.tab26[bld_type]
    except KeyError:
        raise RGRError("unknown survival probability for building type "
                            + str(bld_type))

    try:
        return survivalProb[destruct_degree]
    except KeyError:
        raise RGRError("unknown survival probability for building type "
                            + str(bld_type) + ", zone " + str(destruct_degree))
