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

P_no = 0

E_no, theta_no, det_no = load_hits(
    f"{DATA_DIR}/Results/devito/solid_KCl_2.0mm_1000p_devito2024_MagFieldOff_20260817_092209/output.root"
    # f"{DATA_DIR}/Results/devito/solid_KCl_0.5mm_1000p_devito2023_MagFieldOff_20260817_093636/output.root"
    
)

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
    weight = W(E, theta, A, P) * S(E, transition) * np.sin(np.radians(theta))

    front_mask, back_mask = det == 0, det == 1
    NF, NR = weight[front_mask].sum(), weight[back_mask].sum()
    asymmetry = (NF - NR) / (NF + NR)

    return (asymmetry, NF, NR, weight[front_mask], weight[back_mask], E[front_mask], E[back_mask])


results = {}
for transition in TRANSITIONS:
    asym, NF, NR, front_w, back_w, front_E, back_E = asymmetry_np(
        E_no, theta_no, P_no, det_no, transition
    )
    results[transition] = {
        "asymmetry": asym, "NF": NF, "NR": NR,
        "front_weight": front_w, "back_weight": back_w,
        "front_E": front_E, "back_E": back_E,
    }

#====================================================================
#     Plot histograms for the two transitions (field off, no flip)
#====================================================================
fig, axes = plt.subplots(1, 2, figsize=(11, 4), sharex=True)

for ax, transition in zip(axes, results.keys()):
    r = results[transition]
    bins = np.linspace(
        min(r["front_E"].min(), r["back_E"].min()),
        max(r["front_E"].max(), r["back_E"].max()),
        100,
    )
    ax.hist(r["front_E"], bins=bins, weights=r["front_weight"], color="deeppink",
            alpha=0.5, edgecolor="black", linewidth=0.5, label="Front")
    ax.hist(r["back_E"], bins=bins, weights=r["back_weight"], color="mediumpurple",
            alpha=0.7, edgecolor="black", linewidth=0.5, label="Back")

    ax.set_xlabel("Energy [MeV]", fontsize=12)
    ax.set_ylabel("Counts", fontsize=12)
    ax.set_yticks([])
    ax.set_xlim(0, 4.1)

    ax.legend(title=f"{transition}, Field Off")

fig.tight_layout()
plt.savefig("2024hist_fieldoff")
plt.show()


