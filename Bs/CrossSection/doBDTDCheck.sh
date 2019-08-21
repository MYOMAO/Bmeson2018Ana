#!/bin/bash
#source clean.sh
CENTPbPbMIN=0
CENTPbPbMAX=90

DOANALYSISPP_FONLL=0
DOANALYSISPP_FIT=0
DOANALYSISPP_FITONSAVED=0
DOANALYSISPP_ROOFIT=0
DOANALYSISPP_ROOFITONSAVED=0
DOANALYSISPP_MCSTUDY=0
DOANALYSISPP_CROSS=0

DOANALYSISPbPb_FIT=1
DOANALYSISPbPb_FITONSAVED=0
DOANALYSISPbPb_ROOFIT=0
DOANALYSISPbPb_ROOFITONSAVED=0
DOANALYSISPbPb_MCSTUDY=0
DOANALYSISPbPb_CROSS=0
DORAA=0
DORAARATIO=0
DOANALYSISPbPb_REWEIGHTBPT=0
DOANALYSISPbPb_REWEIGHTPVZ=0


DOANALYSISPbPb_FITNP=0

#Closure Test

DOCLOSUREPbPb=0
FINCLOSUREPbPb=0


#ONY For Acceptance Studies
DOANALYSISPbPb_FITONY=0
DOANALYSISPbPb_MCSTUDYONY=0


DOANALYSISPbPb_PTSHAPESYST=0
DOANALYSISPbPb_PTSHAPESYSTPLOT=0
### pt inclusive interval mass fit
DOANALYSISPP_FIT_Inc=0
DOANALYSISPP_FITONSAVED_Inc=0
DOANALYSISPP_ROOFIT_Inc=0
DOANALYSISPP_ROOFITONSAVED_Inc=0
DOANALYSISPbPb_FIT_Inc=0
DOANALYSISPbPb_FITONSAVED_Inc=0
DOANALYSISPbPb_ROOFIT_Inc=0
DOANALYSISPbPb_ROOFITONSAVED_Inc=0

### pp 3-bin result
DOANALYSISPP_FONLL_750=0
DOANALYSISPP_FIT_750=0
DOANALYSISPP_FITONSAVED_750=0
DOANALYSISPP_ROOFIT_750=0
DOANALYSISPP_ROOFITONSAVED_750=0
DOANALYSISPP_MCSTUDY_750=0
DOANALYSISPP_CROSS_750=0

### pp cut base MVA (CUTBASE)
DOANALYSISPP_FIT_CUTBASE=0
DOANALYSISPP_FITONSAVED_CUTBASE=0
DOANALYSISPP_ROOFIT_CUTBASE=0
DOANALYSISPP_ROOFITONSAVED_CUTBASE=0
DOANALYSISPP_MCSTUDY_CUTBASE=0
DOANALYSISPP_CROSS_CUTBASE=0

### pp sample with PbPb MVA (BDT)
DOANALYSISPP_FIT_PbPbBDT=0
DOANALYSISPP_FITONSAVED_PbPbBDT=0
DOANALYSISPP_ROOFIT_PbPbBDT=0
DOANALYSISPP_ROOFITONSAVED_PbPbBDT=0
DOANALYSISPP_MCSTUDY_PbPbBDT=0
DOANALYSISPP_CROSS_PbPbBDT=0

### pp sample with old PbPb MVA (BDT)
DOANALYSISPP_FIT_OLDPbPbBDT=0
DOANALYSISPP_FITONSAVED_OLDPbPbBDT=0
DOANALYSISPP_ROOFIT_OLDPbPbBDT=0
DOANALYSISPP_ROOFITONSAVED_OLDPbPbBDT=0
DOANALYSISPP_MCSTUDY_OLDPbPbBDT=0
DOANALYSISPP_CROSS_OLDPbPbBDT=0

### pp 4-bin result for acceptance
DOANALYSISPP_FIT_750_acc=0
DOANALYSISPP_MCSTUDY_750_acc=0

### Rapidity result for acceptance
DOANALYSISPP_FIT_Y=0
DOANALYSISPP_MCSTUDY_Y=0

### Reweighting
DOPTREWEIGHT=0

### Cross checks
DOClosure=0

### Project some varibles
PROJECTVAR=0

### MC pp PbPb comparison
DOVARCOMPARE_MC=0

### Data - MC comparison
DOVARCOMPARE_PP=0

### Checking prompt background structure
CHECKPROMPT=0

### PDF Syst.
DOPDFSYST=0

### Pt Shape Syst.
DOPTSHAPESYST=0

### Track Eta Weight Syst.
DOTKETAWEIGHTSYST=0

