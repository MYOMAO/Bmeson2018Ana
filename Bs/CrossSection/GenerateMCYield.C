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
//#include "fitB.h"
using namespace std;

using std::cout;
using std::endl;
#endif



void GenerateMCYield(){

	double Yield;
	double YieldErr;



	TFile * fin = new TFile("/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/2018Ana/Samples/FinalAnaSamples/PrivateMC-Data-Official-SemiFinal/MC_Bs_PbPb_TMVA_BDT_PbPb.root");

	TTree * ntMC = (TTree*)fin->Get("Bfinder/ntphi");
	ntMC->AddFriend("hltanalysis/HltTree");
	ntMC->AddFriend("hiEvtAnalyzer/HiTree");
	ntMC->AddFriend("skimanalysis/HltTree");
	ntMC->AddFriend("BDT_pt_15_20");
	ntMC->AddFriend("BDT_pt_5_10");
	ntMC->AddFriend("BDT_pt_10_15");	
	ntMC->AddFriend("BDT_pt_20_50");
	ntMC->AddFriend("CentWeightTree");
	ntMC->AddFriend("Recalculated");
	ntMC->AddFriend("Bfinder/ntGen");

	//	TString BptWeight = "(12.248948)*TMath::Exp(-0.432495 * Bgenpt) + 0.654161";
	TString BptWeight = "(1)";
	//	TString BptWeight = "0.850412 + 0.009982 * Bpt";

	TString	CentWeight = "CentWeight";
	TString	PVzWeight="0.163562 * TMath::Exp(- 0.021039 * (PVz - 0.426587)*(PVz - 0.426587))/(0.159619 * TMath::Exp(- 0.020011 * (PVz - 0.587652)*(PVz - 0.587652)))";
	TString	pthatweight = "(pthatweight)";

	TString Cut = "((hiBin < 181) && Btrk1Pt > 0.9 && Btrk2Pt > 0.9 && Bchi2cl > 0.05 && BsvpvDistance/BsvpvDisErr > 2.0 && Bpt > 5 && abs(Btrk1Eta-0.0) < 2.4 && abs(Btrk2Eta-0.0) < 2.4 && (TMath::Abs(By)<2.4&&TMath::Abs(Bmumumass-3.096916)<0.15&&((abs(Bmu1eta)<1.2&&Bmu1pt>3.5)||(abs(Bmu1eta)>1.2&&abs(Bmu1eta)<2.1&&Bmu1pt>(5.47-1.89*abs(Bmu1eta)))||(abs(Bmu1eta)>2.1&&abs(Bmu1eta)<2.4&&Bmu1pt>1.5))&&((abs(Bmu2eta)<1.2&&Bmu2pt>3.5)||(abs(Bmu2eta)>1.2&&abs(Bmu2eta)<2.1&&Bmu2pt>(5.47-1.89*abs(Bmu2eta)))||(abs(Bmu2eta)>2.1&&abs(Bmu2eta)<2.4&&Bmu2pt>1.5))&&Bmu1TMOneStationTight&&Bmu2TMOneStationTight&&Bmu1InPixelLayer>0&&(Bmu1InPixelLayer+Bmu1InStripLayer)>5&&Bmu2InPixelLayer>0&&(Bmu2InPixelLayer+Bmu2InStripLayer)>5&&Bmu1dxyPV<0.3&&Bmu2dxyPV<0.3&&Bmu1dzPV<20&&Bmu2dzPV<20&&Bmu1isTrackerMuon&&Bmu2isTrackerMuon&&Bmu1isGlobalMuon&&Bmu2isGlobalMuon&&Btrk1highPurity&&Btrk2highPurity&&abs(Btrk1Eta)<2.4&&abs(Btrk2Eta)<2.4&&Btrk1Pt>1.&&Btrk2Pt>1.&&abs(Btktkmass-1.019455)<0.015) && (abs(PVz)<15&&pclusterCompatibilityFilter&&pprimaryVertexFilter) && (Btrk1PixelHit + Btrk1StripHit > 10) && (Btrk2PixelHit + Btrk2StripHit > 10) && (Btrk1PtErr/Btrk1Pt < 0.1)&& (Btrk2PtErr/Btrk2Pt < 0.1) && Btrk1Chi2ndf/(Btrk1nStripLayer+Btrk1nPixelLayer) < 0.18 && Btrk2Chi2ndf/(Btrk2nStripLayer+Btrk2nPixelLayer) < 0.18&&((Bpt>5&&Bpt<10&&BDT_pt_5_10>0.32)||(Bpt>10&&Bpt<15&&BDT_pt_10_15> 0.29)||(Bpt>15&&Bpt<20&&BDT_pt_15_20>0.35)||(Bpt>20&&Bpt<50&&BDT_pt_20_50>0.33))&&abs(PVz)<15&&pclusterCompatibilityFilter&&pprimaryVertexFilter && phfCoincFilter2Th4)";

	TString CutPT; 

	/*
	TH1D* hPtMC = new TH1D("hPtMC","",nBins,ptBins);
	TH1D* hPtMCfine = new TH1D("hPtMCfine","",46,5.0,50);
	hPtMCfine->GetXaxis()->SetTitle("B_{s} p_{T} (GeV/c)");
	hPtMCfine->GetYaxis()->SetTitle("Normalized Counts");
	hPtMCfine->GetYaxis()->SetTitleOffset(1.6);
	hPtMCfine->GetXaxis()->CenterTitle();
	hPtMCfine->GetYaxis()->CenterTitle();

	TCanvas *c = new TCanvas("c","c",600,600);
	c->cd();
	ntMC->Project("hPtMCfine","Bpt",(TCut(BptWeight.Data())*TCut(CentWeight.Data())*TCut(PVzWeight.Data())*TCut(pthatweight.Data()))*(TCut(Cut.Data())));
	hPtMCfine->Sumw2();
	hPtMCfine->Scale(1.0/hPtMCfine->Integral());
	hPtMCfine->Draw("ep");;

	hPtMCfine->SaveAs("MCBptRawYield.png");





	for(int i = 0; i < nBins; i++){

		CutPT = Form("(%s) &&(Bpt > %f && Bpt < %f && Bgen == 23333)",Cut.Data(),ptBins[i] ,ptBins[i+1]);

		cout << "CutPT =   " << CutPT.Data() << endl;

		ntMC->Project("his","Bmass",(TCut(BptWeight.Data())*TCut(CentWeight.Data())*TCut(PVzWeight.Data())*TCut(pthatweight.Data()))*(TCut(CutPT.Data())));

		Yield = his->Integral();
		YieldErr = TMath::Sqrt(Yield);


		cout << "pt =  " << ptBins[i] << " - " << ptBins[i+1] << "    Yield =  " << Yield << "    YieldErr = " <<  YieldErr << endl;

		hPtMC->SetBinContent(i+1,Yield);
		hPtMC->SetBinError(i+1,YieldErr);


	}

	TFile * fout = new TFile("MCYIELDOUTPT.root","RECREATE");
	fout->cd();

	hPtMC->Write();
	hPtMCfine->Write();
	fout->Close();
	*/

	//Comp//

	const int NComp = 6;

	TString BptWeightComp[NComp] ={"1","0.850412 + 0.009982 * Bgenpt","1.093947 - 0.016091 * Bgenpt + 0.000559 * Bgenpt * Bgenpt","7.187263 / Bgenpt + 0.029820*Bgenpt","1.844009 + 0.058935 * Bgenpt - 0.454120 * sqrt(Bgenpt)","1.810010 + 0.033953 * Bgenpt - 0.499745 * log(Bgenpt)"};
	TString WeightName[NComp] ={"No Weight","Linear","Quadratic","Linear + Inverse","Linear + Square Root","Linear + Log"};

	TH1D* hPtMCNew[NComp];

	TFile * fout2 = new TFile("MCYIELDOUTPTComp.root","RECREATE");
	fout2->cd();

	cout << "Now Working on Comp" << endl;
	for(int j = 0; j < NComp; j++){

		cout << "j = " << j << "  Working on  " << WeightName[j].Data() << endl;
		TH1D * his = new TH1D("his","his",60,5.0,6.0);




		hPtMCNew[j] = new TH1D(Form("hPtMCNew_%d",j),"",nBins,ptBins);
		
		
		for(int i = 0; i < nBins; i++){

			CutPT = Form("(%s) &&(Bpt > %f && Bpt < %f && Bgen == 23333)",Cut.Data(),ptBins[i] ,ptBins[i+1]);

			cout << "CutPT =   " << CutPT.Data() << endl;

			ntMC->Project("his","Bmass",(TCut(BptWeightComp[j].Data())*TCut(CentWeight.Data())*TCut(PVzWeight.Data())*TCut(pthatweight.Data()))*(TCut(CutPT.Data())));

			Yield = his->Integral();
			YieldErr = TMath::Sqrt(Yield);


			cout << "pt =  " << ptBins[i] << " - " << ptBins[i+1] << "    Yield =  " << Yield << "    YieldErr = " <<  YieldErr << endl;

			hPtMCNew[j]->SetBinContent(i+1,Yield);
			hPtMCNew[j]->SetBinError(i+1,YieldErr);


		}


		hPtMCNew[j]->GetYaxis()->SetTitle(WeightName[j].Data());
		hPtMCNew[j]->Sumw2();
		hPtMCNew[j]->Scale(1.0/hPtMCNew[j]->Integral());

		hPtMCNew[j]->Write();
	}	

	
	TH1D* hPtMCfine = new TH1D("hPtMCfine","",46,5.0,50);
	hPtMCfine->GetXaxis()->SetTitle("B_{s} p_{T} (GeV/c)");
	hPtMCfine->GetYaxis()->SetTitle("Normalized Counts");
	hPtMCfine->GetYaxis()->SetTitleOffset(1.6);
	hPtMCfine->GetXaxis()->CenterTitle();
	hPtMCfine->GetYaxis()->CenterTitle();

	TCanvas *c = new TCanvas("c","c",600,600);
	c->cd();
	ntMC->Project("hPtMCfine","Bpt",(TCut(BptWeight.Data())*TCut(CentWeight.Data())*TCut(PVzWeight.Data())*TCut(pthatweight.Data()))*(TCut(Cut.Data())));
	hPtMCfine->Sumw2();
	hPtMCfine->Scale(1.0/hPtMCfine->Integral());
	hPtMCfine->Draw("ep");;

	hPtMCfine->SaveAs("MCBptRawYield.png");
	hPtMCfine->Write();




	fout2->Close();





}
