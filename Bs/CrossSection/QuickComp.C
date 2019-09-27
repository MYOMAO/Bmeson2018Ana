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


//#include "his.h"
using namespace std;

using std::cout;
using std::endl;


void QuickComp(){

	TFile * fin1 = new TFile("ROOTfiles/CrossSectionPbPb.root");
	TFile * fin2 = new TFile("PtSpectraReweight.root");


	TH1D * CorrectedYield = (TH1D*) fin1->Get("hPtSigma");
	



	TH1D * hNominalPP = (TH1D*) fin2->Get("hNominalPP");
	TH1D * hVariationPP = (TH1D*) fin2->Get("hVariationPP");
	TH1D * hNominalTAMU = (TH1D*) fin2->Get("hNominalTAMU");
	TH1D * hVariationTAMU = (TH1D*) fin2->Get("hVariationTAMU");
	
	TH1D * hNominalTAMUComb = (TH1D*) hNominalTAMU->Clone("hNominalTAMUComb");
	hNominalTAMUComb->Add(hNominalPP,4);

	TH1D * hVariationTAMUComb = (TH1D*) hVariationTAMU->Clone("hVariationTAMUComb");
	hVariationTAMUComb->Add(hNominalPP,4);

	CorrectedYield->Sumw2();
	CorrectedYield->Scale(1.0/CorrectedYield->Integral());

	hNominalPP->Sumw2();
	hNominalPP->Scale(1.0/hNominalPP->Integral());
	
	hVariationPP->Sumw2();
	hVariationPP->Scale(1.0/hVariationPP->Integral());

	hNominalTAMU->Sumw2();
	hNominalTAMU->Scale(1.0/hNominalTAMU->Integral());
	
	hVariationTAMU->Sumw2();
	hVariationTAMU->Scale(1.0/hVariationTAMU->Integral());




	hNominalTAMUComb->Sumw2();
	hNominalTAMUComb->Scale(1.0/hNominalTAMUComb->Integral());


	hVariationTAMUComb->Sumw2();
	hVariationTAMUComb->Scale(1.0/hVariationTAMUComb->Integral());



	hNominalPP->Divide(CorrectedYield);
	hVariationPP->Divide(CorrectedYield);
	hNominalTAMU->Divide(CorrectedYield);
	hVariationTAMU->Divide(CorrectedYield);


	hNominalTAMUComb->Divide(CorrectedYield);
	hVariationTAMUComb->Divide(CorrectedYield);


	hNominalPP->GetXaxis()->SetTitle("B p_{T} (GeV/c)");
	hNominalPP->GetYaxis()->SetTitle("Spectra Shape Ratio");
	hNominalPP->SetMinimum(0);
	hNominalPP->SetMaximum(2.5);


	hNominalPP->SetTitle("");
	hNominalPP->SetMarkerStyle(20);
	hNominalPP->SetMarkerColor(1);
	hNominalPP->SetMarkerSize(1);
	hNominalPP->SetLineColor(1);


	hVariationPP->SetTitle(0);
	hVariationPP->SetMarkerStyle(20);
	hVariationPP->SetMarkerColor(2);
	hVariationPP->SetMarkerSize(1);
	hVariationPP->SetLineColor(2);


	hNominalTAMU->SetMarkerStyle(20);
	hNominalTAMU->SetMarkerColor(3);
	hNominalTAMU->SetMarkerSize(1);
	hNominalTAMU->SetLineColor(3);

	hVariationTAMU->SetMarkerStyle(20);
	hVariationTAMU->SetMarkerColor(4);
	hVariationTAMU->SetMarkerSize(1);
	hVariationTAMU->SetLineColor(4);



	hNominalTAMUComb->SetMarkerStyle(20);
	hNominalTAMUComb->SetMarkerColor(6);
	hNominalTAMUComb->SetMarkerSize(1);
	hNominalTAMUComb->SetLineColor(6);


	hVariationTAMUComb->SetMarkerStyle(20);
	hVariationTAMUComb->SetMarkerColor(7);
	hVariationTAMUComb->SetMarkerSize(1);
	hVariationTAMUComb->SetLineColor(7);



	TCanvas *c = new TCanvas("c","c",600,600);
	c->cd();
	
	hNominalPP->Draw("ep");
	hVariationPP->Draw("epSAME");
	hNominalTAMU->Draw("epSAME");
	hVariationTAMU->Draw("epSAME");
	hNominalTAMUComb->Draw("epSAME");
	//hVariationTAMUComb->Draw("epSAME");


	TLine *l1 = new TLine(5.0,1,50.0,1);
	l1->SetLineStyle(2);
	l1->SetLineWidth(2);
	l1->Draw("SAME");

	TLegend* leg = new TLegend(0.12,0.55,0.60,0.92,NULL,"brNDC");
	leg->SetBorderSize(0);
	leg->SetTextSize(0.04);
	leg->SetTextFont(42);
	leg->SetFillStyle(0);
	leg->AddEntry(hNominalPP,"hNominalPP/CorrectedYield" ,"pl");
	leg->AddEntry(hVariationPP,"hVariationPP/CorrectedYield" ,"pl");
	leg->AddEntry(hNominalTAMU,"hNominalTAMU/CorrectedYield" ,"pl");
	leg->AddEntry(hVariationTAMU,"hVariationTAMU/CorrectedYield" ,"pl");

	leg->AddEntry(hNominalTAMUComb,"hNominalTAMU * 0.2 + hNominalPP * 0.8" ,"pl");
	//leg->AddEntry(hVariationTAMUComb,"hVariationTAMUComb * 0.2 + hNominalPP * 0.8" ,"pl");

	
	leg->Draw("SAME");


	c->SaveAs("CorrectedYieldRatioSpec.png");

}
