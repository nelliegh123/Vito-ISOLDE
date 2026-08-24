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


# ==============================================================
# Polarization
# ==============================================================

P_plus = 0


# ==============================================================
# 2024 Setup, no magnetic field
# ==============================================================

E_plus, theta_plus, det_plus = load_hits(
    f"{DATA_DIR}/Results/devito/"
    "solid_KCl_2.0mm_1000p_devito2024_MagFieldOff_20260817_092209/"
    "output.root"
)


# ==============================================================
# Transitions
# ==============================================================

TRANSITIONS = {
    2599: {
        "A": -0.67,
        "file": f"{DATA_DIR}/EnergyDistributions/K47_2599.txt"
    },
    2578: {
        "A": 0.33,
        "file": f"{DATA_DIR}/EnergyDistributions/K47_2578.txt"
    },
}


for t in TRANSITIONS.values():

    E_data, dNdE_data, _ = np.loadtxt(
        t["file"],
        unpack=True
    )

    t["interp"] = interp1d(
        E_data,
        dNdE_data,
        kind="cubic",
        bounds_error=False,
        fill_value=0.0
    )


# ==============================================================
# Angular distribution
# ==============================================================

def W(E, theta, A, P):
    """Angular distribution weight."""

    vc = np.sqrt(
        1 - (0.511 / (E + 0.511)) ** 2
    )

    return (
        1
        + vc
        * A
        * P
        * np.cos(np.radians(theta))
    )


# ==============================================================
# Energy distribution
# ==============================================================

def S(E, transition):
    """
    Energy-distribution weight.

    Data is stored in keV,
    while E is in MeV.
    """

    return TRANSITIONS[transition]["interp"](
        E * 1000
    )


# ==============================================================
# Asymmetry calculation
# ==============================================================

def asymmetry_np(E, theta, P, det, transition):
    """
    Returns:

        asymmetry = (NF - NR) / (NF + NR)

        NF = summed weights in front detector
        NR = summed weights in rear detector
    """

    A = TRANSITIONS[transition]["A"]

    weight = (
        2
        * np.pi
        * W(E, theta, A, P)
        * S(E, transition)
        * np.sin(np.radians(theta))
    )

    front_mask = det == 0
    back_mask = det == 1

    NF = weight[front_mask].sum()
    NR = weight[back_mask].sum()

    asymmetry = (NF - NR) / (NF + NR)

    return (
        asymmetry,
        NF,
        NR,
        weight[front_mask],
        weight[back_mask],
        E[front_mask],
        E[back_mask]
    )


# ==============================================================
# Apply energy cut
# ==============================================================

mask_plus = E_plus >= 0

E_plus = E_plus[mask_plus]
theta_plus = theta_plus[mask_plus]
det_plus = det_plus[mask_plus]


# ==============================================================
# Calculate results
# ==============================================================

results = {}

for transition in TRANSITIONS:

    (
        asym,
        NF,
        NR,
        front_w,
        back_w,
        front_E,
        back_E
    ) = asymmetry_np(
        E_plus,
        theta_plus,
        P_plus,
        det_plus,
        transition
    )

    results[transition] = {
        "asymmetry": asym,
        "NF": NF,
        "NR": NR,
        "front_weight": front_w,
        "back_weight": back_w,
        "front_E": front_E,
        "back_E": back_E,
    }


# ==============================================================
# Print asymmetry parameters
# ==============================================================

print()
print("-----------------------------2024--2.0mm--------------------------")

for transition in TRANSITIONS:

    r = results[transition]

    print(
        f"Transition = {transition}, magnetic field = +z"
    )

    print(
        "The ratio of counts (front/back) are ",
        r["NF"] / r["NR"]
    )

    print(
        "The experimental asymmetry parameter is",
        r["asymmetry"]
    )

    print()