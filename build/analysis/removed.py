import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import interp1d
import ROOT
ROOT.gROOT.SetBatch(True)

DATA_DIR = "/home/ngustafs/ISOLDE/build"
 
def load_hits(path, columns=("rmPosX", "rmPosY", "rmPosZ", "rmAngle", "rmEnergy")):
    f = ROOT.TFile(path)
    df = ROOT.RDataFrame("removed", f)
    data = df.AsNumpy(columns=list(columns))
    return data["rmPosX"], data["rmPosY"], data["rmPosZ"], data["rmAngle"], data["rmEnergy"]

# # #===============2024 Setup===============
# x_plus, y_plus, z_plus, theta_plus, E_plus = load_hits(f"{DATA_DIR}/Results/devito/solid_KCl_2.0mm_1000p_devito2024_plus_20260813_152306/output.root")
# x_minus, y_minus, z_minus, theta_minus, E_minus = load_hits(f"{DATA_DIR}/Results/devito/solid_KCl_2.0mm_1000p_devito2024_minus_20260813_153845/output.root")
# # #===============2024 Setup===============

# # #===============2023 Setup===============
# x_plus, y_plus, z_plus, theta_plus, E_plus = load_hits(f"{DATA_DIR}/Results/devito/solid_KCl_0.5mm_1000p_devito2023_plus_20260817_133550/output.root")
# x_minus, y_minus, z_minus, theta_minus, E_minus = load_hits(f"{DATA_DIR}/Results/devito/solid_KCl_0.5mm_1000p_devito2023_minus_20260817_135602/output.root")
# # #===============2023 Setup===============


x_plus, y_plus, z_plus, theta_plus, E_plus = load_hits(f"{DATA_DIR}/Results/default/solid_MgO_0.0mm_100p_default_minus_20260821_111741/output.root")


print(len(E_plus))
# print(len(E_minus))
# x = np.linspace(0, 4, len(E_plus))
# plt.hist(x, bins=97, weights=E_plus)
# plt.show()


# x = np.linspace(0, 4, len(theta_plus))
# plt.hist(x, bins=97, weights=theta_plus)
# plt.show()


# plt.hist(E_plus, weigh)
# plt.show()