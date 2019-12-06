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

	gStyle->SetPadRightMargin(0.043);
	gStyle->SetPadLeftMargin(0.18);
	gStyle->SetPadTopMargin(0.05);
	gStyle->SetPadBottomMargin(0.145);


	TString infile = "ROOTfiles/Phi-J_By_PbPb.root";

	TFile * fin = new TFile(infile.Data());

	TString Name;




	double Width;
	double WidthErr;
	const int nvar = 2;

	TString PatName[nvar] ={"J/#Psi","#Phi Meson"};

	TCanvas *c = new TCanvas("c","c",600,600);
	c->cd();


	TFile * fout = new TFile("ROOTfiles/WidthvsY.root","RECREATE");
	fout->cd();
	double p1min[nvar]={3.05,1.018};
	double p1max[nvar]={3.15,1.022};
	double p2min[nvar]={0.005,0.001};
	double p2max[nvar]={0.04,0.007};

	double rangemin[nvar] = {3.05,1.013};
	double rangemax[nvar] = {3.15,1.028};

	double cutvalue[nvar] = {0.015,0.15};

	double Mass;

	double	MassErr;

	for(int j = 0; j < nvar; j++){
		TH1D * WidthYhis = new TH1D(Form("WidthY_%d",j),Form("WidthY_%d",j),_nBins,_ptBins);
		WidthYhis->GetYaxis()->SetTitle(Form("%s Width", PatName[j].Data()));
		WidthYhis->GetXaxis()->SetTitle("|y|");
		WidthYhis->SetTitle("");


		TH1D * MassYhis = new TH1D(Form("MassYhis_%d",j),Form("MassYhis_%d",j),_nBins,_ptBins);
		MassYhis->GetYaxis()->SetTitle(Form("%s Inv Mass Peak", PatName[j].Data()));
		MassYhis->GetXaxis()->SetTitle("|y|");
		MassYhis->SetTitle("");


		TF1  *f = new TF1("f","[0]*TMath::Exp(-(x-[1])*(x-[1])/(2*[2]*[2]))",rangemin[j],rangemax[j]);

		f->SetParLimits(1,p1min[j],p1max[j]);
		f->SetParLimits(2,p2min[j],p2max[j]);


		for(int i=0;i<_nBins;i++)
		{
			Name = Form("h%d_%d",i,j);



			TH1D * his = (TH1D * ) fin->Get(Name.Data());
			his->GetYaxis()->SetTitle("Counts");


			//f->SetParLimits(0,WidthYhis->GetMaximum()*0.8,WidthYhis->GetMaximum()*1.2);


			//f->Draw("ep");
			//f->Draw();
			his->Fit(f,"R");
			his->Draw("ep");
			f->Draw("SAME");



			TLatex* tex;
			tex = new TLatex(0.48,0.85,Form("%.1f < |y| < %.1f",_ptBins[i],_ptBins[i+1]));
			tex->SetNDC();
			tex->SetTextFont(42);
			tex->SetTextSize(0.045);
			tex->SetLineWidth(2);
			tex->Draw("SAME");


		    TLatex* tex3 = new TLatex(0.20, 0.8,"Data");
		    TLatex* tex4 = new TLatex(0.20, 0.6,"PbPb");
			

			tex3->Draw("SAME");
			tex4->Draw("SAME");


			c->SaveAs(Form("plotSthBy/Fit%d_%d.png",i,j));


			Width = f->GetParameter(2);
			WidthErr = f->GetParError(2);
		
			
			Mass = f->GetParameter(1);
			MassErr = f->GetParError(1);

			//Width = his->GetRMS();
			//WidthErr = his->GetRMSError();
			cout << " Rapidity = " << _ptBins[i] << "  Width = " << Width << "  WdithErr = " << WidthErr << endl;

			WidthYhis->SetBinContent(i+1,Width);
			WidthYhis->SetBinError(i+1,WidthErr);

			MassYhis->SetBinContent(i+1,Mass);
			MassYhis->SetBinError(i+1,MassErr);



		}

		WidthYhis->SetMarkerSize(1.55);
		WidthYhis->SetMarkerStyle(20);
		WidthYhis->SetMarkerColor(kBlack);
		WidthYhis->SetLineColor(kBlack);
		WidthYhis->GetYaxis()->SetTitleOffset(2.2);
		WidthYhis->GetXaxis()->CenterTitle();
		WidthYhis->GetYaxis()->CenterTitle();

		if(j == 0) WidthYhis->SetMinimum(0.012);
		WidthYhis->Draw("ep");




		TLine *l1 = new TLine(0,cutvalue[j],2.4,cutvalue[j]);
		l1->SetLineStyle(2);
		l1->SetLineWidth(2);
		l1->Draw("SAME");
			
		


		c->SaveAs(Form("plotSthBy/WidthvsY_%d.png",j));



		WidthYhis->Write();


		MassYhis->SetMarkerSize(1.55);
		MassYhis->SetMarkerStyle(20);
		MassYhis->SetMarkerColor(kBlack);
		MassYhis->SetLineColor(kBlack);
		MassYhis->GetYaxis()->SetTitleOffset(2.2);
		MassYhis->GetXaxis()->CenterTitle();
		MassYhis->GetYaxis()->CenterTitle();
		MassYhis->Draw("ep");
	
		c->SaveAs(Form("plotSthBy/MassvsY_%d.png",j));

		
		MassYhis->Write();




	}






	fout->Close();


}


