#include "TMVAGui.C"

void test(TString outfileName="ROOT/TMVA_pp_0_0.root")
{
  if(!gROOT->IsBatch()) TMVAGui(outfileName);
}
