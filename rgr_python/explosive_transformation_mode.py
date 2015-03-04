# calculagin explosive transformation mode

import rgr_types
from rgr_types import RGRError

import tables

def getExplosiveClass(hydrocarbon):
    """Return explosive class of hydrocarbon"""
    try :
        return tables.tab1[hydrocarbon]
    except KeyError:
        raise RGRError("unknown explosive class for hydrocarbon " + hydrocarbon)

def getHeatFlow(hydrocarbon):
    """Return heat flow of hydrocarbon"""
    try:
        return tables.tab4[hydrocarbon]
    except KeyError:
        raise RGRError("unknown heat flow for hydrocarbon " + hydrocarbon)

def getExplosiveTransformationMode(space, explosiveClass):
    """Return explosive transformation mode"""
    try :
        row = tables.tab3[space]
    except KeyError:
        raise RGRError("unknown mode for space charasteristics " + space)

    try :
        return row[explosiveClass]
    except KeyError:
        raise RGRError("unknown mode space " + str(space) + \
                " and explosive class " + explosiveClass)
