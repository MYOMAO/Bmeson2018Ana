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

//#include "his.h"
using namespace std;

using std::cout;
using std::endl;

void ReAnalyze(){
	


	gStyle->SetOptTitle(0);
	gStyle->SetOptStat(0);

	TFile * finProj = new TFile("2DProj.root");

	TFile * finEff = new TFile("EffFine.root");


	TH2D * EffBptBy = (TH2D *) finEff->Get("EffBptBy");
	TH2D * Proj2DByBpt = (TH2D *) finProj->Get("Proj2DByBpt");
	TH2D * Proj2DByBptMass = (TH2D *) finProj->Get("Proj2DByBptMass");


	TCanvas * c = new TCanvas("c","c",600,600);
	c->cd();

	


	TH2D * Proj2DByBptWeighted =  (TH2D *) Proj2DByBpt->Clone("Proj2DByBptWeighted");

	Proj2DByBptWeighted->Sumw2();
	EffBptBy->Sumw2();

	Proj2DByBptWeighted->Divide(EffBptBy);


	TH1D * NewInvMass = new TH1D("NewInvMass","",50,5,6);

	NewInvMass->GetXaxis()->SetTitle("B_{s} Inv Mass (GeV/c^{2})");
	NewInvMass->GetYaxis()->SetTitle("Eff Weighted Counts");
	NewInvMass->GetXaxis()->CenterTitle();
	NewInvMass->GetYaxis()->CenterTitle();
	NewInvMass->GetYaxis()->SetTitleOffset(1.5);


	double Weight;
	double WeightError;
	
	double Mass;
	double MassError;

	double TotalError;
	double Eff;

	int BinX;

	EffBptBy->Draw("COLZ");

	c->SaveAs("ReweightMass/Eff2D.png");


	Proj2DByBpt->Draw("COLZ");

	c->SaveAs("ReweightMass/Counts2D.png");

	Proj2DByBptMass->Draw("COLZ");

	c->SaveAs("ReweightMass/Mass2D.png");
	
	
	
	/*
	Proj2DByBptMass->Divide(Proj2DByBpt);
	Proj2DByBptMass->SetMinimum(5);
	Proj2DByBptMass->SetMaximum(6);
	c->SaveAs("ReweightMass/MassAve2D.png");
	*/



	Proj2DByBptWeighted->Draw("COLZ");

	c->SaveAs("ReweightMass/WeightedCounts2D.png");


	for(int i = 0; i < Proj2DByBptWeighted->GetNbinsX(); i++){

		for(int j = 0; j < Proj2DByBptWeighted->GetNbinsY(); j++){

			Weight = Proj2DByBptWeighted->GetBinContent(i+1,j+1);	
			WeightError = Proj2DByBptWeighted->GetBinError(i+1,j+1);	
		
			
			Eff = EffBptBy->GetBinContent(i+1,j+1);	
	
			
			if( Eff < 0.0001 && Eff > 0) cout << "Eff = " << Eff << endl;

			Mass = Proj2DByBptMass->GetBinContent(i+1,j+1);	
			MassError = Proj2DByBptMass->GetBinError(i+1,j+1);	
			
				
			TotalError = MassError * Weight;

			BinX = NewInvMass->GetXaxis()->FindBin(Mass);
	
			NewInvMass->SetBinContent(BinX,Weight);
			NewInvMass->SetBinContent(BinX,TotalError);


		}

	}


	NewInvMass->SetMarkerStyle(20);
	NewInvMass->SetMarkerSize(1);
	NewInvMass->SetMarkerColor(1);
	NewInvMass->SetLineColor(1);
	NewInvMass->Draw("ep");
	c->SaveAs("ReweightMass/ReweightMass.png");

	TFile * fout = new TFile("ReweightMass/ReweightedMass.root","RECREATE");
	fout->cd();
	NewInvMass->Write();
	fout->Close();
	


}