## PP MONTE CARLO
INPUTMCPP="/export/d00/scratch/tawei/HeavyFlavor/Run2Ana/BsTMVA/samples/Bntuple20180316_bPt0_BfinderMC_pp_BsToJpsiPhi_Pythia8_5p02_20180314_bPt0tkPt0MuAll_Bs_pthatweight_BDT7to50.root"
INPUTMCPPCANDWISE="/export/d00/scratch/tawei/HeavyFlavor/Run2Ana/BsTMVA/samples/Bntuple20180316_bPt0_BfinderMC_pp_BsToJpsiPhi_Pythia8_5p02_20180314_bPt0tkPt0MuAll_Bs_pthatweight_BDT7to50_candWise.root"

## PP DATA 
INPUTDATAPP="/export/d00/scratch/tawei/HeavyFlavor/Run2Ana/BsTMVA/samples/Bntuple20180316_bPt0_BfinderData_pp_20180314_bPt0tkPt0MuAll_Bs_BDT7to50.root"
INPUTDATAPPCANDWISE="/export/d00/scratch/tawei/HeavyFlavor/Run2Ana/BsTMVA/samples/Bntuple20180316_bPt0_BfinderData_pp_20180314_bPt0tkPt0MuAll_Bs_BDT7to50_candWise.root"

## PbPb MONTE CARLO
INPUTMCPbPb="/export/d00/scratch/tawei/HeavyFlavor/Run2Ana/BsTMVA/samples/Bntuple20180316_bPt0_BfinderMC_PbPb_BsToJpsiPhi_HydjetCymbMB_5p02_20180314_bPt0tkPt0MuAll_Bs_pthatweight_BDT7to50.root"
#INPUTMCPbPbCANDWISE="/export/d00/scratch/tawei/HeavyFlavor/Run2Ana/BsTMVA/samples/Bntuple20180316_bPt0_BfinderMC_PbPb_BsToJpsiPhi_HydjetCymbMB_5p02_20180314_bPt0tkPt0MuAll_Bs_pthatweight_BDT7to50_candWise.root"

## PbPb DATA 
INPUTDATAPbPb="/export/d00/scratch/tawei/HeavyFlavor/Run2Ana/BsTMVA/samples/Bntuple20180316_bPt0_BfinderData_PbPb_20180314_bPt0tkPt0MuAll_Bs_HIOniaL1DoubleMu0_AllDatamerged_BDT7to50.root"
#INPUTDATAPbPbCANDWISE="/export/d00/scratch/tawei/HeavyFlavor/Run2Ana/BsTMVA/samples/Bntuple20180316_bPt0_BfinderData_PbPb_20180314_bPt0tkPt0MuAll_Bs_HIOniaL1DoubleMu0_AllDatamerged_BDT7to50_candWise.root"

#INPUTMCPbPbCANDWISE="/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/2018Ana/Samples/Bntuple20160816_Bpt7svpv5p5Bpt10svpv3p5_BfinderMC_PbPb_Pythia8_BuToJpsiK_TuneCUEP8M1_20160816_bPt5jpsiPt0tkPt0p8_Bp_pthatweight_JingBDT.root"
#INPUTMCPbPbCANDWISE="/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/ToCopy/MakeBDT/MC/crab_Bfinder_20190115_Hydjet_Pythia8_BsToJpsiPhi_20181231_pt5tkpt0p7dls2_v3_pthatweight_BDTMerged.root"
#INPUTDATAPbPbCANDWISE="/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/2018Ana/Samples/BsAllSoFar.root"
#INPUTDATAPbPbCANDWISE="/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/ToCopy/MakeBDT/Files/crab_Bfinder_20181220_HIDoubleMuon_HIRun2018A_PromptReco_v2_1031_NoJSON_skimhltBsize_ntphi_BDTMerged.root"
#INPUTDATAPbPbCANDWISE="/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/ToCopy/Data/New/crab_Bfinder_20181220_HIDoubleMuon_HIRun2018A_PromptReco_v1v2_1031_NoJSON_skimhltBsize_ntphi_NewBDTMerged.root"


#INPUTMCPbPbCANDWISE="/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/2018Ana/BsTMVA/TMVAHigh/MC_Bs_pp_TMVA_BDT_pp.root"
#INPUTDATAPbPbCANDWISE="/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/2018Ana/BsTMVA/TMVAHigh/Data_Bs_pp_TMVA_BDT_pp.root"

#INPUTMCPbPbCANDWISE="/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/2018Ana/BsTMVA/TMVAHigh4Bins/MC_Bs_pp_TMVA_BDT_pp.root"
#INPUTDATAPbPbCANDWISE="/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/2018Ana/BsTMVA/TMVAHigh4Bins/Data_Bs_pp_TMVA_BDT_pp.root"
#INPUTMCPbPbCANDWISE="/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/2018Ana/BsTMVA/TMVAHigh4BinsRedo/MC_Bs_pp_TMVA_BDT_pp.root"
#INPUTDATAPbPbCANDWISE="/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/2018Ana/BsTMVA/TMVAHigh4BinsRedo/Data_Bs_pp_TMVA_BDT_pp.root"
#INPUTDATAPbPbCANDWISE="/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/2018Ana/Recalculation/Reassigned.root"

