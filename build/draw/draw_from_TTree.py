import ROOT
ROOT.gROOT.SetBatch(True) 

f = ROOT.TFile("TotalAngleScan.root")
f.ls()
tree = f.Get("hits")

c1 = ROOT.TCanvas("c1", "Both detectors", 1200, 500)
c1.Divide(2,1)

c1.cd(1)
tree.Draw("angle:energy>>hF(19, 1, 10, 46, 0, 180)", "detector==0", "colz")
ROOT.gPad.Update()

c1.cd(2)
tree.Draw("angle:energy>>hR(19, 1, 10, 46, 0, 180)", "detector==1", "colz")
ROOT.gPad.Update()

c1.Update()
c1.SaveAs("Plots/forward_hits_TEST.png")



