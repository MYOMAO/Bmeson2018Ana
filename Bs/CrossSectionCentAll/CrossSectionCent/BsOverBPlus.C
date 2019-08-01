#ifndef __CINT__
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
using namespace std;

using std::cout;
using std::endl;
#endif
int _nBins = nBinsCent;
double *_ptBins = ptBinsCent;
//TString BsName, TString BPlusName

void BsOverBPlus(){

	gStyle->SetOptTitle(0);
	gStyle->SetOptStat(0);
	TString BsName = "BsBP/CrossSectionPbPbBs.root";
	TString	BPlusName = "BsBP/CrossSectionPbPbBPlus.root";




	TFile * finBs = new TFile(BsName.Data());
	TFile * finBPlus = new TFile(BPlusName.Data());


	TH1D * hBPlusCross=(TH1D*)finBPlus->Get("hPtSigma");
	TH1D * hBsCross=(TH1D*)finBs->Get("hPtSigma");

	double BinValue[_nBins];
	double BinValueErr[_nBins];

	int dorescale = 1;
	


	if(dorescale == 1){

		for(int i = 0; i < _nBins; i++){
		
			BinValue[i] =  hBsCross->GetBinContent(i+1)/(TAA[i])*5.607;
			BinValueErr[i] =  hBsCross->GetBinError(i+1)/(TAA[i])*5.607;

		}
		hBsCross->Reset();

		for(int i = 0; i < nBinsCent; i++){
		
			hBsCross->SetBinContent(i+1,BinValue[i]);
			hBsCross->SetBinError(i+1,BinValueErr[i]);		
		}

	}

	
	for(int i =0; i < _nBins; i++){

		cout << "Ratio After Rescale = " << 	hBsCross->GetBinContent(i+1) << endl;
	}


	hBsCross->Sumw2();
	hBPlusCross->Sumw2();

	hBsCross->Divide(hBPlusCross);
	hBsCross->SetMaximum(1.2);
	hBsCross->SetMinimum(0);




	for(int i =0; i < _nBins; i++){

		cout << "Ratio = " << 	hBsCross->GetBinContent(i+1) << endl;
	}




	double xBPlus;
	double yBPlus;
	double yBlusErrU;
	double yBlusErrD;

	double xBs;
	double yBs;
	double yBsErrU;
	double yBsErrD;

	double xr[nBins];
	double xrlow[nBins];
	double xrhigh[nBins];

	double Ratio[nBins];
	double RatioErrU[nBins];
	double RatioErrD[nBins];

	int dosyst = 0;

	if(dosyst == 1){
		TGraphAsymmErrors * BsCross = (TGraphAsymmErrors *) finBs->Get("gaeCrossSyst");
		TGraphAsymmErrors * BPlusCross = (TGraphAsymmErrors *) finBPlus->Get("gaeCrossSyst");

		for(int i =0; i < _nBins; i++){

			BsCross->GetPoint(i,xBs,yBs);
			BPlusCross->GetPoint(i,xBPlus,yBPlus);
			yBsErrU = BsCross->GetErrorYhigh(i);
			yBsErrD = BsCross->GetErrorYlow(i);
			yBlusErrU = BPlusCross->GetErrorYhigh(i);
			yBlusErrD = BPlusCross->GetErrorYlow(i);
			xr[i] = xBs;
			xrlow[i] = BsCross->GetErrorXlow(i);
			xrhigh[i] = BsCross->GetErrorXhigh(i);

			Ratio[i] = yBs/yBPlus;
			RatioErrU[i] = Ratio[i]*TMath::Sqrt((yBsErrU/yBs)*(yBsErrU/yBs) + (yBlusErrU/yBPlus)*(yBlusErrU/yBPlus) );
			RatioErrD[i] = Ratio[i]*TMath::Sqrt((yBsErrD/yBs)*(yBsErrD/yBs) + (yBlusErrD/yBPlus)*(yBlusErrD/yBPlus) );
		}

		TGraphAsymmErrors* BstoBPlus = new TGraphAsymmErrors(nBins,xr,Ratio,xrlow,xrhigh,RatioErrD,RatioErrU);

	}

	TCanvas *c = new TCanvas("c","c",600,600);
	c->cd();

	TH1D * NPartPlot = new TH1D("NPartPlot","",_nBins,npart);
	

	for(int i = 0; i < _nBins; i++){

		int j = NPartPlot->FindBin(npart[i]);

		NPartPlot->SetBinContent(j,hBsCross->GetBinContent(i+1));
		
	

	}

	NPartPlot->SetLineWidth(3);
	NPartPlot->SetLineColor(kAzure+10);
	NPartPlot->SetMarkerColor(kAzure+10);
	NPartPlot->SetMarkerStyle(33);
	NPartPlot->SetMarkerSize(2.2);

	NPartPlot->GetXaxis()->SetTitle("<NPart>");
	NPartPlot->GetYaxis()->SetTitle("B^{0}_{S}/B^{+}");
	NPartPlot->GetXaxis()->CenterTitle();
	NPartPlot->Draw("ep");






	TLatex* texlumi = new TLatex(0.96,0.95,"1.5 nb^{-1} (PbPb) 5.02 TeV");
	texlumi->SetNDC();
	texlumi->SetTextAlign(32);
	texlumi->SetTextFont(42);
	texlumi->SetTextSize(0.05);
	texlumi->SetLineWidth(2);
	texlumi->Draw("SAME");


	TLatex* texcms = new TLatex(0.21,0.88,"CMS");
	texcms->SetNDC();
	texcms->SetTextAlign(13);
	texcms->SetTextFont(62);//61
	texcms->SetTextSize(0.06);
	texcms->SetLineWidth(2);
	texcms->Draw("SAME");


	TLatex* texcent = new TLatex(0.62,0.78,"B p_{T}. 5 - 50 GeV/c");
	texcent->SetNDC();
	texcent->SetTextFont(42);
	texcent->SetTextSize(0.05);
	texcent->SetLineWidth(2);
	texcent->Draw("SAME");

	TLatex *texY = new TLatex(0.21,0.79,"|y| < 2.4");
	texY->SetNDC();
	texY->SetTextFont(42);
	texY->SetTextSize(0.05);
	texY->SetLineWidth(2);
	texY->Draw("SAME");

	TLine *line = new TLine(_ptBins[0],1,_ptBins[_nBins],1);
	line->SetLineStyle(2);
	line->SetLineWidth(2);
	line->Draw("SAME");



	TLatex* Bin1 = new TLatex(0.15,0.40,"Cent. 0 - 30%");
	Bin1->SetNDC();
	Bin1->SetTextAlign(13);
	Bin1->SetTextFont(62);//61
	Bin1->SetTextSize(0.06);
	Bin1->SetLineWidth(2);
	Bin1->Draw("SAME");


	TLatex* Bin2 = new TLatex(0.50,0.38,"Cent. 30 - 90%");
	Bin2->SetNDC();
	Bin2->SetTextAlign(13);
	Bin2->SetTextFont(62);//61
	Bin2->SetTextSize(0.06);
	Bin2->SetLineWidth(2);
	Bin2->Draw("SAME");



	c->SaveAs("BstoBPlusCent.pdf");


}