#INPUTMCPbPbCANDWISE="/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/2018Ana/BsTMVA/TMVAHigh3BinsRedo/MC_Bs_PbPb_TMVA_BDT_pp_CentWighted.root"
#INPUTMCPbPbCANDWISE="/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/2018Ana/BsRAA2015RunII/PthatService/output/PthatMergedCentreweightBDT.root"
#INPUTDATAPbPbCANDWISE="/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/2018Ana/BsTMVA/TMVAHigh3BinsRedo/Data_Bs_PbPb_TMVA_BDT_pp.root"

INPUTMCPbPbCANDWISE="/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/BDTAppType/BDTD/MC_Bs_PbPb_TMVA_BDT_PbPb.root"
INPUTDATAPbPbCANDWISE="/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/BDTAppType/BDTD/Data_Bs_PbPb_TMVA_BDT_PbPb.root"
INPUTMCNPPbPbCANDWISE="/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/2018Ana/Samples/BsNPMC/MakeBDT/AllPt/BsNPAllMerged.root"


## ANALYSIS PP TRIGGERED
FONLLDATINPUT="pp_Bplus_5p03TeV_y2p4"
FONLLOUTPUTFILE="ROOTfiles/fonllOutput_pp_Bplus_5p03TeV_y2p4.root"
FONLLOUTPUTFILEREWEIGHT="ROOTfiles/fonllOutput_pp_Bplus_5p03TeV_y2p4_reweightBin.root"
OUTPUTFILERAA="ROOTfiles/outputRAA.root"



##FILENAME PT SHAPE SYST

WEIGHTEDEFFOUTFILE="plotPtShapeSyst/Files/EffWeighted.root"
UNWEIGHTEDEFFOUTFILE="plotPtShapeSyst/Files/EffUnWeighted.root"
PLOTNAME="plotPtShapeSyst/Plots/EffRatio.png"
PTSHAPEOUTFILENAME="plotPtShapeSyst/Files/EffRatio.root"



LABELPP="pp"
LUMIPP=27.748 # paper 20170224

#28.0*0.991
ISMCPP=0
ISDOWEIGHTPP=0
SELGENPP="TMath::Abs(Gy)<2.4&&abs(GpdgId)==531&&GisSignal>0"
SELGENPPACCPP="TMath::Abs(Gy)<2.4&&abs(GpdgId)==531&&GisSignal>0&&((TMath::Abs(Gmu1eta)<1.2&&Gmu1pt>3.5)||(TMath::Abs(Gmu1eta)>1.2&&TMath::Abs(Gmu1eta)<2.1&&Gmu1pt>(5.77-1.8*TMath::Abs(Gmu1eta)))||(TMath::Abs(Gmu1eta)>2.1&&TMath::Abs(Gmu1eta)<2.4&&Gmu1pt>1.8))&&((TMath::Abs(Gmu2eta)<1.2&&Gmu2pt>3.5)||(TMath::Abs(Gmu2eta)>1.2&&TMath::Abs(Gmu2eta)<2.1&&Gmu2pt>(5.77-1.8*TMath::Abs(Gmu2eta)))||(TMath::Abs(Gmu2eta)>2.1&&TMath::Abs(Gmu2eta)<2.4&&Gmu2pt>1.8))&&Gtk1pt>0.&&Gtk2pt>0.&&TMath::Abs(Gtk1eta)<2.4&&TMath::Abs(Gtk2eta)<2.4"
BASECUTPP="TMath::Abs(By)<2.4&&TMath::Abs(Bmumumass-3.096916)<0.15&&((abs(Bmu1eta)<1.2&&Bmu1pt>3.5)||(abs(Bmu1eta)>1.2&&abs(Bmu1eta)<2.1&&Bmu1pt>(5.77-1.8*abs(Bmu1eta)))||(abs(Bmu1eta)>2.1&&abs(Bmu1eta)<2.4&&Bmu1pt>1.8))&&((abs(Bmu2eta)<1.2&&Bmu2pt>3.5)||(abs(Bmu2eta)>1.2&&abs(Bmu2eta)<2.1&&Bmu2pt>(5.77-1.8*abs(Bmu2eta)))||(abs(Bmu2eta)>2.1&&abs(Bmu2eta)<2.4&&Bmu2pt>1.8))&&Bmu1TMOneStationTight&&Bmu2TMOneStationTight&&Bmu1InPixelLayer>0&&(Bmu1InPixelLayer+Bmu1InStripLayer)>5&&Bmu2InPixelLayer>0&&(Bmu2InPixelLayer+Bmu2InStripLayer)>5&&Bmu1dxyPV<0.3&&Bmu2dxyPV<0.3&&Bmu1dzPV<20&&Bmu2dzPV<20&&Bmu1isGlobalMuon&&Bmu2isGlobalMuon&&Bmu1TrgMatchFilterE>0&&Bmu2TrgMatchFilterE>0&&Btrk1highPurity&&Btrk2highPurity&&abs(Btrk1Eta)<2.4&&abs(Btrk2Eta)<2.4&&Btrk1Pt>0.&&Btrk2Pt>0.&&abs(Btktkmass-1.019455)<0.015"
CUTPP=${BASECUTPP}"&&((Bpt>7&&Bpt<15&&BDTStage1_pt7to15>0.191055)||(Bpt>15&&Bpt<50&&BDTStage1_pt15to50>0.208973)||(Bpt>55&&Bpt<100&&BDTStage1_pt15to50>0.10))"
CUTPP=${CUTPP}"&&abs(PVz)<15&&pBeamScrapingFilter&&pPAprimaryVertexFilter"
RECOONLYPP=$CUTPP
TRGPP="(HLT_HIL1DoubleMu0_v1)"
TRGPPMC="(HLT_HIL1DoubleMu0ForPPRef_v1)"
OUTPUTFILEPPSAVEHIST="ROOTfiles/hPtSpectrumSaveHistBplusPP.root"

