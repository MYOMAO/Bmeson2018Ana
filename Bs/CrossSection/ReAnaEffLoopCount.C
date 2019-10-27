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

void ReAnaEffLoopCount(int CentMin, int CentMax,	const int NBins,	int DoTwoD, int PtOpt, int drop, int Weight ){

	int TnP = 1;

	TString WeightName;

	if(Weight == 16) WeightName = "NoWeight";
	if(Weight == 1) WeightName = "FONLL";
	if(Weight == 11) WeightName = "Linear";
	if(Weight == 12) WeightName = "Quadratic";
	if(Weight == 13) WeightName = "LInverse";
	if(Weight == 14) WeightName = "LSqrt";
	if(Weight == 15) WeightName = "LLog";
	if(Weight == 0) WeightName = "NoTnP";

	//gStyle->SetOptTitle(0);
	gStyle->SetOptStat(0);


	TString FileName;



	TString FitName;

	TFile * RawYield;


	ofstream Merge(Form("Merge_%d_%d.sh",CentMin,CentMax), ios::app);
	Merge << "hadd     ";




	TString outfilefolder;


	if(CentMin == 0 && CentMax ==90 && PtOpt == 0) outfilefolder = "7-10";
	if(CentMin == 0 && CentMax ==90 && PtOpt == 1)  outfilefolder = "10-15";
	if(CentMin == 0 && CentMax ==90 && PtOpt == 2)  outfilefolder = "15-20";
	if(CentMin == 0 && CentMax ==90 && PtOpt == 3)	 outfilefolder = "20-50";
	if(CentMin == 0 && CentMax ==90 && PtOpt == -1)  outfilefolder = "10-50";
	if(CentMin == 0 && CentMax ==30 && PtOpt == -1)  outfilefolder = "10-50";
	if(CentMin == 30 && CentMax ==90 && PtOpt == -1)  outfilefolder = "10-50";


	double EffNominal = 18.7588;
	double EffNominalErr = 0.0277877;
	double EffPull;

	TH1D * CorrYieldEffRealONLY = new TH1D("CorrYieldEffRealONLY","CorrYieldEffRealONLY",200,0,0.2);

	CorrYieldEffRealONLY->GetXaxis()->SetTitle("<acc x eff>");
	CorrYieldEffRealONLY->GetYaxis()->SetTitle("Counts");
	CorrYieldEffRealONLY->GetYaxis()->SetTitleOffset(1.5);
	CorrYieldEffRealONLY->GetXaxis()->CenterTitle();
	CorrYieldEffRealONLY->GetYaxis()->CenterTitle();



	TH1D * CorrYieldEffONLY = new TH1D("CorrYieldEffONLY","CorrYieldEffONLY",200,0,40);

	CorrYieldEffONLY->GetXaxis()->SetTitle("<1/(acc x eff)>");
	CorrYieldEffONLY->GetYaxis()->SetTitle("Counts");
	CorrYieldEffONLY->GetYaxis()->SetTitleOffset(1.5);
	CorrYieldEffONLY->GetXaxis()->CenterTitle();
	CorrYieldEffONLY->GetYaxis()->CenterTitle();



	TH1D * CorrYieldEffONLYPull = new TH1D("CorrYieldEffONLYPull","CorrYieldEffONLYPull",80,-10,10);

	CorrYieldEffONLYPull->GetXaxis()->SetTitle("<1/(acc x eff)> Pull Distribution");
	CorrYieldEffONLYPull->GetYaxis()->SetTitle("Counts");
	CorrYieldEffONLYPull->GetYaxis()->SetTitleOffset(1.5);
	CorrYieldEffONLYPull->GetXaxis()->CenterTitle();
	CorrYieldEffONLYPull->GetYaxis()->CenterTitle();





	double CorrYieldNom;

	double SignalCounts;


	if(drop == 0 || drop == 1){

		if(CentMin == 30 && CentMax == 90)  SignalCounts = 33;
		if(CentMin == 0 && CentMax == 90) SignalCounts = 95;

	}



	if(drop == 0){

		if(CentMin == 30 && CentMax == 90) CorrYieldNom = 5.64791e+08/118192*SignalCounts;
		if(CentMin == 0 && CentMax == 90) CorrYieldNom = 5.52002e+08/73424*SignalCounts;
	}


	if(drop == 1){

		if(CentMin == 30 && CentMax == 90) CorrYieldNom = 5.6218e+08/118192*SignalCounts;
//		if(CentMin == 0 && CentMax == 90) CorrYieldNom = 1.13623e+09/150985*SignalCounts;
	//	if(CentMin == 0 && CentMax == 90) CorrYieldNom =4.10385e+08/73424*SignalCounts;
		if(CentMin == 0 && CentMax == 90) CorrYieldNom = 1.13623e+09 /150985*SignalCounts;

	
	}




	double CorrYieldPull;




	TH1D * CorrYieldHisDisRaw = new TH1D("CorrYieldHisDisRaw","CorrYieldHisDisRaw",300,CorrYieldNom*0.1,CorrYieldNom*4);

	CorrYieldHisDisRaw->GetXaxis()->SetTitle("Corr Yield Distribution");
	CorrYieldHisDisRaw->GetYaxis()->SetTitle("Counts");
	CorrYieldHisDisRaw->GetYaxis()->SetTitleOffset(1.5);
	CorrYieldHisDisRaw->GetXaxis()->CenterTitle();
	CorrYieldHisDisRaw->GetYaxis()->CenterTitle();







	int NFiles;

	if(CentMin == 30 && CentMax == 90) NFiles = 5000;
	if(CentMin == 0 && CentMax == 90 && PtOpt == -1) NFiles = 1590;
	if(CentMin == 0 && CentMax == 30) NFiles = 5000;

	double DirectCount;


	for(int q = 0; q < NFiles; q++){ 

			cout << "Now Working on File =  " << q << endl;

			FileName = Form("CheckSystNuno/%s/MCSPlitNew/%d-%d/%s/MC_%d.root",WeightName.Data(),CentMin,CentMax,outfilefolder.Data(),q);

			TFile * fin = new TFile(FileName.Data());
			fin->cd();

			TTree * EffInfoTree = (TTree * ) fin->Get("EffInfoTree");

			int NEvents = EffInfoTree->GetEntries();

			const int NCand = 10;

			Int_t BsizeNew;
			Int_t runNew;
			Int_t lumiNew;
			Int_t evtNew;
			Float_t BmassNew[NCand];
			Float_t BptNew[NCand];
			Float_t ByNew[NCand];
			Float_t BEff[NCand];
			Float_t BEffErr[NCand];


			Float_t BEffInv[NCand];
			Float_t BEffInvErr[NCand];
			Float_t BEffInv1D[NCand];
			Float_t BEffInvErr1D[NCand];

			Float_t BEffInvFit[NCand];
			Float_t BEffInvErrFit[NCand];

			Float_t BEffInvBDTWeighted[NCand];
			Float_t BEffInvErrBDTWeighted[NCand];



			Float_t BEffInvUp[NCand];
			Float_t BEffInvErrUp[NCand];
			Float_t BEffInvDown[NCand];
			Float_t BEffInvErrDown[NCand];

			EffInfoTree->SetBranchAddress("BsizeNew",&BsizeNew);
			EffInfoTree->SetBranchAddress("BmassNew",BmassNew);
			EffInfoTree->SetBranchAddress("ByNew",ByNew);
			EffInfoTree->SetBranchAddress("BptNew",BptNew);
			EffInfoTree->SetBranchAddress("BEffInv",BEffInv);
			EffInfoTree->SetBranchAddress("BEffInvErr",BEffInvErr);
			EffInfoTree->SetBranchAddress("BEff",BEff);
			EffInfoTree->SetBranchAddress("BEffInv1D",BEffInv1D);
			EffInfoTree->SetBranchAddress("BEffInvErr1D",BEffInvErr1D);

			EffInfoTree->SetBranchAddress("BEffInvFit",BEffInvFit);
			EffInfoTree->SetBranchAddress("BEffInvErrFit",BEffInvErrFit);

			EffInfoTree->SetBranchAddress("BEffInv",BEffInv);
			EffInfoTree->SetBranchAddress("BEffInvErr",BEffInvErr);

			EffInfoTree->SetBranchAddress("BEffInvBDTWeighted",BEffInvBDTWeighted);
			EffInfoTree->SetBranchAddress("BEffInvErrBDTWeighted",BEffInvErrBDTWeighted);



			EffInfoTree->SetBranchAddress("BEffInvUp",BEffInvUp);
			EffInfoTree->SetBranchAddress("BEffInvErrUp",BEffInvErrUp);

			EffInfoTree->SetBranchAddress("BEffInvDown",BEffInvDown);
			EffInfoTree->SetBranchAddress("BEffInvErrDown",BEffInvErrDown);


			int hiBinMin = CentMin * 2;
			int hiBinMax = CentMax * 2;

			double ptBins[NBins + 1];

			int Counts[NBins];
			double SumCounts[NBins];
			double SumCountsErr[NBins];
			double NewEff[NBins];
			double NewEffErr[NBins];

			double NewEffReal[NBins];
			double NewEffRealErr[NBins];



			double SumCountsUp[NBins];
			double SumCountsErrUp[NBins];

			double SumCountsDown[NBins];
			double SumCountsErrDown[NBins];


			double SumCountsEff[NBins];
			double SumCountsEffErr[NBins];



			double SumCountsSyst[NBins];
			double SumCountsSystErr[NBins];
			double NewEffSyst[NBins];
			double NewEffSystErr[NBins];



			double NewEffUp[NBins];
			double NewEffErrUp[NBins];
			double NewEffDown[NBins];
			double NewEffErrDown[NBins];


			double TAA;
			std::vector<double> ptbinsvec;
			std::vector<double> corrfactvec;


			if(NBins == 1){

				if(drop == 0 && CentMin == 0) ptbinsvec.push_back(7.0);
				if(drop == 0 && CentMin == 30) ptbinsvec.push_back(9.0);

				if(drop == 1)  ptbinsvec.push_back(10.0);

				ptbinsvec.push_back(50);


			}


			if(NBins == 3){

				ptbinsvec.push_back(5);
				ptbinsvec.push_back(15);
				ptbinsvec.push_back(20);
				ptbinsvec.push_back(50);

			}


			if(NBins == 4){

				if(CentMin == 0) ptbinsvec.push_back(7);
				if(CentMin == 30)  ptbinsvec.push_back(9);
				ptbinsvec.push_back(10);
				ptbinsvec.push_back(15);
				ptbinsvec.push_back(20);
				ptbinsvec.push_back(50);


			}






			for(int i = 0; i < NBins + 1; i++){
				ptBins[i] =  ptbinsvec[i];
			}

			for(int i = 0; i < NBins; i++){
				Counts[i] = 0;
				SumCounts[i] = 0;
				SumCountsErr[i] = 0;
				SumCountsEff[i] = 0;
				SumCountsEffErr[i] = 0;
				SumCountsSyst[i] = 0;
				SumCountsSystErr[i] = 0;
				//	CorrectionFactor[i] = corrfactvec[i];
				SumCountsUp[i] = 0;
				SumCountsErrUp[i] = 0;
				SumCountsDown[i] = 0;
				SumCountsErrDown[i] = 0;
			}




			for( int i = 0; i < NEvents; i++){

				EffInfoTree->GetEntry(i);

				for(int j = 0; j < BsizeNew; j++){


					for(int k = 0; k < NBins; k++){

						if(BptNew[j] > ptBins[k] && BptNew[j] < ptBins[k+1] && (( BptNew[j] < 10 && TMath::Abs(BmassNew[j] - 5.36682) < 0.09) || (BptNew[j] > 10 && TMath::Abs(BmassNew[j] - 5.36682) < 0.08)))
						{
							//	SumCountsReal[k] = SumCountsReal[k] + BEff[j];
							//cout << "BEffInv[j] = " << BEffInv[j] << endl; 
							if(DoTwoD == 0){
								BEffErr[j] = BEffInvErr1D[j]/(BEffInv1D[j] * BEffInv1D[j]);
								if(BEffInv1D[j] > 0){
									//cout << "UnFit =" << BEffInv1D[j] << endl;
									SumCounts[k] = SumCounts[k] + BEffInv1D[j];
									SumCountsErr[k] = SumCountsErr[k] + BEffInvErr1D[j] * BEffInvErr1D[j];
									SumCountsEff[k] = SumCountsEff[k] + BEff[j];
									SumCountsEffErr[k] = SumCountsEffErr[k] + BEffErr[j] * BEffErr[j];
									Counts[k] = Counts[k] + 1;
								}
							}


							if(DoTwoD == 1){
								BEffErr[j] = BEffInvErr[j]/(BEffInv[j] * BEffInv[j]);
								if(BEffInv[j] > 0){
									SumCounts[k] = SumCounts[k] + BEffInv[j];
									SumCountsErr[k] = SumCountsErr[k] + BEffInvErr[j] * BEffInvErr[j];
									SumCountsEff[k] = SumCountsEff[k] + BEff[j];
									SumCountsEffErr[k] = SumCountsEffErr[k] + BEffErr[j] * BEffErr[j];
									SumCountsSyst[k] = 	SumCountsSyst[k]  + BEffInvBDTWeighted[j];
									SumCountsSystErr[k] = 	SumCountsSystErr[k]  + BEffInvErrBDTWeighted[j] * BEffInvErrBDTWeighted[j];

									SumCountsUp[k] = SumCountsUp[k] + BEffInvUp[j];
									SumCountsErrUp[k] = SumCountsErrUp[k] + BEffInvErrUp[j] * BEffInvErrUp[j];

									SumCountsDown[k] = SumCountsDown[k] + BEffInvDown[j];
									SumCountsErrDown[k] = SumCountsErrUp[k] + BEffInvErrDown[j] * BEffInvErrDown[j];

									Counts[k] = Counts[k] + 1;
								}
							}
							if(DoTwoD == 2){
								BEffErr[j] = BEffInvErrFit[j]/(BEffInvFit[j] * BEffInvFit[j]);	
								//cout << "Fit =" << BEffInvFit[j] << endl;
								//cout << "Err = " << BEffInvErrFit[j] << endl;
								SumCounts[k] = SumCounts[k] + BEffInvFit[j];
								SumCountsErr[k] = SumCountsErr[k] + BEffInvErrFit[j] * BEffInvErrFit[j];
								SumCountsEff[k] = SumCountsEff[k] + BEff[j];
								SumCountsEffErr[k] = SumCountsEffErr[k] + BEffErr[j] * BEffErr[j];
								Counts[k] = Counts[k] + 1;
							}
						}

					}


				}

			}


			TH1D * hInvEff = new TH1D("hInvEff","",NBins,ptBins);


			hInvEff->GetXaxis()->SetTitle("B_{s} p_{T} (GeV/c)");
			hInvEff->GetYaxis()->SetTitle("<1/(Eff * Acc)>");
			hInvEff->GetYaxis()->SetTitleOffset(1.4);
			hInvEff->GetXaxis()->CenterTitle();
			hInvEff->GetYaxis()->CenterTitle();
			hInvEff->SetMarkerColor(1);
			hInvEff->SetLineColor(1);
			hInvEff->SetMarkerStyle(20);

			hInvEff->SetMinimum(0);


			TH1D * hInvEffSyst = new TH1D("hInvEffSyst","",NBins,ptBins);

			hInvEffSyst->GetXaxis()->SetTitle("B_{s} p_{T} (GeV/c)");
			hInvEffSyst->GetYaxis()->SetTitle("<1/(Eff * Acc)> - BDT Data-MC Weighted");
			hInvEffSyst->GetYaxis()->SetTitleOffset(1.4);
			hInvEffSyst->GetXaxis()->CenterTitle();
			hInvEffSyst->GetYaxis()->CenterTitle();
			hInvEffSyst->SetMarkerColor(1);
			hInvEffSyst->SetLineColor(2);
			hInvEffSyst->SetMarkerStyle(20);

			hInvEffSyst->SetMinimum(0);

			TH1D * hEff = new TH1D("hEff","",NBins,ptBins);


			hEff->GetXaxis()->SetTitle("B_{s} p_{T} (GeV/c)");
			hEff->GetYaxis()->SetTitle("<(Eff * Acc)>");
			hEff->GetYaxis()->SetTitleOffset(1.4);
			hEff->GetXaxis()->CenterTitle();
			hEff->GetYaxis()->CenterTitle();
			hEff->SetMarkerColor(1);
			hEff->SetLineColor(1);
			hEff->SetMarkerStyle(20);

			hEff->SetMinimum(0);


			TH1D * hInvEffUp = new TH1D("hInvEffUp","",NBins,ptBins);


			hInvEffUp->GetXaxis()->SetTitle("B_{s} p_{T} (GeV/c)");
			hInvEffUp->GetYaxis()->SetTitle("<1./(Eff * Acc)>");
			hInvEffUp->GetYaxis()->SetTitleOffset(1.4);
			hInvEffUp->GetXaxis()->CenterTitle();
			hInvEffUp->GetYaxis()->CenterTitle();
			hInvEffUp->SetMarkerColor(1);
			hInvEffUp->SetLineColor(1);
			hInvEffUp->SetMarkerStyle(20);
			hInvEffUp->SetMinimum(0);



			TH1D * hInvEffDown = new TH1D("hInvEffDown","",NBins,ptBins);


			hInvEffDown->GetXaxis()->SetTitle("B_{s} p_{T} (GeV/c)");
			hInvEffDown->GetYaxis()->SetTitle("<1/(Eff * Acc)>");
			hInvEffDown->GetYaxis()->SetTitleOffset(1.4);
			hInvEffDown->GetXaxis()->CenterTitle();
			hInvEffDown->GetYaxis()->CenterTitle();
			hInvEffDown->SetMarkerColor(1);
			hInvEffDown->SetLineColor(1);
			hInvEffDown->SetMarkerStyle(20);
			hInvEffDown->SetMinimum(0);



			for(int i = 0; i < NBins; i++){


				NewEff[i] = SumCounts[i]/Counts[i];
				NewEffErr[i] = TMath::Sqrt(SumCountsErr[i])/Counts[i];



				NewEffUp[i] = SumCountsUp[i]/Counts[i];
				NewEffErrUp[i] = TMath::Sqrt(SumCountsErrUp[i])/Counts[i];



				NewEffDown[i] = SumCountsDown[i]/Counts[i];
				NewEffErrDown[i] = TMath::Sqrt(SumCountsErrDown[i])/Counts[i];


				NewEffReal[i] = SumCountsEff[i]/Counts[i];
				NewEffRealErr[i] = TMath::Sqrt(SumCountsEffErr[i])/Counts[i];


				hInvEff->SetBinContent(i+1,NewEff[i]);
				hInvEff->SetBinError(i+1,NewEffErr[i]);

				hEff->SetBinContent(i+1,NewEffReal[i]);
				hEff->SetBinError(i+1,NewEffRealErr[i]);


				NewEffSyst[i] = SumCountsSyst[i]/Counts[i];
				NewEffSystErr[i] = TMath::Sqrt(SumCountsSystErr[i])/Counts[i];


				hInvEffSyst->SetBinContent(i+1,	NewEffSyst[i]);
				hInvEffSyst->SetBinError(i+1, NewEffSystErr[i]);



				hInvEffUp->SetBinContent(i+1,NewEffUp[i]);
				hInvEffUp->SetBinError(i+1,NewEffErrUp[i]);


				hInvEffDown->SetBinContent(i+1,NewEffDown[i]);
				hInvEffDown->SetBinError(i+1,NewEffErrDown[i]);

				//	cout << "Real eff = " << SumCountsReal[i]/Counts[i] << endl;
				//cout << "Counts = " << Counts[i] << endl;
				//cout << "Count =  " <<  Counts[i] << "   NewEff = " << NewEff[i] << "     NewEffErr = " << NewEffErr[i] << endl;
				//	cout << "Count =  " <<  Counts[i] << "   NewEffSyst = " << NewEffSyst[i] << "     NewEffSystErr = " << NewEffSystErr[i] << endl;
				
		
				CorrYieldEffRealONLY->Fill(NewEffReal[i]);
			
				CorrYieldEffONLY->Fill(NewEff[i]);
				EffPull = (NewEff[i]-EffNominal)/NewEffErr[i];
				CorrYieldEffONLYPull->Fill(EffPull);
			}








		
			
		fin->Close();
	}


	TCanvas * cPull = new TCanvas("cPull","cPull",600,600);
	cPull->cd();



	CorrYieldEffONLYPull->Draw();

	cPull->SaveAs(Form("ClosurePlotsCount/EffPlots/EffPull_%d_%d_%d.png",CentMin,CentMax,drop));


	CorrYieldEffONLY->Draw();

	TLine *l18 = new TLine(EffNominal,0,EffNominal,CorrYieldEffONLY->GetMaximum());
	l18->SetLineStyle(2);
	l18->SetLineWidth(2);
	l18->SetLineColor(2);
	l18->Draw("SAME");

	cPull->SaveAs(Form("ClosurePlotsCount/EffPlots/EffONLY_%d_%d_%d.png",CentMin,CentMax,drop));

	CorrYieldEffRealONLY->SetTitle("Efficiency Distribution");
	CorrYieldEffRealONLY->Draw();
	cPull->SaveAs(Form("ClosurePlotsCount/EffPlots/EffRealONLY_%d_%d_%d.png",CentMin,CentMax,drop));


	cout << "------------------------------------------------------------------------------" << endl;


	double Mean = CorrYieldEffONLY->GetMean();
	double RMS = CorrYieldEffONLY->GetRMS();


	double SyetErr = (Mean-EffNominal)/EffNominal;
	double StatErr = RMS/Mean;


	cout << "Mean = " << Mean << "  RMS = " << RMS  << "  EffNominal = " << EffNominal << endl; 

	cout << "Stat Error = " << StatErr << "         SyetErr =  " << SyetErr << endl;

	cout << "------------------------------------------------------------------------------" << endl;

}
