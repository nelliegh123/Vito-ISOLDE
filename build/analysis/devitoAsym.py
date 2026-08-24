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
# # # #===============2024 Setup===============
# E_plus, theta_plus, det_plus = load_hits(f"{DATA_DIR}/Results/devito/solid_KCl_2.0mm_1000p_devito2024_plus_20260813_152306/output.root")
# E_minus, theta_minus, det_minus = load_hits(f"{DATA_DIR}/Results/devito/solid_KCl_2.0mm_1000p_devito2024_minus_20260813_153845/output.root")
# # # #===============2024 Setup===============

# # #===============TEST===============
# # #===============2024 Setup, 1mm thickness===============
# E_plus, theta_plus, det_plus = load_hits(f"{DATA_DIR}/Results/devito/test/solid_KCl_1.0mm_1000p_devito2024_plus_20260818_140435/output.root")
# E_minus, theta_minus, det_minus = load_hits(f"{DATA_DIR}/Results/devito/test/solid_KCl_1.0mm_1000p_devito2024_minus_20260818_104404/output.root")
# # #===============2024 Setup, 1 mm thickness===============

# # #===============2024 Setup, 0.01mm thickness===============
# E_plus, theta_plus, det_plus = load_hits(f"{DATA_DIR}/Results/devito/test/solid_KCl_0.01mm_100p_devito2024_plus_20260819_152056/output.root")
# E_minus, theta_minus, det_minus = load_hits(f"{DATA_DIR}/Results/devito/test/solid_KCl_0.01mm_100p_devito2024_minus_20260819_152356/output.root")
# # #===============2024 Setup, 0.01 mm thickness===============

# # #===============2024 Setup, 0.5mm thickness===============
# E_plus, theta_plus, det_plus = load_hits(f"{DATA_DIR}/Results/devito/test/solid_KCl_0.5mm_100p_devito2024_plus_20260819_154106/output.root")
# E_minus, theta_minus, det_minus = load_hits(f"{DATA_DIR}/Results/devito/test/solid_KCl_0.5mm_100p_devito2024_minus_20260819_153551/output.root")
# # #===============2024 Setup, 0.5 mm thickness===============

# #===============2024 Setup, 3mm thickness===============
# E_plus, theta_plus, det_plus = load_hits(f"{DATA_DIR}/Results/devito/test/solid_KCl_3.0mm_100p_devito2024_plus_20260819_170445/output.root")
# E_minus, theta_minus, det_minus = load_hits(f"{DATA_DIR}/Results/devito/test/solid_KCl_3.0mm_100p_devito2024_minus_20260819_170725/output.root")
# #===============2024 Setup, 3mm thickness===============


# # #===============2024 Setup, 2mm thickness, plus 1 mm pos===============
# E_plus, theta_plus, det_plus = load_hits(f"{DATA_DIR}/Results/devito/test/solid_KCl_2.0mm_100p_devito2024_plus_plus1mm_20260819_161428/output.root")
# E_minus, theta_minus, det_minus = load_hits(f"{DATA_DIR}/Results/devito/test/solid_KCl_2.0mm_100p_devito2024_minus_plus1mm_20260819_161236/output.root")
# # #===============2024 Setup, 2mm thickness, plus 1 mm pos===============

# # #===============2024 Setup, 2mm thickness, minus 1 mm pos===============
# E_plus, theta_plus, det_plus = load_hits(f"{DATA_DIR}/Results/devito/test/solid_KCl_2.0mm_100p_devito2024_plus_minus1mm_20260819_161911/output.root")
# E_minus, theta_minus, det_minus = load_hits(f"{DATA_DIR}/Results/devito/test/solid_KCl_2.0mm_100p_devito2024_minus_minus1mm_20260819_162201/output.root")
# # #===============2024 Setup, 2mm thickness, minus 1 mm pos===============

# ===============2024 Setup, 2mm thickness, x=plus1 mm pos===============
# # E_plus, theta_plus, det_plus = load_hits(f"{DATA_DIR}/Results/devito/test/solid_KCl_2.0mm_100p_devito2024_plus_xplus1mm_20260819_163321/output.root")
# # E_minus, theta_minus, det_minus = load_hits(f"{DATA_DIR}/Results/devito/test/solid_KCl_2.0mm_100p_devito2024_minus_xplus1mm_20260819_163113/output.root")
# ===============2024 Setup, 2mm thickness, x=plus1 mm pos===============

