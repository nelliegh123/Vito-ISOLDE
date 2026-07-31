import ROOT
ROOT.gStyle.SetOptStat(0)

#---No Sample---
# f = ROOT.TFile("/home/ngustafs/ISOLDE/build/Results/solid_0mm_10000p_default_20260731_162120/output.root")

#---Solid---
# f = ROOT.TFile("/home/ngustafs/ISOLDE/build/Results/solid_0.5mm_10000p_default_20260731_144917/output.root")
# f = ROOT.TFile("../Results/solid_1mm_10000p_default_20260731_142706/output.root")
# f = ROOT.TFile("/home/ngustafs/ISOLDE/build/Results/solid_2mm_10000p_default_20260731_151314/output.root")
f = ROOT.TFile("/home/ngustafs/ISOLDE/build/Results/solid_4mm_10000p_default_20260731_153728/output.root")

# f = ROOT.TFile("../Results/solid_1mm_10000_DeVITOp_20260730_164711/output.root")

f.ls()
tree = f.Get("hits")

c1 = ROOT.TCanvas("c1", "Both detectors", 1200, 500)
c1.Divide(2,1)

c1.cd(1)
tree.Draw("angle:energy>>hF(19, 0, 20, 19, 0, 180)", "detector==0", "colz") 
hF = ROOT.gDirectory.Get("hF")
hF.Scale(1.0/10000)
hF.SetTitle("Front Detector;Energy [MeV];Angle [deg]")
hF.Draw("colz")
ROOT.gPad.Update()

c1.cd(2)
tree.Draw("angle:energy>>hR(19, 0, 20, 19, 0, 180)", "detector==1", "colz")
hR = ROOT.gDirectory.Get("hR")
hR.Scale(1.0/10000)
hR.SetTitle("Rear Detector;Energy [MeV];Angle [deg]")
hR.Draw("colz")
ROOT.gPad.Update()


# c1.SetFillColor(0)
# c1.SetFillStyle(1001)
# for i in (1,2):
#     c1.cd(i).SetFillColor(0)
#     c1.cd(i).SetFillStyle(1001)
# c1.Modified()
c1.Update()
c1.SaveAs("../Plots/Default_Solid_4mm.pdf")

input("Press Enter to exit...")



