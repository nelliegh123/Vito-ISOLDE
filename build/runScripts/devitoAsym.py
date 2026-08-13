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
# E_plus, theta_plus, det_plus = load_hits(f"{DATA_DIR}/Results/devito/solid_KCl_2.0mm_1000p_devito2024_plus_20260810_211315/output.root")
# E_minus, theta_minus, det_minus = load_hits(f"{DATA_DIR}/Results/devito/solid_KCl_2.0mm_1000p_devito2024_minus_20260810_204238/output.root")

#=============Testing=============
# E_plus, theta_plus, det_plus = load_hits(f"{DATA_DIR}/Results/devitoTest/solid_KCl_0.0mm_10p_devito2024_plus_20260812_103517/output.root")
# E_minus, theta_minus, det_minus = load_hits(f"{DATA_DIR}/Results/devitoTest/solid_KCl_0.0mm_10p_devito2024_minus_20260812_095433/output.root")

# E_plus, theta_plus, det_plus = load_hits(f"{DATA_DIR}/Results/devitoTest/solid_KCl_0.5mm_100p_devito2024_plus_20260812_102804/output.root")
# E_minus, theta_minus, det_minus = load_hits(f"{DATA_DIR}/Results/devitoTest/solid_KCl_0.5mm_100p_devito2024_minus_20260812_101928/output.root")

# E_plus, theta_plus, det_plus = load_hits(f"{DATA_DIR}/Results/devitoTest/solid_KCl_1.0mm_100p_devito2024_plus_20260812_102942/output.root")
# E_minus, theta_minus, det_minus = load_hits(f"{DATA_DIR}/Results/devitoTest/solid_KCl_1.0mm_100p_devito2024_minus_20260812_102106/output.root")

# E_plus, theta_plus, det_plus = load_hits(f"{DATA_DIR}/Results/devitoTest/solid_KCl_2.0mm_100p_devito2024_plus_20260812_103120/output.root")
# E_minus, theta_minus, det_minus = load_hits(f"{DATA_DIR}/Results/devitoTest/solid_KCl_2.0mm_100p_devito2024_minus_20260812_102244/output.root")
#=============Testing=============

# E_plus, theta_plus, det_plus = load_hits(f"{DATA_DIR}/Results/solid_KCl_2.0mm_100p_devito2024_plus_20260813_120424/output.root")
# E_minus, theta_minus, det_minus = load_hits(f"{DATA_DIR}/Results/solid_KCl_2.0mm_100p_devito2024_minus_20260813_120956/output.root")

E_plus, theta_plus, det_plus = load_hits(f"{DATA_DIR}/Results/solid_KCl_0.5mm_100p_devito2023_plus_20260813_145448/output.root")
E_minus, theta_minus, det_minus = load_hits(f"{DATA_DIR}/Results/solid_KCl_0.5mm_100p_devito2023_minus_20260813_125402/output.root")



TRANSITIONS = {
    2599: {"A": -0.67, "file": f"{DATA_DIR}/EnergyDistributions/K47_2599.txt"},
    2578: {"A": 0.33,  "file": f"{DATA_DIR}/EnergyDistributions/K47_2578.txt"},
}
for t in TRANSITIONS.values():
    E_data, dNdE_data, _ = np.loadtxt(t["file"], unpack=True)
    t["interp"] = interp1d(E_data, dNdE_data, kind="cubic", bounds_error=False, fill_value=0.0)
 
 
def W(E, theta, A, P):
    """Angular distribution weight."""
    vc = np.sqrt(1 - (0.511 / (E + 0.511)) ** 2)
    return 1 + vc * A * P * np.cos(np.radians(theta))
 
 
def S(E, transition):
    """Energy-distribution weight (data is stored in keV, E is in MeV)."""
    return TRANSITIONS[transition]["interp"](E * 1000)
 
 
