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

void QuickMake(){

	TFile * fin = new TFile("yields_Bs_binned_cent.root");
	
	TH1D * hPtOld = (TH1D *) fin->Get("hPt");
	
	TH1D * hPt1 = new TH1D("hPt","hPt030",1,5,50);
	TH1D * hPt2 = new TH1D("hPt","hPt3090",1,5,50);

	double Value[2];
	double ValueErr[2];

	for(int i = 0; i < hPtOld->GetNbinsX(); i++){
		
		Value[i] = hPtOld->GetBinContent(i+1);
		ValueErr[i] = hPtOld->GetBinError(i+1);
	}


	hPt1->SetBinContent(1,Value[0]);
	hPt1->SetBinError(1,ValueErr[0]);

	hPt2->SetBinContent(1,Value[1]);
	hPt2->SetBinError(1,ValueErr[1]);



	TFile * fout1 = new TFile("RawYields/0-30/yields_Bs_full.root","RECREATE");
	fout1->cd();

	hPt1->Write();
	fout1->Close();
	

	TFile * fout2 = new TFile("RawYields/30-90/yields_Bs_full.root","RECREATE");

	fout2->cd();

	hPt2->Write();
	fout2->Close();



}
