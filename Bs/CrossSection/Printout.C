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
#include "parameters.h"

//#include "his.h"
using namespace std;

using std::cout;
using std::endl;

void Printout(){

	const int NTrials = 2;

//	TString RawYield[NTrials]={"New3Bins/yields_Bs_binned_cent_2nd_pt_3bins.root","JuliaDiff/yields_Bs_binned_cent_2nd_pt.root"};
//	TString Efficiency[NTrials]={"plotPtShapeSyst/Files/EffWeightedLLOG_30_90.root","plotPtShapeSyst/Files4bins/EffWeightedLLOG_30_90.root"};

//	TString RawYield[NTrials]={"New3Bins/yields_Bs_binned_pt_cent090.root","JuliaDiff/yields_Bs_binned_pt.root"};
//	TString Efficiency[NTrials]={"plotPtShapeSyst/Files/EffWeightedLLOG_0_90.root","plotPtShapeSyst/Files4bins/EffWeightedLLOG_0_90.root"};
		
	TString RawYield[NTrials]={"New3Bins/yields_Bs_binned_cent_1st_pt_3bins.root","JuliaDiff/yields_Bs_binned_cent_1st_pt.root"};
	TString Efficiency[NTrials]={"plotPtShapeSyst/Files/EffWeightedLLOG_0_30.root","plotPtShapeSyst/Files4bins/EffWeightedLLOG_0_30.root"};



	TFile * fYield;
	TFile * fEff;

	TString OutName[NTrials] ={"3bin.dat","4bin.dat"};




	for(int i = 0; i < NTrials; i++){

		fYield = new TFile(RawYield[i].Data());
		fEff = new TFile(Efficiency[i].Data());

		ofstream outratio(OutName[i].Data());


		outratio << "pT Bin Min    " << "   pT Bin Max   "  <<  "    Raw Yield    "  <<  "     Raw Yield Error" << endl; 

		fYield = new TFile(RawYield[i].Data());

		TH1D * hPt = (TH1D *) fYield->Get("hPt");

		for(int j = 0; j < hPt->GetNbinsX(); j++){

			outratio << hPt->GetBinLowEdge(j+1) <<  "     " << hPt->GetBinLowEdge(j+2) << "    " << hPt->GetBinContent(j+1) <<  "    " <<  hPt->GetBinError(j+1) << endl; 

		}

		TH1D * hPtMC = (TH1D *) fEff->Get("hPtMC");

		outratio << "  "  << endl;
		outratio << "pT Bin Min    " << "   pT Bin Max   "  <<  "    Reco MC Yield    "  <<  "    Reco MC Yield Error" << endl; 


		for(int j = 0; j < hPt->GetNbinsX(); j++){

			outratio << hPtMC->GetBinLowEdge(j+1) <<  "     " << hPtMC->GetBinLowEdge(j+2) << "    " << hPtMC->GetBinContent(j+1) <<  "    " <<  hPtMC->GetBinError(j+1) << endl; 

		}

		TH1D * hPtGen = (TH1D *) fEff->Get("hPtGen");

		outratio << "  "  << endl;
		outratio << "pT Bin Min    " << "   pT Bin Max   "  <<  "    Gen Level Yield    "  <<  "    Gen Level Yield Error" << endl; 


		for(int j = 0; j < hPt->GetNbinsX(); j++){

			outratio << hPtMC->GetBinLowEdge(j+1) <<  "     " << hPtGen->GetBinLowEdge(j+2) << "    " << hPtGen->GetBinContent(j+1) <<  "    " <<  hPtGen->GetBinError(j+1) << endl; 

		}


	
		TH1D * hEff = (TH1D *) fEff->Get("hEff");

		outratio << "  "  << endl;
		outratio << "pT Bin Min    " << "   pT Bin Max   "  <<  "   Efficiency    "  <<  "    Efficiency Error" << endl; 


		for(int j = 0; j < hPt->GetNbinsX(); j++){

			outratio << hEff->GetBinLowEdge(j+1) <<  "     " << hEff->GetBinLowEdge(j+2) << "    " << hEff->GetBinContent(j+1) <<  "    " <<  hEff->GetBinError(j+1) << endl; 

		}



		TH1D * hPtSigma = (TH1D *) hPt->Clone("hPtSigma");

		hPtSigma->Sumw2();
		hEff->Sumw2();
		hPtSigma->Divide(hEff);

		outratio << "  "  << endl;
		outratio << "pT Bin Min    " << "   pT Bin Max   "  <<  "   Corrected Yield    "  <<  "    Corrected Yield Error" << endl; 
	

		for(int j = 0; j < hPt->GetNbinsX(); j++){

			outratio << hPtSigma->GetBinLowEdge(j+1) <<  "     " << hPtSigma->GetBinLowEdge(j+2) << "    " << hPtSigma->GetBinContent(j+1) <<  "    " <<  hPtSigma->GetBinError(j+1) << endl; 

		}



		outratio << "   Total Corrected Yield =   " << hPtSigma->Integral("Width") << endl;



	}


	cout << "DONE BRO" << endl;
}
