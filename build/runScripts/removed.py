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


x_plus, y_plus, z_plus, theta_plus, E_plus = load_hits(f"{DATA_DIR}/Results/devitoTest/solid_KCl_0.0mm_10p_devito2024_plus_20260812_103517/output.root")
x_minus, y_minus, z_minus, theta_minus, E_minus = load_hits(f"{DATA_DIR}/Results/devitoTest/solid_KCl_0.0mm_10p_devito2024_minus_20260812_095433/output.root")

print(len(E_plus))
x = np.linspace(0, 4, len(E_plus))
plt.hist(x, bins=97, weights=E_plus)
plt.show()


x = np.linspace(0, 4, len(theta_plus))
plt.hist(x, bins=97, weights=theta_plus)
plt.show()



print()
print(x_plus)

# plt.hist(rmE, weights=rmX)
# plt.show()

