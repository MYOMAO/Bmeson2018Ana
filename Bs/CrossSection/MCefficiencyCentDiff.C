#include "uti.h"
#include "parametersDiff.h"
#include "TMath.h"

using namespace std;

Double_t setparam0=100.;
Double_t setparam1=1.865;
Double_t setparam2=0.03;
Double_t setparam10=0.005;
Double_t setparam8=0.1;
Double_t setparam9=0.1;
Double_t fixparam1=1.865;
Double_t minhisto=	1.7;
Double_t maxhisto=2.0;
Double_t nbinsmasshisto=60;
Double_t binwidthmass=(maxhisto-minhisto)/nbinsmasshisto;

TString weightfunctiongen = "1";
TString weightfunctionreco = "1";
Float_t hiBinMin,hiBinMax,centMin,centMax;

int _nBins = nBins;
double *_ptBins = ptBins;
double *_sf_pp = sf_pp;
double *_sf_pbpb = sf_pbpb;
int customizedOpt = 0; //1: remove pt weight, 2: do tk eta weihting

void MCefficiencyCentDiff(int isPbPb=0, TString inputmc="", TString selmcgen="", TString selmcgenacceptance="", TString cut_recoonly="", TString cut="", TString _varExp = "", TString _varGenExp = "", TString label="", TString outputfile="", TString outplotf="", int PbPbweight=0, Float_t centmin=0., Float_t centmax=100.)
{    
	int dofine = 0;
	TString varExp = _varExp;
	TString varGenExp = _varGenExp;
	std::string str = _varExp.Data();
	std::size_t found = str.find("Bpt");
	if (found!=std::string::npos){
		varExp = "Bpt";
	}
	found = str.find("noPtWeight");
	if (found!=std::string::npos){
		customizedOpt = 1;
	}
	found = str.find("tkEtaWeight");
	if (found!=std::string::npos){
		customizedOpt = 2;
	}
	if(_varExp == "Bpt750" || _varExp == "Bpt750_noPtWeight"){
		_nBins = nBins750;
		_ptBins = ptBins750;
		_sf_pp = sf_pp_750;
	}
	if(_varExp == "Bpt750_acc"){
		_nBins = nBins750_acc;
		_ptBins = ptBins750_acc;
		_sf_pp = sf_pp_750_acc;
	}
	if(_varExp == "BptCutBase"){
		_sf_pp = sf_pp_CutBase;
	}
	if(_varExp == "BptPbPbBDT"){
		_sf_pp = sf_pp_PbPbBDT;
	}
	if(_varExp == "BptOLDPbPbBDT"){
		_sf_pp = sf_pp_OldPbPbBDT;
	}
	if(_varExp == "abs(By)"){
		_nBins = nBinsY;
		_ptBins = ptBinsY;
		_sf_pp = sf_pp_Y;
	} 

	str = label.Data();
	found = str.find("Fine");
	if (found!=std::string::npos){MCefficiencyCentDiff.C
		_nBins = nBinsFine;
		_ptBins = ptBinsFine;
	}
	if(label=="ppInc"){
		_nBins = nBinsInc;
		_ptBins = ptBinsInc;
	}

	hiBinMin = centmin*2;
	hiBinMax = centmax*2;
	centMin = centmin;
	centMax = centmax;

	if(isPbPb==1)
	{
		selmcgen = selmcgen+Form("&&hiBin>=%f&&hiBin<=%f",hiBinMin,hiBinMax);
		selmcgenacceptance=selmcgenacceptance+Form("&&hiBin>=%f&&hiBin<=%f",hiBinMin,hiBinMax);
		cut=cut+Form("&&hiBin>=%f&&hiBin<=%f",hiBinMin,hiBinMax);
		cut_recoonly=cut_recoonly+Form("&&hiBin>=%f&&hiBin<=%f ",hiBinMin,hiBinMax);
	}

	printf("############## input parameters\n");
	std::cout<<"selmcgen = "<<selmcgen<<std::endl;
	std::cout<<"selmcgenacceptance = "<<selmcgenacceptance<<std::endl;
	std::cout<<"cut = "<<cut<<std::endl;
	std::cout<<"cut_recoonly = "<<cut_recoonly<<std::endl;
	std::cout<<"PbPbweight = "<<PbPbweight<<std::endl;
	printf("############## input parameters\n");

	gStyle->SetOptTitle(0);
	gStyle->SetOptStat(0);
	gStyle->SetEndErrorSize(0);
	gStyle->SetMarkerStyle(20);

	TFile* infMC = new TFile(inputmc.Data());

	TTree* ntMC = (TTree*)infMC->Get("Bfinder/ntphi");
	TTree* ntGen = (TTree*)infMC->Get("Bfinder/ntGen");


	ntMC->AddFriend("hltanalysis/HltTree");
	ntMC->AddFriend("hiEvtAnalyzer/HiTree");
	ntMC->AddFriend("skimanalysis/HltTree");
//	ntMC->AddFriend("BDT");
	ntMC->AddFriend("CentWeightTree");

	
	ntMC->AddFriend("BDT_pt_15_20");
	ntMC->AddFriend("BDT_pt_10_15");
	ntMC->AddFriend("BDT_pt_20_50");
	ntMC->AddFriend("BDT_pt_5_10");
	
	ntMC->AddFriend("Bfinder/ntGen");
	ntGen->AddFriend("hiEvtAnalyzer/HiTree");
	ntGen->AddFriend("Bfinder/ntphi");

	//ntMC->AddFriend(ntGen);
	//ntMC->AddFriend("ntSkim");
	//ntMC->AddFriend("ntHlt");
	//ntMC->AddFriend("ntHi");
	//ntMC->AddFriend("BDTStage1_pt15to50");

	//ntGen->AddFriend(ntMC);
	//ntGen->AddFriend("ntSkim");
	//ntGen->AddFriend("ntHlt");
	//ntGen->AddFriend("ntHi");
	//ntGen->AddFriend("BDTStage1_pt15to50");

	// weigths
	TCut weighpthat = "pthatweight";
	TCut weightGpt = "1";
	TCut weightBgenpt = "1";
	TCut weightHiBin = "1";
	TCut weightPVz = "1";
	if(PbPbweight==0) {
		weightGpt = weightGpt_pp;
		weightBgenpt = weightBgenpt_pp;
	}
	if(PbPbweight==1) {
		weightGpt = weightGpt_PbPb;
		weightBgenpt = weightBgenpt_PbPb;
		weightHiBin = weightHiBin_PbPb;
		weightPVz = weightPVz_PbPb;
	}
	if(PbPbweight==2) {
		weightGpt = weightGpt_PbPb_NLO;
		weightBgenpt = weightBgenpt_PbPb_NLO;
		weightHiBin = weightHiBin_PbPb;
		weightPVz = weightPVz_PbPb;
	}
	if(PbPbweight==3) {
		weightGpt = weightGpt_PbPb_NominalPP;
		weightBgenpt = weightBgenpt_PbPb_NominalPP;
		weightHiBin = weightHiBin_PbPb;
		weightPVz = weightPVz_PbPb;
	}
	if(PbPbweight==4) {
		weightGpt = weightGpt_PbPb_VariationPP;
		weightBgenpt = weightBgenpt_PbPb_VariationPP;
		weightHiBin = weightHiBin_PbPb;
		weightPVz = weightPVz_PbPb;
	}
	if(PbPbweight==5) {
		weightGpt = weightGpt_PbPb_NominalTAMU;
		weightBgenpt = weightBgenpt_PbPb_NominalTAMU;
		weightHiBin = weightHiBin_PbPb;
		weightPVz = weightPVz_PbPb;
	}
	if(PbPbweight==6) {
		weightGpt = weightGpt_PbPb_VariationTAMU;
		weightBgenpt = weightBgenpt_PbPb_VariationTAMU;
		weightHiBin = weightHiBin_PbPb;
		weightPVz = weightPVz_PbPb;
	}
	if(PbPbweight==7) {
		weightGpt = weightGpt_PbPb_TAMUPP;
		weightBgenpt = weightBgenpt_PbPb_TAMUPP;
		weightHiBin = weightHiBin_PbPb;
		weightPVz = weightPVz_PbPb;
	}
	if(PbPbweight==8) {
		weightGpt = weightGpt_PbPb_DataCentral;
		weightBgenpt = weightBgenpt_PbPb_DataCentral;
		weightHiBin = weightHiBin_PbPb;
		weightPVz = weightPVz_PbPb;
	}
	if(PbPbweight==9) {
		weightGpt = weightGpt_PbPb_Data1PS;
		weightBgenpt = weightBgenpt_PbPb_Data1PS;;
		weightHiBin = weightHiBin_PbPb;
		weightPVz = weightPVz_PbPb;
	}
	if(PbPbweight==10) {
		weightGpt = weightGpt_PbPb_Data1MS;
		weightBgenpt = weightBgenpt_PbPb_Data1MS;
		weightHiBin = weightHiBin_PbPb;
		weightPVz = weightPVz_PbPb;
	}
	if(PbPbweight==11) {
		weightGpt = weightGpt_PbPb_Linear;
		weightBgenpt = weightBgenpt_PbPb_Linear;
		weightHiBin = weightHiBin_PbPb;
		weightPVz = weightPVz_PbPb;
	}
	if(PbPbweight==12) {
		weightGpt = weightGpt_PbPb_Quadratic;
		weightBgenpt = weightBgenpt_PbPb_Quadratic;
		weightHiBin = weightHiBin_PbPb;
		weightPVz = weightPVz_PbPb;
	}
	if(PbPbweight==13) {
		weightGpt = weightGpt_PbPb_LInverse;
		weightBgenpt = weightBgenpt_PbPb_LInverse;
		weightHiBin = weightHiBin_PbPb;
		weightPVz = weightPVz_PbPb;
	}
	if(PbPbweight==14) {
		weightGpt = weightGpt_PbPb_LSqrt;
		weightBgenpt = weightBgenpt_PbPb_LSqrt;
		weightHiBin = weightHiBin_PbPb;
		weightPVz = weightPVz_PbPb;
	}
	if(PbPbweight==15) {
		weightGpt = weightGpt_PbPb_LLog;
		weightBgenpt = weightBgenpt_PbPb_LLog;
		weightHiBin = weightHiBin_PbPb;
		weightPVz = weightPVz_PbPb;
	}
	if(PbPbweight==16) {
		weightGpt = "1";
		weightBgenpt = "1";
		weightHiBin = weightHiBin_PbPb;
		weightPVz = weightPVz_PbPb;
	}






	if(customizedOpt == 1) { weightGpt = "1"; weightBgenpt = "1"; }
	//if(customizedOpt == 2) { weightGpt = weightGpt*TCut(weightGtk1eta); weightBgenpt = weightBgenpt*TCut(weightBtk1eta); }
	if(customizedOpt == 2) { weightGpt = weightGpt*TCut(weightGtk1eta)*TCut(weightGtk2eta); weightBgenpt = weightBgenpt*TCut(weightBtk1eta)*TCut(weightBtk2eta); }

	TH1D* hPtMC = new TH1D("hPtMC","",_nBins,_ptBins);
	TH1D* hPtMCrecoonly = new TH1D("hPtMCrecoonly","",_nBins,_ptBins);
	TH1D* hPtGen = new TH1D("hPtGen","",_nBins,_ptBins);
	TH1D* hPtGenWeighted = new TH1D("hPtGenWeighted","",_nBins,_ptBins);
	TH1D* hPtGenAcc = new TH1D("hPtGenAcc","",_nBins,_ptBins);
	TH1D* hPtGenAccWeighted = new TH1D("hPtGenAccWeighted","",_nBins,_ptBins);
	
	ntMC->Project("hPtMC",varExp.Data(),
			TCut(weighpthat)*TCut(weightBgenpt)*TCut(weightHiBin)*TCut(weightPVz)*(TCut(cut.Data())&&"(Bgen==23333)"));
	ntMC->Project("hPtMCrecoonly",varExp.Data(),
			TCut(weighpthat)*TCut(weightBgenpt)*TCut(weightHiBin)*TCut(weightPVz)*(TCut(cut_recoonly.Data())&&"(Bgen==23333)"));
	

//	ntMC->Project("hPtMC",varExp.Data(),TCut(weighpthat)*TCut(weightBgenpt)*TCut(weightHiBin)*TCut(weightPVz)*TCut(Form("%s && Bgen==23333", cut.Data())));
//	ntMC->Project("hPtMCrecoonly",varExp.Data(),TCut(weighpthat)*TCut(weightBgenpt)*TCut(weightHiBin)*TCut(weightPVz)*TCut(Form("%s && Bgen==23333", cut.Data())));


	//weightGpt = "1.844009 + 0.058935 * Bgenpt - 0.454120 * sqrt(Gpt)";
	//weightBgenpt = "1.844009 + 0.058935 * Bgenpt - 0.454120 * sqrt(Bgenpt)";

//	ntMC->Project("hPtMC","Bpt",TCut(weighpthat)*TCut(weightBgenpt)*TCut(weightHiBin)*TCut(weightPVz)*(TCut(cut.Data())&&"(Bgen==23333)"));
  //  ntMC->Project("hPtMCrecoonly","Bpt",TCut(weighpthat)*TCut(weightBgenpt)*TCut(weightHiBin)*TCut(weightPVz)*(TCut(cut_recoonly.Data())&&"(Bgen==23333)"));



	ntGen->Project("hPtGen",varGenExp.Data(),
			TCut(weighpthat)*TCut(weightGpt)*(TCut(selmcgen.Data())));
	ntGen->Project("hPtGenWeighted",varGenExp.Data(),
			TCut(weighpthat)*TCut(weightGpt)*TCut(weightHiBin)*TCut(weightPVz)*(TCut(selmcgen.Data())));
	ntGen->Project("hPtGenAcc",varGenExp.Data(),
			TCut(weighpthat)*TCut(weightGpt)*(TCut(selmcgenacceptance.Data())));
	ntGen->Project("hPtGenAccWeighted",varGenExp.Data(),
			TCut(weighpthat)*TCut(weightGpt)*TCut(weightHiBin)*TCut(weightPVz)*(TCut(selmcgenacceptance.Data())));

	TH1D* hPthat = new TH1D("hPthat","",100,0,500);
	TH1D* hPthatweight = new TH1D("hPthatweight","",100,0,500);
	ntMC->Project("hPthat","pthat","1");
	ntMC->Project("hPthatweight","pthat",TCut("1"));

	////// tag & probe scaling factor
	for(int i = 0; i < _nBins; i++){printf("before muon sf: %.2f, ", hPtMC->GetBinContent(i+1));}printf("\n");//check entries
	for(int i = 0; i < _nBins; i++){
		if(label == "pp"){
			hPtMC->SetBinContent(i+1, hPtMC->GetBinContent(i+1)*_sf_pp[i]);
			hPtMCrecoonly->SetBinContent(i+1, hPtMCrecoonly->GetBinContent(i+1)*_sf_pp[i]);
		}
		if(label == "PbPb" && centmax == 90 && centmin ==0){
			hPtMC->SetBinContent(i+1, hPtMC->GetBinContent(i+1)*sf_pbpb090[i]);
			hPtMCrecoonly->SetBinContent(i+1, hPtMCrecoonly->GetBinContent(i+1)*sf_pbpb090[i]);
		}
		if(label == "PbPb" && centmax == 30 && centmin ==0){
			hPtMC->SetBinContent(i+1, hPtMC->GetBinContent(i+1)*sf_pbpb030[i]);
			hPtMCrecoonly->SetBinContent(i+1, hPtMCrecoonly->GetBinContent(i+1)*sf_pbpb030[i]);
		}
		if(label == "PbPb" && centmax == 90 && centmin ==30){
			hPtMC->SetBinContent(i+1, hPtMC->GetBinContent(i+1)*sf_pbpb3090[i]);
			hPtMCrecoonly->SetBinContent(i+1, hPtMCrecoonly->GetBinContent(i+1)*sf_pbpb3090[i]);
		}


	}
	for(int i = 0; i < _nBins; i++){printf("after muon sf: %.2f, ", hPtMC->GetBinContent(i+1));}printf("\n");//check entries
	////// tag & probe scaling factor

	if(dofine == 1){
		cout << "Working on Fine" << endl;




		TH1D* hPtMCFine = new TH1D("hPtMCFine","",46,5,50);
		TH1D* hPtGenWeightedFine = new TH1D("hPtGenWeightedFine","",46,5,50);
		TH1D* hPtMCrecoonlyFine  = new TH1D("hPtMCrecoonlyFine","",46,5,50);
		TH1D* hPtGenFine  = new TH1D("hPtGenFine","",46,5,50);
		TH1D* hPtGenAccFine  = new TH1D("hPtGenAccFine","",46,5,50);
		TH1D* hPtGenAccWeightedFine  = new TH1D("hPtGenAccWeightedFine","",46,5,50);


		divideBinWidth(hPtMCFine);
		divideBinWidth(hPtMCrecoonlyFine);
		divideBinWidth(hPtGenFine);
		divideBinWidth(hPtGenWeightedFine);
		divideBinWidth(hPtGenAccFine);
		divideBinWidth(hPtGenAccWeightedFine);

		ntMC->Project("hPtMCFine",varExp.Data(), TCut(weighpthat)*TCut(weightBgenpt)*TCut(weightHiBin)*TCut(weightPVz)*(TCut(cut.Data())&&"(Bgen==23333)"));
		ntGen->Project("hPtGenWeightedFine",varGenExp.Data(),TCut(weighpthat)*TCut(weightGpt)*TCut(weightHiBin)*TCut(weightPVz)*(TCut(selmcgen.Data())));


		ntGen->Project("hPtGenFine",varGenExp.Data(), TCut(weighpthat)*TCut(weightGpt)*(TCut(selmcgen.Data())));
		ntGen->Project("hPtGenWeightedFine",varGenExp.Data(),TCut(weighpthat)*TCut(weightGpt)*TCut(weightHiBin)*TCut(weightPVz)*(TCut(selmcgen.Data())));
		ntGen->Project("hPtGenAccFine",varGenExp.Data(),	TCut(weighpthat)*TCut(weightGpt)*(TCut(selmcgenacceptance.Data())));
		ntGen->Project("hPtGenAccWeightedFine",varGenExp.Data(),TCut(weighpthat)*TCut(weightGpt)*TCut(weightHiBin)*TCut(weightPVz)*(TCut(selmcgenacceptance.Data())));


		hPtMCFine->Sumw2();
		hPtMCrecoonlyFine->Sumw2();
		hPtGenFine->Sumw2();
		hPtGenAccFine->Sumw2();
		hPtGenAccWeightedFine->Sumw2();
		//Acceptance
		TH1D* hEffAccFine = (TH1D*)hPtGenAccFine->Clone("hEffAccFine");
		hEffAccFine->Sumw2();
		hEffAccFine->Divide(hEffAccFine,hPtGenFine,1,1,"b");
		//Selection
		TH1D* hEffSelectionFine = (TH1D*)hPtMCFine->Clone("hEffSelectionFine");
		hEffSelectionFine->Sumw2();
		hEffSelectionFine->Divide(hEffSelectionFine,hPtGenAccWeightedFine,1,1,"b");





		TH1D* hEffFineInv = (TH1D * ) hPtGenWeightedFine->Clone("hEffFineInv");
		hEffFineInv->Sumw2();
		hPtMCFine->Sumw2();
		hEffFineInv->Divide(hPtMCFine);

		TH1D* hEffFine = (TH1D * ) hPtMCFine->Clone("hEffFine");
		hEffFine->Sumw2();
		hPtGenWeightedFine->Sumw2();
		hEffFine->Divide(hPtGenWeightedFine);

		int labelmax = int(centmax);
		int labelmin = int(centmin);
	
		TFile * foutFine = new TFile(Form("ROOTfiles/EffFine_%d_%d.root",labelmin,labelmax),"RECREATE");
		foutFine->cd();
		hPtMCFine->Write();
		hPtGenWeightedFine->Write();
		hEffFine->Write();
		hEffAccFine->Write();
		hEffSelectionFine->Write();
		hEffFineInv->Write();
		foutFine->Close();

	}



	divideBinWidth(hPtMC);
	divideBinWidth(hPtMCrecoonly);
	divideBinWidth(hPtGen);
	divideBinWidth(hPtGenWeighted);
	divideBinWidth(hPtGenAcc);
	divideBinWidth(hPtGenAccWeighted);

	hPtMC->Sumw2();
	hPtMCrecoonly->Sumw2();
	hPtGen->Sumw2();
	hPtGenAcc->Sumw2();
	hPtGenAccWeighted->Sumw2();
	//Acceptance
	TH1D* hEffAcc = (TH1D*)hPtGenAcc->Clone("hEffAcc");
	hEffAcc->Sumw2();
	hEffAcc->Divide(hEffAcc,hPtGen,1,1,"b");
	//Selection
	TH1D* hEffSelection = (TH1D*)hPtMC->Clone("hEffSelection");
	hEffSelection->Sumw2();
	hEffSelection->Divide(hEffSelection,hPtGenAccWeighted,1,1,"b");
	//Acc * Eff
	TH1D* hEff = (TH1D*)hEffSelection->Clone("hEff");
	hEff->Sumw2();
	hEff->Multiply(hEff,hEffAcc,1,1);
	//Acc * Eff (one shot)
	TH1D* hEffOneShot = (TH1D*)hPtMC->Clone("hEffOneShot");
	hEffOneShot->Sumw2();
	hEffOneShot->Divide(hEffOneShot,hPtGenWeighted,1,1,"b");

	////// Draw hEff, hEffAcc
	TH2F* hemptyEff=new TH2F("hemptyEff","",50,_ptBins[0]-5.,_ptBins[_nBins]+5.,10.,0,0.8);  
	if(varExp == "abs(By)") hemptyEff=new TH2F("hemptyEff","",50,_ptBins[0],_ptBins[_nBins],10.,0,0.8);  
	hemptyEff->GetXaxis()->CenterTitle();
	hemptyEff->GetYaxis()->CenterTitle();
	hemptyEff->GetYaxis()->SetTitle("#alpha x #epsilon");
	hemptyEff->GetXaxis()->SetTitle("p_{T} (GeV/c)");
	hemptyEff->GetXaxis()->SetTitleOffset(0.9);
	hemptyEff->GetYaxis()->SetTitleOffset(0.95);
	hemptyEff->GetXaxis()->SetTitleSize(0.05);
	hemptyEff->GetYaxis()->SetTitleSize(0.05);
	hemptyEff->GetXaxis()->SetTitleFont(42);
	hemptyEff->GetYaxis()->SetTitleFont(42);
	hemptyEff->GetXaxis()->SetLabelFont(42);
	hemptyEff->GetYaxis()->SetLabelFont(42);
	hemptyEff->GetXaxis()->SetLabelSize(0.035);
	hemptyEff->GetYaxis()->SetLabelSize(0.035);  
	hemptyEff->SetMaximum(1.0);
	hemptyEff->SetMinimum(0.);
	hemptyEff->Draw();
	TH2F* hemptyEffAcc=(TH2F*)hemptyEff->Clone("hemptyEffAcc");
	hemptyEffAcc->SetYTitle("#alpha");

	TCanvas*canvasEff=new TCanvas("canvasEff","canvasEff",1000.,500);
	canvasEff->Divide(2,1);
	canvasEff->cd(1);
	hemptyEffAcc->Draw();
	hEffAcc->Draw("same");
	canvasEff->cd(2);
	hemptyEff->Draw();
	hEff->Draw("same");
	
	for(int i = 0; i < _nBins; i++){		
		cout << _ptBins[i] <<  "< pT < " <<  _ptBins[i+1]  <<  "    Efficiency =  " <<  hEff->GetBinContent(i+1) << "    Efficiency Error = " << hEff->GetBinError(i+1) << endl;
	}

	canvasEff->SaveAs(Form("%s/canvasEff_study%s.pdf",outplotf.Data(),Form(label.Data())));

	////// Draw hPthat, hPthatweight
	TH2F* hemptyPthat=new TH2F("hemptyPthat","",50,0.,300.,10,1e-5,1e9);  
	hemptyPthat->GetXaxis()->CenterTitle();
	hemptyPthat->GetYaxis()->CenterTitle();
	hemptyPthat->GetYaxis()->SetTitle("Entries");
	hemptyPthat->GetXaxis()->SetTitle("pthat");
	hemptyPthat->GetXaxis()->SetTitleOffset(0.9);
	hemptyPthat->GetYaxis()->SetTitleOffset(0.95);
	hemptyPthat->GetXaxis()->SetTitleSize(0.05);
	hemptyPthat->GetYaxis()->SetTitleSize(0.05);
	hemptyPthat->GetXaxis()->SetTitleFont(42);
	hemptyPthat->GetYaxis()->SetTitleFont(42);
	hemptyPthat->GetXaxis()->SetLabelFont(42);
	hemptyPthat->GetYaxis()->SetLabelFont(42);
	hemptyPthat->GetXaxis()->SetLabelSize(0.035);
	hemptyPthat->GetYaxis()->SetLabelSize(0.035);  
	hemptyPthat->SetMaximum(1.0);
	hemptyPthat->SetMinimum(0.);
	TH2F* hemptyPthatWeighted=(TH2F*)hemptyPthat->Clone("hemptyPthatWeighted");
	hemptyPthatWeighted->GetXaxis()->SetTitle("pthat reweighted");

	TCanvas*canvasPthat=new TCanvas("canvasPthat","canvasPthat",1000.,500);
	canvasPthat->Divide(2,1);
	canvasPthat->cd(1);
	gPad->SetLogy();
	hemptyPthat->Draw("same");
	hPthat->Draw("same");
	canvasPthat->cd(2);
	gPad->SetLogy();
	hemptyPthatWeighted->Draw();
	hPthatweight->Draw("same");
	canvasPthat->SaveAs(Form("%s/canvasPthat_%s.pdf",outplotf.Data(),Form(label.Data())));

	////// Draw hPtMC, hPtGen
	TH2F* hemptySpectra=new TH2F("hemptySpectra","",50,0.,130.,10,1,1e5);  
	if(varExp=="abs(By)") hemptySpectra=new TH2F("hemptySpectra","",50,_ptBins[0],_ptBins[_nBins],10,1,1e7);  
	hemptySpectra->GetXaxis()->CenterTitle();
	hemptySpectra->GetYaxis()->CenterTitle();
	hemptySpectra->GetYaxis()->SetTitle("Entries");
	hemptySpectra->GetXaxis()->SetTitle("p_{T}");
	hemptySpectra->GetXaxis()->SetTitleOffset(0.9);
	hemptySpectra->GetYaxis()->SetTitleOffset(0.95);
	hemptySpectra->GetXaxis()->SetTitleSize(0.05);
	hemptySpectra->GetYaxis()->SetTitleSize(0.05);
	hemptySpectra->GetXaxis()->SetTitleFont(42);
	hemptySpectra->GetYaxis()->SetTitleFont(42);
	hemptySpectra->GetXaxis()->SetLabelFont(42);
	hemptySpectra->GetYaxis()->SetLabelFont(42);
	hemptySpectra->GetXaxis()->SetLabelSize(0.035);
	hemptySpectra->GetYaxis()->SetLabelSize(0.035);  

	TCanvas*canvasSpectra=new TCanvas("canvasSpectra","canvasSpectra",1000.,500);
	canvasSpectra->Divide(2,1);
	canvasSpectra->cd(1);
	gPad->SetLogy();
	hemptySpectra->Draw();
	hPtMC->Draw("same");
	canvasSpectra->cd(2);
	gPad->SetLogy();
	hemptySpectra->Draw();
	hPtGen->Draw("same");
	canvasSpectra->SaveAs(Form("%s/canvasSpectra_%s.pdf",outplotf.Data(),Form(label.Data())));

	//### 1D histogram
	//hEffAcc = hPtGenAcc / hPtGen
	//hEffSelection = hPtMC / hPtMC
	//hEff = hEffSelection * hEffAcc
	//hEffOneShot = hPtMC / hPtGen

	TString text;
	if (isPbPb) { text="1.5 nb^{-1} (5.02 TeV PbPb)";}
	else {text="28.0 pb^{-1} (5.02 TeV pp)";}
	TLatex* texlumi = new TLatex(0.9,0.92,text.Data());
	texlumi->SetNDC();
	texlumi->SetTextAlign(31);
	texlumi->SetTextFont(42);
	texlumi->SetTextSize(0.038);
	texlumi->SetLineWidth(2);

	TString texper="%";
	TLatex* texCent = new TLatex(0.5,0.815,Form("Centrality %.0f - %.0f%s",centMin,centMax,texper.Data()));
	texCent->SetNDC();
	texCent->SetTextFont(42);
	texCent->SetTextSize(0.05);

	TLatex* texY = new TLatex(0.5,0.750,"|y| < 2.4");
	texY->SetNDC();
	texY->SetTextFont(42);
	texY->SetTextSize(0.05);
	texY->SetLineWidth(2);

	TLatex* texCms = new TLatex(0.08,0.95, "CMS Preliminary");
	texCms->SetNDC();
	texCms->SetTextAlign(13);
	texCms->SetTextSize(0.038);
	texCms->SetTextFont(62);

	TCanvas*canvas1D=new TCanvas("canvas1D","",600,600);
	canvas1D->cd();
	gPad->SetLogy(1);
	hemptyEff->GetXaxis()->SetTitle("p_{T} GeV^{-1}c)");
	hEff->SetLineColor(2);
	hEff->SetMarkerColor(2);

	hemptySpectra->SetYTitle("Entries of hPtMC");
	hemptySpectra->Draw(); 
	hPtMC->Draw("same");
	if(isPbPb) texCent->Draw();
	texY->Draw(); texCms->Draw(); texlumi->Draw();
	canvas1D->SaveAs(Form("%s/canvas1DhPtMC_%s.pdf",outplotf.Data(),Form(label.Data())));
	canvas1D->Clear();

	hemptySpectra->SetYTitle("Entries of hPtMCrecoonly");
	hemptySpectra->Draw(); 
	hPtMCrecoonly->Draw("same");
	if(isPbPb) texCent->Draw();
	texY->Draw(); texCms->Draw(); texlumi->Draw();
	canvas1D->SaveAs(Form("%s/canvas1DhPtMCrecoonly_%s.pdf",outplotf.Data(),Form(label.Data())));
	canvas1D->Clear();

	hemptySpectra->SetYTitle("Entries of hPtGen");
	hemptySpectra->Draw(); 
	hPtGen->Draw("same");
	if(isPbPb) texCent->Draw();
	texY->Draw(); texCms->Draw(); texlumi->Draw();
	canvas1D->SaveAs(Form("%s/canvas1DhPtGen_%s.pdf",outplotf.Data(),Form(label.Data())));
	canvas1D->Clear();

	hemptySpectra->SetYTitle("Entries of hPtGenAcc");
	hemptySpectra->Draw(); 
	hPtGenAcc->Draw("same");
	if(isPbPb) texCent->Draw();
	texY->Draw(); texCms->Draw(); texlumi->Draw();
	canvas1D->SaveAs(Form("%s/canvas1DhPtGenAcc_%s.pdf",outplotf.Data(),Form(label.Data())));
	canvas1D->Clear();

	gPad->SetLogy(0);
	//hemptyEff->SetYTitle("hPtGenAcc/hPtGen");
	hemptyEff->SetYTitle("#alpha");
	hemptyEff->Draw(); 
	hEffAcc->Draw("same");
	if(isPbPb) texCent->Draw();
	texY->Draw(); texCms->Draw(); texlumi->Draw();
	canvas1D->SaveAs(Form("%s/canvas1DhEffAcc_%s.pdf",outplotf.Data(),Form(label.Data())));
	canvas1D->Clear();

	hemptyEff->SetYTitle("#epsilon");
	hemptyEff->Draw(); 
	hEffSelection->Draw("same");
	if(isPbPb) texCent->Draw();
	texY->Draw(); texCms->Draw(); texlumi->Draw();
	canvas1D->SaveAs(Form("%s/canvas1DhEffSelection_%s.pdf",outplotf.Data(),Form(label.Data())));
	canvas1D->Clear();

	hemptyEff->SetYTitle("#alpha x #epsilon");
	hemptyEff->Draw();
	hEff->Draw("same");
	if(isPbPb) texCent->Draw();
	texY->Draw(); texCms->Draw(); texlumi->Draw();
	canvas1D->SaveAs(Form("%s/canvas1DhEff_%s.pdf",outplotf.Data(),Form(label.Data())));
	canvas1D->SaveAs(Form("%s/canvas1DhEff_%s.C",outplotf.Data(),Form(label.Data())));
	canvas1D->Clear();

	hemptyEff->SetYTitle("#alpha x #epsilon (1-shot)");
	hemptyEff->Draw(); 
	hEffOneShot->Draw("same");
	if(isPbPb) texCent->Draw();
	texY->Draw(); texCms->Draw(); texlumi->Draw();
	canvas1D->SaveAs(Form("%s/canvas1DhEffOneShot_%s.pdf",outplotf.Data(),Form(label.Data())));
	canvas1D->Clear();

	TFile *fout=new TFile(outputfile.Data(),"recreate");
	fout->cd();
	hPtMC->Write();
	hPtGen->Write();
	hPtGenWeighted->Write();
	hPtGenAcc->Write();
	hPtGenAccWeighted->Write();
	hEffAcc->Write();
	hEffSelection->Write();
	hEffOneShot->Write();
	hEff->Write();
	fout->Close();  

}

int main(int argc, char *argv[])
{
	if(argc == 15)
		MCefficiencyCentDiff(atoi(argv[1]),argv[2],argv[3],argv[4],argv[5],argv[6],argv[7],argv[8],argv[9],argv[10],argv[11],atoi(argv[12]),atof(argv[13]),atof(argv[14]));
	else
	{
		std::cout << "Wrong number of inputs" << std::endl;
		return 1;
	}
	return 0;
}




