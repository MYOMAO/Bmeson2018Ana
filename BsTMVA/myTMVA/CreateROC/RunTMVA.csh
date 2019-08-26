set low=1
set high=2



root -b -l -q /afs/cern.ch/user/s/szhaozho/Analysis/CMSSW_7_5_8_patch3/LowPtAnalysis/newTMVA/tmvaBntupleRunII/myTMVA/TMVAClassification.C++'('0,$low,$high')'

source /afs/cern.ch/user/s/szhaozho/Analysis/CMSSW_7_5_8_patch3/LowPtAnalysis/newTMVA/tmvaBntupleRunII/mva/macros/BDT2.sh $low $high

root -b -l -q /afs/cern.ch/user/s/szhaozho/Analysis/CMSSW_7_5_8_patch3/LowPtAnalysis/newTMVA/tmvaBntupleRunII/mva/macros/BDT.C++'('"/data/wangj/MC2016/ntD_EvtBase_20170612_DfinderMC_20170503_pp_Pythia8_prompt_D0pt0p0_Pthat0-5-10_pp502_TuneCUETP8M1_AOD_dPt0tkPt0p2_D0_BDT_pp_0p0_0p5_trkpt0p2.root",$low,$high')'

source /afs/cern.ch/work/s/szhaozho/DSignal/Bfinder/Bfinder/Dntuple/DntupleMerger/doDntupleMerger.sh Data_${low}_${high}.root
hadd /data/zhaozhong/newMinBias/1-2/MC_${low}_${high}.root /data/wangj/MC2016/ntD_EvtBase_20170612_DfinderMC_20170503_pp_Pythia8_prompt_D0pt0p0_Pthat0-5-10_pp502_TuneCUETP8M1_AOD_dPt0tkPt0p2_D0_BDT_pp_0p0_0p5_trkpt0p2.root  /data/zhaozhong/MVAFiles/BDT_pp_${low}_${high}_MC.root

root -b -l -q /afs/cern.ch/user/s/szhaozho/Analysis/CMSSW_7_5_8_patch3/LowPtAnalysis/newTMVA/tmvaBntupleRunII/readxml/readxml_BDT.cc++'('0,$low,$high,1')'







