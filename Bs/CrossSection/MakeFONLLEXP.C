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
#include <vector>
#include "uti.h"
#include "parameters.h"
using namespace std;


void MakeFONLLEXP(TString FONLL, TString FONLLEXP, TString EXPOut){


	TFile * finFONLL = new TFile(FONLL.Data());
	TFile * finFONLLEXP = new TFile(FONLLEXP.Data());

	
	TGraphAsymmErrors* gaeBplusReference = (TGraphAsymmErrors*)finFONLL->Get("gaeSigmaBplus");




	TH1D * GptFONLL = new TH1D("GptFONLL","",nBinsReweight,ptBinsReweight);

	double x;
	double y;
	double yErr;
	for(int i=0;i<nBinsReweight;i++){

		gaeBplusReference->GetPoint(i,x,y);

		cout << "i = " << i << "  x = " << x << "  y = "  << y << endl;

		
		yErr = gaeBplusReference->GetErrorY(i);
		GptFONLL->SetBinContent(i+1,y);
		//GptFONLL->SetBinError(i+1,yErr);
	}
	
	TGraphAsymmErrors* gaeBplusReferenceEXP = (TGraphAsymmErrors*)finFONLLEXP->Get("gaeSigmaBplus");


	TH1D * GptFONLLEXP = new TH1D("GptFONLL","",nBinsReweight,ptBinsReweight);

	
	for(int i=0;i<nBinsReweight;i++){

		gaeBplusReferenceEXP->GetPoint(i,x,y);

		cout << "i = " << i << "  x = " << x << "  y = "  << y << endl;

		
		yErr = gaeBplusReferenceEXP->GetErrorY(i);
		GptFONLLEXP->SetBinContent(i+1,y);
		//GptFONLL->SetBinError(i+1,yErr);
	}


	TH1D * hPP = new TH1D("hPP","",nBinsReweight,ptBinsReweight);
	
	double PPDATAEXP[nBinsReweight]={1.1759072952895486,0.36073630193389816,0.08545220621622793,0.007361929116663382};
	double PPDATAEXPUP[nBinsReweight]={1.3013578653007176,0.399221116921108,0.09456859491581338,0.008386745719584903};
	double YErr[nBinsReweight];
	
	for(int i = 0; i < nBinsReweight; i++){
		YErr[i] = PPDATAEXPUP[i] - PPDATAEXP[i];
		hPP->SetBinContent(i+1,PPDATAEXP[i]);
		hPP->SetBinError(i+1,YErr[i]);
	}






	TH1D * hPPEXP = (TH1D *) hPP->Clone("hPPEXP");
	
	
	hPPEXP->Sumw2();
	GptFONLL->Sumw2();

	hPPEXP->Divide(GptFONLL);
	
	hPPEXP->Sumw2();
	GptFONLLEXP->Sumw2();

	hPPEXP->Multiply(GptFONLLEXP);

	
	double YGErr[nBinsReweight];
	double XG[nBinsReweight] = {10,14,19.5,36.5};
	double YG[nBinsReweight];

	for(int i = 0; i < nBinsReweight; i++){
		YG[i] = hPPEXP->GetBinContent(i+1);
		YGErr[i] = hPPEXP->GetBinError(i+1);

		cout << "i = " << i << "    YG[i] =  " << YG[i]  << endl;
	}

	

	TGraphAsymmErrors* gaeSigmaBplus= new TGraphAsymmErrors(nBinsReweight, XG, YG, 0, 0, YGErr, YGErr);;
	gaeSigmaBplus->SetName("gaeSigmaBplus");
	gaeSigmaBplus->SetFillColor(2);
	gaeSigmaBplus->SetFillStyle(3001); 
	gaeSigmaBplus->SetTitle(";p_{T}(GeV/c);d#sigma/dp_{T} (B_{s}) (pb GeV-1c)");

	
	TFile * fout = new TFile(EXPOut.Data(),"RECREATE");
	fout->cd();
	gaeSigmaBplus->Write();
	fout->Close();

}




int main(int argc, char *argv[])
{
	
	if(argc==4)
	{
		MakeFONLLEXP(argv[1],argv[2],argv[3]);
		return 0;
	}
	else
	{
		std::cout<<"  Error: invalid argument number - BDT()"<<std::endl;
		return 1;
	}

	return 1;
}




