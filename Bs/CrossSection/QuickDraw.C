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



	TCanvas * c1 = new TCanvas("c1","c1",600,600);
	c1->cd();
	c1->SetLogy();

	TFile * Eff090 =  new TFile("EffFine_0_90.root");
	Eff090->cd();

	TH1D * h090 = (TH1D * ) Eff090->Get("hEffFineInv");
	h090->GetXaxis()->SetTitle("B_{s} p_{T} (GeV/c)");
	h090->GetYaxis()->SetTitle("1/(eff x acc)");
	h090->SetTitle("Centrality 0 - 90%");

	h090->GetYaxis()->SetTitleOffset(1.3);	
	h090->GetXaxis()->CenterTitle();
	h090->GetYaxis()->CenterTitle();
	h090->SetMarkerSize(0.5);
	h090->SetMinimum(1.0);
	h090->SetMaximum(1.0e+07);

	h090->Draw("ep");

	TLine *l1 = new TLine(6.2,1,6.2,1.0e+07);
	l1->SetLineStyle(2);
	l1->SetLineWidth(2);
	l1->SetLineColor(2);
	l1->Draw("SAME");


	c1->SaveAs("EffInv/EffInv_0_90.png");




	TFile * Eff030 =  new TFile("EffFine_0_30.root");
	Eff030->cd();

	TH1D * h030 = (TH1D * ) Eff030->Get("hEffFineInv");
	h030->GetXaxis()->SetTitle("B_{s} p_{T} (GeV/c)");
	h030->GetYaxis()->SetTitle("1/(eff x acc)");
	h030->SetTitle("Centrality 0 - 90%");

	h030->GetYaxis()->SetTitleOffset(1.3);	
	h030->GetXaxis()->CenterTitle();
	h030->GetYaxis()->CenterTitle();
	h030->SetMarkerSize(0.5);
	h030->SetMinimum(1.0);
	h030->SetMaximum(1.0e+07);

	h030->Draw("ep");

	TLine *l2 = new TLine(6.2,1,6.2,1.0e+07);
	l2->SetLineStyle(2);
	l2->SetLineWidth(2);
	l2->SetLineColor(2);
	l2->Draw("SAME");




	c1->SaveAs("EffInv/EffInv_0_30.png");



	TFile * Eff3090 =  new TFile("EffFine_30_90.root");
	Eff3090->cd();

	TH1D * h3090 = (TH1D * ) Eff3090->Get("hEffFineInv");
	h3090->GetXaxis()->SetTitle("B_{s} p_{T} (GeV/c)");
	h3090->GetYaxis()->SetTitle("1/(eff x acc)");
	h3090->SetTitle("Centrality 0 - 90%");

	h3090->GetYaxis()->SetTitleOffset(1.3);	
	h3090->GetXaxis()->CenterTitle();
	h3090->GetYaxis()->CenterTitle();
	h3090->SetMarkerSize(0.5);
	h3090->SetMinimum(1.0);
	h3090->SetMaximum(1.0e+07);
	h3090->Draw("ep");

	TLine *l3 = new TLine(9.1,1,9.1,1.0e+07);
	l3->SetLineStyle(2);
	l3->SetLineWidth(2);
	l3->SetLineColor(2);
	l3->Draw("SAME");

	c1->SaveAs("EffInv/EffInv_30_90.png");


	TCanvas * c2 = new TCanvas("c2","c2",600,600);
	c2->cd();




	TFile * finEffInfo = new TFile("10BptBinsTnP/EffInfo_0_90.root");
	finEffInfo->cd();

	TTree * EffInfoTree = (TTree * ) finEffInfo->Get("EffInfoTree");


	TH2D * hBptCentData = new TH2D("hBptCentData","hBptCentData",9,0,90,5,5,10);

	EffInfoTree->Project("hBptCentData","BptNew:hiBinNew/2"," BptNew >5 && BptNew < 10" );
	hBptCentData->GetXaxis()->SetTitle("Centrality (%)");
	hBptCentData->GetYaxis()->SetTitle("B_{s} p_{T} (GeV/c)");
	hBptCentData->GetXaxis()->CenterTitle();
	hBptCentData->GetYaxis()->CenterTitle();
	hBptCentData->GetYaxis()->SetTitleOffset(1.2);


	hBptCentData->Draw("COLZ");

	TLine *l4 = new TLine(30,5,30,10);
	l4->SetLineStyle(2);
	l4->SetLineWidth(2);
	l4->SetLineColor(1);
	l4->Draw("SAME");


	c2->SaveAs("EffInv/hBptCentData.png");



	TH2D * hBptCentDataFull = new TH2D("hBptCentDataFull","hBptCentDataFull",9,0,90,45,5,50);

	EffInfoTree->Project("hBptCentDataFull","BptNew:hiBinNew/2"," BptNew >5 && BptNew < 50" );
	hBptCentDataFull->GetXaxis()->SetTitle("Centrality (%)");
	hBptCentDataFull->GetYaxis()->SetTitle("B_{s} p_{T} (GeV/c)");
	hBptCentDataFull->GetXaxis()->CenterTitle();
	hBptCentDataFull->GetYaxis()->CenterTitle();
	hBptCentDataFull->GetYaxis()->SetTitleOffset(1.2);


	hBptCentDataFull->Draw("COLZ");

	TLine *l7 = new TLine(30,5,30,50);
	l7->SetLineStyle(2);
	l7->SetLineWidth(2);
	l7->SetLineColor(1);
	l7->Draw("SAME");


	TLine *l8 = new TLine(0,10,180,10);
	l8->SetLineStyle(2);
	l8->SetLineWidth(2);
	l8->SetLineColor(1);
	l8->Draw("SAME");




	c2->SaveAs("EffInv/hBptCentDataFull.png");




	TH2D * hBptCentDataMass = new TH2D("hBptCentDataMass","hBptCentDataMass",9,0,90,5,5,10);

	EffInfoTree->Project("hBptCentDataMass","BptNew:hiBinNew/2"," BptNew >5 && BptNew < 10  && TMath::Abs(BmassNew - 5.36682) < 0.22" );
	hBptCentDataMass->GetXaxis()->SetTitle("Centrality (%)");
	hBptCentDataMass->GetYaxis()->SetTitle("B_{s} p_{T} (GeV/c)");
	hBptCentDataMass->GetXaxis()->CenterTitle();
	hBptCentDataMass->GetYaxis()->CenterTitle();
	hBptCentDataMass->GetYaxis()->SetTitleOffset(1.2);


	hBptCentDataMass->Draw("COLZ");

	TLine *l14 = new TLine(30,5,30,10);
	l14->SetLineStyle(2);
	l14->SetLineWidth(2);
	l14->SetLineColor(1);
	l14->Draw("SAME");


	c2->SaveAs("EffInv/hBptCentDataMass.png");


	TH2D * hBptByData = new TH2D("hBptByData","hBptByData",45,5,50,24,0,2.4);

	EffInfoTree->Project("hBptByData","ByNew:BptNew"," BptNew >5 && BptNew < 50 && TMath::Abs(BmassNew - 5.36682) < 0.08" );
	hBptByData->GetXaxis()->SetTitle("B_{s} p_{T} (GeV/c)");
	hBptByData->GetYaxis()->SetTitle("B |y|");
	hBptByData->GetXaxis()->CenterTitle();
	hBptByData->GetYaxis()->CenterTitle();
	hBptByData->GetYaxis()->SetTitleOffset(1.2);

	hBptByData->Draw("COLZ");

	TLine *l9 = new TLine(5,1.5,10,1.5);
	l9->SetLineStyle(2);
	l9->SetLineWidth(2);
	l9->SetLineColor(1);
	l9->Draw("SAME");


	TLine *l10 = new TLine(10,0,10,1.5);
	l10->SetLineStyle(2);
	l10->SetLineWidth(2);
	l10->SetLineColor(1);
	l10->Draw("SAME");

	c2->SaveAs("EffInv/hBptByData.png");


	TFile * finEffFine = new TFile("EffFine_0_90.root");
	finEffFine->cd();

	TH2D * EffBptByMC = (TH2D *) finEffFine->Get("EffBptByMC");

	EffBptByMC->GetYaxis()->SetTitle("B |y|");
	EffBptByMC->GetXaxis()->SetTitle("B_{s} p_{T} (GeV/c)");
	EffBptByMC->GetXaxis()->CenterTitle();
	EffBptByMC->GetYaxis()->CenterTitle();
	EffBptByMC->GetYaxis()->SetTitleOffset(1.2);

	EffBptByMC->Draw("COLZ");
	l9->Draw("SAME");
	l10->Draw("SAME");

	c2->SaveAs("EffInv/hBptByMC.png");



	TH2D * hBptCentMC = (TH2D *) finEffFine->Get("hBptCent");


	hBptCentMC->GetXaxis()->SetTitle("Centrality (%)");
	hBptCentMC->GetYaxis()->SetTitle("B_{s} p_{T} (GeV/c)");
	hBptCentMC->GetXaxis()->CenterTitle();
	hBptCentMC->GetYaxis()->CenterTitle();
	hBptCentMC->GetYaxis()->SetTitleOffset(1.2);


	hBptCentMC->Draw("COLZ");

	TLine *l5 = new TLine(30,5,30,10);
	l5->SetLineStyle(2);
	l5->SetLineWidth(2);
	l5->SetLineColor(1);
	l5->Draw("SAME");


	c2->SaveAs("EffInv/hBptCentMC.png");

	TH2D * EffBptByInvhyperfine = (TH2D *) finEffFine->Get("EffBptByInvhyperfine");



	EffBptByInvhyperfine->GetXaxis()->SetTitle("Centrality (%)");
	EffBptByInvhyperfine->GetYaxis()->SetTitle("B_{s} p_{T} (GeV/c)");
	EffBptByInvhyperfine->GetXaxis()->CenterTitle();
	EffBptByInvhyperfine->GetYaxis()->CenterTitle();
	EffBptByInvhyperfine->GetYaxis()->SetTitleOffset(1.2);
	EffBptByInvhyperfine->GetZaxis()->SetTitleOffset(0.2);
	c2->cd();
	EffBptByInvhyperfine->Draw("COLZ");

	TLine *l6;
	double yint[3] = {2.0,2.1,2.2};
	double xint[3] = {9.5,9.75,10};

	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){

			l6 = new TLine(5,yint[j],xint[i],0);
			l6->SetLineStyle(1);
			l6->SetLineWidth(2);
			l6->SetLineColor(i*3+j);
			l6->Draw("SAME");
		}
	}
	c2->SetLogz();
	c2->SaveAs("EffInv/EffBptByInvhyperfine.png");


	TCanvas * c3 = new TCanvas("c3","c3",2400,800);
	c3->Divide(3,1);




	TFile * finGolden = new TFile("10BptBinsTnP/EffInfo_0_90.root");


	finGolden->cd();

	TTree * EffInfoTreeOld = (TTree * ) finGolden->Get("EffInfoTree");
	TH1D * BmassGolden = new TH1D("BmassGolden","BmassGolden",50,5,6);
	EffInfoTreeOld->Project("BmassGolden","BmassNew","BptNew > 0");


	TFile * finMUON = new TFile("/Users/zhaozhongshi/Desktop/TempDownload/EffInfo_0_90.root");
	finMUON->cd();


	TTree * EffInfoTreeNew = (TTree * ) finMUON->Get("EffInfoTree");
	cout << "Entries = " << EffInfoTreeNew->GetEntries() << endl;
	TH1D * BmassMUON = new TH1D("BmassMUON","BmassMUON",50,5,6);
	EffInfoTreeNew->Project("BmassMUON","BmassNew");
	TH1D * BmassDiff = new TH1D("BmassDiff","BmassDiff",50,5,6);
	EffInfoTreeNew->Project("BmassDiff","BmassNew");

	BmassDiff->Add(BmassGolden,-1);


	BmassGolden->GetYaxis()->SetTitle("Counts");
	BmassGolden->GetXaxis()->SetTitle("B_{s} Mass (GeV/c^{2})");
	BmassGolden->GetXaxis()->CenterTitle();
	BmassGolden->GetYaxis()->CenterTitle();
	BmassGolden->GetYaxis()->SetTitleOffset(1.2);
	BmassGolden->SetMarkerSize(2);
	BmassGolden->SetMarkerStyle(20);
	BmassGolden->SetMarkerColor(1);
	BmassGolden->SetLineColor(1);



	BmassMUON->GetYaxis()->SetTitle("Counts");
	BmassMUON->GetXaxis()->SetTitle("B_{s} Mass (GeV/c^{2})");
	BmassMUON->GetXaxis()->CenterTitle();
	BmassMUON->GetYaxis()->CenterTitle();
	BmassMUON->GetYaxis()->SetTitleOffset(1.2);
	BmassMUON->SetMarkerSize(2);
	BmassMUON->SetMarkerStyle(20);
	BmassMUON->SetMarkerColor(1);
	BmassMUON->SetLineColor(1);


	BmassDiff->GetYaxis()->SetTitle("Counts");
	BmassDiff->GetXaxis()->SetTitle("B_{s} Mass (GeV/c^{2})");
	BmassDiff->GetXaxis()->CenterTitle();
	BmassDiff->GetYaxis()->CenterTitle();
	BmassDiff->GetYaxis()->SetTitleOffset(1.2);
	BmassDiff->SetMarkerSize(2);
	BmassDiff->SetMarkerStyle(20);
	BmassDiff->SetMarkerColor(1);
	BmassDiff->SetLineColor(1);



	c3->cd(1);
	BmassGolden->Draw("ep");
	c3->cd(2);
	BmassMUON->Draw("ep");
	c3->cd(3);
	BmassDiff->Draw("ep");

	c3->SaveAs("EffInv/NewOldData.png");



	TCanvas *c4 = new TCanvas("c4","c4",600,600);
	c4->cd();



	TString Eff1Name[3]={"Files4bins/EffWeightedLLOG_0_90.root","Files4bins/EffWeightedLLOG_0_30.root","Files4bins/EffWeightedLLOG_30_90.root"};
	TString Eff2Name[3]={"ReAnaEff_0_90.root","ReAnaEff_0_30.root","ReAnaEff_30_90.root"};
	TString Effoutname[3] = {"EffInv/InvComp_0_90.png","EffInv/InvComp_0_30.png","EffInv/InvComp_30_90.png"};
	TString TitleName[3] = {"Cent: 0 - 90%","Cent: 0 - 30%","Cent: 30 - 90%"};

	TLatex* texChi;

	TLegend* leg[3];
	for(int j = 0; j < 3; j++){



		TFile * finEff1 = new TFile(Eff1Name[j].Data());
		finEff1->cd();

		TH1D * hEff1 = (TH1D *) finEff1->Get("hEff");
		hEff1->Sumw2();
		TH1D * EffInvComp = (TH1D *) hEff1->Clone("EffInvComp");
		EffInvComp->Reset();
		double EffInvValue;
		double EffInvValueErr;

		for(int i = 0; i < EffInvComp->GetNbinsX(); i++)
		{
			EffInvValue = 1.0/hEff1->GetBinContent(i+1);
			EffInvValueErr =  hEff1->GetBinError(i+1)/(hEff1->GetBinContent(i+1) * hEff1->GetBinContent(i+1));
			cout << "EffInvValue = " << EffInvValue << "   EffInvValueErr = " << EffInvValueErr << endl;
			EffInvComp->SetBinContent(i+1,EffInvValue);
			EffInvComp->SetBinError(i+1,EffInvValueErr);
		}


		EffInvComp->GetYaxis()->SetTitle("1/(acc x eff)");
		EffInvComp->GetXaxis()->SetTitle("B_{s} p_{T} (GeV/c)");
		EffInvComp->GetXaxis()->CenterTitle();
		EffInvComp->GetYaxis()->CenterTitle();
		EffInvComp->GetYaxis()->SetTitleOffset(1.2);
		EffInvComp->SetMarkerSize(1);
		EffInvComp->SetMarkerStyle(20);
		EffInvComp->SetMarkerColor(2);
		EffInvComp->SetLineColor(2);


		TFile * finEff2 = new TFile(Eff2Name[j].Data());
		finEff2->cd();
		TH1D * hEff2 =  (TH1D *)  finEff2->Get("hEff");

		hEff2->GetYaxis()->SetTitle("1/(acc x eff)");
		hEff2->GetXaxis()->SetTitle("B_{s} p_{T} (GeV/c)");
		hEff2->GetXaxis()->CenterTitle();
		hEff2->GetYaxis()->CenterTitle();
		hEff2->GetYaxis()->SetTitleOffset(1.2);
		hEff2->SetMarkerSize(1);
		hEff2->SetMarkerStyle(20);
		hEff2->SetMarkerColor(4);
		hEff2->SetLineColor(4);
		hEff2->SetMinimum(1);

		leg[j] = new TLegend(0.39,0.60,0.59,0.87,NULL,"brNDC");
		leg[j]->SetBorderSize(0);
		leg[j]->SetTextSize(0.04);
		leg[j]->SetTextFont(42);
		leg[j]->SetFillStyle(0);

		leg[j]->AddEntry(EffInvComp,"Preapprove Results","pl");
		leg[j]->AddEntry(hEff2,"Nuno's Proposal","pl");


		EffInvComp->SetMinimum(1);
		EffInvComp->SetMaximum(EffInvComp->GetBinContent(1)*1.5);

		EffInvComp->Draw("ep");
		hEff2->Draw("epSAME");
		leg[j]->Draw("SAME");

		texChi = new TLatex(0.30,0.95, TitleName[j].Data());
		texChi->SetNDC();
		texChi->SetTextAlign(12);
		texChi->SetTextSize(0.06);
		texChi->SetTextFont(42);
		texChi->SetTextColor(1);

		texChi->Draw("SAME");
		c4->SetLogy();
		c4->SaveAs(Effoutname[j].Data());
	}




	TCanvas *c5 = new TCanvas("c5","c5",1200,600);
	c5->Divide(2,1);


	TString DataName[3]={"10BptBinsTnP/EffInfo_0_90.root","10BptBinsTnP/EffInfo_0_30.root","10BptBinsTnP/EffInfo_30_90.root"};
	TString MCName[3]={"EffFine_0_90.root","EffFine_0_30.root","EffFine_30_90.root"};
	TString OutDataMCName[3]={"EffInv/DataMCPt_0_90.png","EffInv/DataMCPt_0_30.png","EffInv/DataMCPt_30_90.png"};


	for(int j = 0; j < 3; j++){


		TFile * finEffInfo2 = new TFile(DataName[j].Data());
		finEffInfo2->cd();

		TTree * EffInfoTree2 = (TTree * ) finEffInfo2->Get("EffInfoTree");

		TH1D * hPtDataDis = new TH1D("hPtDataDis","hPtDataDis",90,5,50);

		EffInfoTree2->Project("hPtDataDis","BptNew");

		TFile * Eff090More =  new TFile(MCName[j].Data());
		Eff090More->cd();

		TH1D * hPtMCComp = (TH1D *) Eff090More->Get("hPtMCFine");

		hPtDataDis->GetYaxis()->SetTitle("Data Candidates Counts");
		hPtDataDis->GetXaxis()->SetTitle("B_{s} p_{T} (GeV/c)");
		hPtDataDis->GetXaxis()->CenterTitle();
		hPtDataDis->GetYaxis()->CenterTitle();
		hPtDataDis->GetYaxis()->SetTitleOffset(1.2);
		hPtDataDis->SetMarkerSize(1);
		hPtDataDis->SetMarkerStyle(20);
		hPtDataDis->SetMarkerColor(1);
		hPtDataDis->SetLineColor(1);
		hPtDataDis->SetMinimum(0);
		c5->cd(1);
		hPtDataDis->Draw("ep");


		hPtMCComp->GetYaxis()->SetTitle("MC Candidates Counts");
		hPtMCComp->GetXaxis()->SetTitle("B_{s} p_{T} (GeV/c)");
		hPtMCComp->GetXaxis()->CenterTitle();
		hPtMCComp->GetYaxis()->CenterTitle();
		hPtMCComp->GetYaxis()->SetTitleOffset(1.4);
		hPtMCComp->SetMarkerSize(1);
		hPtMCComp->SetMarkerStyle(20);
		hPtMCComp->SetMarkerColor(1);
		hPtMCComp->SetLineColor(1);
		hPtMCComp->SetMinimum(0);
		c5->cd(2);
		hPtMCComp->Draw("ep");

		c5->SaveAs(OutDataMCName[j]);
	}

}