OUTPUTFILEPP="ROOTfiles/hPtSpectrumBplusPP.root"
OUTPUTFILEPPSAVEHIST_ROOFIT="ROOTfiles/hPtSpectrumSaveHistBplusPP_roofit.root"
OUTPUTFILEPP_ROOFIT="ROOTfiles/hPtSpectrumBplusPP_roofit.root"
OUTPUTFILEMCSTUDYPP="ROOTfiles/MCstudiesPP.root"
OUTPUTFILEPlotPP="ROOTfiles/CrossSectionPP.root"
OUTPUTFILEPPDATA="ROOTfiles/data_pp.root"
OUTPUTFILEPPMC="ROOTfiles/mc_pp.root"

#SETTING for NP fit
NPFIT_PP="1"
#NPFIT_PP="1.238105*Gaus(x,5.067665,0.015902)/(sqrt(2*3.14159)*0.015902)+30.054860*TMath::Erf((x-5.142158)/-0.114983)+30.054860+10.468093*(0.390932*Gaus(x,5.375549,0.166548)/(sqrt(2*3.14159)*0.166548)+(1-0.390932)*Gaus(x,5.375549,0.040095)/(sqrt(2*3.14159)*0.040095))"
NPROOFIT_PP="1"
#NPROOFIT_PP="1.238105*TMath::Gaus(Bmass,5.067665,0.015902)/(sqrt(2*3.14159)*0.015902)+30.054860*TMath::Erf((Bmass-5.142158)/-0.114983)+30.054860+10.468093*(0.390932*TMath::Gaus(Bmass,5.375549,0.166548)/(sqrt(2*3.14159)*0.166548)+(1-0.390932)*TMath::Gaus(Bmass,5.375549,0.040095)/(sqrt(2*3.14159)*0.040095))"





if [ $DOANALYSISPP_FONLL -eq 1 ]; then      
g++ Bplusdsigmadpt.cc $(root-config --cflags --libs) -g -o Bplusdsigmadpt.exe 
./Bplusdsigmadpt.exe "$FONLLDATINPUT" "$FONLLOUTPUTFILE" "$LABELPP"
rm Bplusdsigmadpt.exe
fi 

if [ $DOANALYSISPP_FIT -eq 1 ]; then      
g++ fitBPP.C $(root-config --cflags --libs) -g -o fitB.exe 
#./fitB.exe 0 0 "$INPUTDATAPPCANDWISE" "$INPUTMCPPCANDWISE" "Bpt" "$TRGPP" "$CUTPP" "$SELGENPP" "$ISMCPP" 1 "$ISDOWEIGHTPP" "$LABELPP" "$OUTPUTFILEPPSAVEHIST" "plotFits/plotFits" "$NPFIT_PP" 0 "0" "100"
./fitB.exe 0 0 "$INPUTDATAPPCANDWISE" "$INPUTMCPPCANDWISE" "Bpt" "$TRGPP" "$CUTPP" "$SELGENPP" "$ISMCPP" 1 "$ISDOWEIGHTPP" "$LABELPP" "$OUTPUTFILEPP" "plotFits/plotFits" "$NPFIT_PP" 0 "0" "100"
rm fitB.exe
fi 

if [ $DOANALYSISPP_FITONSAVED -eq 1 ]; then      
g++ fitB.C $(root-config --cflags --libs) -g -o fitB.exe 
./fitB.exe 0 1 "$OUTPUTFILEPPSAVEHIST" "$OUTPUTFILEPPSAVEHIST" "Bpt" "$TRGPP" "$CUTPP" "$SELGENPP" "$ISMCPP" 1 "$ISDOWEIGHTPP" "$LABELPP" "$OUTPUTFILEPP" "plotFits/plotFitsOnSaved" "$NPFIT_PP" 0 "0" "100"
rm fitB.exe
fi 

