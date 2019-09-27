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

void DataMCBptReweight(){


	TFile * finData = new TFile("../CrossSectionCentAll/FromJulia/yields_Bs_binned_pt.root");
	TFile * finMC = new TFile("MCYIELDOUTPTComp.root");

	gStyle->SetOptTitle(0);
	gStyle->SetOptStat(0);


	TH1D * hPt = (TH1D * ) finData->Get("hPt");
	TH1D * hPtMC = (TH1D * ) finMC->Get("hPtMCNew_0");
	/*
	TH1D * hPtMCfine = (TH1D * ) finMC->Get("hPtMCfine");

	
	hPtMCfine->Sumw2();
	hPtMCfine->Scale(1.0/hPtMCfine->Integral());
	
	hPtMCfine->SetMarkerStyle(20);
	hPtMCfine->SetMarkerColor(1);
	hPtMCfine->SetMarkerSize(1);
	hPtMCfine->SetLineColor(1);
	hPtMCfine->GetXaxis()->CenterTitle();
	hPtMCfine->GetYaxis()->CenterTitle();
	TCanvas * c1 = new TCanvas("c1","c1",600,600);
	c1->cd();
	hPtMCfine->Draw("ep");
	c1->SaveAs("hPtMCDistributionFine.png");
	*/
	TH1D * hPtWidth = new TH1D("hPtWidth","",nBins,ptBins);
	

	double Chi2;

	double CorrectedBin;

	for(int i = 0; i < nBins; i++){
	
	CorrectedBin = ptBins[i+1] - ptBins[i];

	hPtWidth->SetBinContent(i+1,CorrectedBin);
	hPtWidth->SetBinError(i+1,0);

	}
	hPtMC->Divide(hPtWidth);

	hPt->Sumw2();
	hPt->Scale(1.0/hPt->Integral());

	hPtMC->Sumw2();
	hPtMC->Scale(1.0/hPtMC->Integral());

	TH1D * Ratio = (TH1D*) hPt->Clone("Ratio");
	Ratio->Divide(hPtMC);


	TCanvas * c = new TCanvas("c","c",1200,600);
	c->Divide(2,1);
		
	c->cd(1);

	hPt->GetXaxis()->SetTitle("B_{s} p_{T} (GeV/c)");
	hPt->GetYaxis()->SetTitle("Normalized Raw Yield");
	hPt->GetYaxis()->SetTitleOffset(1.6);


	hPt->SetMarkerStyle(20);
	hPt->SetMarkerColor(2);
	hPt->SetMarkerSize(1);
	hPt->SetLineColor(2);


	hPtMC->SetMarkerStyle(20);
	hPtMC->SetMarkerColor(3);
	hPtMC->SetMarkerSize(1);
	hPtMC->SetLineColor(3);

	hPt->GetXaxis()->CenterTitle();
	hPt->GetYaxis()->CenterTitle();


	hPt->Draw("ep");
	hPtMC->Draw("pSAME");

	TLegend* leg = new TLegend(0.48,0.53,0.40,0.85,NULL,"brNDC");
	leg->SetBorderSize(0);
	leg->SetTextSize(0.04);
	leg->SetTextFont(42);
	leg->SetFillStyle(0);
	leg->AddEntry(hPt,"Data Raw Yield Shape","pl");
	leg->AddEntry(hPtMC,"MC Raw Yield Shape","pl");
	leg->Draw("SAME");
	
	c->cd(2);

	Ratio->GetXaxis()->SetTitle("B_{s} p_{T} (GeV/c)");
	Ratio->GetYaxis()->SetTitle("Data/MC Raw Yield Shape");
	Ratio->GetYaxis()->SetTitleOffset(1.6);
	Ratio->GetXaxis()->CenterTitle();
	Ratio->GetYaxis()->CenterTitle();

	Ratio->SetMarkerStyle(20);
	Ratio->SetMarkerColor(1);
	Ratio->SetMarkerSize(1);
	Ratio->SetLineColor(1);
	const int NForm = 5;

	TLegend* leg2 = new TLegend(0.30,0.13,0.58,0.40,NULL,"brNDC");
	leg2->SetBorderSize(0);
	leg2->SetTextSize(0.04);
	leg2->SetTextFont(42);
	leg2->SetFillStyle(0);
	leg2->AddEntry(Ratio,"Data/MC Points","pl");

	TString Func[NForm] = {"[0]+[1]*x","[0]+[1]*x+[2]*x*x","[0]/x+[1]*x","[0]+[1]*x+[2]*sqrt(x)","[0]+[1]*x+[2]*log(x)"};
	TString FuncName[NForm] = {"Linear","Quadratic","Linear + Inverse","Linear + Square root","Linear + Log"};
	double p0;
	double p1;
	double p2;

//	TString FuncOut[NForm] = {Form("%f + %f * x",p0,p1),Form("%f + %f * x + %f * x * x",p0,p1,p2),Form("%f * x + %f/x",p0,p1),Form("%f*TMath::Exp(-%f*x)+%f*TMath::Sqrt(x)",p0,p1,p2)}

	TF1 * f1[NForm];
	int color[NForm]={2,3,4,6,7}; 

	for(int i =0; i< NForm; i++){

	f1[i] = new TF1(Form("f1_%d",i),Func[i].Data(),5,50);
	
	Ratio->Fit(f1[i],"R");

	f1[i]->SetLineColor(color[i]);

  	p0 = f1[i]->GetParameter(0);
//	double p0Err = f1->GetParError(0);

	p1 = f1[i]->GetParameter(1);
//	double p1Err = f1->GetParError(1);

	p2 = f1[i]->GetParameter(2);
//	double p2Err = f1->GetParError(2);

//	double p3 = f1->GetParameter(3);
//	double p3Err = f1->GetParError(3);


	/*
	TF1 * f1P =  new TF1("f1P",Form("%f + %f * x",p0,p1+p1Err),5,50);

	TF1 * f1M =  new TF1("f1M",Form("%f + %f * x",p0,p1-p1Err),5,50);

	f1->SetLineColor(kRed);
	f1P->SetLineColor(kGreen);
	f1M->SetLineColor(kBlue);

	Ratio->Draw("ep");
	f1->Draw("SAME");
	f1P->Draw("SAME");	texChi->SetTextFont(42);
	f1M->Draw("SAME");
	*/

	Ratio->SetMinimum(0.0);
	Ratio->SetMaximum(2.0);

	Chi2 =  f1[i]->GetChisquare();


	leg2->AddEntry(f1[i],Form("%s : #chi^{2} = %.2f",FuncName[i].Data(),Chi2),"l");
//	leg2->AddEntry(f1P,"Minimum #chi^{2} Linear Fit with Slope + 1 Sigma","l");
//	leg2->AddEntry(f1M,"Minimum #chi^{2} Linear Fit with Slope - 1 Sigma","l");
	
	leg2->Draw("SAME");
	f1[i]->Draw("SAME");


	//cout << "Central Fit = " << FuncOut[i].Data() << endl;
	//cout << "+1 Sigma Fit = " << Form("%f + %f * x",p0,p1+p1Err) << endl;
//	cout << "-1 Sigma Fit = " << Form("%f + %f * x",p0,p1-p1Err) << endl;

	if(i==0) cout << "i = " << i << "   Func = " << Form("%f + %f * x",p0,p1) << endl;
	if(i==1) cout << "i = " << i << "   Func = " << Form("%f + %f * x + %f * x * x",p0,p1,p2) << endl;
	if(i==2) cout << "i = " << i << "   Func = " << Form("%f / x + %f*x",p0,p1) << endl;
	if(i==3) cout << "i = " << i << "   Func = " << Form("%f + %f * x + %f * sqrt(x)",p0,p1,p2) << endl;
	if(i==4) cout << "i = " << i << "   Func = " << Form("%f + %f * x + %f * log(x)",p0,p1,p2) << endl;


	}
	
	/*
	c->cd(2);
	Ratio->Draw("ep");
	f1->Draw("SAME");
	*/

	c->SaveAs("BptDataMCReweight.png");


	TCanvas * c2 = new TCanvas("c2","c2",1200,600);
	c2->Divide(2,1);
	c2->cd(1);
	//hPtMCfine->Draw("histp");
	c2->cd(2);
	hPtMC->Draw("p");

	c2->SaveAs("MCStatCheck.png");

}
