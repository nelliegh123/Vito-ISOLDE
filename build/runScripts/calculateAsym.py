import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import interp1d
import ROOT

A = -1/3 #Asymmetry parameter for Li8
# A = -0.67 #Asymmetry parameter for 2599 transition in K47
# A = 0.33 #Asymmetry parameter for 2578 transition in K47

P = -1    #Polarization factor

ROOT.gROOT.SetBatch(True) 



#----Solid right place for emission??-----
# f = ROOT.TFile("/output.root")
f = ROOT.TFile("/home/ngustafs/ISOLDE/build/Results/solid_MgO_0.5mm_100p_default_20260805_162700/output.root")
# f = ROOT.TFile("/home/ngustafs/ISOLDE/build/Results/solid_MgO_1.0mm_100p_default_20260805_162200/output.root")
# f = ROOT.TFile("/home/ngustafs/ISOLDE/build/Results/solid_MgO_2.0mm_100p_default_20260805_162321/output.root")

#----DeVITO-----
# f = ROOT.TFile("/home/ngustafs/ISOLDE/build/Results/solid_KCl_0.5mm_1000p_devitoCircle2023_20260804_172034/output.root")
# f = ROOT.TFile("/home/ngustafs/ISOLDE/build/Results/solid_KCl_0.5mm_1000p_devito2023_20260804_172239/output.root")


df = ROOT.RDataFrame("hits", f)
data = df.AsNumpy(columns=["energy", "angle", "detector"])
E, theta, det = data["energy"], data["angle"], data["detector"]


def W(E, theta, A, P):
    vc = np.sqrt(1-((0.511)/(E+0.511))**2)
    return 1 + vc*A*P*np.cos(np.radians(theta))

E_data, dNdE_data, _ = np.loadtxt("/home/ngustafs/ISOLDE/build/EnergyDistributions/Li8.txt", unpack=True)
def S(E):
    func = interp1d(E_data, dNdE_data, kind="cubic", bounds_error=False, fill_value=0.0)
    return(func(E*1000))      #I take E*1000 since data is in keV


def asymmetry_np(A):
    w = W(E, theta, A, P)
    s = S(E)

    weight = w * s

    NF = weight[det == 0].sum()
    NR = weight[det == 1].sum()
    return (NF - NR) / (NF + NR), NF, NR

# for A in np.linspace(-1, 1, 21):
print(f)
print()
print("The counts are ", asymmetry_np(A)[1], "in the front detector and", asymmetry_np(A)[2], "in the back detector")
print()
print("The experimental asymmetry parameter is", asymmetry_np(A)[0])
