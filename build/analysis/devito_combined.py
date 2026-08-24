import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import interp1d
import ROOT
ROOT.gROOT.SetBatch(True)
 
DATA_DIR = "/home/ngustafs/ISOLDE/build"
 
def load_hits(path, columns=("energy", "angle", "detector")):
    f = ROOT.TFile(path)
    df = ROOT.RDataFrame("hits", f)
    data = df.AsNumpy(columns=list(columns))
    return data["energy"], data["angle"], data["detector"]
 
P_plus, P_minus = 0.15, -0.15
# # #===============2024 Setup===============
# E_plus, theta_plus, det_plus = load_hits(f"{DATA_DIR}/Results/devito/solid_KCl_2.0mm_1000p_devito2024_plus_20260813_152306/output.root")
# E_minus, theta_minus, det_minus = load_hits(f"{DATA_DIR}/Results/devito/solid_KCl_2.0mm_1000p_devito2024_minus_20260813_153845/output.root")
# # #===============2024 Setup===============

# #===============2023 Setup===============
# E_plus, theta_plus, det_plus = load_hits(f"{DATA_DIR}/Results/devito/solid_KCl_0.5mm_1000p_devito2023_plus_20260817_133550/output.root")
# E_minus, theta_minus, det_minus = load_hits(f"{DATA_DIR}/Results/devito/solid_KCl_0.5mm_1000p_devito2023_minus_20260817_135602/output.root")
# #===============2023 Setup===============


TRANSITION = {
    "A": -0.47528, "file": f"{DATA_DIR}/EnergyDistributions/47K.txt",
}

E_data, dNdE_data, _ = np.loadtxt(TRANSITION["file"], unpack=True)
E_interp = interp1d(E_data, dNdE_data, kind="cubic", bounds_error=False, fill_value=0.0,)



def W(E, theta, A, P):
    """Angular distribution weight."""
    vc = np.sqrt(1 - (0.511 / (E + 0.511)) ** 2)
    return 1 + vc * A * P * np.cos(np.radians(theta))
 

def S(E):
    """Energy-distribution weight (data is stored in keV, E is in MeV)."""
    return E_interp(E * 1000)


def asymmetry_np(E, theta, P, det):
    """
    Returns:
        asymmetry: (NF - NR) / (NF + NR), NF, NR: summed weights in front/rear detectors
    """
    A = TRANSITION["A"]
    weight = 2*np.pi * W(E, theta, A, P) * S(E) * np.sin(np.radians(theta))
    
    front_mask, back_mask = det == 0, det == 1
    NF, NR = weight[front_mask].sum(), weight[back_mask].sum()
    asymmetry = (NF - NR) / (NF + NR)

    return (asymmetry, NF, NR, weight[front_mask], weight[back_mask], E[front_mask], E[back_mask])

 

mask_plus = E_plus >= 0.5
E_plus = E_plus[mask_plus]
theta_plus = theta_plus[mask_plus]
det_plus = det_plus[mask_plus]

mask_minus = E_minus >= 0.5
E_minus = E_minus[mask_minus]
theta_minus = theta_minus[mask_minus]
det_minus = det_minus[mask_minus]


results = {}
for label, (E, theta, det, P) in {
    "plus":  (E_plus,  theta_plus,  det_plus,  P_plus),
    "minus": (E_minus, theta_minus, det_minus, P_minus), }.items():

    asym, NF, NR, front_w, back_w, front_E, back_E = asymmetry_np(E, theta, P, det)
    results[label] = {
        "asymmetry": asym, "NF": NF, "NR": NR,
        "front_weight": front_w, "back_weight": back_w,
        "front_E": front_E, "back_E": back_E,
    }

POLARITY_SIGN = {"plus": "+z", "minus": "-z"}


print()
print("-----------------------------2024--2.0mm--------------------------")

for label in ("plus", "minus"):
    r = results[label]
    print(f"magnetic field = {POLARITY_SIGN[label]}")
    print("The ratio of counts (front/back) are ", r["NF"] / r["NR"])
    print("The experimental asymmetry parameter is", r["asymmetry"])
    print()