def asymmetry_np(E, theta, P, det, transition):
    """
    Returns:
        asymmetry (float): (NF - NR) / (NF + NR)
        NF, NR (float): summed weights in front/rear detectors
        front_weight, back_weight (array): per-event weights, front/back detector
        front_E, back_E (array): energies matching front_weight/back_weight
    """
    A = TRANSITIONS[transition]["A"]
    weight = W(E, theta, A, P) * S(E, transition) * np.sin(np.radians(theta))
 
    front_mask, back_mask = det == 0, det == 1
    NF, NR = weight[front_mask].sum(), weight[back_mask].sum()
    asymmetry = (NF - NR) / (NF + NR)
 
    return (asymmetry, NF, NR,
            weight[front_mask], weight[back_mask],
            E[front_mask], E[back_mask])
 
 
results = {}
for transition in TRANSITIONS:
    for label, (E, theta, det, P) in {
        "plus":  (E_plus,  theta_plus,  det_plus,  P_plus),
        "minus": (E_minus, theta_minus, det_minus, P_minus),
    }.items():
        asym, NF, NR, front_w, back_w, front_E, back_E = asymmetry_np(E, theta, P, det, transition)
        results[(transition, label)] = {
            "asymmetry": asym, "NF": NF, "NR": NR,
            "front_weight": front_w, "back_weight": back_w,
            "front_E": front_E, "back_E": back_E,
        }
 
POLARITY_SIGN = {"plus": "+z", "minus": "-z"}


#======================================================================
#               Prints asymmetry parameters for 2024
#======================================================================
# print()
# print("-----------------------------2024--0.5mm--------------------------")
# for transition in TRANSITIONS:
#     for label in ("plus", "minus"):
#         r = results[(transition, label)]
#         print(f"Transition = {transition}, magnetic field = {POLARITY_SIGN[label]}")
#         print("The ratio of counts (front/back) are ", r["NF"] / r["NR"])
#         print("The experimental asymmetry parameter is", r["asymmetry"])
#         print()
 


# #====================================================================
# #            Plots histograms for combinations of 2024
# #====================================================================
# # ---- Plot front + back histograms for all 4 combos ----
# fig, axes = plt.subplots(2, 2, figsize=(11, 8), sharex=True)
 
# for ax, (transition, label) in zip(axes.flat, results.keys()):
#     r = results[(transition, label)]
#     bins = np.linspace(
#         min(r["front_E"].min(), r["back_E"].min()),
#         max(r["front_E"].max(), r["back_E"].max()),
#         100,
#     )
#     ax.hist(r["front_E"], bins=bins, weights=r["front_weight"], alpha=0.5, label="Front")
#     ax.hist(r["back_E"], bins=bins, weights=r["back_weight"], alpha=0.5, label="Back")
    
#     ax.set_title(f"{transition}, B({POLARITY_SIGN[label]}), 2.0 mm")
#     ax.set_xlabel("Energy (MeV)")
#     ax.set_ylabel("Counts")
#     # ax.set_yscale("log")
#     ax.legend()
 
# fig.tight_layout()
# plt.savefig("detector_counts_2mm")
# plt.show()





#====================================================================
#            Plots histograms for combinations of 2023
#====================================================================
# ---- Plot front + back histograms for all 4 combos ----
fig, axes = plt.subplots(2, 2, figsize=(11, 8), sharex=True)
 
for ax, (transition, label) in zip(axes.flat, results.keys()):
    r = results[(transition, label)]
    bins = np.linspace(
        min(r["front_E"].min(), r["back_E"].min()),
        max(r["front_E"].max(), r["back_E"].max()),
        100,
    )
    ax.hist(r["front_E"], bins=bins, weights=r["front_weight"], alpha=0.5, label="Front")
    ax.hist(r["back_E"], bins=bins, weights=r["back_weight"], alpha=0.5, label="Back")
    
    ax.set_title(f"2023, {transition}, B({POLARITY_SIGN[label]}), 0.5 mm")
    ax.set_xlabel("Energy (MeV)")
    ax.set_ylabel("Counts")
    # ax.set_yscale("log")
    ax.legend()
 
fig.tight_layout()
plt.savefig("2023detector_counts_05m")
plt.show()
 