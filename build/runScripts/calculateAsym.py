import ROOT
import numpy as np 
import matplotlib.pyplot as plt

A = -1/3 #Experimental asymmetry parameter
P = 1    #Polarization factor

ROOT.gROOT.SetBatch(True) 
f = ROOT.TFile("../TotalAngleScan.root")
df = ROOT.RDataFrame("hits", f)

data = df.AsNumpy(columns=["energy", "angle", "detector"])
E, theta, det = data["energy"], data["angle"], data["detector"]


def W(E, theta, A, P):
    vc = np.sqrt(1-((0.511)/(E+0.511))**2)
    return 1 + vc*A*P*np.cos(np.radians(theta))


def asymmetry_np(A, Ecut=0.0):
    mask = E > Ecut
    w = W(E, theta, A, P)
    NF = w[det == 0].sum()
    NR = w[det == 1].sum()
    return (NF - NR) / (NF + NR)

# for A in np.linspace(-1, 1, 21):
print(A, asymmetry_np(A))
