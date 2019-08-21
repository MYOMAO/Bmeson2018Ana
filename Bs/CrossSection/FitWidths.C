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
#include "parameters.h"

//#include "his.h"
using namespace std;

using std::cout;
using std::endl;


int _nBins = nBinsY;
double *_ptBins = ptBinsY;

void FitWidths(){

	gStyle->SetOptStat(0);	

	TString infile = "ROOTfiles/Phi-J_By_PbPb.root";

	TFile * fin = new TFile(infile.Data());

	TString Name;


	TF1  *f = new TF1("f","[0]*TMath::Exp(-(x-[1])*(x-[1])/(2*[2]*[2]))",1.013,1.029);
	f->SetParLimits(1,1.018,1.022);
	f->SetParLimits(2,0.001,0.007);



	double Width;
	double WidthErr;
	const int nvar = 2;

	TString PatName[nvar] ={"J/#Psi","#Phi Meson"};

	TCanvas *c = new TCanvas("c","c",600,600);
	c->cd();


	TFile * fout = new TFile("ROOTfiles/WidthvsY.root","RECREATE");
	fout->cd();


	for(int j = 0; j < nvar; j++){
		TH1D * WidthYhis = new TH1D(Form("WidthY_%d",j),Form("WidthY_%d",j),_nBins,_ptBins);
		WidthYhis->GetYaxis()->SetTitle(Form("%s Width", PatName[j].Data()));
		WidthYhis->GetXaxis()->SetTitle("|y|");
		WidthYhis->SetTitle("");



		for(int i=0;i<_nBins;i++)
		{
			Name = Form("h%d_%d",i,j);



			TH1D * his = (TH1D * ) fin->Get(Name.Data());
			his->GetYaxis()->SetTitle("Counts");


			f->SetParLimits(0,WidthYhis->GetMaximum()*0.8,WidthYhis->GetMaximum()*1.2);


			f->Draw("ep");
			his->Fit(f,"R");

			c->SaveAs(Form("plotSthBy/Fit%d_%d.png",i,j));


			//	Width = f->GetParameter(2);
			//	WidthErr = f->GetParError(2);

			Width = his->GetRMS();
			WidthErr = his->GetRMSError();
			cout << " Rapidity = " << _ptBins[i] << "  Width = " << Width << "  WdithErr = " << WidthErr << endl;

			WidthYhis->SetBinContent(i+1,Width);
			WidthYhis->SetBinError(i+1,WidthErr);
		}

		WidthYhis->SetMarkerSize(1.55);
		WidthYhis->SetMarkerStyle(20);
		WidthYhis->SetMarkerColor(kBlack);
		WidthYhis->SetLineColor(kBlack);
		WidthYhis->Draw("ep");

		c->SaveAs(Form("plotSthBy/WidthvsY_%d.png",j));



		WidthYhis->Write();

	}






	fout->Close();


}


