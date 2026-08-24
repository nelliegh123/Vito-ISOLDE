import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import interp1d
import ROOT

A = -1/3 #Asymmetry parameter for Li8
# A = -0.67 #Asymmetry parameter for 2599 transition in K47
# A = 0.33 #Asymmetry parameter for 2578 transition in K47
P = 1    #Polarization factor

ROOT.gROOT.SetBatch(True) 


f = [ROOT.TFile("/home/ngustafs/ISOLDE/build/Results/default/solid_MgO_0.0mm_100p_default_minus_20260821_111741/output.root"),
    ROOT.TFile("/home/ngustafs/ISOLDE/build/Results/default/solid_MgO_0.01mm_1000p_default_minus_20260810_215051/output.root"),
    ROOT.TFile("/home/ngustafs/ISOLDE/build/Results/default/solid_MgO_0.5mm_1000p_default_minus_20260810_234341/output.root"),
    ROOT.TFile("/home/ngustafs/ISOLDE/build/Results/default/solid_MgO_1.0mm_1000p_default_minus_20260811_013336/output.root"),
    ROOT.TFile("/home/ngustafs/ISOLDE/build/Results/default/solid_MgO_1.5mm_1000p_default_minus_20260811_032456/output.root"),
    ROOT.TFile("/home/ngustafs/ISOLDE/build/Results/default/solid_MgO_2.0mm_1000p_default_minus_20260811_051508/output.root"),
    ROOT.TFile("/home/ngustafs/ISOLDE/build/Results/default/solid_MgO_2.5mm_1000p_default_minus_20260811_070433/output.root")]


def W(E, theta, A, P):
    vc = np.sqrt(1-((0.511)/(E+0.511))**2)
    return 1 + vc*A*P*np.cos(np.radians(180-theta))

E_data, dNdE_data, _ = np.loadtxt("/home/ngustafs/ISOLDE/build/EnergyDistributions/Li8.txt", unpack=True)
def S(E):
    func = interp1d(E_data, dNdE_data, kind="cubic", bounds_error=False, fill_value=0.0)
    return(func(E*1000))      #I take E*1000 since data is in keV



def asymmetry_np(A):
    w = W(E, theta, A, P)
    s = S(E)

    weight = w * s
    front, back = weight[det == 0], weight[det==1]

    NF = weight[det == 0].sum()
    NR = weight[det == 1].sum()
    return (NF - NR) / (NF + NR), NF, NR, front, back

# plt.plot(E_data, asymmetry_np(A)[3])

for i in range(len(f)):
    file = f[i]

    df = ROOT.RDataFrame("hits", file)
    data = df.AsNumpy(columns=["energy", "angle", "detector"])
    E, theta, det = data["energy"], data["angle"], data["detector"]
    print()
    print(file)
    print("The experimental asymmetry parameter is", asymmetry_np(A)[0])



df = ROOT.RDataFrame("hits", f[1])
data = df.AsNumpy(columns=["energy", "angle", "detector"])
E, theta, det = data["energy"], data["angle"], data["detector"]


#========================================
#    Detector Counts vs beta energy
#========================================
# E_front, E_back = E[det == 0], E[det == 1]
# # plt.hist(E, bins=97, weights=S(E))
# plt.hist(E_back, bins=97, weights=asymmetry_np(A)[4], label="Rear Detector")
# plt.hist(E_front, bins=97, weights=asymmetry_np(A)[3], alpha=0.7, label="Front Detector")
# plt.legend()
# plt.show()

# print(len(E))


x = np.linspace(0, 13, 100)
# plt.plot(x, S(x))
# plt.ylabel("dN/dE")
# plt.xlabel("Energy [MeV]")
# plt.yticks([])
# plt.show()

ax = plt.gca()
ax.plot(x, S(x), label=r"$\beta^-$", color="black")
ax.spines['top'].set_visible(False)
ax.spines['right'].set_visible(False)
ax.tick_params(axis='y', which='both',left=False,right=False, labelleft=False)
ax.spines['left'].set_position('zero')
ax.spines['bottom'].set_position('zero')
ax.spines['left'].set_capstyle('butt')
ax.spines['bottom'].set_capstyle('butt')
ax.set_xlim(left=0)
ax.set_ylim(bottom=0)
ax.set_xlabel("Energy [MeV]")
ax.set_ylabel("N(E)")
ax.legend()
plt.show()