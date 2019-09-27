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


void ReweightBptNew(TString inputMC,TString inputFONLL){



	gStyle->SetOptStat(0); 
	TCanvas * c = new TCanvas("c","c",1800,600);
	c->Divide(3,1);
	TCut weighpthat = "(pthatweight)";
	TCut GenCut ="(TMath::Abs(Gy)<2.4&&TMath::Abs(GpdgId)==531&&GisSignal>0 )";


	TFile *finMC = new TFile(inputMC.Data());
	TFile *finFONLL = new TFile(inputFONLL.Data());

	TTree * tMC = (TTree*) finMC->Get("Bfinder/ntGen");
	tMC->AddFriend("hltanalysis/HltTree");
	tMC->AddFriend("hiEvtAnalyzer/HiTree");  
	tMC->AddFriend("skimanalysis/HltTree");

	TH1D * GptMC = new TH1D("GptMC","GptMC",nBinsReweight,ptBinsReweight);

	GptMC->GetYaxis()->SetTitleOffset(1.1);

	tMC->Project("GptMC","Gpt",TCut(weighpthat)*TCut(GenCut));
	GptMC->Sumw2();

	divideBinWidth(GptMC);

	GptMC->GetXaxis()->SetTitle("Gen p_{T} (GeV)");
	GptMC->GetYaxis()->SetTitle("PYTHIA, #entries ");
	GptMC->SetTitle("");
	GptMC->Sumw2();
	GptMC->Scale(1.0/GptMC->Integral());
	GptMC->SetMarkerStyle(20);
	GptMC->SetMarkerColor(kBlack);
	GptMC->SetMarkerSize(1);
	c->cd(1);
	gPad->SetLogy();
	GptMC->Draw("ep");

	TF1 *f1;


	TString BptReweightFunc;

	int LastBin;

	double BinX;
	double BinY;

	const int NMethod = 5;

	TString SpecHisName[NMethod] = {"hNominalPP","hVariationPP","hNominalTAMU","hVariationTAMU","hNominalTAMUComb"};

	TFile * Inter = new TFile("PtSpectraReweight.root","RECREATE");
	Inter->cd();

	for(int i = 0; i < NMethod; i++){

		TH1D * SpecHis =  (TH1D *) finFONLL->Get(SpecHisName[i].Data());


		//SpecHis->Rebin(nBinsReweight,"SpecHis",ptBinsReweight);

		//LastBin = SpecHis->GetXaxis()->FindBin(ptBinsReweight[nBinsReweight+1]);
		LastBin = SpecHis->GetXaxis()->FindBin(50);

		//cout << "ptBinsReweight[nBinsReweight+1] = " << ptBinsReweight[nBinsReweight+1] << endl;
		
		cout << "i = " << i  << "   Last Bin = " << LastBin << endl;

		TH1D * SpecRebin = new TH1D(SpecHisName[i].Data(),SpecHisName[i].Data(),nBinsReweight,ptBinsReweight);

		for(int j = 0; j < LastBin; j++){

			BinY = SpecHis->GetBinContent(j+1);
			BinX = SpecHis->GetBinCenter(j+1);

			//cout << "BinX = " << BinX << "   BinY = " << BinY << endl;
			
			SpecRebin->Fill(BinX,BinY);
		}
	
		for(int j = 0; j < SpecRebin->GetNbinsX(); j++){
			
			SpecRebin->SetBinError(j+1,0);
		
		}



		divideBinWidth(SpecRebin);


		SpecRebin->Scale(1.0/SpecRebin->Integral());



		SpecRebin->GetYaxis()->SetTitleOffset(1.4);
		SpecRebin->GetXaxis()->SetTitle(Form("%s p_{T} (GeV)",SpecHisName[i].Data()));
		SpecRebin->GetYaxis()->SetTitle(Form("%s_pp, #entries ",SpecHisName[i].Data()));
		SpecRebin->SetMarkerStyle(20);
		SpecRebin->SetMarkerColor(kBlack);
		SpecRebin->SetMarkerSize(1);
		c->cd(2);
		gPad->SetLogy();
		SpecRebin->SetTitle("");
		SpecRebin->Draw("ep");
		SpecRebin->Write();


		TH1D * BptRatio = (TH1D *) SpecRebin->Clone("BptRatio");
		BptRatio->GetYaxis()->SetTitleOffset(1.3); 
		BptRatio->Divide(GptMC);
		c->cd(3);

		f1 =  new TF1("f1"," ([0])*TMath::Exp(-[1]*x) + [2]",5,50);

		f1->SetParLimits(0,0,100);	


		f1->SetParLimits(1,0,10);
		//f1->FixParameter(1,0);
		f1->SetParLimits(2,0,1);

		BptRatio->Fit(f1,"R");
		BptRatio->GetXaxis()->SetTitle("B_{s}^{0} p_{T}");
		BptRatio->GetYaxis()->SetTitle(Form("%s_pp/PYTHIA",SpecHisName[i].Data()));
		BptRatio->SetMarkerStyle(20);
		BptRatio->SetMarkerColor(kBlack);
		BptRatio->SetMarkerSize(1);
		BptRatio->SetMinimum(0);
		BptRatio->SetMaximum(2.5);

		float Chi2 =  f1->GetChisquare();
		int nDOF = f1->GetNDF();
		float nChi2 =  Chi2/nDOF;
		// float nChi2 = f->GetChisquare();
		int nDigit_chi2BakerCousins = 2;
		int nDigit_nChi2 = 2;
		nChi2 = roundToNdigit(nChi2);
		nDigit_chi2BakerCousins = 2;


		cout << "Chi2 = " <<  Chi2 << "  nChi2 = " << nChi2 << endl;

		nDigit_nChi2 = sigDigitAfterDecimal(nChi2);
		TLatex* texChi = new TLatex(0.35,0.70, Form("#chi^{2}/nDOF: %.*f/%d = %.*f", nDigit_chi2BakerCousins, Chi2, nDOF, nDigit_nChi2, nChi2));
		texChi->SetNDC();
		texChi->SetTextAlign(12);
		texChi->SetTextSize(0.04);
		texChi->SetTextFont(42);
		texChi->Draw("SAME");
		BptReweightFunc = Form("(%f)*TMath::Exp(-%f * x) + %f",f1->GetParameter(0),f1->GetParameter(1),f1->GetParameter(2));
		cout << "Bpt Func = " << BptReweightFunc.Data() << endl;

		ofstream outfiledat(Form("CutVar%d.dat",i));

		outfiledat << "Par 0 = " << f1->GetParameter(0) <<  " Par 0 Err =  " << f1->GetParError(0) << endl;
		outfiledat << "Par 1 = " << f1->GetParameter(1) <<  " Par 1 Err =  " << f1->GetParError(1) << endl;
		outfiledat << "Par 2 = " << f1->GetParameter(2) <<  " Par 2 Err =  " << f1->GetParError(2) << endl;

		c->SaveAs(Form("plotReweight/BptReweigt%s.png",SpecHisName[i].Data()));
		c->SaveAs(Form("plotReweight/BptReweigt%s.pdf",SpecHisName[i].Data()));

	}
	Inter->Close();






}

int main(int argc, char *argv[])
{
	if((argc !=3))
	{
		std::cout << "Wrong number of inputs" << std::endl;
		return 1;
	}

	if(argc == 3)
		ReweightBptNew(argv[1],argv[2]);
	return 0;
}


