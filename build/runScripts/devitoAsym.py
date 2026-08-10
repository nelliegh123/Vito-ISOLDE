import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import interp1d
import ROOT

ROOT.gROOT.SetBatch(True)

# # # #------------------------Magnetic field pointing +z----------------------------------
P_plus = 0.15    #Polarization factor
f_plus = ROOT.TFile("/home/ngustafs/ISOLDE/build/Results/devito/solid_KCl_2.0mm_1000p_devitoCircle2024_plus_20260807_111307/output.root")
# f_plus = ROOT.TFile("/home/ngustafs/ISOLDE/build/Results/devito/solid_KCl_0.5mm_1000p_devitoCircle2023_plus_20260807_115654/output.root")

df_plus = ROOT.RDataFrame("hits", f_plus)
data_plus = df_plus.AsNumpy(columns=["energy", "angle", "detector"])
E_plus, theta_plus, det_plus = data_plus["energy"], data_plus["angle"], data_plus["detector"]



# ------------------------Magnetic field pointing -z----------------------------------
P_minus = -0.15    #Polarization factor
# f_minus = ROOT.TFile("/home/ngustafs/ISOLDE/build/Results/devito/solid_KCl_2.0mm_1000p_devitoCircle2024_minus_20260807_105611/output.root")
# f_minus = ROOT.TFile("/home/ngustafs/ISOLDE/build/Results/devito/solid_KCl_0.5mm_1000p_devitoCircle2023_minus_20260807_130020/output.root")






#----------------------------------------------------------------
#            THESE ARE JUST FOR TESTING PURPOSES
#----------------------------------------------------------------
# #100 particles, normal
# f_minus = ROOT.TFile("/home/ngustafs/ISOLDE/build/Results/test/solid_KCl_2.0mm_100p_devitoCircle2024_minus_20260807_152720/output.root")
## A_minus = 0.39537672566722004 0.4561144899028587

# #100 particles, front detector 5 mm larger radius
# f_minus = ROOT.TFile("/home/ngustafs/ISOLDE/build/Results/test/solid_KCl_2.0mm_100p_devitoCircle2024_minus_20260807_154316/output.root")
## A_minus = 0.45526901780722495 and 0.5113529576866451

# #100 particles, both detectors 5 mm larger radius
# f_minus = ROOT.TFile("/home/ngustafs/ISOLDE/build/Results/test/solid_KCl_2.0mm_100p_devitoCircle2024_minus_20260807_154956/output.root")
## A_minus = 0.4136526498011403 and 0.4723104671483723


# #100 particles, using devito2024 (cut corners) with no rotation 
# f_minus = ROOT.TFile("/home/ngustafs/ISOLDE/build/Results/test/solid_KCl_2.0mm_100p_devito2024_minus_20260807_155556/output.root")
# ## A_minus = 0.3731488729036506 and 0.4352334138162951

# #100 particles, both detectors 26 mm larger radius (i.e "flat sides" of devito detector)
# f_minus = ROOT.TFile("/home/ngustafs/ISOLDE/build/Results/test/solid_KCl_2.0mm_100p_devitoCircle2024_minus_20260807_155951/output.root")
# ## A_minus = 0.3488552301245087 and 0.41456153883414504
#----------------------------------------------------------------
#                         END OF TEST
#----------------------------------------------------------------





df_minus = ROOT.RDataFrame("hits", f_minus)
data_minus = df_minus.AsNumpy(columns=["energy", "angle", "detector"])
E_minus, theta_minus, det_minus = data_minus["energy"], data_minus["angle"], data_minus["detector"]




#-------------------------Transitions----------------------------------
A_2599 = -0.67     #Asymmetry parameter for 2599 transition in K47
E_data2599, dNdE_data2599, _ = np.loadtxt("/home/ngustafs/ISOLDE/build/EnergyDistributions/K47_2599.txt", unpack=True)

A_2578 = 0.33    #Asymmetry parameter for 2578 transition in K47
E_data2578, dNdE_data2578, _ = np.loadtxt("/home/ngustafs/ISOLDE/build/EnergyDistributions/K47_2578.txt", unpack=True)




def W(E, theta, A, P):
    vc = np.sqrt(1-((0.511)/(E+0.511))**2)
    return 1 + vc*A*P*np.cos(np.radians(theta))


def S(E, transition):
    if transition==2599:
        func = interp1d(E_data2599, dNdE_data2599, kind="cubic", bounds_error=False, fill_value=0.0)
    else:
        func = interp1d(E_data2578, dNdE_data2578, kind="cubic", bounds_error=False, fill_value=0.0)
    return(func(E*1000))      #I take E*1000 since data is in keV


def asymmetry_np(E, theta, A, P, det, transition):
    theta_rad = np.radians(theta)
    w = W(E, theta, A, P)
    s = S(E, transition)

    weight = w * s * np.sin(theta_rad)

    NF = weight[det == 0].sum()
    NR = weight[det == 1].sum()
    return (NF - NR) / (NF + NR), NF, NR





A_plus = asymmetry_np(E_plus, theta_plus, A_2599, P_plus, det_plus, 2599)
A_minus = asymmetry_np(E_minus, theta_minus, A_2599, P_minus, det_minus, 2599)
print()
print("-----------------------------2024--------------------------")
print("Transition = 2599, magnetic field = +z")
print("The ratio of counts (front/back) are ", A_plus[1]/A_plus[2])
print("The experimental asymmetry parameter is", A_plus[0])
print()
print("Transition = 2599, magnetic field = -z")
print("The ratio of counts (front/back) are ", A_minus[1]/A_minus[2])
print("The experimental asymmetry parameter is", A_minus[0])
print()


A_plus = asymmetry_np(E_plus, theta_plus, A_2578, P_plus, det_plus, 2578)
A_minus = asymmetry_np(E_minus, theta_minus, A_2578, P_minus, det_minus, 2578)
print()
print("Transition = 2578, magnetic field = +z")
print("The ratio of counts (front/back) are ", A_plus[1]/A_plus[2])
print("The experimental asymmetry parameter is", A_plus[0])
print()
print("Transition = 2578, magnetic field = -z")
print("The ratio of counts (front/back) are ", A_minus[1]/A_minus[2])
print("The experimental asymmetry parameter is", A_minus[0])
print()