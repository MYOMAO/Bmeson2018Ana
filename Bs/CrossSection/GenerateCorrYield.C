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
#include "parametersDiff.h"

//#include "his.h"
using namespace std;

using std::cout;
using std::endl;

void GenerateCorrYield(int BinOption){
	cout << "Pass 0" << endl;

	Double_t BRchain=3.118974e-5; 

	TString infileYield;
	TString infileEff;

	TFile * finY;

	TFile * finE;
	TFile * fout;

	int NCentBin1[3] = {0,30,90};
	int NCentBin2[2] = {0,90};

	int hiBin1[3] = {0,30*2,90*2};
	int hiBin2[2] = {0,90*2};

	int NCentBin3[2] = {0,30};
	int hiBin3[2] = {0,30*2};

	std::vector<int> CentBins;
	std::vector<int> hiBins;
	int NCentBins;


	if(BinOption == 0 ){
		NCentBins = 2;
		for(int i = 0; i< NCentBins+1; i++){
			CentBins.push_back(NCentBin1[i]);
			hiBins.push_back(hiBin1[i]);
		}
	}

	if(BinOption == 1 ){
		NCentBins = 1;
		for(int i = 0; i< NCentBins+1; i++){
			CentBins.push_back(NCentBin2[i]);
			hiBins.push_back(hiBin2[i]);
		}
	}


	if(BinOption == 2){
		NCentBins = 1;
		for(int i = 0; i< NCentBins+1; i++){
			CentBins.push_back(NCentBin3[i]);
			hiBins.push_back(hiBin3[i]);
		}
	}
	
	
	const int NArrSize = NCentBins;
	
	float hiBinsHis[NArrSize];

	for(int i = 0; i < NArrSize; i++){
		
		hiBinsHis[i] = hiBins[i];
		
	}
	
	
	cout << "Pass 1" << endl;

	const int NType = 7;


	double Binwidth[3] = {10,5,30};

	//TString TypeName[NType] ={"NOMINALPP","VARIATIONPP","NOMINALTAMU","VARIATIONTAMU","LINEAR","QUADRATIC","LINVERSE","LSQRT","LLOG"};
	//TString outfile[NType] ={"ROOTfiles/FilesCorr/CorrYieldNOMINALPP.root","ROOTfiles/FilesCorr/CorrYieldVARIATIONPP.root","ROOTfiles/FilesCorr/CorrYieldNOMINALTAMU.root","ROOTfiles/FilesCorr/CorrYieldVARIATIONTAMU.root","ROOTfiles/FilesCorr/CorrYieldLINEAR.root","ROOTfiles/FilesCorr/CorrYieldQUADRATIC.root","ROOTfiles/FilesCorr/CorrYieldLINVERSE.root","ROOTfiles/FilesCorr/CorrYieldLSQRT.root","ROOTfiles/FilesCorr/CorrYieldLLOG.root"};
	TString TypeName[NType] ={"LINEAR","QUADRATIC","LINVERSE","LSQRT","LLOG","NOMINALPP","VARIATIONPP"};
	TString outfile[NType] ={"ROOTfiles/FilesCorr/CorrYieldLINEAR.root","ROOTfiles/FilesCorr/CorrYieldQUADRATIC.root","ROOTfiles/FilesCorr/CorrYieldLINVERSE.root","ROOTfiles/FilesCorr/CorrYieldLSQRT.root","ROOTfiles/FilesCorr/CorrYieldLLOG.root","ROOTfiles/FilesCorr/CorrYieldNOMINALPP.root","ROOTfiles/FilesCorr/CorrYieldVARIATIONPP.root"};


	TString PreNameYield = "ROOTfiles/hPtSpectrumBplusPbPb";
	TString PreNameEff = "plotPtShapeSyst/Files/EffWeighted";

//	TH1D * CorrYield =  new TH1D("hPtSigma","CorrYield",NCentBins,hiBins[0],hiBins[NCentBins]);
	//TH1D * CorrYield =  new TH1D("CorrYield","CorrYield",NArrSize,hiBinsHis);

	TH1D * CorrYield =  new TH1D("CorrYield","CorrYield",nBins,ptBins);

	double CorrectedYield;
	double CorrectedYieldError;
	double CorrectedYieldCheck;

	cout << "Pass 2" << endl;

		for(int j = 0; j < NType; j++){



	
			fout = new TFile(outfile[j].Data(),"RECREATE");


			for(int i = 0; i < NCentBins; i++){
				 CorrectedYieldError = 0;
						CorrectedYieldCheck = 0;
				infileYield = Form("%s_%d_%d.root",PreNameYield.Data(),CentBins[i],CentBins[i+1]);
				infileEff = Form("%s%s_%d_%d.root",PreNameEff.Data(),TypeName[j].Data(),CentBins[i],CentBins[i+1]);
				//infileEff = Form("%s%s.root",PreNameEff.Data(),TypeName[j].Data());

				cout << "j = " << j << "  infile = " << infileYield.Data() << endl;
				finY = new TFile(infileYield.Data());
				


				TH1D * hPt = (TH1D *) finY->Get("hPt");
				hPt->Sumw2();

				finE = new TFile(infileEff.Data());
	
				TH1D * hEff = (TH1D *) finE->Get("hEff");

				TH1D * hPtSigma = (TH1D *) hPt->Clone("hPtSigma2");
		
				cout << "hPtSigma Bins = " << hPtSigma->GetNbinsX() << "  hEff Bins = " << hEff->GetNbinsX() << endl;
				hPtSigma->Sumw2();
				hEff->Sumw2();
				/*
				if(i==1){
				hPtSigma->SetBinContent(1,4.085242e-01);
				hPtSigma->SetBinError(1,0.3);
				}
				*/
				
				hPtSigma->Divide(hEff);
	
				for(int l = 0; l < hPtSigma->GetNbinsX();l++){
						
					cout << "l Bin = " << l  << "   hPt =   " << hPt->GetBinContent(l+1) << endl;
					cout << "l Bin = " << l  << "   hEff =   " << hEff->GetBinContent(l+1) << endl;
					cout << "l Bin = " << l  << "   hPtSigma =   " << hPtSigma->GetBinContent(l+1) << endl;

				}
				hPtSigma->Sumw2();
				hPtSigma->Scale(1.0/(BRchain*2*45));
			
				/*
				CorrectedYield = hPtSigma->Integral("width");
		
				
				for(int k = 0; k < hPtSigma->GetNbinsX(); k++){
				
					CorrectedYieldError = hPtSigma->GetBinError(k+1) * hPtSigma->GetBinError(k+1) + CorrectedYieldError;
					cout << "i = " << i << "  k = " << k  << "  hPtSigma->GetBinError(k+1) = " << hPtSigma->GetBinError(k+1) << endl;
				}
					

				CorrectedYieldError = TMath::Sqrt(CorrectedYieldError);
				*/

				for(int k = 0; k < hPtSigma->GetNbinsX(); k++){
			
				if(TypeName[j]== "LLOG")	cout << "k = " << k << "  Center = " <<  hPtSigma->GetBinContent(k+1) * Binwidth[k] << "   Error = " << hPtSigma->GetBinError(k+1) * Binwidth[k] << endl;;
				CorrectedYieldCheck = hPtSigma->GetBinContent(k+1) * Binwidth[k] + CorrectedYieldCheck;
				}

	
				CorrectedYield = hPtSigma->IntegralAndError(1,hPtSigma->GetNbinsX(),CorrectedYieldError,"width");

				if(TypeName[j]== "LLOG") cout << "i = "  << i << "   Corrected Yield = " << CorrectedYield << " CorrectedYieldCheck =   " << CorrectedYieldCheck  << "   CorrectedYieldError = " << CorrectedYieldError << endl;
				CorrYield->SetBinContent(i+1,CorrectedYield);
				CorrYield->SetBinError(i+1,CorrectedYieldError);

			}

	
			finY->Close();
			finE->Close();
			fout->cd();
			CorrYield->Write();
			fout->Close();
		}



}