# # #===============2024 Setup, no field===============
# E_plus, theta_plus, det_plus = load_hits(f"{DATA_DIR}/Results/devito/test/solid_KCl_2.0mm_1000p_devito2024_MagFieldOff_20260817_092209/output.root")
# # #===============2024 Setup,no field===============
# # #===============TEST===============







# #===============2023 Setup===============
# E_plus, theta_plus, det_plus = load_hits(f"{DATA_DIR}/Results/devito/solid_KCl_0.5mm_1000p_devito2023_plus_20260817_133550/output.root")
# E_minus, theta_minus, det_minus = load_hits(f"{DATA_DIR}/Results/devito/solid_KCl_0.5mm_1000p_devito2023_minus_20260817_135602/output.root")
# #===============2023 Setup===============







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
        asymmetry: (NF - NR) / (NF + NR)
        NF, NR: summed weights in front/rear detectors
    """
    A = TRANSITIONS[transition]["A"]
    weight = 2*np.pi * W(E, theta, A, P) * S(E, transition) * np.sin(np.radians(theta))
 
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




# def double_ratio_asymmetry(NF_plus, NR_plus, NF_minus, NR_minus, beta_eff, P):
#     """
#     Computes the double-ratio (super-ratio) asymmetry, which cancels
#     any instrumental/geometric front-back asymmetry that doesn't
#     depend on field polarity.

#     Parameters
#     ----------
#     NF_plus, NR_plus   : summed front/rear weights with B in +z (P = +|P|)
#     NF_minus, NR_minus : summed front/rear weights with B in -z (P = -|P|)
#     beta_eff : effective <beta> for this transition/energy distribution
#                (see note below on how to get this properly)
#     P : magnitude of polarization used (e.g. 0.15)

#     Returns
#     -------
#     R   : the double ratio itself (should deviate from 1 by ~ a few %)
#     A_exp : extracted experimental asymmetry parameter (~ the true A,
#             once beta_eff and P are correctly divided out)
#     """
#     R = np.sqrt((NF_plus * NR_minus) / (NR_plus * NF_minus))
#     A_exp = (1.0 / (beta_eff * P)) * (R - 1) / (R + 1)
#     return R, A_exp


# def beta_of_E(E):
#     """Relativistic beta, E in MeV kinetic energy."""
#     return np.sqrt(1 - (0.511 / (E + 0.511)) ** 2)


# def weighted_mean_beta(E, weight):
#     """Weight-averaged <beta> over the same S(E) weighting used elsewhere,
#     so beta_eff matches the population actually contributing to NF/NR."""
#     return np.sum(beta_of_E(E) * weight) / np.sum(weight)




# print()
# print("-----------------------------Double Ratio--------------------------")
# for transition in TRANSITIONS:
#     r_plus  = results[(transition, "plus")]
#     r_minus = results[(transition, "minus")]

#     # beta_eff: average beta over ALL contributing events (front+back, both polarities)
#     E_all = np.concatenate([r_plus["front_E"], r_plus["back_E"],
#                              r_minus["front_E"], r_minus["back_E"]])
#     w_all = np.concatenate([r_plus["front_weight"], r_plus["back_weight"],
#                              r_minus["front_weight"], r_minus["back_weight"]])
#     beta_eff = weighted_mean_beta(E_all, w_all)

#     R, A_exp = double_ratio_asymmetry(
#         r_plus["NF"], r_plus["NR"],
#         r_minus["NF"], r_minus["NR"],
#         beta_eff, P=abs(P_plus),
#     )

#     A_true = TRANSITIONS[transition]["A"]
#     print(f"Transition = {transition}")
#     print(f"  <beta>_eff        = {beta_eff:.4f}")
#     print(f"  Double ratio R    = {R:.4f}")
#     print(f"  Extracted A_exp   = {A_exp:.4f}")
#     print(f"  Input A (truth)   = {A_true:.4f}")
#     print(f"  Ratio A_exp/A_true = {A_exp/A_true:.4f}")
#     print()











# #======================================================================
# #               Prints asymmetry parameters for 2024
# #======================================================================
# print()
# print("-----------------------------2024--2.0mm--------------------------")
# for transition in TRANSITIONS:
#     for label in ("plus", "minus"):
#         r = results[(transition, label)]
#         print(f"Transition = {transition}, magnetic field = {POLARITY_SIGN[label]}")
#         print("The ratio of counts (front/back) are ", r["NF"] / r["NR"])
#         print("The experimental asymmetry parameter is", r["asymmetry"])
#         print()
 


