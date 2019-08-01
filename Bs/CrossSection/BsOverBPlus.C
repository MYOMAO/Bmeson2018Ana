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

//TString BsName, TString BPlusName

void BsOverBPlus(){

	gStyle->SetOptTitle(0);
	gStyle->SetOptStat(0);
	TString BsName = "BsBP/CrossSectionPbPbBs.root";
	TString	BPlusName = "BsBP/CrossSectionPbPbBPlus.root";



	TFile * finBs = new TFile(BsName.Data());
	TFile * finBPlus = new TFile(BPlusName.Data());

	TGraphAsymmErrors * BsCross = (TGraphAsymmErrors *) finBs->Get("gaeCrossSyst");
	TGraphAsymmErrors * BPlusCross = (TGraphAsymmErrors *) finBPlus->Get("gaeCrossSyst");

	TH1D * hBPlusCross=(TH1D*)finBPlus->Get("hPtSigma");
	TH1D * hBsCross=(TH1D*)finBs->Get("hPtSigma");

	hBsCross->Divide(hBPlusCross);
	hBsCross->SetMaximum(1.2);
	hBsCross->SetMinimum(0.0);


	hBsCross->SetLineWidth(3);
	hBsCross->SetLineColor(BsPointColor);
	hBsCross->SetMarkerColor(BsPointColor);
	hBsCross->SetMarkerStyle(33);
	hBsCross->SetMarkerSize(2.2);

	double tpadr = 1;
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




	for(int i =0; i < nBins; i++){

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


	TCanvas *c = new TCanvas("c","c",600,600);
	c->cd();

	hBsCross->GetXaxis()->CenterTitle();
	hBsCross->GetYaxis()->CenterTitle();
	hBsCross->GetYaxis()->SetTitleOffset(1.3);

	hBsCross->GetXaxis()->SetTitle("p_{T} (GeV/c)");
	hBsCross->GetYaxis()->SetTitle("B^{0}_{S}/B^{+}");


	for(int i =0; i < nBins; i++){

		cout << "Ratio = " << 	hBsCross->GetBinContent(i+1) << endl;
		cout << "Ratio Error = " << 	hBsCross->GetBinError(i+1) << endl;

	}

	hBsCross->Draw("ep");

	double X[nBins];
	double Y[nBins];
	double YErr[nBins];
	double SystErr[nBins] ={0.46,0.336,0.356,0.393};
	double YErrorHigh[nBins];
	double YErrorLow[nBins];
	double MyXlow[nBins] = {2.5,2.5,2.5,15};
	double MyXhigh[nBins] = {2.5,2.5,2.5,15};


	for(int i = 0; i < nBins; i++){
		Y[i] = hBsCross->GetBinContent(i+1);
		YErr[i] = hBsCross->GetBinError(i+1);
		X[i] = (ptBins[i]+ptBins[i+1])/2;
		YErrorHigh[i] =  hBsCross->GetBinContent(i+1)*SystErr[i];
		YErrorLow[i] =  hBsCross->GetBinContent(i+1)*SystErr[i];

	}




	TGraphAsymmErrors* MyCrossSyst = new TGraphAsymmErrors(nBins,X,Y,MyXlow,MyXhigh,YErrorLow,YErrorHigh);
	MyCrossSyst->SetName("gaeCrossSyst");
	MyCrossSyst->SetMarkerStyle(20);
	MyCrossSyst->SetMarkerSize(0.8/tpadr);
	MyCrossSyst->SetFillColor(1);
	MyCrossSyst->SetFillStyle(0); 
	MyCrossSyst->SetLineWidth(2);
	MyCrossSyst->SetLineColor(1);
	MyCrossSyst->Draw("5same");  




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


	TLatex* texcent = new TLatex(0.62,0.78,"Cent. 0-90%");
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

	/*
	   TLine *line = new TLine(ptBins[0],10.5/40.5,ptBins[nBins],10.5/40.5);
	   line->SetLineStyle(2);
	   line->SetLineWidth(2);
	   line->Draw("SAME");
	   */


	double BsFrag = 10.3;
	double BsErr = 0.5;

	double BPFrag = 40.5;
	double BPErr = 0.6;

	const int BandBin = 1;
	double BandX[BandBin] = {(ptBins[nBins]+ptBins[0])/2};
	double BandY[BandBin] = {BsFrag/BPFrag};


	double BandXErr[BandBin] = {(ptBins[nBins]-ptBins[0])/2};
	double BandYErr[BandBin] = { BsFrag/BPFrag*TMath::Sqrt((BsErr/BsFrag)*(BsErr/BsFrag)+(BPErr/BPFrag)*(BPErr/BPFrag)) };



	TGraphAsymmErrors * FragBand = new TGraphAsymmErrors(BandBin,BandX,BandY,BandXErr,BandXErr,BandYErr,BandYErr);
	FragBand->SetName("BandErr");
	FragBand->SetMarkerStyle(20);
	FragBand->SetMarkerSize(0.8);
	FragBand->SetFillColorAlpha(kGreen,0.5);
	FragBand->SetFillStyle(3004);
	FragBand->SetLineWidth(2);
	FragBand->SetLineColor(kGreen);

	FragBand->Draw("5same");  


	TLegend* leg = new TLegend(0.40,0.38,0.76,0.68,NULL,"brNDC");
	leg->SetBorderSize(0);
	leg->SetTextSize(0.04);
	leg->SetTextFont(42);
	leg->SetFillStyle(0);
	leg->AddEntry(hBsCross,"Data Points","pl");
	leg->AddEntry(MyCrossSyst,"Systematics Uncertainties","l");
	leg->AddEntry(FragBand,"f_{s}/f_{u} in vacuum","l");
	leg->Draw("same");
	
	c->SaveAs("BstoBPlus.pdf");


}
