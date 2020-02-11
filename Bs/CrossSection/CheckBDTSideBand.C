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
//#include "uti.h"
using namespace std;

using std::cout;
using std::endl;
#endif

void CheckBDTSideBand(int IsMC){

	gStyle->SetOptStat(0);	

	gStyle->SetPadRightMargin(0.043);
	gStyle->SetPadLeftMargin(0.15);
	gStyle->SetPadTopMargin(0.10);
	gStyle->SetPadBottomMargin(0.10);

	double BsMassPeak = 5.3663;
	double Start = 0.3;
	const int N = 6;
	double step = Start * 2/N;


	double MassLeft = BsMassPeak - Start;

	double MassUp[N] = {5.09,5.19,5.29,5.55,5.65,5.75};
	double MassDown[N] = {5.0,5.10,5.20,5.46,5.56,5.66};

	

	TString MCTag;

	if(IsMC == 0) MCTag ="Data";
	if(IsMC == 1) MCTag ="MC";


	TString infile;

	TString Cut;


	double BptLow[4] = {5,10,15,20};
	double BptHigh[4] = {10,15,20,50};



	if(IsMC == 0) infile = "/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/BsBDTMUONJSONAllMerge/DataCheck.root";
	if(IsMC == 1) infile = "/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/2018Ana/Samples/FinalAnaSamples/PrivateMC-Data-Official-SemiFinal/MC_Bs_PbPb_TMVA_BDT_PbPb.root";


	TFile * fin = new TFile(infile.Data());

	TTree * nt = (TTree *) fin->Get("Bfinder/ntphi");

	nt->AddFriend("hltanalysis/HltTree");
	nt->AddFriend("hiEvtAnalyzer/HiTree");
	nt->AddFriend("skimanalysis/HltTree");
	nt->AddFriend("BDT_pt_15_20");
	nt->AddFriend("BDT_pt_5_10");
	nt->AddFriend("BDT_pt_10_15");	
	nt->AddFriend("BDT_pt_20_50");		
	//nt->AddFriend("Recalculated");

	TString BptCut;

	TString BaseCut = "((hiBin < 181) && Btrk1Pt > 1.0 && Btrk2Pt > 1.0 && Bchi2cl > 0.05 && BsvpvDistance/BsvpvDisErr > 2.6 && Bpt > 5 && abs(Btrk1Eta-0.0) < 2.4  && abs(Btrk2Eta-0.0) < 2.4 && (TMath::Abs(By)<2.4&&TMath::Abs(Bmumumass-3.096916)<0.15&&((abs(Bmu1eta)<1.2&&Bmu1pt>3.5)||(abs(Bmu1eta)>1.2&&abs(Bmu1eta)<2.1&&Bmu1pt>(5.47-1.89*abs(Bmu1eta)))||(abs(Bmu1eta)>2.1&&abs(Bmu1eta)<2.4&&Bmu1pt>1.5))&&((abs(Bmu2eta)<1.2&&Bmu2pt>3.5)||(abs(Bmu2eta)>1.2&&abs(Bmu2eta)<2.1&&Bmu2pt>(5.47-1.89*abs(Bmu2eta)))||(abs(Bmu2eta)>2.1&&abs(Bmu2eta)<2.4&&Bmu2pt>1.5))&&Bmu1TMOneStationTight&&Bmu2TMOneStationTight&&Bmu1InPixelLayer>0&&(Bmu1InPixelLayer+Bmu1InStripLayer)>5&&Bmu2InPixelLayer>0&&(Bmu2InPixelLayer+Bmu2InStripLayer)>5&&Bmu1dxyPV<0.3&&Bmu2dxyPV<0.3&&Bmu1dzPV<20&&Bmu2dzPV<20&&Bmu1isTrackerMuon&&Bmu2isTrackerMuon&&Bmu1isGlobalMuon&&Bmu2isGlobalMuon&&Btrk1highPurity&&Btrk2highPurity&&abs(Btrk1Eta)<2.4&&abs(Btrk2Eta)<2.4&&Btrk1Pt>1.&&Btrk2Pt>1.&&abs(Btktkmass-1.019455)<0.015) && (abs(PVz)<15&&pclusterCompatibilityFilter&&pprimaryVertexFilter) && (Btrk1PixelHit + Btrk1StripHit > 10) && (Btrk2PixelHit + Btrk2StripHit > 10) && (Btrk1PtErr/Btrk1Pt < 0.1)&& (Btrk2PtErr/Btrk2Pt < 0.1) && Btrk1Chi2ndf/(Btrk1nStripLayer+Btrk1nPixelLayer) < 0.18 && Btrk2Chi2ndf/(Btrk2nStripLayer+Btrk2nPixelLayer) < 0.18 && (HLT_HIL3Mu0NHitQ10_L2Mu0_MAXdR3p5_M1to5_v1) && (abs(PVz)<15&&pclusterCompatibilityFilter&&pprimaryVertexFilter && phfCoincFilter2Th4))";

	TString FinalCut;

	TH1D * BMassHistobkgd[4][N];

	TH1D * BMassHistobkgdRatio[4][N];
	
	TString XName[4] = {"BDT_pt_5_10","BDT_pt_10_15","BDT_pt_15_20","BDT_pt_20_50"};
	TString HisName;

	TString TitleName[4] = {"B_{s} p_{T} = 5 - 10 GeV/c","B_{s} p_{T} = 10 - 15 GeV/c","B_{s} p_{T} = 15 - 20 GeV/c","B_{s} p_{T} = 20 - 50 GeV/c"};

	double RatioBin[24]={-0.6,-0.5,-0.45,-0.40,-0.375,-0.35,-0.325,-0.30,-0.275,-0.25,-0.225,-0.20,-0.175,-0.15,-0.125,-0.10,-0.075,-0.05,-0.025,0,0.075,0.15,0.30,1.0};




	for(int i = 0; i < N ; i++){



		Cut = Form(" (Bmass > %f && Bmass < %f)",MassDown[i], MassUp[i]);
		if(IsMC == 1) Cut = Form("Bmass > %f && Bmass < %f && Bgen !=23333",MassDown[i], MassUp[i]);

		HisName = Form("Bmass Range %.2f - %.2f GeV/c^{2}",MassDown[i],MassUp[i]);


		for(int j = 0; j < 4; j++){

			BptCut = Form("Bpt > %f && Bpt < %f",BptLow[j],BptHigh[j]);

			FinalCut = Form("(%s) && (%s)",Cut.Data(),BptCut.Data());
			cout << "Now Working on  i = " << i << "   j = " << j << endl; 




			BMassHistobkgdRatio[j][i] = new TH1D(Form("BMassHistobkgdRatio%d%d",j,i),"",23,RatioBin);


			BMassHistobkgdRatio[j][i]->SetTitle(HisName.Data());
			BMassHistobkgdRatio[j][i]->GetXaxis()->SetTitle(XName[j].Data());
			BMassHistobkgdRatio[j][i]->GetYaxis()->SetTitle("Normalized Ratio Counts");

			BMassHistobkgdRatio[j][i]->Sumw2();
			BMassHistobkgdRatio[j][i]->Scale(1.0/BMassHistobkgdRatio[j][i]->Integral());	
			BMassHistobkgdRatio[j][i]->SetMarkerSize(1.0);
			BMassHistobkgdRatio[j][i]->SetMarkerStyle(20);
			BMassHistobkgdRatio[j][i]->SetMarkerColor(i+1);
			BMassHistobkgdRatio[j][i]->GetYaxis()->SetTitleOffset(2.2);


			BMassHistobkgd[j][i] = new TH1D(Form("BMassHistobkgd%d%d",j,i),"",40,-1.0,1.0);
			BMassHistobkgd[j][i]->SetTitle(HisName.Data());
			BMassHistobkgd[j][i]->GetXaxis()->SetTitle(XName[j].Data());
			BMassHistobkgd[j][i]->GetYaxis()->SetTitle("Normalized Counts");
			nt->Project(Form("BMassHistobkgd%d%d",j,i),XName[j].Data(),FinalCut.Data());
			nt->Project(Form("BMassHistobkgdRatio%d%d",j,i),XName[j].Data(),FinalCut.Data());

			BMassHistobkgd[j][i]->Sumw2();
			BMassHistobkgd[j][i]->Scale(1.0/BMassHistobkgd[j][i]->Integral());	
			BMassHistobkgd[j][i]->SetMarkerSize(1.0);
			BMassHistobkgd[j][i]->SetMarkerStyle(20);
			BMassHistobkgd[j][i]->SetMarkerColor(i+1);
			BMassHistobkgd[j][i]->GetYaxis()->SetTitleOffset(2.2);


		}

	}


	TCanvas *c = new TCanvas("c","c",600,600);
	c->cd();

	TString SaveName;
	TLegend* leg[4]; 



	for(int j = 0; j < 4; j++){
		leg[j] = new TLegend(0.60,0.55,0.92,0.85);
		SaveName = Form("Plots/%s%s.png",MCTag.Data(),XName[j].Data());

		for(int i = 0; i < N ; i++){

			if(i == 0){
				BMassHistobkgd[j][i]->SetTitle(TitleName[j].Data());
				BMassHistobkgd[j][i]->GetXaxis()->CenterTitle();
				BMassHistobkgd[j][i]->GetYaxis()->CenterTitle();
				BMassHistobkgd[j][i]->Draw();
			}

			if(i > 0)	BMassHistobkgd[j][i]->Draw("epSAME");

			leg[j]->AddEntry(BMassHistobkgd[j][i],Form("Bmass = %.2f - %.2f",MassDown[i],MassUp[i]),"pl");
		}


		leg[j]->Draw("same");  
		c->SaveAs(SaveName.Data());
	}

	TH1D * Ratio[4][N];
	TLegend* legRatio[4]; 
	int NReorder[N];
	int NthBin = 3;

	for(int i = 0; i < N ; i++){
		if(i == 0) NReorder[0] = NthBin;
		if(i == NthBin) NReorder[i] = 0;
		if(i != 0 && i != NthBin)  NReorder[i]  = i;
	}


	for(int j = 0; j < 4; j++){
		for(int i = 0; i < N ; i++){
	//		Ratio[j][NReorder[i]] = (TH1D * ) BMassHistobkgdRatio[j][NReorder[i]]->Clone(Form("Ratio_%d_%d",j,i));
			Ratio[j][i] = (TH1D * ) BMassHistobkgdRatio[j][i]->Clone(Form("Ratio_%d_%d",j,i));

		}
	}

	TString SaveNameRatio;
	TLine *unity = new TLine(-1.0,1,1,1);
	unity->SetLineStyle(2);
	unity->SetLineWidth(2);
	unity->SetLineColor(2);

	for(int j = 0; j < 4; j++){
		legRatio[j] = new TLegend(0.60,0.55,0.92,0.85);
		SaveNameRatio = Form("Plots/%sRatio%s.png",MCTag.Data(),XName[j].Data());

		for(int i = 1; i < N ; i++){
			Ratio[j][i]->Sumw2();
			Ratio[j][i]->Sumw2();
			Ratio[j][i]->Divide(Ratio[j][0]);
			if(i == 1) Ratio[j][i]->Draw();
			if(i > 1) Ratio[j][i]->Draw("SAME");

			legRatio[j]->AddEntry(Ratio[j][i],Form("Bmass = %.2f - %.2f",MassDown[i],MassUp[i]),"pl");
		}


		unity->Draw("SAME");
		legRatio[j]->Draw("same");  
		c->SaveAs(SaveNameRatio.Data());
	}



	TFile * fout = new TFile("output.root","RECREATE");
	fout->cd();

	for(int j = 0; j < 4; j++){
		for(int i = 0; i < N ; i++){

			BMassHistobkgd[j][i]->Write();
			Ratio[j][i]->Write();
		}
	}

	fout->Close();




}