#====================================================================
#            Plots histograms for combinations of 2024
#====================================================================
# # ---- Plot front + back histograms for all 4 combos ----
# fig, axes = plt.subplots(2, 2, figsize=(11, 8), sharex=True)

# for ax, (transition, label) in zip(axes.flat, results.keys()):
#     r = results[(transition, label)]
#     bins = np.linspace(
#         min(r["front_E"].min(), r["back_E"].min()),
#         max(r["front_E"].max(), r["back_E"].max()),
#         91, #100
#     )
#     ax.hist(r["front_E"], bins=bins, weights=r["front_weight"], color="deeppink", alpha=0.5, edgecolor="black", linewidth=0.5, label="Front")
#     # ax.hist(r["back_E"], bins=bins, weights=r["back_weight"], color="white", alpha=1)
#     ax.hist(r["back_E"], bins=bins, weights=r["back_weight"], color="mediumpurple", alpha=0.7, edgecolor="black", linewidth=0.5, label="Back")

#     ax.set_xlabel("Energy [MeV]", fontsize=12)
#     ax.set_ylabel("Counts", fontsize=12)
#     ax.set_yticks([])
#     ax.set_xlim(0, 4.1)

#     sign = "+" if POLARITY_SIGN[label] == "+z" else "-"
#     ax.legend(title=f"{transition}, B{sign}")

# fig.tight_layout()
# plt.savefig("2024Hist")
# plt.show()





# #====================================================================
# #            Plots histograms for combinations of 2023
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
    
#     ax.set_title(f"2024, {transition}, B({POLARITY_SIGN[label]}), 2.0 mm")
#     ax.set_xlabel("Energy (MeV)")
#     ax.set_ylabel("Counts")
#     # ax.set_yscale("log")
#     ax.legend()
 
# fig.tight_layout()
# plt.savefig("2023detector_counts_05mm")
# plt.show()
 




def synthetic_baseline(E_array, transition, P, A, n_synthetic=1000, n_theta=2000):
    """
    For each energy in E_array, compute the baseline front/back weight to a
    flat distribution of events, instead of using real detector hits.
    """
    theta_front = np.linspace(0, 90, n_theta)   
    theta_back  = np.linspace(90, 180, n_theta)  

    dtheta_front_rad, dtheta_back_rad = np.radians(theta_front[1] - theta_front[0]), np.radians(theta_back[1] - theta_back[0])

    s_vals = S(E_array, transition)  
    E_col = E_array[:, None]  

    W_front, W_back  = W(E_col, theta_front[None, :], A, P), W(E_col, theta_back[None, :],  A, P)
    integrand_front, integrand_back = W_front * np.sin(np.radians(theta_front))[None, :], W_back  * np.sin(np.radians(theta_back))[None, :]
    front_integral, back_integral = integrand_front.sum(axis=1) * dtheta_front_rad, integrand_back.sum(axis=1)  * dtheta_back_rad

    NF_baseline = n_synthetic * 2 * np.pi * s_vals * front_integral
    NR_baseline = n_synthetic * 2 * np.pi * s_vals * back_integral
    return NF_baseline, NR_baseline

# E_array = np.linspace(0.01, 4.1, 300)
# fig, ax = plt.subplots(figsize=(8, 5))
# for transition in TRANSITIONS:
#     A = TRANSITIONS[transition]["A"]
#     P = P_plus
#     NF_baseline, NR_baseline = synthetic_baseline(E_array, transition, P, A)
#     ax.plot(E_array, NF_baseline, label=f"{transition} Front")
#     ax.plot(E_array, NR_baseline, label=f"{transition} Back", linestyle="--")
# ax.set_xlabel("Energy (MeV)")
# ax.set_ylabel("Baseline Counts (P plus)")
# ax.set_xlim(0, 4.1)
# ax.legend()
# fig.tight_layout()
# plt.savefig("BaselineDetectorCountsPlus.png")
# plt.show()


# E_array = np.linspace(0.01, 4.1, 300)
# fig, ax = plt.subplots(figsize=(8, 5))
# for transition in TRANSITIONS:
#     A = TRANSITIONS[transition]["A"]
#     P = P_minus
#     NF_baseline, NR_baseline = synthetic_baseline(E_array, transition, P, A)
#     ax.plot(E_array, NF_baseline, label=f"{transition} Front")
#     ax.plot(E_array, NR_baseline, label=f"{transition} Back", linestyle="--")
# ax.set_xlabel("Energy (MeV)")
# ax.set_ylabel("Baseline Counts (P minus)")
# ax.set_xlim(0, 4.1)
# ax.legend()
# fig.tight_layout()
# plt.savefig("BaselineDetectorCountsMinus.png")
# plt.show()

