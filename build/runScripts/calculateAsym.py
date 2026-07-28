import ROOT
import numpy as np 
import matplotlib.pyplot as plt

A = -1/3 #Experimental asymmetry parameter
P = 1    #Polarization factor

ROOT.gROOT.SetBatch(True) 
f = ROOT.TFile("../Results/test/output.root")
df = ROOT.RDataFrame("hits", f)

data = df.AsNumpy(columns=["energy", "angle", "detector"])
E, theta, det = data["energy"], data["angle"], data["detector"]


def W(E, theta, A, P):
    vc = np.sqrt(1-((0.511)/(E+0.511))**2)
    return 1 + vc*A*P*np.cos(np.radians(theta))

def S(E):
    alpha = 1/137
    Ezero = 16 #Q-E_recoil 
    Z = -1
    m_e=0.511
    
    p = np.sqrt(E**2 - m_e**2)
    eta = Z*alpha*E/(p)
    F = (2*np.pi*eta)/(1-np.exp(-2*np.pi*eta))
    return p*E*(Ezero - E)**2 * F

def asymmetry_np(A):
    w = W(E, theta, A, P)
    s = S(E)
    s = s/s.max()

    weight = w * s

    NF = weight[det == 0].sum()
    NR = weight[det == 1].sum()
    return (NF - NR) / (NF + NR)

# for A in np.linspace(-1, 1, 21):
print(A, asymmetry_np(A))
