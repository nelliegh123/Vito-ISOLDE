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


#=============Testing=============
x_plus, y_plus, z_plus, theta_plus, E_plus = load_hits(f"{DATA_DIR}/Results/devitoTest/solid_KCl_0.0mm_100p_devito2024_plus_20260812_141913/output.root")
x_minus, y_minus, z_minus, theta_minus, E_minus = load_hits(f"{DATA_DIR}/Results/devitoTest/solid_KCl_0.0mm_10p_devito2024_minus_20260812_095433/output.root")

# x_plus, y_plus, z_plus, theta_plus, E_plus = load_hits(f"{DATA_DIR}/Results/devitoTest/solid_KCl_0.5mm_100p_devito2024_plus_20260812_102804/output.root")
# x_minus, y_minus, z_minus, theta_minus, E_minus = load_hits(f"{DATA_DIR}/Results/devitoTest/solid_KCl_0.5mm_100p_devito2024_minus_20260812_101928/output.root")

# x_plus, y_plus, z_plus, theta_plus, E_plus = load_hits(f"{DATA_DIR}/Results/devitoTest/solid_KCl_1.0mm_100p_devito2024_plus_20260812_102942/output.root")
# x_minus, y_minus, z_minus, theta_minus, E_minus = load_hits(f"{DATA_DIR}/Results/devitoTest/solid_KCl_1.0mm_100p_devito2024_minus_20260812_102106/output.root")

# x_plus, y_plus, z_plus, theta_plus, E_plus = load_hits(f"{DATA_DIR}/Results/devitoTest/solid_KCl_2.0mm_100p_devito2024_plus_20260812_103120/output.root")
# x_minus, y_minus, z_minus, theta_minus, E_minus = load_hits(f"{DATA_DIR}/Results/devitoTest/solid_KCl_2.0mm_100p_devito2024_minus_20260812_102244/output.root")
#=============Testing=============




# print(len(E_plus))
# x = np.linspace(0, 4, len(E_plus))
# plt.hist(x, bins=97, weights=E_plus)
# plt.show()


# x = np.linspace(0, 4, len(theta_plus))
# plt.hist(x, bins=97, weights=theta_plus)
# plt.show()


# plt.hist(E_plus, weigh)
# plt.show()