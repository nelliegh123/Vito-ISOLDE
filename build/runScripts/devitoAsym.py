import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import interp1d
import ROOT

ROOT.gROOT.SetBatch(True)


A = -0.67 #Asymmetry parameter for 2599 transition in K47
# A = 0.33 #Asymmetry parameter for 2578 transition in K47
P = 0.15    #Polarization factor
 

# f = ROOT.TFile("/home/ngustafs/ISOLDE/build/Results/solid_KCl_0.5mm_1000p_devitoCircle2023_20260804_172034/output.root")
f = ROOT.TFile("/home/ngustafs/ISOLDE/build/Results/solid_KCl_2mm_1000p_devitoCircle2024_20260805_154928/output.root")

# E_data, dNdE_data, _ = np.loadtxt("/home/ngustafs/ISOLDE/build/EnergyDistributions/K47_2578.txt", unpack=True)
E_data, dNdE_data, _ = np.loadtxt("/home/ngustafs/ISOLDE/build/EnergyDistributions/K47_2599.txt", unpack=True)



df = ROOT.RDataFrame("hits", f)
data = df.AsNumpy(columns=["energy", "angle", "detector"])
E, theta, det = data["energy"], data["angle"], data["detector"]


def W(E, theta, A, P):
    vc = np.sqrt(1-((0.511)/(E+0.511))**2)
    return 1 + vc*A*P*np.cos(np.radians(theta))

def S(E):
    func = interp1d(E_data, dNdE_data, kind="cubic", bounds_error=False, fill_value=0.0)
    return(func(E*1000))      #I take E*1000 since data is in keV

# x = np.linspace(0, 4, 1000)
# plt.plot(x, S(x))
# plt.xlabel("Energy [MeV]")
# plt.show() 

def asymmetry_np(A):
    theta_rad = np.radians(theta)
    w = W(E, theta, A, P)
    s = S(E)

    weight = w * s * np.sin(theta_rad)

    NF = weight[det == 0].sum()
    NR = weight[det == 1].sum()
    return (NF - NR) / (NF + NR), NF, NR


print(f)
print()
print("The counts are ", asymmetry_np(A)[1], "in the front detector and", asymmetry_np(A)[2], "in the back detector")
print()
print("The experimental asymmetry parameter is", asymmetry_np(A)[0])
