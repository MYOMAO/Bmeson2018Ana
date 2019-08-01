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
//#include "parameters.h"

//#include "his.h"
using namespace std;

using std::cout;
using std::endl;


void ReweightPVz(TString DataName, TString MCName){



	const int NPVz = 30;
	const double PVzMin = -30;
	const double PVzMax = 30;

	gStyle->SetOptStat(0); 

	TFile *finData = new TFile(DataName.Data());
	TFile *finMC = new TFile(MCName.Data());



	TTree * tData = (TTree*) finData->Get("Bfinder/ntphi");
	TTree * tMC = (TTree*) finMC->Get("Bfinder/ntphi");




	TH1D * PVzData = new TH1D("PVzData","PVzData",NPVz,PVzMin,PVzMax);

	TH1D * PVzMC = new TH1D("PVzMC","PVzMC",NPVz,PVzMin,PVzMax);


	tData->Project("PVzData","PVz");

	PVzData->GetXaxis()->SetTitle("PVz (mm)");
	PVzData->GetYaxis()->SetTitle("Normalized Counts");
	PVzData->SetTitle("PVz Distribution in B_{S}^{0} Data");
	PVzData->SetMarkerStyle(20);
	PVzData->SetMarkerColor(kBlack);
	PVzData->SetMarkerSize(1);

	PVzData->Sumw2();
	PVzData->Scale(1.0/PVzData->Integral());

	tMC->Project("PVzMC","PVz - 0.321904");



	PVzMC->GetXaxis()->SetTitle("PVz (mm)");
	PVzMC->GetYaxis()->SetTitle("Normalized Counts");
	PVzMC->SetTitle("PVz Distribution in B_{S}^{0} MC");
	PVzMC->SetMarkerStyle(20);
	PVzMC->SetMarkerColor(kBlack);
	PVzMC->SetMarkerSize(1);

	PVzMC->Sumw2();
	PVzMC->Scale(1.0/PVzMC->Integral());

	TString FuncData;
	TString FuncMC;

	TF1 * f1 = new TF1("f1","[0]*TMath::Exp(-[1]*(x-[2])*(x-[2]))",PVzMin,PVzMax);
	f1->SetParLimits(1,0,1);
	f1->SetParLimits(2,0,10);


	PVzData->Draw("ep");

	PVzData->Fit(f1,"R");

	double p0Data = f1->GetParameter(0);
	double p1Data = f1->GetParameter(1);
	double p2Data = f1->GetParameter(2);


	TCanvas* c = new TCanvas("c","",600,600);
	c->cd();
	PVzData->Draw("ep");
	c->SaveAs("PVzFitResults/PVzDataFit.png");

	FuncData = Form("%f * TMath::Exp(- %f * (PVz - %f)*(PVz - %f))",f1->GetParameter(0),f1->GetParameter(1),f1->GetParameter(2),f1->GetParameter(2));

	cout << "Funct Data = " << FuncData.Data() << endl;

	ofstream foutResults(Form("ResultFile/ReweightPVzData.tex"));
	foutResults	<< "Functional Form: [0]*TMath::Exp(-[1]*(PVz-[2])*(PVz-[2]))" << endl;
	foutResults	<< "Fitting Results: " << FuncData.Data() << endl;
	foutResults << "Paramater 0 = " << f1->GetParameter(0) << endl;
	foutResults << "Paramater 0 Error = " << f1->GetParError(0) << endl;
	foutResults << "Paramater 1 = " << f1->GetParameter(1) << endl;
	foutResults << "Paramater 1 Error = " << f1->GetParError(1) << endl;
	foutResults << "Paramater 2 = " << f1->GetParameter(2) << endl;
	foutResults << "Paramater 2 Error = " << f1->GetParError(2) << endl;



	PVzMC->Fit(f1,"R");

	double p0MC = f1->GetParameter(0);
	double p1MC = f1->GetParameter(1);
	double p2MC = f1->GetParameter(2);



	FuncMC = Form("%f * TMath::Exp(- %f * (PVz - %f)*(PVz - %f))",f1->GetParameter(0),f1->GetParameter(1),f1->GetParameter(2),f1->GetParameter(2));

	cout << "Funct MC = " << FuncMC.Data() << endl;



	ofstream foutResults2(Form("ResultFile/ReweightPVzMC.tex"));
	foutResults2	<< "Functional Form: [0]*TMath::Exp(-[1]*(PVz-[2])*(PVz-[2]))" << endl;
	foutResults2	<< "Fitting Results: " << FuncMC.Data() << endl;
	foutResults2 << "Paramater 0 = " << f1->GetParameter(0) << endl;
	foutResults2 << "Paramater 0 Error = " << f1->GetParError(0) << endl;
	foutResults2 << "Paramater 1 = " << f1->GetParameter(1) << endl;
	foutResults2 << "Paramater 1 Error = " << f1->GetParError(1) << endl;
	foutResults2 << "Paramater 2 = " << f1->GetParameter(2) << endl;
	foutResults2 << "Paramater 2 Error = " << f1->GetParError(2) << endl;


	PVzMC->Draw("ep");
	c->SaveAs("PVzFitResults/PVzMCFit.png");


	TCanvas *cAll = new TCanvas("cAll","cAll",1800,600);
	cAll->Divide(3,1);
	cAll->cd(1);
	PVzData->Draw("ep");

	cAll->cd(2);
	PVzMC->Draw("ep");

	cAll->cd(3);

	TH1D * PVzMCReWeighted = new TH1D("PVzMCReWeighted","PVzMCReWeighted",NPVz,PVzMin,PVzMax);
	PVzMCReWeighted->GetXaxis()->SetTitle("PVz (mm)");
	PVzMCReWeighted->GetYaxis()->SetTitle("Normalized Counts");
	PVzMCReWeighted->SetTitle("Reweighted in B_{S}^{0} MC vs Data");

	TString PVzWeight = Form("(%s)/(%s)",FuncData.Data(),FuncMC.Data());

	tMC->Project("PVzMCReWeighted","PVz - 0.321904",TCut(PVzWeight.Data()));
	PVzMCReWeighted->SetMarkerStyle(20);
	PVzMCReWeighted->SetMarkerColor(kRed);
	PVzMCReWeighted->SetMarkerSize(1);
	PVzMCReWeighted->Scale(1.0/PVzMCReWeighted->Integral());

	TH1D * PVzData2 = new TH1D("PVzData2","PVzData2",NPVz,PVzMin,PVzMax);
	tData->Project("PVzData2","PVz");


	PVzData2->SetMarkerStyle(20);
	PVzData2->SetMarkerColor(kBlue);
	PVzData2->SetMarkerSize(1);
	PVzData2->Sumw2();
	PVzData2->Scale(1.0/PVzData2->Integral());



	PVzMCReWeighted->Draw("ep");
	PVzData2->Draw("epSAME");


	TLegend * leg = new TLegend(0.15,0.68,0.25,0.88,NULL,"brNDC");
	leg->SetBorderSize(0);
	leg->SetTextSize(0.04);
	leg->SetTextFont(42);
	leg->SetFillStyle(0);
	leg->AddEntry(PVzMCReWeighted,"MC Reweighted","pl");
	leg->AddEntry(PVzData2,"Data","pl");
	leg->Draw("SAME");

	cAll->SaveAs("PVzFitResults/PVzBoth.pdf");

}

int main(int argc, char *argv[])
{
	if((argc !=3))
	{
		std::cout << "Wrong number of inputs" << std::endl;
		return 1;
	}

	if(argc == 3)
		ReweightPVz(argv[1],argv[2]);
	return 0;
}