if [ $DOANALYSISPP_ROOFIT -eq 1 ]; then      
root -b -q 'roofitB.C++('0','0','\"$INPUTDATAPPCANDWISE\"','\"$INPUTMCPPCANDWISE\"','\"Bpt\"','\"$TRGPP\"','\"$CUTPP\"','\"$SELGENPP\"','$ISMCPP','1','$ISDOWEIGHTPP','\"$LABELPP\"','\"$OUTPUTFILEPPSAVEHIST_ROOFIT\"','\"plotFits/plotFits_roofit\"','\"$NPROOFIT_PP\"','0','0','100')'
rm roofitB_C.d roofitB_C_ACLiC_dict_rdict.pcm roofitB_C.so
fi 

if [ $DOANALYSISPP_ROOFITONSAVED -eq 1 ]; then      
root -b -q 'roofitB.C++('0','1','\"$OUTPUTFILEPPSAVEHIST_ROOFIT\"','\"$OUTPUTFILEPPSAVEHIST_ROOFIT\"','\"Bpt\"','\"$TRGPP\"','\"$CUTPP\"','\"$SELGENPP\"','$ISMCPP','1','$ISDOWEIGHTPP','\"$LABELPP\"','\"$OUTPUTFILEPP_ROOFIT\"','\"plotFits/plotFitsOnSaved_roofit\"','\"$NPROOFIT_PP\"','0','0','100')'
rm roofitB_C.d roofitB_C_ACLiC_dict_rdict.pcm roofitB_C.so
fi 

if [ $DOANALYSISPP_MCSTUDY -eq 1 ]; then      
g++ MCefficiencyPP.C $(root-config --cflags --libs) -g -o MCefficiency.exe 
#./MCefficiency.exe 0 "$INPUTMCPP" "$SELGENPP" "$SELGENPPACCPP" "$RECOONLYPP" "$CUTPP&&$TRGPPMC" "Bpt" "Gpt" "$LABELPP" "$OUTPUTFILEMCSTUDYPP" "plotEff/plotEff" "$ISDOWEIGHTPP" "0" "100"
./MCefficiency.exe 0 "$INPUTMCPPCANDWISE" "$SELGENPP" "$SELGENPPACCPP" "$RECOONLYPP" "$CUTPP&&$TRGPPMC" "Bpt" "Gpt" "$LABELPP" "$OUTPUTFILEMCSTUDYPP" "plotEff/plotEff" "$ISDOWEIGHTPP" "0" "100"
rm MCefficiency.exe
fi

if [ $DOANALYSISPP_CROSS -eq 1 ]; then      
g++ CrossSectionRatio.C $(root-config --cflags --libs) -g -o CrossSectionRatio.exe 
./CrossSectionRatio.exe "$FONLLOUTPUTFILE" "$OUTPUTFILEPP" "$OUTPUTFILEMCSTUDYPP" "$OUTPUTFILEPlotPP" "plotCrossSection/plotCrossSection" 0 "$LABELPP" 0 0 "$LUMIPP" "0" "100" 
./CrossSectionRatio.exe "$FONLLOUTPUTFILE" "$OUTPUTFILEPP" "$OUTPUTFILEMCSTUDYPP" "$OUTPUTFILEPlotPP" "plotCrossSection/plotCrossSection" 0 "$LABELPP" 0 0 "$LUMIPP" "0" "100" 0 1
#./CrossSectionRatio.exe "$FONLLOUTPUTFILE" "$OUTPUTFILEPP_ROOFIT" "$OUTPUTFILEMCSTUDYPP" "$OUTPUTFILEPlotPP" "plotCrossSection/plotCrossSection" 0 "$LABELPP" 0 0 "$LUMIPP" "0" "100"
#./CrossSectionRatio.exe "$FONLLOUTPUTFILE" "$OUTPUTFILEPP_ROOFIT" "$OUTPUTFILEMCSTUDYPP" "$OUTPUTFILEPlotPP" "plotCrossSection/plotCrossSection" 0 "$LABELPP" 0 0 "$LUMIPP" "0" "100" 0 1
rm CrossSectionRatio.exe
fi

