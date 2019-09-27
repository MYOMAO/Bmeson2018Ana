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

void QuickDraw(){

	gStyle->SetOptTitle(0);
	gStyle->SetOptStat(0);



	TFile * fin = new TFile("EffInfo.root");
	fin->cd();
	
	TTree * t = (TTree *) fin->Get("EffInfoTree");


	TH1D * Bmass = new TH1D("Bmass","Bmass",60,5,6);

	
	t->Project("Bmass","BmassNew");

	Bmass->SetLineColor(1);
	Bmass->SetMarkerColor(1);
	Bmass->SetMarkerStyle(20);

	Bmass->GetXaxis()->SetTitle("Bmass (GeV/c^{2})");
	Bmass->GetYaxis()->SetTitle("Counts");
	

	Bmass->GetXaxis()->CenterTitle();
	Bmass->GetYaxis()->CenterTitle();
	Bmass->GetYaxis()->SetTitleOffset(1.4);
	
	Bmass->SetMaximum(55);
	Bmass->SetMinimum(0);

	
	TCanvas *c = new TCanvas("c","c",600,600);
	c->cd();
	
	Bmass->Draw("ep");


	TLine * l1 = new TLine(5.36682 -0.08,0 ,5.36682 -0.08,55);
	l1->SetLineStyle(2);
	l1->SetLineWidth(2);
	l1->SetLineColor(2);
	l1->Draw("SAME");
		
	TLine * l2 = new TLine(5.36682 +0.08,0 ,5.36682 +0.08,55);
	l2->SetLineStyle(2);
	l2->SetLineWidth(2);
	l2->SetLineColor(2);
	l2->Draw("SAME");

	
	c->SaveAs("InvMassCut.png");



}
