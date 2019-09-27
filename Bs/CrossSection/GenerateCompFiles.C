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

void GenerateCompFiles(int BinOption){

	TString infile;
	TFile * fin;
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



	const int NType = 9;

	TString TypeName[NType] ={"NOMINALPP","VARIATIONPP","NOMINALTAMU","VARIATIONTAMU","LINEAR","QUADRATIC","LINVERSE","LSQRT","LLOG"};
	TString outfile[NType] ={"plotPtShapeSyst/FilesDiff/CorrYieldNOMINALPP.root","plotPtShapeSyst/FilesDiff/CorrYieldVARIATIONPP.root","plotPtShapeSyst/FilesDiff/CorrYieldNOMINALTAMU.root","plotPtShapeSyst/FilesDiff/CorrYieldVARIATIONTAMU.root","plotPtShapeSyst/FilesDiff/CorrYieldLINEAR.root","plotPtShapeSyst/FilesDiff/CorrYieldQUADRATIC.root","plotPtShapeSyst/FilesDiff/CorrYieldLINVERSE.root","plotPtShapeSyst/FilesDiff/CorrYieldLSQRT.root","plotPtShapeSyst/FilesDiff/CorrYieldLLOG.root"};
	//TString outfile[NType] ={"ROOTfiles/FilesCorr/CorrYieldNOMINALPP.root","ROOTfiles/FilesCorr/CorrYieldVARIATIONPP.root","plotPtShapeSyst/FilesDiff/CorrYieldNOMINALTAMU.root","plotPtShapeSyst/FilesDiff/CorrYieldVARIATIONTAMU.root","ROOTfiles/FilesCorr/CorrYieldLINEAR.root","ROOTfiles/FilesCorr/CorrYieldQUADRATIC.root","ROOTfiles/FilesCorr/CorrYieldLINVERSE.root","ROOTfiles/FilesCorr/CorrYieldLSQRT.root","ROOTfiles/FilesCorr/CorrYieldLLOG.root"};


	TString PreName = "ROOTfiles/CrossSectionPbPb";

	TH1D * CorrYield =  new TH1D("CorrYield","CorrYield",nBins,ptBins);



	double CorrectedYield;
	double CorrectedYieldError;


		for(int j = 0; j < NType; j++){



	
			fout = new TFile(outfile[j].Data(),"RECREATE");


			for(int i = 0; i < NCentBins; i++){
				 CorrectedYieldError = 0;

				infile = Form("%s%s_%d_%d.root",PreName.Data(),TypeName[j].Data(),CentBins[i],CentBins[i+1]);
				cout << "j = " << j << "  infile = " << infile.Data() << endl;
				fin = new TFile(infile.Data());
				


				TH1D * hPtSigma = (TH1D *) fin->Get("hPtSigma");
				CorrectedYield = hPtSigma->Integral("width");

				for(int k = 0; k < hPtSigma->GetNbinsX(); k++){
				
					CorrectedYieldError = hPtSigma->GetBinError(k+1) * hPtSigma->GetBinError(k+1) + CorrectedYieldError;
					cout << "i = " << i << "  k = " << k  << "  hPtSigma->GetBinError(k+1) = " << hPtSigma->GetBinError(k+1) << endl;
				}
		

				CorrectedYieldError = TMath::Sqrt(CorrectedYieldError);
				cout << "i = " << i << "   CorrectedYieldError = " << CorrectedYieldError << endl;
				CorrYield->SetBinContent(i+1,CorrectedYield);
				CorrYield->SetBinError(i+1,CorrectedYieldError);

			}


			fin->Close();
			fout->cd();
			CorrYield->Write();
			fout->Close();
		}


}
