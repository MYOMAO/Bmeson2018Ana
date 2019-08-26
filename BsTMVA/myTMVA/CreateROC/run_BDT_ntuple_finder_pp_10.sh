cd /afs/cern.ch/user/s/szhaozho/Analysis/CMSSW_7_5_8_patch3/LowPtAnalysis/newTMVA/tmvaBntupleRunII/myTMVA/
export X509_USER_PROXY=~/x509_user_proxy
eval `scram runtime -sh`
cd -
/afs/cern.ch/user/s/szhaozho/Analysis/CMSSW_7_5_8_patch3/LowPtAnalysis/newTMVA/tmvaBntupleRunII/myTMVA/BDT.exe root://eoscms//eos/cms//store/group/phys_heavyions/wangj/tmvaBDTfiles/ntD_EvtBase_20170619_MinimumBias4_DfinderData_pp_20170618_D0_zz_BDT_pp_0p0_0p5_trkpt0p2//BDT_ntuple_finder_pp_10.root root://eoscms//eos/cms//store/group/phys_heavyions/zshi/BDTFiles/Minbiased4dedx/BDT_BDT_ntuple_finder_pp_10.root 3 4
hadd root://eoscms//eos/cms//store/group/phys_heavyions/zshi/BDTFiles/Minbiased4dedx/MergeBDT/BDTCUT_BDT_ntuple_finder_pp_10.root root://eoscms//eos/cms//store/group/phys_heavyions/zshi/BDTFiles/Minbiased4dedx/BDT_BDT_ntuple_finder_pp_10.root root://eoscms//eos/cms//store/group/phys_heavyions/wangj/tmvaBDTfiles/ntD_EvtBase_20170619_MinimumBias4_DfinderData_pp_20170618_D0_zz_BDT_pp_0p0_0p5_trkpt0p2//BDT_ntuple_finder_pp_10.root
rm root://eoscms//eos/cms//store/group/phys_heavyions/zshi/BDTFiles/Minbiased4dedx/BDT_BDT_ntuple_finder_pp_10.root
