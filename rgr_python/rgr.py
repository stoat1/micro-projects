# BGD RGR

# imports

import util
from util import interpolate, print_list

import destruct_zones
from destruct_zones import getDestructionZones

import rgr_types
from rgr_types import DestructDegree, BldType, Building

import explosive_transformation_mode
from explosive_transformation_mode import getExplosiveClass, getExplosiveTransformationMode, getHeatFlow

import defeat_zones
from defeat_zones import getDefeatProbs, getDefeatZones, getIndoorSurvivalProb

import termal_action
from termal_action import dose2Prob

import math

import table2strings
from table2strings import print_table

def getDoseIndex(t, Q, R, X):
    return t * (Q * R**2 / X**2) ** (4/3)

def getDistance(t, Q, R, J):
    return R * math.sqrt(Q) * (t / J)**(3/8)
    

# source data
class MyProblem:
    mass = 105000
    hydrocarbon = "propane"
    space = 2
    buildings = {
        "I1": Building(BldType.IND, dist=800,    area=50,    density=0.2),
        "I2": Building(BldType.IND, dist=850,    area=50,    density=0.2),
        "I3": Building(BldType.IND, dist=1000,   area=100,   density=0.1),
        "A1": Building(BldType.ADM, dist=300,    area=50,    density=0.24),
        "A2": Building(BldType.ADM, dist=1500,   area=50,    density=0.3)
    }
    outdoorDensity = 1.5e-4

class ExampleProblem:
    mass = 50000
    hydrocarbon = "propane"
    space = 2
    buildings = {
        "A": Building(BldType.ADM, dist=700, area=100, density=0.2),          
        "I": Building(BldType.IND, dist=600, area=100, density=0.1)
    }
    outdoorDensity = 1.5e-4
    
Problem = MyProblem


# calculations

# (2) determine explosive transformation mode
explosiveClass = getExplosiveClass(Problem.hydrocarbon)
mode = getExplosiveTransformationMode(space=Problem.space,
                                      explosiveClass=explosiveClass)

# (3) estimate air shock wave influence
# (3.1) determine destruction zones
destructionZones = {
    BldType.IND: getDestructionZones(mode, BldType.IND, Problem.mass),
    BldType.ADM: getDestructionZones(mode, BldType.ADM, Problem.mass)
}
for b in Problem.buildings.values():
    b.zone = min([z for z in DestructDegree.iterable if
        b.dist < destructionZones[b.bld_type][z]])
del b

# (3.2) determine wave victims number
# (3.2.1) out of doors
defeatBoundaryProbs = getDefeatProbs()
defeatZones = getDefeatZones(mode=mode, mass=Problem.mass)
defeatAreas = []
defeatAverageProbs = []
outdoorVictims = []
lastProb = 1
lastRadius = 0
it = iter(defeatBoundaryProbs)
for radius in defeatZones :
    prob = next(it)
    averageProb = (lastProb + prob) / 2
    area = math.pi * (radius ** 2 - lastRadius ** 2)
    n = area * Problem.outdoorDensity * averageProb

    defeatAreas.append(area)
    defeatAverageProbs.append(averageProb)
    outdoorVictims.append(n)
    
    lastProb = prob
    lastRadius = radius
    
outdoorVictimNumber = sum(outdoorVictims)

# (3.2.2) indoors
indoorVictims = {BldType.ADM: 0, BldType.IND: 0}                  
for b in Problem.buildings.values():
    p = getIndoorSurvivalProb(destruct_degree=b.zone, bld_type=b.bld_type)
    b.p_surv = p
    b.people = b.area * b.density
    b.victims = ( 1 - p ) * b.people
    indoorVictims[b.bld_type] += b.victims
indoorVictimNumber = sum(indoorVictims.values())

# total
waveVictimNumber = outdoorVictimNumber + indoorVictimNumber

# (4) estimate termal action

# (4.1) fire ball parameters
burnedMass = 0.6 * Problem.mass
fireBallRadius = 3.2 * burnedMass ** 0.325
fireBallTime = 0.85 * burnedMass ** 0.26
heatFlow = getHeatFlow(Problem.hydrocarbon)

# (4.2) heat flow victims

# (4.2.1) heat flow victims inside the ball
fireBallArea = math.pi * fireBallRadius ** 2
ballVictimNumber = fireBallArea * Problem.outdoorDensity * 1

