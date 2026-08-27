import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import interp1d
import ROOT
ROOT.gROOT.SetBatch(True) 

#-------------------------------Modify here---------------------------------------------
A = 1/3   #Your theoretical asymmetry parameter
P = -1    #Your polarization factor

DATA_DIR = "/home/ngustafs/ISOLDE/run/Results" #CHANGE TO YOUR PATH
f = ROOT.TFile("data_file/INSERT_YOUR_FILENAME_HERE/output.root")
E_data, dNdE_data, _ = np.loadtxt("energy_dist/INSERT_YOUR_FILENAME_HERE", unpack=True)
#----------------------------------------------------------------------------------------


df = ROOT.RDataFrame("hits", f)
data = df.AsNumpy(columns=["energy", "angle", "detector"])
E, theta, det = data["energy"], data["angle"], data["detector"]

def W(E, theta, A, P):
    """Calculates the (theoretical) angular distribution of beta decay.

    Parameters
    ----------
    E : array_like
        Electron energy in MeV.
    theta : array_like
        Electron angle in degrees.
    A : float
        Theoretical asymmetry parameter.
    P : float
        Polarization.

    Returns
    -------
        Angular distribution of beta decay.
    """
    vc = np.sqrt(1 - (0.511 / (E + 0.511))**2)
    return 1 + vc * A * P * np.cos(np.radians(theta))


def S(E):
    """Energy distribution of beta decay, takes data points and interpolates them. 
    
        Parameters
        ----------
        E : array_like
            Electron energy in MeV.
    
        Returns
        -------
            Counts [arbitrary units] as a function of energy.
        """
    func = interp1d(E_data, dNdE_data, kind="cubic", bounds_error=False, fill_value=0.0)
    return(func(E*1000))      #Take E*1000 since data is in keV

def asymmetry_np(E, theta, A, P):
    """Calculates the (experimental) asymmetry parameter by weighing simulated detector counts with angular and energy distribution.
    
    Parameters
    ----------
    E : array_like
        Electron energy in MeV.
    theta : array_like
        Electron angle in degrees.
    A : float
        Theoretical asymmetry parameter.
    P : float
        Polarization.

    Returns
    -------
        Experimental asymmetry: (NF - NR) / (NF + NR)
        NF, NR: summed weights in front/rear detectors
    """
    w = W(E, theta, A, P)
    s = S(E)
    weight = w * s

    front, back = weight[det == 0], weight[det==1]
    NF = weight[det == 0].sum()
    NR = weight[det == 1].sum()
    return (NF - NR) / (NF + NR), NF/NR, NF, NR




A = asymmetry_np(E, theta, A, P)[0]
R =  asymmetry_np(E, theta, A, P)[1]  

print("The experimental asymmetry parameter is:", A)
print("The ratio of counts in the front and rear detector are:", R)