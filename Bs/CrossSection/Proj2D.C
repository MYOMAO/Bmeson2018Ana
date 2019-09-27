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

void Proj2D(){

	gStyle->SetOptTitle(0);
	gStyle->SetOptStat(0);

	TString Cut = "((hiBin < 181) && Btrk1Pt > 1.0 && Btrk2Pt > 1.0 && Bchi2cl > 0.05 && BsvpvDistance/BsvpvDisErr > 2.2 && Bpt > 5 && abs(Btrk1Eta-0.0) < 2.4 && abs(Btrk2Eta-0.0) < 2.4 && (TMath::Abs(By)<2.4&&TMath::Abs(Bmumumass-3.096916)<0.15&&((abs(Bmu1eta)<1.2&&Bmu1pt>3.5)||(abs(Bmu1eta)>1.2&&abs(Bmu1eta)<2.1&&Bmu1pt>(5.47-1.89*abs(Bmu1eta)))||(abs(Bmu1eta)>2.1&&abs(Bmu1eta)<2.4&&Bmu1pt>1.5))&&((abs(Bmu2eta)<1.2&&Bmu2pt>3.5)||(abs(Bmu2eta)>1.2&&abs(Bmu2eta)<2.1&&Bmu2pt>(5.47-1.89*abs(Bmu2eta)))||(abs(Bmu2eta)>2.1&&abs(Bmu2eta)<2.4&&Bmu2pt>1.5))&&Bmu1TMOneStationTight&&Bmu2TMOneStationTight&&Bmu1InPixelLayer>0&&(Bmu1InPixelLayer+Bmu1InStripLayer)>5&&Bmu2InPixelLayer>0&&(Bmu2InPixelLayer+Bmu2InStripLayer)>5&&Bmu1dxyPV<0.3&&Bmu2dxyPV<0.3&&Bmu1dzPV<20&&Bmu2dzPV<20&&Bmu1isTrackerMuon&&Bmu2isTrackerMuon&&Bmu1isGlobalMuon&&Bmu2isGlobalMuon&&Btrk1highPurity&&Btrk2highPurity&&abs(Btrk1Eta)<2.4&&abs(Btrk2Eta)<2.4&&Btrk1Pt>1.&&Btrk2Pt>1.&&abs(Btktkmass-1.019455)<0.015) && (abs(PVz)<15&&pclusterCompatibilityFilter&&pprimaryVertexFilter) && (Btrk1PixelHit + Btrk1StripHit > 10) && (Btrk2PixelHit + Btrk2StripHit > 10) && (Btrk1PtErr/Btrk1Pt < 0.1)&& (Btrk2PtErr/Btrk2Pt < 0.1) && Btrk1Chi2ndf/(Btrk1nStripLayer+Btrk1nPixelLayer) < 0.18 && Btrk2Chi2ndf/(Btrk2nStripLayer+Btrk2nPixelLayer) < 0.18&&((Bpt>5&&Bpt<10&&BDT_pt_5_10>0.32)||(Bpt>10&&Bpt<15&&BDT_pt_10_15> 0.29)||(Bpt>15&&Bpt<20&&BDT_pt_15_20>0.35)||(Bpt>20&&Bpt<50&&BDT_pt_20_50>0.33))&&abs(PVz)<15&&pclusterCompatibilityFilter&&pprimaryVertexFilter && phfCoincFilter2Th4)";
	
	TString FileName ="/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/2018Ana/Samples/FinalAnaSamples/PrivateMC-Data-Official-SemiFinal/Data_Bs_PbPb_TMVA_BDT_PbPb.root";

	TFile * fin = new TFile(FileName.Data());
	fin->cd();
	

	TTree * nt = (TTree * ) fin->Get("Bfinder/ntphi");
	nt->AddFriend("hltanalysis/HltTree");
	nt->AddFriend("hiEvtAnalyzer/HiTree");
	nt->AddFriend("skimanalysis/HltTree");
	nt->AddFriend("BDT_pt_15_20");
	nt->AddFriend("BDT_pt_5_10");
	nt->AddFriend("BDT_pt_10_15");	
	nt->AddFriend("BDT_pt_20_50");		



	TH2D * Proj2Dhis = new TH2D("Proj2Dhis","",45,5,50,50,5,6);
	
	cout << "START PROJECTING" << endl;

	nt->Project("Proj2Dhis","Bmass:Bpt",Cut.Data());
	

	Proj2Dhis->GetXaxis()->SetTitle("Bpt (GeV/c)");
	Proj2Dhis->GetYaxis()->SetTitle("Bmass (GeV/c^{2})");

	Proj2Dhis->GetXaxis()->CenterTitle();
	Proj2Dhis->GetYaxis()->CenterTitle();

	TCanvas * c = new TCanvas("c","c",600,600);

	c->cd();
	Proj2Dhis->Draw("COLZ");

	c->SaveAs("2DProj.png");


//	TH2D * Proj2DByBpt = new TH2D("Proj2DByBpt","",45,5,50,48,-2.4,2.4);
	TH2D * Proj2DByBpt = new TH2D("Proj2DByBpt","",20000,5,50,20000,-2.4,2.4);


	nt->Project("Proj2DByBpt","By:Bpt",Form("%s &&(Bmass < 6 && Bmass > 5)",Cut.Data()));

	Proj2DByBpt->GetYaxis()->SetTitle("B y");
	Proj2DByBpt->GetXaxis()->SetTitle("B_{s} p_{T} (GeV/c)");
	Proj2DByBpt->GetYaxis()->SetTitleOffset(1.5);

	Proj2DByBpt->GetXaxis()->CenterTitle();
	Proj2DByBpt->GetYaxis()->CenterTitle();

	Proj2DByBpt->Draw("COLZ");
	c->SaveAs("2DProjByBpt.png");
	

//	TH2D * Proj2DByBptMass = new TH2D("Proj2DByBptMass","",45,5,50,48,-2.4,2.4);
	TH2D * Proj2DByBptMass = new TH2D("Proj2DByBptMass","",20000,5,50,20000,-2.4,2.4);

	TString MassWeight = "(Bmass)";
	TString AllCut = Form("%s &&(Bmass < 6 && Bmass > 5)",Cut.Data());

	nt->Project("Proj2DByBptMass","By:Bpt",TCut(MassWeight.Data())*TCut(AllCut.Data()));

	Proj2DByBptMass->GetYaxis()->SetTitle("B y");
	Proj2DByBptMass->GetXaxis()->SetTitle("B_{s} p_{T} (GeV/c)");
	Proj2DByBptMass->GetYaxis()->SetTitleOffset(1.5);

	Proj2DByBptMass->GetXaxis()->CenterTitle();
	Proj2DByBptMass->GetYaxis()->CenterTitle();

	Proj2DByBptMass->Draw("COLZ");
	c->SaveAs("Proj2DByBptMass.png");
	


	TFile * fout = new TFile("2DProj.root","RECREATE");
	fout->cd();
	Proj2Dhis->Write();
	Proj2DByBpt->Write();
	Proj2DByBptMass->Write();
	fout->Close();
	cout << "DONE" << endl;

}


