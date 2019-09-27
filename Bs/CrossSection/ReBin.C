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

void ReBin(){

	TFile * fin = new TFile("/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/2018Ana/BsRAA2015RunII/CrossSection/ROOTfiles/hPtSpectrumBplusPbPb_Y_30_90.root");
	
	TH1D * hPtBefore = (TH1D*)  fin->Get("hPt");


	const int nBins = 3;
	double yBins[nBins +1] = {0 , 0.5 ,1.0, 1.5}; 
	
	TH1D * hPt = new TH1D("hPt","",nBins,yBins);
	
	double Y;
	double YErr;

	for( int i = 0; i < nBins; i++){
		Y = hPtBefore->GetBinContent(i+1);
		YErr = hPtBefore->GetBinError(i+1);

		hPt->SetBinContent(i+1,Y);
		hPt->SetBinError(i+1,YErr);

	}


	TFile * fout = new TFile("/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/2018Ana/BsRAA2015RunII/Systematics/InputfileforreweightingCent3090/hPtSpectrumBplusPbPb_Y.root","RECREATE");
	fout->cd();
	hPt->Write();
	fout->Close();




	TFile * fin2 = new TFile("/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/2018Ana/BsRAA2015RunII/CrossSection/plotPtShapeSyst/Files/EffWeightedLLOG_30_90.root");
	TH1D * hPtMCBefore = (TH1D*)  fin->Get("hPtMC");
	TH1D * hPtMC = new TH1D("hPtMC","",nBins,yBins);



	for( int i = 0; i < nBins; i++){
		Y = hPtMCBefore->GetBinContent(i+1);
		YErr = hPtMCBefore->GetBinError(i+1);

		hPtMC->SetBinContent(i+1,Y);
		hPtMC->SetBinError(i+1,YErr);

	}




	TFile * fout2 = new TFile("/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/2018Ana/BsRAA2015RunII/Systematics/InputfileforreweightingCent3090/MCstudiesPbPb_Y.root","RECREATE");
	fout2->cd();
	hPtMC->Write();
	fout2->Close();



}