#====================================================================
#     Plots "beta acceptance" (real weighted / synthetic baseline)
#====================================================================
fig, axes = plt.subplots(2, 2, figsize=(11, 8), sharex=True)

for ax, (transition, label) in zip(axes.flat, results.keys()):
    r = results[(transition, label)]
    A = TRANSITIONS[transition]["A"]
    P = P_plus if label == "plus" else P_minus

    bins = np.linspace(min(r["front_E"].min(), r["back_E"].min()), max(r["front_E"].max(), r["back_E"].max()), 91)
    bin_centers = 0.5 * (bins[:-1] + bins[1:])

    front_num, _ = np.histogram(r["front_E"], bins=bins, weights=r["front_weight"])
    back_num, _ = np.histogram(r["back_E"], bins=bins, weights=r["back_weight"])

    NF_baseline, NR_baseline = synthetic_baseline(bin_centers, transition, P, A)

    front_ratio = np.divide(front_num, NF_baseline, out=np.zeros_like(front_num), where=NF_baseline > 0)
    back_ratio = np.divide(back_num, NR_baseline, out=np.zeros_like(back_num), where=NR_baseline > 0)

    width = bins[1] - bins[0]

    ax.bar(bin_centers, front_ratio, width=width, alpha=0.9, label="Front", edgecolor="black", linewidth=0.5, color="mediumpurple")
    ax.bar(bin_centers, back_ratio, width=width, alpha=0.6, label="Back", edgecolor="black", linewidth=0.5, color="dodgerblue")

    ax.set_title(f"{transition}, B({POLARITY_SIGN[label]}), 2.0 mm")
    ax.set_xlabel("Energy (MeV)")
    ax.set_ylabel("Beta acceptance")
    ax.set_xlim(0, 4.1)
    ax.legend()

fig.tight_layout()
plt.savefig("2024beta_acceptance_001mm")
plt.show()











# # #====================================================================
# # #            Plotting combined hist 2024
# # #====================================================================

# TRANSITION_WEIGHTS = {2599: 0.8, 2578: 0.184}
 
# # ---- Common binning across everything: both transitions, both polarities ----
# all_E = np.concatenate(
#     [results[(t, label)]["front_E"] for t in TRANSITIONS for label in ("plus", "minus")]
#     + [results[(t, label)]["back_E"] for t in TRANSITIONS for label in ("plus", "minus")]
# )
# bins = np.linspace(all_E.min(), all_E.max(), 100)
# bin_centers = 0.5 * (bins[:-1] + bins[1:])
 
# # ---- Sum the weighted histograms across transitions AND polarities ----
# front_counts = np.zeros(len(bins) - 1)
# back_counts = np.zeros(len(bins) - 1)
 
# for transition in TRANSITIONS:
#     w = TRANSITION_WEIGHTS[transition]
#     for label in ("plus", "minus"):
#         r = results[(transition, label)]
 
#         fc, _ = np.histogram(r["front_E"], bins=bins, weights=r["front_weight"] * w)
#         bc, _ = np.histogram(r["back_E"], bins=bins, weights=r["back_weight"] * w)
 
#         front_counts += fc
#         back_counts += bc
 
# # ---- Combined asymmetry / NF, NR for reference ----
# NF, NR = front_counts.sum(), back_counts.sum()
# asym = (NF - NR) / (NF + NR)
# print(f"Combined (plus+minus): NF/NR = {NF / NR:.5f}, asymmetry = {asym:.5f}")
 
# # ---- One plot, two graphs: combined front, combined back ----
# fig, ax = plt.subplots(figsize=(9, 6))
# ax.step(bin_centers, front_counts, where="mid", label="Front (combined)")
# ax.step(bin_centers, back_counts, where="mid", label="Back (combined)")
 
# ax.set_xlabel("Energy (MeV)")
# ax.set_ylabel("Weighted Counts")
# ax.set_title("Combined transitions (2599x0.8 + 2578x0.184), B(+z) and B(-z) combined")
# ax.legend()
 
# fig.tight_layout()
# plt.savefig("2024detector_counts_combined_all")
# plt.show()