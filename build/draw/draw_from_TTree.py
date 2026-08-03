# import ROOT
# ROOT.gStyle.SetOptStat(0)

# #---No Sample---
# # f = ROOT.TFile("/home/ngustafs/ISOLDE/build/Results/solid_0mm_10000p_default_20260731_162120/output.root")

# #---Solid---
# f = ROOT.TFile("/home/ngustafs/ISOLDE/build/Results/solid_0.5mm_10000p_default_20260731_144917/output.root")
# # f = ROOT.TFile("../Results/solid_1mm_10000p_default_20260731_142706/output.root")
# # f = ROOT.TFile("/home/ngustafs/ISOLDE/build/Results/solid_1.5mm_10000p_default_20260731_183018/output.root")
# # f = ROOT.TFile("/home/ngustafs/ISOLDE/build/Results/solid_2mm_10000p_default_20260731_151314/output.root")
# # f = ROOT.TFile("/home/ngustafs/ISOLDE/build/Results/solid_4mm_10000p_default_20260731_153728/output.root")

# # f = ROOT.TFile("../Results/solid_1mm_10000_DeVITOp_20260730_164711/output.root")

# f.ls()
# tree = f.Get("hits")

# c1 = ROOT.TCanvas("c1", "Both detectors", 1200, 500)
# c1.Divide(2,1)

# c1.cd(1)
# # tree.Draw("angle:energy>>hF(20, 0, 20, 20, 0, 180)", "detector==0", "colz") 

# tree.Draw(
#     "angle:energy>>hF(20, -0.5263158, 20.5263158, 20, -4.736842, 184.736842)",
#     "detector==0",
#     "colz"
# );

# hF = ROOT.gDirectory.Get("hF")
# hF.Scale(1.0/10000)
# hF.SetTitle("Front Detector;Energy [MeV];Angle [deg]")
# hF.Draw("colz")
# ROOT.gPad.Update()

# c1.cd(2)
# tree.Draw("angle:energy>>hR(20, 0, 20, 20, 0, 180)", "detector==1", "colz")
# hR = ROOT.gDirectory.Get("hR")
# hR.Scale(1.0/10000)
# hR.SetTitle("Rear Detector;Energy [MeV];Angle [deg]")
# hR.Draw("colz")
# ROOT.gPad.Update()


# # c1.SetFillColor(0)
# # c1.SetFillStyle(1001)
# # for i in (1,2):
# #     c1.cd(i).SetFillColor(0)
# #     c1.cd(i).SetFillStyle(1001)
# # c1.Modified()
# c1.Update()
# # c1.SaveAs("../Plots/Default_Solid_15mm.pdf")



# input("Press Enter to exit...")



import ROOT

ROOT.gStyle.SetOptStat(0)

f = ROOT.TFile("/home/ngustafs/ISOLDE/build/Results/solid_0.5mm_10000p_default_20260731_144917/output.root")
tree = f.Get("hits")

c = ROOT.TCanvas("c", "All / Front / Rear", 1800, 550)
c.Divide(3, 1)

# Same binning for all three histograms
hist_args = "(20, -0.5263158, 20.5263158, 20, -4.736842, 184.736842)"

# -------------------------
# All events
# -------------------------
c.cd(1)

tree.Draw(
    f"angle:energy>>hAll{hist_args}",
    "",
    "colz"
)

hAll = ROOT.gDirectory.Get("hAll")
hAll.SetTitle("All events;Energy [MeV];Angle [deg]")
hAll.Draw("colz")

# -------------------------
# Front detector
# -------------------------
c.cd(2)

tree.Draw(
    f"angle:energy>>hF2{hist_args}",
    "detector==0",
    "colz"
)

hF2 = ROOT.gDirectory.Get("hF2")
hF2.SetTitle("Front detector;Energy [MeV];Angle [deg]")
hF2.Draw("colz")

# -------------------------
# Rear detector
# -------------------------
c.cd(3)

tree.Draw(
    f"angle:energy>>hR2{hist_args}",
    "detector==1",
    "colz"
)

hR2 = ROOT.gDirectory.Get("hR2")
hR2.SetTitle("Rear detector;Energy [MeV];Angle [deg]")
hR2.Draw("colz")

c.Update()
input("Press Enter to exit...")
