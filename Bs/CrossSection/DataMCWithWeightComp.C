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
#include "uti.h"
using namespace std;

using std::cout;
using std::endl;
#endif
const int nBins=4;
double ptBins[nBins+1] = {5,10,15,20,50};

void DataMCWithWeightComp(){


	gStyle->SetOptTitle(0);
	gStyle->SetOptStat(0);

	const int NComp = 6;

	TFile * finData = new TFile("../CrossSectionCentAll/FromJulia/yields_Bs_binned_pt.root");
	TFile * finMC = new TFile("MCYIELDOUTPTComp.root");
	TString WeightName[NComp] ={"No Weight","Linear","Quadratic","Linear + Inverse","Linear + Square Root","Linear + Log"};

	int color[NComp]={1,2,3,4,6,7};

	TH1D * hPt = (TH1D * ) finData->Get("hPt");
	hPt->Sumw2();
	hPt->Scale(1.0/hPt->Integral());
	cout << "hPt = " << hPt->Integral() << endl;
	TH1D * hRatio[NComp]; 

	for(int i = 0; i < NComp; i++){
		hRatio[i] = (TH1D * ) hPt->Clone("hRatio");
		hRatio[i]->Sumw2();
		hRatio[i]->Scale(1.0/hRatio[i]->Integral());
		hRatio[i]->SetMarkerColor(color[i]);
		hRatio[i]->SetMarkerStyle(20);
		hRatio[i]->SetMarkerSize(1);
		hRatio[i]->SetLineColor(color[i]);

	}

	TH1D * hPtWidth = new TH1D("hPtWidth","",nBins,ptBins);

	for(int i = 0; i < nBins; i++){

		CorrectedBin = ptBins[i+1] - ptBins[i];

		hPtWidth->SetBinContent(i+1,CorrectedBin);
		hPtWidth->SetBinError(i+1,0);

	}




	for(int i = 0; i < NComp; i++){

		TH1D * hPtMC = (TH1D * ) finMC->Get(Form("hPtMCNew_%d",i));

		hPtMC->Divide(hPtWidth);
		hPtMC->Sumw2();
		hPtMC->Scale(1.0/hPtMC->Integral());
		cout << "Integral = " << hRatio[i]->Integral() <<  "  hPtMC = " << hPtMC->Integral() << endl;

		hRatio[i]->Divide(hPtMC);


	}

	hRatio[0]->GetXaxis()->SetTitle("B_{s} p_{T} (GeV/c)");
	hRatio[0]->GetYaxis()->SetTitle("Spectra Shape Ratio");

	hRatio[0]->GetXaxis()->CenterTitle();
	hRatio[0]->GetYaxis()->CenterTitle();

	TLegend* leg = new TLegend(0.48,0.62,0.40,0.90,NULL,"brNDC");
	leg->SetBorderSize(0);
	leg->SetTextSize(0.04);
	leg->SetTextFont(42);
	leg->SetFillStyle(0);




	for(int i = 0; i < NComp; i++){
		//	hRatio[i]->Divide(hPtMC);
		leg->AddEntry(hRatio[i],WeightName[i].Data(),"pl");	
	}

	TCanvas *c = new TCanvas("c","c",600,600);
	c->cd();
	hRatio[0]->SetMinimum(0.5);
	hRatio[0]->SetMaximum(2);


	hRatio[0]->Draw("ep");


	for(int i = 1; i < NComp; i++){
		hRatio[i]->Draw("epSAME");
	}

	TLine *l1 = new TLine(5.0,1,50.0.0,1);
	l1->SetLineStyle(2);
	l1->SetLineWidth(2);
	l1->SetLineColor(1);
	l1->Draw("SAME");

	leg->Draw("SAME");


	c->SaveAs("SPAfterWeight.png");


}
