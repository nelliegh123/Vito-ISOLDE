import ROOT
import numpy as np 

ROOT.gROOT.SetBatch(True) 
f = ROOT.TFile("../TotalAngleScan.root")
df = ROOT.RDataFrame("hits", f)

data = df.AsNumpy(columns=["energy", "angle", "detector"])
E, theta, det = data["energy"], data["angle"], data["detector"]


print("Energy", E)
print("Angle", theta)
print("det", det)




A = -1/3 #Experimental asymmetry parameter
P = 1    #Polarization factor

def W(E, theta, A, P):
    vc = np.sqrt(1-((0.511)/(E+0.511))**2)
    return 1 + vc*A*P*np.cos(np.radians(theta))


def asymmetry_np(A, Ecut=0.0):
    mask = E > Ecut
    w = W(E, theta, A, P)
    NF = w[(det == 0) & mask].sum()
    NR = w[(det == 1) & mask].sum()
    return (NF - NR) / (NF + NR)

# for A in np.linspace(-1, 1, 21):
# print(A, asymmetry_np(A))


















# import ROOT
# import numpy as np 

# ROOT.gROOT.SetBatch(True) 
# f = ROOT.TFile("../TotalAngleScan.root")
# df = ROOT.RDataFrame("hits", f)

# data = df.AsNumpy(columns=["energy", "angle", "detector"])

# E = data["energy"]
# theta_deg = data["angle"]
# theta = np.deg2rad(theta_deg)

# det = data["detector"]
# counts = data["counts"]

# # Physics parameters
# P = ...
# A = ...

# c = 299792458.0

# # beta/c as a function of energy
# def beta(E):
#     me = 0.511  # MeV
#     gamma = (E + me) / me
#     return np.sqrt(1.0 - 1.0/gamma**2)

# # S(E)
# def S(E):
#     return ...

# W = S(E) * (1 + beta(E) * P * A * np.cos(theta))

# dE = 0.5                     # MeV
# dtheta = np.deg2rad(4.0)     # radians

# prefactor = 2*np.pi*dE*dtheta

# NF = prefactor * np.sum(counts[det == "F"] * W[det == "F"])
# NR = prefactor * np.sum(counts[det == "R"] * W[det == "R"])

# print(NF)
# print(NR)