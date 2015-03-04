# termal action

import rgr_types
from rgr_types import RGRError

import tables

import util
from util import inverse_dict, interpolate

def dose2Prob(dose):
    try:
        dose2ProbDict = inverse_dict(tables.tab8)
    except KeyError:
        raise RGRError("table 8 sets uninvertable mapping")
    
    lowerKeys = [j for j in dose2ProbDict.keys() if j <= dose]
    upperKeys = [j for j in dose2ProbDict.keys() if j >= dose]
    if len(lowerKeys) == 0:
        return 0.00
    if len(upperKeys) == 0:
        return 1.00
    
    lowerBound = max(lowerKeys)
    upperBound = min(upperKeys)
    
    return interpolate((lowerBound, dose2ProbDict[lowerBound]),
                       (upperBound, dose2ProbDict[upperBound]),
                       dose)

    
    
