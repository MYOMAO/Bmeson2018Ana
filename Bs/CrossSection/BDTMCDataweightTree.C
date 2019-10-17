#include "TROOT.h"
#include "TH1.h"
#include "TTree.h"
#include "TH2.h"
#include "TF1.h"
#include "TFile.h"
#include "TMath.h"
#include "TSystem.h"
#include "TVector2.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "TRandom.h"
#include <iostream>
#include <fstream>




//#include "his.h"
using namespace std;

using std::cout;
using std::endl;

void BDTMCDataweightTree(int NBins){

	TString infile = "/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/2018Ana/Samples/FinalAnaSamples/PrivateMC-Data-Official-SemiFinal/MC_Bs_PbPb_TMVA_BDT_PbPb.root";
//	TString BDTWeightFile = "/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/2018Ana/BsRAA2015RunII/FromGJ/weights.root";
	TString BDTWeightFile = "/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/2018Ana/BsRAA2015RunII/FromGJ/weights_5bins.root";
	TString outfile = Form("BDTMCDataWeightTree%dBins.root",NBins);

	TFile * fin = new TFile(infile.Data());
	fin->cd();

	TTree * ntphi = (TTree * ) fin->Get("Bfinder/ntphi");
	TTree * BDT1 = (TTree * ) fin->Get("BDT_pt_5_10");
	TTree * BDT2 = (TTree * ) fin->Get("BDT_pt_10_15");
	TTree * BDT3 = (TTree * ) fin->Get("BDT_pt_15_20");
	TTree * BDT4 = (TTree * ) fin->Get("BDT_pt_20_50");
	TTree * ntHi = (TTree * ) fin->Get("hiEvtAnalyzer/HiTree");
	TTree * ntSkim = (TTree * ) fin->Get("skimanalysis/HltTree");
	TTree * ntHlt = (TTree *) fin->Get("hltanalysis/HltTree");

	const int NCand = 20000;


	int run;
	int lumi;
	int evt;
	int hiBin;
	Int_t   Bsize;
	Double_t BDT_pt_5_10[NCand];
	Double_t BDT_pt_10_15[NCand];
	Double_t BDT_pt_15_20[NCand];
	Double_t BDT_pt_20_50[NCand];

	Float_t Bpt[NCand];

	Float_t BptCheck[NCand];


	Float_t BEffInvBDTWeighted[NCand];
	Float_t BEffInvErrBDTWeighted[NCand];



	
	ntHi->SetBranchAddress("hiBin",&hiBin);
	BDT1->SetBranchAddress("run",&run);
	BDT1->SetBranchAddress("evt",&evt);
	BDT1->SetBranchAddress("lumi",&lumi);

	BDT1->SetBranchAddress("BDT_pt_5_10",BDT_pt_5_10);
	BDT2->SetBranchAddress("BDT_pt_10_15",BDT_pt_10_15);
	BDT3->SetBranchAddress("BDT_pt_15_20",BDT_pt_15_20);
	BDT4->SetBranchAddress("BDT_pt_20_50",BDT_pt_20_50);

	ntphi->SetBranchAddress("Bpt",Bpt);
	ntphi->SetBranchAddress("Bsize",&Bsize);


	TFile * finSystWeight =  new TFile(BDTWeightFile.Data());

	finSystWeight->cd();

	TH1D * weights_BDT_pt_5_10 = (TH1D *) finSystWeight->Get("weights_BDT_pt_5_10");
	TH1D * weights_BDT_pt_10_15 = (TH1D *) finSystWeight->Get("weights_BDT_pt_10_15");
	TH1D * weights_BDT_pt_15_20 = (TH1D *) finSystWeight->Get("weights_BDT_pt_15_20");
	TH1D * weights_BDT_pt_20_50 = (TH1D *) finSystWeight->Get("weights_BDT_pt_20_50");



	TFile * fout = new TFile(outfile.Data(),"RECREATE");


	fout->cd();
	TTree* BDTMCDataWeightTree = new TTree("BDTMCDataWeightTree","BDTMCDataWeightTree");	


	Int_t BsizeSyst;
	Int_t runSyst;
	Int_t lumiSyst;
	Int_t evtSyst;
	Int_t hiBinSyst;

	BDTMCDataWeightTree->Branch("BsizeSyst",&BsizeSyst,"BsizeSyst/I");
	BDTMCDataWeightTree->Branch("runSyst",&runSyst,"runSyst/I");
	BDTMCDataWeightTree->Branch("evtSyst",&evtSyst,"evtSyst/I");
	BDTMCDataWeightTree->Branch("lumiSyst",&lumiSyst,"lumiSyst/I");
	BDTMCDataWeightTree->Branch("hiBinSyst",&hiBinSyst,"hiBinSyst/I");
	BDTMCDataWeightTree->Branch("BptCheck",BptCheck,"BptCheck/F");


	BDTMCDataWeightTree->Branch("BEffInvBDTWeighted",BEffInvBDTWeighted,"BEffInvBDTWeighted/F");
	BDTMCDataWeightTree->Branch("BEffInvErrBDTWeighted",BEffInvErrBDTWeighted,"BEffInvErrBDTWeighted/F");

	int NEvents = ntphi->GetEntries();
	
	int LocWeightX;
	double BDTSystemWeight;
	double BDTSystemWeightErr;

	for(int i = 0; i < NEvents; i++){
		ntphi->GetEntry(i);
		ntSkim->GetEntry(i);
		ntHi->GetEntry(i);
		BDT1->GetEntry(i);
		BDT2->GetEntry(i);
		BDT3->GetEntry(i);
		BDT4->GetEntry(i);

		

		runSyst = run;
		lumiSyst = lumi;
		evtSyst = evt;
		hiBinSyst = hiBin;
		BsizeSyst = Bsize;


		if(i%100000==0) std::cout<<std::setiosflags(std::ios::left)<<"  [ \033[1;36m"<<std::setw(10)<<i<<"\033[0m"<<" / "<<std::setw(10)<<ntphi->GetEntries()<<" ] "<<"\033[1;36m"<<Form("%.0f",100.*i/ntphi->GetEntries())<<"%\033[0m"<<"\r"<<std::flush;

		for(int j =0; j < Bsize; j++){
				
				BptCheck[j] = Bpt[j];
	
				if(BptCheck[j] > 5 && BptCheck[j] < 10){
					
					

					LocWeightX = weights_BDT_pt_5_10->GetXaxis()->FindBin(BDT_pt_5_10[j]);
					BDTSystemWeight = weights_BDT_pt_5_10->GetBinContent(LocWeightX);
					BDTSystemWeightErr  = weights_BDT_pt_5_10->GetBinError(LocWeightX);

					BEffInvBDTWeighted[j] =   BDTSystemWeight; 
					BEffInvErrBDTWeighted[j] = BDTSystemWeightErr;

				}

				if(BptCheck[j] > 10 && BptCheck[j] < 15){

					LocWeightX = weights_BDT_pt_10_15->GetXaxis()->FindBin(BDT_pt_10_15[j]);
					BDTSystemWeight = weights_BDT_pt_10_15->GetBinContent(LocWeightX);
					BDTSystemWeightErr  = weights_BDT_pt_10_15->GetBinError(LocWeightX);
	
					BEffInvBDTWeighted[j] =    BDTSystemWeight; 
					BEffInvErrBDTWeighted[j] =  BDTSystemWeightErr;

				}

				if(BptCheck[j] > 15 && BptCheck[j] < 20){

					LocWeightX = weights_BDT_pt_15_20->GetXaxis()->FindBin(BDT_pt_15_20[j]);
					BDTSystemWeight = weights_BDT_pt_15_20->GetBinContent(LocWeightX);
					BDTSystemWeightErr  = weights_BDT_pt_15_20->GetBinError(LocWeightX);


					BEffInvBDTWeighted[j] =  BDTSystemWeight; 
					BEffInvErrBDTWeighted[j] =   BDTSystemWeightErr;

				}

				if(BptCheck[j] > 20 && BptCheck[j] < 50){

					LocWeightX = weights_BDT_pt_20_50->GetXaxis()->FindBin(BDT_pt_20_50[j]);
					BDTSystemWeight = weights_BDT_pt_20_50->GetBinContent(LocWeightX);
					BDTSystemWeightErr  = weights_BDT_pt_20_50->GetBinError(LocWeightX);


					BEffInvBDTWeighted[j] = BDTSystemWeight; 
					BEffInvErrBDTWeighted[j] =   BDTSystemWeightErr;

				}
	
				if(BptCheck[j] < 5 || BptCheck[j] > 50){


					BEffInvBDTWeighted[j] = 0; 
					BEffInvErrBDTWeighted[j] =  0;


				}
		
		
		}

		BDTMCDataWeightTree->Fill();
	}

	fout->Write();
	fout->Close();


}