LABELPbPb="PbPb"
#LUMIPbPb=13.1983052423 #paper 20170227
LUMIPbPb=56.564165324
#NEW NMB from https://twiki.cern.ch/twiki/pub/CMS/HINUpsilonRaa2016/Jason_MinBiasCounting_2017-02-02.pdf
#2339717742. 2339717742*5.607/1e9 = 13.1187973794
#13.1187973794/0.99*0.996 = 13.1983052423
NMBEVT=2329685794.627413
ISMCPbPb=0
ISDOWEIGHTPbPb=1
SELGENPbPb="TMath::Abs(Gy)<2.4&&TMath::Abs(GpdgId)==531&&GisSignal>0"
SELGENPbPbACCPbPb="TMath::Abs(Gy)<2.4&&abs(GpdgId)==531&&GisSignal>0&&((TMath::Abs(Gmu1eta)<1.2&&Gmu1pt>3.5)||(TMath::Abs(Gmu1eta)>1.2&&TMath::Abs(Gmu1eta)<2.1&&Gmu1pt>(5.77-1.8*TMath::Abs(Gmu1eta)))||(TMath::Abs(Gmu1eta)>2.1&&TMath::Abs(Gmu1eta)<2.4&&Gmu1pt>1.8))&&((TMath::Abs(Gmu2eta)<1.2&&Gmu2pt>3.5)||(TMath::Abs(Gmu2eta)>1.2&&TMath::Abs(Gmu2eta)<2.1&&Gmu2pt>(5.77-1.8*TMath::Abs(Gmu2eta)))||(TMath::Abs(Gmu2eta)>2.1&&TMath::Abs(Gmu2eta)<2.4&&Gmu2pt>1.8))&&Gtk1pt>0.&&Gtk2pt>0.&&TMath::Abs(Gtk1eta)<2.4&&TMath::Abs(Gtk2eta)<2.4"
#BASECUTPbPb="TMath::Abs(By)<2.4&&TMath::Abs(Bmumumass-3.096916)<0.15&&((abs(Bmu1eta)<1.2&&Bmu1pt>3.5)||(abs(Bmu1eta)>1.2&&abs(Bmu1eta)<2.1&&Bmu1pt>(5.77-1.8*abs(Bmu1eta)))||(abs(Bmu1eta)>2.1&&abs(Bmu1eta)<2.4&&Bmu1pt>1.8))&&((abs(Bmu2eta)<1.2&&Bmu2pt>3.5)||(abs(Bmu2eta)>1.2&&abs(Bmu2eta)<2.1&&Bmu2pt>(5.77-1.8*abs(Bmu2eta)))||(abs(Bmu2eta)>2.1&&abs(Bmu2eta)<2.4&&Bmu2pt>1.8))&&Bmu1TMOneStationTight&&Bmu2TMOneStationTight&&Bmu1InPixelLayer>0&&(Bmu1InPixelLayer+Bmu1InStripLayer)>5&&Bmu2InPixelLayer>0&&(Bmu2InPixelLayer+Bmu2InStripLayer)>5&&Bmu1dxyPV<0.3&&Bmu2dxyPV<0.3&&Bmu1dzPV<20&&Bmu2dzPV<20&&Bmu1isGlobalMuon&&Bmu2isGlobalMuon&&Bmu1TrgMatchFilterE>0&&Bmu2TrgMatchFilterE>0&&Btrk1highPurity&&Btrk2highPurity&&abs(Btrk1Eta)<2.4&&abs(Btrk2Eta)<2.4&&Btrk1Pt>0.&&Btrk2Pt>0.&&abs(Btktkmass-1.019455)<0.015"

#BASECUTPbPb="(TMath::Abs(By)<2.4&&TMath::Abs(Bmumumass-3.096916)<0.15&&((abs(Bmu1eta)<1.2&&Bmu1pt>3.5)||(abs(Bmu1eta)>1.2&&abs(Bmu1eta)<2.1&&Bmu1pt>(5.77-1.8*abs(Bmu1eta)))||(abs(Bmu1eta)>2.1&&abs(Bmu1eta)<2.4&&Bmu1pt>1.8))&&((abs(Bmu2eta)<1.2&&Bmu2pt>3.5)||(abs(Bmu2eta)>1.2&&abs(Bmu2eta)<2.1&&Bmu2pt>(5.77-1.8*abs(Bmu2eta)))||(abs(Bmu2eta)>2.1&&abs(Bmu2eta)<2.4&&Bmu2pt>1.8)))&&Bmu1TMOneStationTight&&Bmu2TMOneStationTight &&Bmu1InPixelLayer>0 && (Bmu1InPixelLayer+Bmu1InStripLayer)>5 &&abs(Btrk1Eta)<2.4&&abs(Btrk2Eta)<2.4&&Btrk1Pt>0.&&Btrk2Pt>0.&&abs(Btktkmass-1.019455) < 0.015"

#BASECUTPbPb="TMath::Abs(By)<2.4&&TMath::Abs(Bmumumass-3.096916)<0.15&&((abs(Bmu1eta)<1.2&&Bmu1pt>3.5)||(abs(Bmu1eta)>1.2&&abs(Bmu1eta)<2.1&&Bmu1pt>(5.77-1.8*abs(Bmu1eta)))||(abs(Bmu1eta)>2.1&&abs(Bmu1eta)<2.4&&Bmu1pt>1.8))&&((abs(Bmu2eta)<1.2&&Bmu2pt>3.5)||(abs(Bmu2eta)>1.2&&abs(Bmu2eta)<2.1&&Bmu2pt>(5.77-1.8*abs(Bmu2eta)))||(abs(Bmu2eta)>2.1&&abs(Bmu2eta)<2.4&&Bmu2pt>1.8))&&Bmu1TMOneStationTight&&Bmu2TMOneStationTight&&Bmu1InPixelLayer>0&&(Bmu1InPixelLayer+Bmu1InStripLayer)>5&&Bmu2InPixelLayer>0&&(Bmu2InPixelLayer+Bmu2InStripLayer)>5&&Bmu1dxyPV<0.3&&Bmu2dxyPV<0.3&&Bmu1dzPV<20&&Bmu2dzPV<20&&Bmu1isGlobalMuon&&Bmu2isGlobalMuon&&Btrk1highPurity&&Btrk2highPurity&&abs(Btrk1Eta)<2.4&&abs(Btrk2Eta)<2.4&&Btrk1Pt>1.&&Btrk2Pt>1.&&abs(Btktkmass-1.019455)<0.015"

