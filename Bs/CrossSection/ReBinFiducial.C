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

void ReBinFiducial(){

	TFile * fin = new TFile("../CrossSectionCentAll/FromJulia/yields_Bs_binned_pt.root");
	fin->cd();


	TH1D * hPtin = (TH1D * ) fin->Get("hPt");


	const int Nptbins = 4;
	double ptrebins[Nptbins + 1] = {6,10,15,20,50};

	double Value;
	double ValueErr;

	double Correction[Nptbins] = {5.0/4.0,1.0,1.0,1.0};

	TH1D * hPt = new TH1D("hPt","",Nptbins,ptrebins);

	for(int i = 0; i < 4; i++){
		
		Value = hPtin->GetBinContent(i+1);
		ValueErr = hPtin->GetBinError(i+1);

		hPt->SetBinContent(i+1,Value * Correction[i]);
		hPt->SetBinError(i+1,ValueErr * Correction[i]);

	}

	TFile * fout = new TFile("/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/2018Ana/BsRAA2015RunII/CrossSection/ROOTfiles/hPtSpectrumBplusPbPb.root ","RECREATE");
	fout->cd();
	hPt->Write();
	fout->Close();	
}


