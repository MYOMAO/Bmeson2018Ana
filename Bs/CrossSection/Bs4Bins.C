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

void Bs4Bins(){

	gStyle->SetOptTitle(0);
	gStyle->SetOptStat(0);
	TString BsName = "BsBP/CrossSectionPbPbBs.root";



	TFile * finBs = new TFile(BsName.Data());


	//TGraphAsymmErrors * BsCross = (TGraphAsymmErrors *) finBs->Get("gaeCrossSyst");
	//TGraphAsymmErrors * BPlusCross = (TGraphAsymmErrors *) finBPlus->Get("gaeCrossSyst");

	TH1D * hBsCross=(TH1D*)finBs->Get("hPtSigma");


	double BsCorr[nBins];
	double BPCorr[nBins];





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



	/*

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
	*/


//	TGraphAsymmErrors* BstoBPlus = new TGraphAsymmErrors(nBins,xr,Ratio,xrlow,xrhigh,RatioErrD,RatioErrU);


	TCanvas *c = new TCanvas("c","c",600,600);
	c->cd();

	hBsCross->GetXaxis()->CenterTitle();
	hBsCross->GetYaxis()->CenterTitle();
	hBsCross->GetYaxis()->SetTitleOffset(1.0);

	hBsCross->GetXaxis()->SetTitle("p_{T} (GeV/c)");
	hBsCross->GetYaxis()->SetTitle("#frac{1}{T_{AA}} #frac{dN}{dp_{T}} ( pb GeV^{-1}c)");


	for(int i =0; i < nBins; i++){

		cout << "Bs Corrected Yield = " << 	hBsCross->GetBinContent(i+1) << "     Start Err = " << hBsCross->GetBinError(i+1)/hBsCross->GetBinContent(i+1) << endl;
		cout << "Bs Corrected Yield Error = " << 	hBsCross->GetBinError(i+1) << endl;

	}



	hBsCross->SetMinimum(1000);
	hBsCross->SetMaximum(10000000);

	double NMB = 11.1;

	hBsCross->Scale(1.0/NMB);
	hBsCross->Draw("ep");


	double SystErrUp[nBins];
	double SystErrDown[nBins];

	//Bs//
	double TrackingErrBs[nBins] ={0.10,0.10,0.10,0.10};
	double SelErrBs[nBins] ={0.378,0.0479,0.0441,0.1044};
	double PTErrBs[nBins] ={0.0017,0.00013,0.00008,0.00093};
	double AccErrBs[nBins] ={0.0046,0.0343,0.0468,0.0425};
	double PDFBErrBs[nBins] ={0,0.0272,0.0156,0.0329};
	double PDFSErrBs[nBins] ={0.0862,0.0262,0.0099,0.00177};
	double TnPErrBsUp[nBins] ={0.20,0.0934,0.0601,0.0605};
	double TnPErrBsDown[nBins] ={0.142,0.0808,0.0568,0.0573};
	double MCStat[nBins] ={0.288,0.0626,0.0317,0.0349};




	for(int i = 0; i < nBins; i++){

		SystErrUp[i] =  sqrt(TrackingErrBs[i] * TrackingErrBs[i] + SelErrBs[i] *SelErrBs[i] + PTErrBs[i] * PTErrBs[i] + AccErrBs[i]* AccErrBs[i] + PDFBErrBs[i]* PDFBErrBs[i] + PDFSErrBs[i]* PDFSErrBs[i] + TnPErrBsUp[i]* TnPErrBsUp[i] + MCStat[i] * MCStat[i]);
		SystErrDown[i] =  sqrt(TrackingErrBs[i] * TrackingErrBs[i] + SelErrBs[i] *SelErrBs[i] + PTErrBs[i] * PTErrBs[i] + AccErrBs[i]* AccErrBs[i] + PDFBErrBs[i]* PDFBErrBs[i] + PDFSErrBs[i]* PDFSErrBs[i] + TnPErrBsDown[i]* TnPErrBsDown[i]+ MCStat[i] * MCStat[i]);


		cout << "i = "  << i  << "    SystErrUp = "  << SystErrUp[i]  <<  "    SystErrDown = "  << SystErrDown[i] << endl;

	}


	double Y[nBins];


	double X[nBins];
	double YErr[nBins];
	//	double SystErr[nBins] ={0.220,0.182,0.201,0.147};
	double YErrorHigh[nBins];
	double YErrorLow[nBins];
	double MyXlow[nBins] = {2.5,2.5,2.5,15};
	double MyXhigh[nBins] = {2.5,2.5,2.5,15};


	for(int i = 0; i < nBins; i++){
		Y[i] = hBsCross->GetBinContent(i+1);
		YErr[i] = hBsCross->GetBinError(i+1);
		X[i] = (ptBins[i]+ptBins[i+1])/2;
		YErrorHigh[i] =  hBsCross->GetBinContent(i+1)*SystErrUp[i];
		YErrorLow[i] =  hBsCross->GetBinContent(i+1)*SystErrDown[i];

	}


	ofstream outratio("/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/2018Ana/HIN-19-011Final/ratio_pt.txt");
	outratio << "ptmin" << "   " << "ptmax" << "   " <<  "xsec" << "   " << "statUncert" << "   " << "SystUncert Up" <<  "   " << "SystUncert Down"  << "   " <<  "glbUncert" << endl;

	for(int i = 0; i < nBins; i++){
		outratio << ptBins[i] << "   " << ptBins[i+1] << "   " << Y[i] << "   " <<YErr[i]/Y[i] << "   " << SystErrUp[i]  << "   " << SystErrDown[i] << "   "  << 0 << endl;

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

	TLatex *texY = new TLatex(0.11,0.70," 10 GeV/c < B p_{T} < 50 GeV/c and B |y| < 2.4");
	texY->SetNDC();
	texY->SetTextFont(42);
	texY->SetTextSize(0.04);
	texY->SetLineWidth(2);
	texY->Draw("SAME");

	TLatex *texY2 = new TLatex(0.11,0.60," 7 GeV/c < B p_{T} < 10 GeV/c and 1.5 < B |y| < 2.4");
	texY2->SetNDC();
	texY2->SetTextFont(42);
	texY2->SetTextSize(0.04);
	texY2->SetLineWidth(2);
	texY2->Draw("SAME");



	c->SetLogy();
	c->SaveAs("BsCorrYieldPt.pdf");


}
