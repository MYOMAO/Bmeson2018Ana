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
#include "Rebin.h"



//#include "his.h"
using namespace std;

using std::cout;
using std::endl;

void ReAnaEffSyst(int CentMin, int CentMax, int Weight ){

	int TnP = 1;

	gStyle->SetOptTitle(0);
	gStyle->SetOptStat(0);


	TString WeightName;
	/*

	if(Weight == 16) WeightName = "NoWeight";
	if(Weight == 1) WeightName = "FONLL";
	if(Weight == 11) WeightName = "Linear";
	if(Weight == 12) WeightName = "Quadratic";
	if(Weight == 13) WeightName = "LInverse";
	if(Weight == 14) WeightName = "LSqrt";
	if(Weight == 15) WeightName = "LLog";
	*/

	if(Rebin == 0){
		if(Weight == 16) WeightName = "NoWeight";
		if(Weight == 1) WeightName = "FONLL";
		if(Weight == 11) WeightName = "Linear";
		if(Weight == 12) WeightName = "Quadratic";
		if(Weight == 13) WeightName = "LInverse";
		if(Weight == 14) WeightName = "LSqrt";
		if(Weight == 15) WeightName = "LLog";
	}

	if(Rebin == 1){
		if(Weight == 16) WeightName = "Rebin/NoWeight";
		if(Weight == 1) WeightName = "Rebin/FONLL";
		if(Weight == 11) WeightName = "Rebin/Linear";
		if(Weight == 12) WeightName = "Rebin/Quadratic";
		if(Weight == 13) WeightName = "Rebin/LInverse";
		if(Weight == 14) WeightName = "Rebin/LSqrt";
		if(Weight == 15) WeightName = "Rebin/LLog";

	}


	TString FileName;
	/*
	if(TnP == 0) FileName = Form("%dBptBins/EffInfo_%d_%d.root",NBptBins,CentMin,CentMax);
	if(TnP == 1) FileName = Form("%dBptBinsTnP/EffInfo_%d_%d.root",NBptBins,CentMin,CentMax);
	if(TnP == 2) FileName = Form("NonFiducial/EffInfo_%d_%d.root",CentMin,CentMax);
	*/

	FileName = Form("CheckSystNuno/%s/EffInfo_%d_%d.root",WeightName.Data(),CentMin,CentMax);

	TFile * fin = new TFile(FileName.Data());
	fin->cd();

	TTree * EffInfoTree = (TTree * ) fin->Get("EffInfoTree");

	int NEvents = EffInfoTree->GetEntries();

	const int NCand = 10;

	Int_t BsizeNew;
	Int_t runNew;
	Int_t lumiNew;
	Int_t evtNew;
	Float_t BmassNew[NCand];
	Float_t BptNew[NCand];
	Float_t ByNew[NCand];



	EffInfoTree->SetBranchAddress("BsizeNew",&BsizeNew);
	EffInfoTree->SetBranchAddress("BmassNew",BmassNew);
	EffInfoTree->SetBranchAddress("ByNew",ByNew);
	EffInfoTree->SetBranchAddress("BptNew",BptNew);



	TFile * finSystMap = new TFile(Form("10kTH2D/GenStatSyst_%d_%d.root",CentMin,CentMax));
	//TFile * finSystMap = new TFile(	Form("CheckSystNuno/%s/EffFine_%d_%d.root",WeightName.Data(),CentMin,CentMax));

	finSystMap->cd();

	TH2D * EffInv2DTrial;
	TH2D * SelInv2DTrial;
	TH2D * AccInv2DTrial;

	const int NTrials = 10000;

//	const int NTrials = 1;

	TString outfileSyst;

	if(Rebin == 0 ) outfileSyst = Form("NunoSyst/%d-%d/AllTrials.root",CentMin,CentMax);
	if(Rebin == 1 ) outfileSyst = Form("NunoSyst/Rebin/%d-%d/AllTrials.root",CentMin,CentMax);


	TFile * finSyst = new TFile(outfileSyst.Data(),"RECREATE");
	finSyst->cd();
	TTree * NunoSyst = new TTree("NunoSyst","NunoSyst");


	Int_t BsizeNewSyst;
	Float_t BmassNewSyst[NCand];
	Float_t BptNewSyst[NCand];
	Float_t ByNewSyst[NCand];
	Float_t BEffInvSyst[NCand];
	Float_t BSelInvSyst[NCand];
	Float_t BAccInvSyst[NCand];


	NunoSyst->Branch("BsizeNewSyst",&BsizeNewSyst,"BsizeNewSyst/I");
	NunoSyst->Branch("BmassNewSyst",BmassNewSyst,"BmassNewSyst/F");
	NunoSyst->Branch("BptNewSyst",BptNewSyst,"BptNewSyst/F");
	NunoSyst->Branch("ByNewSyst",ByNewSyst,"ByNewSyst/F");
	NunoSyst->Branch("BEffInvSyst",BEffInvSyst,"BEffInvSyst/F");
	NunoSyst->Branch("BSelInvSyst",BSelInvSyst,"BSelInvSyst/F");
	NunoSyst->Branch("BAccInvSyst",BAccInvSyst,"BAccInvSyst/F");



	int XLOC;
	int YLOC;

	for(int iTrial = 0; iTrial < NTrials;  iTrial++){

		cout << "Now Working on Trial = " << iTrial << " OUT OF " << NTrials << endl;

		EffInv2DTrial = (TH2D * ) finSystMap->Get(Form("EffBptByInvTrial%d",iTrial));
		//EffInv2DTrial = (TH2D * ) finSystMap->Get("EffBptByInv");
		SelInv2DTrial = (TH2D * ) finSystMap->Get(Form("SelBptByInvTrial%d",iTrial));
		AccInv2DTrial = (TH2D * ) finSystMap->Get(Form("AccBptByInvTrial%d",iTrial));



		for( int i = 0; i < NEvents; i++){

			EffInfoTree->GetEntry(i);
			BsizeNewSyst = BsizeNew;

			for(int j = 0; j < BsizeNewSyst; j++){

				BmassNewSyst[j] = BmassNew[j];
				BptNewSyst[j] = BptNew[j];
				ByNewSyst[j] = ByNew[j];
				
				XLOC = EffInv2DTrial->GetXaxis()->FindBin(BptNewSyst[j]);
				YLOC = EffInv2DTrial->GetYaxis()->FindBin(ByNewSyst[j]);

				BEffInvSyst[j] = EffInv2DTrial->GetBinContent(XLOC,YLOC);
				BSelInvSyst[j] = SelInv2DTrial->GetBinContent(XLOC,YLOC);
				BAccInvSyst[j] = AccInv2DTrial->GetBinContent(XLOC,YLOC);


			}


			NunoSyst->Fill();
		}

	}

	finSyst->Write();
	finSyst->Close();

}