BASECUTPbPb="(hiBin < 181) && Btrk1Pt > 1.0 && Btrk2Pt > 1.0 && Bchi2cl > 0.05 && BsvpvDistance/BsvpvDisErr > 2.2 && Bpt > 5 && abs(Btrk1Eta-0.0) < 2.4  && abs(Btrk2Eta-0.0) < 2.4 && (TMath::Abs(By)<2.4&&TMath::Abs(Bmumumass-3.096916)<0.15&&((abs(Bmu1eta)<1.2&&Bmu1pt>3.5)||(abs(Bmu1eta)>1.2&&abs(Bmu1eta)<2.1&&Bmu1pt>(5.47-1.89*abs(Bmu1eta)))||(abs(Bmu1eta)>2.1&&abs(Bmu1eta)<2.4&&Bmu1pt>1.5))&&((abs(Bmu2eta)<1.2&&Bmu2pt>3.5)||(abs(Bmu2eta)>1.2&&abs(Bmu2eta)<2.1&&Bmu2pt>(5.47-1.89*abs(Bmu2eta)))||(abs(Bmu2eta)>2.1&&abs(Bmu2eta)<2.4&&Bmu2pt>1.5))&&Bmu1TMOneStationTight&&Bmu2TMOneStationTight&&Bmu1InPixelLayer>0&&(Bmu1InPixelLayer+Bmu1InStripLayer)>5&&Bmu2InPixelLayer>0&&(Bmu2InPixelLayer+Bmu2InStripLayer)>5&&Bmu1dxyPV<0.3&&Bmu2dxyPV<0.3&&Bmu1dzPV<20&&Bmu2dzPV<20&&Bmu1isTrackerMuon&&Bmu2isTrackerMuon&&Bmu1isGlobalMuon&&Bmu2isGlobalMuon&&Btrk1highPurity&&Btrk2highPurity&&abs(Btrk1Eta)<2.4&&abs(Btrk2Eta)<2.4&&Btrk1Pt>1.&&Btrk2Pt>1.&&abs(Btktkmass-1.019455)<0.015) && (abs(PVz)<15&&pclusterCompatibilityFilter&&pprimaryVertexFilter) && (Btrk1PixelHit + Btrk1StripHit > 10) && (Btrk2PixelHit + Btrk2StripHit > 10) && (Btrk1PtErr/Btrk1Pt < 0.1)&& (Btrk2PtErr/Btrk2Pt < 0.1) && Btrk1Chi2ndf/(Btrk1nStripLayer+Btrk1nPixelLayer) < 0.18 && Btrk2Chi2ndf/(Btrk2nStripLayer+Btrk2nPixelLayer) < 0.18"


#CUTPbPb=${BASECUTPbPb}"&&((Bpt>7&&Bpt<15&&BDTStage1_pt7to15>0.213755)||(Bpt>15&&Bpt<50&&BDTStage1_pt15to50>0.254413))"
#CUTPbPb=${CUTPbPb}"&&abs(PVz)<15&&pclusterCompatibilityFilter&&pprimaryVertexFilter&&phfCoincFilter3"


#CUTPbPb=${BASECUTPbPb}"&&((Bpt>7&&Bpt<15&&BDT>0.213755)||(Bpt>15&&Bpt<50&&BDT>0.254413))"
#CUTPbPb=${CUTPbPb}"&&abs(PVz)<15&&pclusterCompatibilityFilter&&pprimaryVertexFilter&&phfCoincFilter3"
#CUTPbPb=${CUTPbPb}"&&abs(PVz)<15&&pclusterCompatibilityFilter&&pprimaryVertexFilter"


#CUTPbPb=${BASECUTPbPb}"&&((Bpt>7&&Bpt<15&&BDT_pt_7_15>0.213755)||(Bpt>15&&Bpt<50&&BDT_pt_15_50>0.254413)||(Bpt>50&&Bpt<100&&BDT_pt_15_50>-0.4) )"

#CUTPbPb=${BASECUTPbPb}"&&((Bpt>7&&Bpt<15&&BDT_pt_7_15>0.57)||(Bpt>15&&Bpt<30&&BDT_pt_15_30> 0.59)||(Bpt>30&&Bpt<50&&BDT_pt_30_50>0.70) )"
CUTPbPb=${BASECUTPbPb}"&&((Bpt>5&&Bpt<10&&BDT_pt_5_10>0.10))"

