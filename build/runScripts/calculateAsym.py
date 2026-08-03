import ROOT
import numpy as np 
import matplotlib.pyplot as plt

A = -1/3 #Experimental asymmetry parameter
P = -1    #Polarization factor

ROOT.gROOT.SetBatch(True) 
# #---No Sample---
# # f = ROOT.TFile("/home/ngustafs/ISOLDE/build/Results/solid_0mm_10000p_default_20260731_162120/output.root")

# #---Solid---
# # f = ROOT.TFile("/home/ngustafs/ISOLDE/build/Results/solid_0.5mm_10000p_default_20260731_144917/output.root")
# # f = ROOT.TFile("../Results/solid_1mm_10000p_default_20260731_142706/output.root")
# # f = ROOT.TFile("/home/ngustafs/ISOLDE/build/Results/solid_1.5mm_10000p_default_20260731_183018/output.root")
# # f = ROOT.TFile("/home/ngustafs/ISOLDE/build/Results/solid_2mm_10000p_default_20260731_151314/output.root")
# # f = ROOT.TFile("/home/ngustafs/ISOLDE/build/Results/solid_4mm_10000p_default_20260731_153728/output.root")

# # f = ROOT.TFile("../Results/solid_1mm_10000_DeVITOp_20260730_164711/output.root")





#----Solid right place for emission??-----
f = ROOT.TFile("/home/ngustafs/ISOLDE/build/Results/solid_0.001mm_1000p_default_20260803_094509/output.root")
# f = ROOT.TFile("/home/ngustafs/ISOLDE/build/Results/solid_0.5mm_1000p_default_20260803_093312/output.root")
# f = ROOT.TFile("/home/ngustafs/ISOLDE/build/Results/solid_1mm_1000p_default_20260803_093852/output.root")
# f = ROOT.TFile("/home/ngustafs/ISOLDE/build/Results/solid_2mm_1000p_default_20260803_094147/output.root")




df = ROOT.RDataFrame("hits", f)

data = df.AsNumpy(columns=["energy", "angle", "detector"])
E, theta, det = data["energy"], data["angle"], data["detector"]


def W(E, theta, A, P):
    vc = np.sqrt(1-((0.511)/(E+0.511))**2)
    return 1 + vc*A*P*np.cos(np.radians(theta))

def S(E): ##need to double check which energy to use, and also if E0 should be +m_e
    alpha = 1/137
    m_e = 0.511
    Energy = E + m_e
    Ezero = 16 + m_e #Q-E_recoil 
    Z = 1 ##is it?
    
    
    p = np.sqrt(Energy**2 - m_e**2)
    eta = Z*alpha*Energy/(p)
    F = (2*np.pi*eta)/(1-np.exp(-2*np.pi*eta))
    return p*Energy*(Ezero - Energy)**2 * F

def asymmetry_np(A):
    w = W(E, theta, A, P)
    s = S(E)

    weight = w * s

    NF = weight[det == 0].sum()
    NR = weight[det == 1].sum()
    return (NF - NR) / (NF + NR)

# for A in np.linspace(-1, 1, 21):
print(f)
print(asymmetry_np(A))