# (4.2.2) heat flow victims outside the ball
# in wave zone
X2J = lambda X: getDoseIndex(t=fireBallTime, Q=heatFlow, R=fireBallRadius, X=X)
doseIndeces = list(map(X2J, defeatZones))
termalActionBoundaryProb = map(dose2Prob, doseIndeces)
termalAcitionAverageProb = []
if fireBallRadius >= defeatZones[0]:
    raise RGRError("fire ball entirely covers 6th defeat zone")
#lastRadius = fireBallRadius
lastProb = 0
for curentProb in termalActionBoundaryProb:
    p = (lastProb + curentProb) / 2
    termalAcitionAverageProb.append(p)

    lastProb = curentProb

# out of wave zone


# the act

# source data
wear = [0.05, 0.18, 0.13, 0.06, 0.15, 0.25, 0.2, 0.2, 0.0, 0.0, 0.3]
destr = [0.1, 0.35, 0.3, 0.4, 0.3, 0.25, 0.5, 0.85, 1.0, 0.0, 0.5]
role = [12, 22, 6, 12, 8, 10, 12, 5, 4.2, 2.2, 6.6]
coef = [1.7, 1.4, 2.3, 1.0, 1.6, 0.6, 1.0, 2.8, 1.4, 1.8, 0.9]

assert(len(wear) == len(destr) == len(role) == len(coef))
N = len(wear)

# calculations
damage = [wear[i] + destr[i] - wear[i] * destr[i] for i in range(N)]
total_damage = sum([damage[i] * role[i] for i in range(N)]) / sum(role)

k = [role[i] * coef[i] for i in range(N)]
conv_role = [k_i / sum(k) for k_i in k]
del k
damage_mul_conv_role = [damage[i] * conv_role[i] for i in range(N)]
conv_total_damage = \
    sum([damage[i] * conv_role[i] for i in range(N)]) / sum(conv_role)

def print_rgr():
    print("RGR #3")
    print("hydrocarbon: " + Problem.hydrocarbon)
    print("mass: " + str(Problem.mass) + " kg")
    print("space: " + str(Problem.space))
    print("")
    print("Buildings")
    table = [
        ["", "distance", "area", "density"]
    ] + [
        [name, b.dist, b.area, b.density] for (name, b) in sorted(Problem.buildings.items())
    ]
    print_table(table)
    del table
    print("outdoor personal density: " + str(Problem.outdoorDensity) + " 1/m^2")
    print("")
    print("2. Determine mode")
    print("Hydrocarbon class is " + str(explosiveClass))
    print("The mode is " + str(mode))

    print("3. Wave")
    print("3.1. Destruction zones")
    table = [
        ["", "Full", "Strong", "Middle", "Weak", "Light"],
        ["admin"] + list(destructionZones[BldType.ADM].values()),
        ["indust"] + list(destructionZones[BldType.IND].values())
    ]
    print_table(table)
    del table
    for (name, b) in sorted(Problem.buildings.items()):
        print(name + " - zone " + str(b.zone))
    print("3.2. People defeat")
    print("3.2.1 Outdoor")
    table = [
        ["", 6, 5, 4, 3, 2, 1],
        ["P_border"] + list(defeatBoundaryProbs),
        ["P_average"] + defeatAverageProbs,
        ["R"] + defeatZones,
        ["S"] + defeatAreas,
        ["N_i"] + outdoorVictims
    ]
    print_table(table)
    del table
    print("outdoor victims: " + str(outdoorVictimNumber))
    print("3.2.2. Indoor")
    table = [
        ["", "area", "density", "people", "P_surv", "victims"]
    ] + [
        [name, b.area, b.density, b.people, b.p_surv, b.victims] \
        for (name, b) in sorted(Problem.buildings.items())
    ]
    print_table(table)
    del table
    print("admin. buildings victims is " + str(indoorVictims["A"]))
    print("admin. buildings victims is " + str(indoorVictims["A"]))
    print("tital indoor victims is " + str(indoorVictimNumber))
    print("4. Heat flow")
    print("4.1. Fair ball parameters")
    print("burned mass is " + str(burnedMass) + " kg")
    print("radius is " + str(fireBallRadius) + " m")
    print("time is " + str(fireBallTime) + " s")
    print("heat flow is " + str(heatFlow) + " kW/m^2")
    print("4.2. Fair ball victims")
    print("4.2.1. Inside the ball")
    print("fire ball covering area is " + str(fireBallArea) + " m^2")
    print("victims inside the ball is " + str(ballVictimNumber))
    print("4.2.2 Outside of ball victims")                              
                                           

print_rgr()