CUTPbPb=${CUTPbPb}"&&abs(PVz)<15&&pclusterCompatibilityFilter&&pprimaryVertexFilter && phfCoincFilter2Th4"
CUTPbPbNP=${CUTPbPb}"&&abs(PVz)<15&&pclusterCompatibilityFilter&&pprimaryVertexFilter && Bgen != 23333"


echo "NEW CUT CUTPbPb = " $CUTPbPb


#CUTPbPbData=${BASECUTPbPb}"&&((Bpt>7&&Bpt<15&&BDT_pt_7_15>0.213755)||(Bpt>15&&Bpt<50&&BDT_pt_15_50>0.254413))"
#CUTPbPbData=${CUTPbPbData}"&&abs(PVz)<15&&pclusterCompatibilityFilter&&pprimaryVertexFilter"


RECOONLYPbPb=$CUTPbPb
#TRGPbPb="(HLT_HIL1DoubleMu0_v1||HLT_HIL1DoubleMu0_part1_v1||HLT_HIL1DoubleMu0_part2_v1||HLT_HIL1DoubleMu0_part3_v1)"
#TRGPbPbMC="(HLT_HIL1DoubleMu0_v1||HLT_HIL1DoubleMu0_part1_v1||HLT_HIL1DoubleMu0_part2_v1||HLT_HIL1DoubleMu0_part3_v1)"
TRGPbPb="(HLT_HIL3Mu0NHitQ10_L2Mu0_MAXdR3p5_M1to5_v1)"
TRGPbPbMC="(HLT_HIL3Mu0NHitQ10_L2Mu0_MAXdR3p5_M1to5_v1)"

OUTPUTFILEPbPbSAVEHIST="ROOTfiles/hPtSpectrumSaveHistBplusPbPb.root"
OUTPUTFILEPbPb="ROOTfiles/hPtSpectrumBplusPbPbBDTD.root"
OUTPUTFILEPbPbONY="ROOTfiles/hPtSpectrumBplusPbPb_Y.root"

OUTPUTFILEPbPbSAVEHIST_ROOFIT="ROOTfiles/hPtSpectrumSaveHistBplusPbPb_roofit.root"
OUTPUTFILEPbPb_ROOFIT="ROOTfiles/hPtSpectrumBplusPbPb_roofit.root"
OUTPUTFILEMCSTUDYPbPb="ROOTfiles/MCstudiesPbPb.root"
OUTPUTFILEMCSTUDYPbPbONY="ROOTfiles/MCstudiesPbPb_Y.root"


OUTPUTFILECLOSUREPbPb="ROOTfiles/hPtMCPbPbClosurePart1.root"
CLOSUREPPMB="ROOTfiles/hPtMCPbPbClosurePart2.root"


OUTPUTFILEPlotPbPb="ROOTfiles/CrossSectionPbPb.root"
OUTPUTFILEPbPbDATA="ROOTfiles/data_PbPb.root"
OUTPUTFILEPbPbMC="ROOTfiles/mc_PbPb.root"

OUTPUTFILEPbPbNP="BsNPSpectra.root"

#SETTING for NP fit
NPFIT_PbPb="1"
#NPFIT_PbPb="1.299998*Gaus(x,6.099828,-0.242801)/(sqrt(2*3.14159)*-0.242801)+8.186179*TMath::Erf((x-5.000000)/-0.205218)+8.186179+1.263652*(0.426611*Gaus(x,5.383307,0.249980)/(sqrt(2*3.14159)*0.249980)+(1-0.426611)*Gaus(x,5.383307,0.037233)/(sqrt(2*3.14159)*0.037233))" 
NPROOFIT_PbPb="1"
#NPROOFIT_PbPb="1.299998*TMath::Gaus(Bmass,6.099828,-0.242801)/(sqrt(2*3.14159)*-0.242801)+8.186179*TMath::Erf((Bmass-5.000000)/-0.205218)+8.186179+1.263652*(0.426611*TMath::Gaus(Bmass,5.383307,0.249980)/(sqrt(2*3.14159)*0.249980)+(1-0.426611)*TMath::Gaus(Bmass,5.383307,0.037233)/(sqrt(2*3.14159)*0.037233))" 


if [ $DOANALYSISPbPb_FIT -eq 1 ]; then      
g++ fitBBDTD.C $(root-config --cflags --libs) -g -o fitBBDTD.exe 
./fitBBDTD.exe 1 0 "$INPUTDATAPbPbCANDWISE" "$INPUTMCPbPbCANDWISE" "Bpt" "$TRGPbPb" "$CUTPbPb" "$SELGENPbPb" "$ISMCPbPb" 1 "$ISDOWEIGHTPbPb" "$LABELPbPb" "$OUTPUTFILEPbPb" "plotFits/plotFitsBDTD" "$NPFIT_PbPb" 0 "$CENTPbPbMIN" "$CENTPbPbMAX"
rm fitBBDTD.exe
fi 

