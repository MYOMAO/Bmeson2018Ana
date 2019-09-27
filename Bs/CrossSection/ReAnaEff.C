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

void ReAnaEff(int CentMin, int CentMax,	const int NBins,	int DoTwoD ){


	gStyle->SetOptTitle(0);
	gStyle->SetOptStat(0);


	TString FileName = Form("EffInfo_%d_%d.root",CentMin,CentMax);

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
	Float_t BEff[NCand];
	Float_t BEffInv[NCand];
	Float_t BEffInvErr[NCand];
	Float_t BEffInv1D[NCand];
	Float_t BEffInvErr1D[NCand];


	EffInfoTree->SetBranchAddress("BsizeNew",&BsizeNew);
	EffInfoTree->SetBranchAddress("BmassNew",BmassNew);
	EffInfoTree->SetBranchAddress("ByNew",ByNew);
	EffInfoTree->SetBranchAddress("BptNew",BptNew);
	EffInfoTree->SetBranchAddress("BEffInv",BEffInv);
	EffInfoTree->SetBranchAddress("BEffInvErr",BEffInvErr);
	EffInfoTree->SetBranchAddress("BEff",BEff);
	EffInfoTree->SetBranchAddress("BEffInv1D",BEffInv1D);
	EffInfoTree->SetBranchAddress("BEffInvErr1D",BEffInvErr1D);




	double ptBins[NBins + 1];

	int Counts[NBins];
	double SumCounts[NBins];
	double SumCountsErr[NBins];
	double NewEff[NBins];
	double NewEffErr[NBins];




	std::vector<int> ptbinsvec;
	
	if(NBins == 1){

		ptbinsvec.push_back(5);
		ptbinsvec.push_back(50);

	}

		
	if(NBins == 3){

		ptbinsvec.push_back(5);
		ptbinsvec.push_back(15);
		ptbinsvec.push_back(20);
		ptbinsvec.push_back(50);

	}

	
	if(NBins == 4){

		ptbinsvec.push_back(5);
		ptbinsvec.push_back(10);
		ptbinsvec.push_back(15);
		ptbinsvec.push_back(20);
		ptbinsvec.push_back(50);

	}



	for(int i = 0; i < NBins + 1; i++){
		ptBins[i] =  ptbinsvec[i];
	}
	
	for(int i = 0; i < NBins; i++){
		Counts[i] = 0;
		SumCounts[i] = 0;
		SumCountsErr[i] = 0;
	}


	
	
	/*
	   const int NBins = 3;
	   double ptBins[NBins + 1] ={5,15,20,50};

	   int Counts[NBins]={0,0,0};
	   double SumCounts[NBins]={0,0,0};
	   double SumCountsErr[NBins]={0,0,0};
	   */


	/*
	   const int NBins = 4;
	   double ptBins[NBins + 1] ={5,10,15,20,50};

	   int Counts[NBins]={0,0,0,0};
	   double SumCounts[NBins]={0,0,0,0};
	   double SumCountsErr[NBins]={0,0,0,0};
	   */



	for( int i = 0; i < NEvents; i++){

		EffInfoTree->GetEntry(i);

		for(int j = 0; j < BsizeNew; j++){


			for(int k = 0; k < NBins; k++){
				if(BptNew[j] > ptBins[k] && BptNew[j] < ptBins[k+1] && TMath::Abs(BmassNew[j] - 5.36682) < 0.08)
				{
					//	SumCountsReal[k] = SumCountsReal[k] + BEff[j];
					//cout << "BEffInv[j] = " << BEffInv[j] << endl; 
					if(DoTwoD == 1){
						if(BEffInv[j] > 0){
							SumCounts[k] = SumCounts[k] + BEffInv[j];
							SumCountsErr[k] = SumCounts[k] + BEffInvErr[j] * BEffInvErr[j];
							Counts[k] = Counts[k] + 1;
						}
					}

					if(DoTwoD == 0){
						if(BEffInv1D[j] > 0){
							SumCounts[k] = SumCounts[k] + BEffInv1D[j];
							SumCountsErr[k] = SumCounts[k] + BEffInvErr1D[j] * BEffInvErr1D[j];
							Counts[k] = Counts[k] + 1;
						}
					}


				}

			}


		}

	}


	TH1D * hEff = new TH1D("hEff","",NBins,ptBins);


	hEff->GetXaxis()->SetTitle("B_{s} p_{T} (GeV/c)");
	hEff->GetYaxis()->SetTitle("<1/(Eff * Acc)>");
	hEff->GetYaxis()->SetTitleOffset(1.4);
	hEff->GetXaxis()->CenterTitle();
	hEff->GetYaxis()->CenterTitle();
	hEff->SetMarkerColor(1);
	hEff->SetLineColor(1);
	hEff->SetMarkerStyle(20);

	hEff->SetMinimum(0);



	for(int i = 0; i < NBins; i++){



		NewEff[i] = SumCounts[i]/Counts[i];
		NewEffErr[i] = TMath::Sqrt(SumCountsErr[i])/Counts[i];
		hEff->SetBinContent(i+1,NewEff[i]);
		hEff->SetBinError(i+1,NewEffErr[i]);
		//	cout << "Real eff = " << SumCountsReal[i]/Counts[i] << endl;
		//cout << "Counts = " << Counts[i] << endl;
		cout << "Count =  " <<  Counts[i] << "   NewEff = " << NewEff[i] << "     NewEffErr = " << NewEffErr[i] << endl;

	}

	hEff->SetMaximum(NewEff[0]*1.2);
	TCanvas *c = new TCanvas("c","c",600,600);

	c->cd();

	hEff->Draw("ep");

	c->SaveAs(Form("ReAnaEff_%d_%d.png",CentMin,CentMax));

	TFile * fout = new TFile(Form("ReAnaEff_%d_%d.root",CentMin,CentMax),"RECREATE");
	fout->cd();
	hEff->Write();
	fout->Close();


	TFile * RawYield;


	if(CentMin ==0 && CentMax==90){

	if(NBins == 1)	RawYield = new TFile(Form("RawYields/%d-%d/yields_Bs_full.root",CentMin,CentMax));
	if(NBins == 3)	RawYield = new TFile(Form("RawYields/%d-%d/yields_Bs_binned_pt_cent090.root",CentMin,CentMax));
	if(NBins == 4)	RawYield = new TFile(Form("RawYields/%d-%d/yields_Bs_binned_pt.root",CentMin,CentMax));

	}


	if(CentMin ==0 && CentMax==30){

	if(NBins == 1)	RawYield = new TFile(Form("RawYields/%d-%d/yields_Bs_full.root",CentMin,CentMax));
	if(NBins == 3)	RawYield = new TFile(Form("RawYields/%d-%d/yields_Bs_binned_cent_1st_pt_3bins.root",CentMin,CentMax));
	if(NBins == 4)	RawYield = new TFile(Form("RawYields/%d-%d/yields_Bs_binned_cent_1st_pt.root",CentMin,CentMax));

	}

	if(CentMin ==30 && CentMax==90){

	if(NBins == 1)	RawYield = new TFile(Form("RawYields/%d-%d/yields_Bs_full.root",CentMin,CentMax));
	if(NBins == 3)	RawYield = new TFile(Form("RawYields/%d-%d/yields_Bs_binned_cent_2nd_pt_3bins.root",CentMin,CentMax));
	if(NBins == 4)	RawYield = new TFile(Form("RawYields/%d-%d/yields_Bs_binned_cent_2nd_pt.root",CentMin,CentMax));

	}




	RawYield->cd();

	TH1D * hPt = (TH1D *) RawYield->Get("hPt");

	double RawCount;
	double RawCountErr;

	double CorrYield= 0;
	double CorrYieldErr= 0;


	for(int i = 0; i < hPt->GetNbinsX();i++){

		RawCount = hPt->GetBinContent(i+1);
		RawCountErr = hPt->GetBinError(i+1);

		CorrYield = RawCount * (ptBins[i+1] - ptBins[i]) *  NewEff[i] + CorrYield;
		CorrYieldErr = (RawCountErr * (ptBins[i+1] - ptBins[i]) *  NewEff[i]) *(RawCountErr * (ptBins[i+1] - ptBins[i]) *  NewEff[i]) + (RawCountErr * (ptBins[i+1] - ptBins[i]) *  NewEffErr[i]) * (RawCount * (ptBins[i+1] - ptBins[i]) *  NewEffErr[i]) + CorrYieldErr;

	}

	CorrYieldErr = TMath::Sqrt(CorrYieldErr);

	cout << "CorrYield Final = " << CorrYield << "   CorrYield Error = " << CorrYieldErr  << endl;


}
