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
#include "fitB.h"
using namespace std;

using std::cout;
using std::endl;
#endif


void CutVariation(TString collsyst, TString inputdata, TString inputMC, TString PreCut){
	gStyle->SetOptStat(0); 

	ofstream fout("CutVar.dat");

	TCanvas * c = new TCanvas("c","c",600,600);
	TCanvas * cMC = new TCanvas("cMC","cMC",600,600);

	TCanvas *cResults = new TCanvas("cResults","cResults",600,600);

	int doweight = 1;



	if(collsyst == "pp") weightfunctionreco="(pthatweight)*(maxDgenpt<pthat/1.2)*(0.0116437+Dgenpt*(0.0602697)+Dgenpt*Dgenpt*(-0.00226879)+Dgenpt*Dgenpt*Dgenpt*(3.91035e-05)+Dgenpt*Dgenpt*Dgenpt*Dgenpt*(-3.0699e-07)+Dgenpt*Dgenpt*Dgenpt*Dgenpt*Dgenpt*(8.73234e-10))";




	if(collsyst == "PbPb" && doweight == 1)
	{
		//BptWeight="0.475953*TMath::Exp(-0.001731*Bpt)+38.069448/(Bpt*Bpt+0.001237*0.001237)";
		//	BptWeight="0.474599*TMath::Exp(-0.001406*Bpt)+38.036016/(Bpt*Bpt+0.000330*0.000330)";
		//	BptWeight = "0.603534*TMath::Exp(-0.006505*Bpt)+13.177674/(Bpt*Bpt -4.418950 * Bpt + 0.009566*0.009566)";
		//BptWeight = "0.329452*TMath::Exp(-0.019321*Bpt)+41.766452/(Bpt*Bpt -0.003756 * Bpt + 0.000029*0.000029)";
		BptWeight = "(5.786639 - 0.517519*Bgenpt)*TMath::Exp(-0.226562 * Bgenpt) + 0.763430";
		CentWeight = "CentWeight";
		//PVzWeight = "TMath::Exp(0.057104 + -0.020908 * PVz + -0.001864 * PVz * PVz)";
		//PVzWeight =	"(0.162740 * TMath::Exp(- 0.020823 * (PVz - 0.428205)*(PVz - 0.428205)))/(0.159489 * TMath::Exp(- 0.019979 * (PVz - 0.594276)*(PVz - 0.594276)))";
		PVzWeight="0.163562 * TMath::Exp(- 0.021039 * (PVz - 0.426587)*(PVz - 0.426587))/(0.159619 * TMath::Exp(- 0.020011 * (PVz - 0.587652)*(PVz - 0.587652)))";
		pthatweight = "(pthatweight)";
		cout << "WEIGHT = YES" << endl;

	}

	if(collsyst == "PbPb" && doweight == 0)
	{
		//BptWeight="0.475953*TMath::Exp(-0.001731*Bpt)+38.069448/(Bpt*Bpt+0.001237*0.001237)";
		BptWeight="(1)";
		CentWeight = "(1)";
		//PVzWeight = "TMath::Exp(0.057104 + -0.020908 * PVz + -0.001864 * PVz * PVz)";
		//PVzWeight =	"(0.162740 * TMath::Exp(- 0.020823 * (PVz - 0.428205)*(PVz - 0.428205)))/(0.159489 * TMath::Exp(- 0.019979 * (PVz - 0.594276)*(PVz - 0.594276)))";
		PVzWeight="(1)";
		pthatweight = "pthatweight";

		cout << "WEIGHT = NO" << endl;

	}




	if(collsyst == "pp") isPbPb = 0;
	if(collsyst == "PbPb") isPbPb = 1;

	TFile *fin = new TFile(inputdata.Data());
	TFile *finMC = new TFile(inputMC.Data());



	TTree* nt = (TTree*) fin->Get("Bfinder/ntphi");
	nt->AddFriend("hltanalysis/HltTree");
	nt->AddFriend("hiEvtAnalyzer/HiTree");
	nt->AddFriend("skimanalysis/HltTree");
	nt->AddFriend("BDT_pt_15_20");
	nt->AddFriend("BDT_pt_5_10");
	nt->AddFriend("BDT_pt_10_15");	
	nt->AddFriend("BDT_pt_20_50");	
	//	nt->AddFriend("BDT");	


	TTree* ntMC = (TTree*)finMC->Get("Bfinder/ntphi");
	ntMC->AddFriend("hltanalysis/HltTree");
	ntMC->AddFriend("hiEvtAnalyzer/HiTree");
	ntMC->AddFriend("skimanalysis/HltTree");
	ntMC->AddFriend("BDT_pt_15_20");
	ntMC->AddFriend("BDT_pt_5_10");
	ntMC->AddFriend("BDT_pt_10_15");	
	ntMC->AddFriend("BDT_pt_20_50");	
	/*
	   ntMC->AddFriend("BDT_pt_15_20");
	   ntMC->AddFriend("BDT_pt_7_15");
	   ntMC->AddFriend("BDT_pt_20_50");	
	   */
	//ntMC->AddFriend("BDT");	
	ntMC->AddFriend("CentWeightTree");	


	TH1D * alphahis = new TH1D("alphahis","alphahis",Nalpha,alphaini-0.5*alphastep,alphafin+0.5*alphastep);
	TH1D * ratiohis = new TH1D("ratiohis","ratiohis",Nratio,ratioini-0.5*ratiostep,ratiofin+0.5*ratiostep);
	TH1D * Probhis = new TH1D("Probhis","Probhis",NProb,Probini-0.5*Probstep,Probfin+0.5*Probstep);



	TString PreCutMC = Form("(%s && Bgen == 23333)",PreCut.Data());


	ofstream out1(Form("Alpha_%s.dat",collsyst.Data()));
	ofstream out2(Form("Ratio_%s.dat",collsyst.Data()));
	ofstream out3(Form("Prob_%s.dat",collsyst.Data()));




	//nt->Project("DataPre","Bmass",PreCut.Data());



	//ntMC->Project("MCPre","Bmass",(TCut(BptWeight.Data())*TCut(CentWeight.Data())*TCut(PVzWeight.Data())*TCut(weightfunctionreco.Data()))*(TCut(PreCutMC.Data())));



	TFile * CutVarOut = new TFile("CutVarOut.root","RECREATE");
	CutVarOut->cd();
	/*
	   fDataPre = fit(c, cMC, DataPre, MCPre, ptMin, ptMax, 0, isPbPb, total, CentMinBin, CentMaxBin, "1");

	   PreCutMC = Form("%s *  (%s && Bgen == 23333)",weightfunctionreco.Data(),PreCut.Data());
	   fMCPre = fit(c, cMC, MCPre, MCPre, ptMin, ptMax, 1, isPbPb, total, CentMinBin, CentMaxBin, "1");

	   PreCutYieldData = fDataPre->Integral(BMassMin,BMassMax)/binwidthmass;
	   PreCutYieldErrData = fDataPre->Integral(BMassMin,BMassMax)/binwidthmass*fDataPre->GetParError(0)/fDataPre->GetParameter(0);

	   PreCutYieldMC = fMCPre->Integral(BMassMin,BMassMax)/binwidthmass;
	   PreCutYieldErrMC = fMCPre->Integral(BMassMin,BMassMax)/binwidthmass*fMCPre->GetParError(0)/fMCPre->GetParameter(0);
	   */	
	//	PreCutYieldMC = MCPre->Integral();
	//	PreCutYieldErrMC = TMath::Sqrt(PreCutYieldMC);
	//	cout << "PreCutYieldData = " << PreCutYieldData << "   PreCutYieldMC  = " << PreCutYieldMC << endl;

	double MaxDeviation[NVar];
	double CurrentMax[NVar];
	for(int i = 0; i < NVar; i++){
		MaxDeviation[i] = 0;
		CurrentMax[i] = 0;
	}


	//Alpha Variation//

	for(int j = 3; j < 7; j++){

		ResultHis = new TH1D(Form("%s",VarHisName[j].Data()),Form("%s",VarHisName[j].Data()),VarHisN[j],VarHisMin[j],VarHisMax[j]);
		ResultHis->GetXaxis()->SetTitle(Form("%s ",VarXName[j].Data()));
		ResultHis->GetYaxis()->SetTitle("Yield Double Ratio");
		ResultHis->GetYaxis()->SetTitle(Form("%s Cut Variation",VarXName[j].Data()));
		NVarStep[j] = (VarCutMax[j] - VarCutMin[j])/NCut[j];

		cout << "j = " << j << "   Var[j].Data()  = " << Var[j].Data()  << endl;


		VarPreCutData = Form("((%s %s %f && %s) && (%s))",Var[j].Data(),Direction[j].Data(),WorkingPoint[j],VarPtRange[j].Data(),PreCut.Data());

		cout << "VarPreCutData = " << VarPreCutData.Data() << endl;

		VarPreCutMC = Form("((%s) && Bgen == 23333)",VarPreCutData.Data());

		TH1D * DataPre = new TH1D("DataPre","DataPre",NBMassBin,BMassMin,BMassMax);
		DataPre->SetBinErrorOption(TH1::kPoisson);
		nt->Project("DataPre","Bmass",VarPreCutData.Data());
		cout << "DataPre = " << DataPre->Integral() << endl;

		TH1D * MCPre = new TH1D("MCPre","MCPre",NBMassBin,BMassMin,BMassMax);
		ntMC->Project("MCPre","Bmass",(TCut(BptWeight.Data())*TCut(CentWeight.Data())*TCut(PVzWeight.Data())*TCut(weightfunctionreco.Data()))*(TCut(VarPreCutMC.Data())));
		cout << "MCPre = " << MCPre->Integral() << endl;


		fDataPre = fit(c, cMC, DataPre, MCPre, ptMin, ptMax, 0, isPbPb, total, CentMinBin, CentMaxBin, "1");
		PreCutYieldData = fDataPre->Integral(BMassMin,BMassMax)/binwidthmass;
		PreCutYieldErrData = fDataPre->Integral(BMassMin,BMassMax)/binwidthmass*fDataPre->GetParError(0)/fDataPre->GetParameter(0);

		c->SaveAs(Form("Plots/Fits/Data_%s_Work.png",VarName[j].Data()));
		c->SaveAs(Form("Plots/Fits/Data_%s_Work.pdf",VarName[j].Data()));





	//	fMCPre = fit(c, cMC, MCPre, MCPre, ptMin, ptMax, 1, isPbPb, total, CentMinBin, CentMaxBin, "1");
	//	MCPre->SetBinErrorOption(TH1::kPoisson);
		//PreCutYieldMC = fMCPre->Integral(BMassMin,BMassMax)/binwidthmass;
		//PreCutYieldErrMC = fMCPre->Integral(BMassMin,BMassMax)/binwidthmass*fMCPre->GetParError(0)/fMCPre->GetParameter(0);
		PreCutYieldMC = MCPre->Integral();
		PreCutYieldErrMC = TMath::Sqrt(PreCutYieldMC);

		//c->SaveAs(Form("Plots/Fits/MC_%s_Work.png",VarName[j].Data()));
		//c->SaveAs(Form("Plots/Fits/MC_%s_Work.pdf",VarName[j].Data()));



		cout << "PreCutYieldData = " << PreCutYieldData << "   PreCutYieldMC  = " << PreCutYieldMC << endl;


		for(int i = 0; i < NCut[j]; i++)
			//for(int i = 0; i < 0; i++)
		{

			cout << "Now Working on " << Var[j].Data() << "  i = " << i <<  "  CutValue = " << CutValue << endl;

			CutValue = VarCutMin[j] + i * 	NVarStep[j];
			//VarCutData = Form("((%s %s %f && %s)||(%s)) && (%s)",Var[j].Data(),Direction[j].Data(),CutValue,VarPtRange[j].Data(),OutSideCut[j].Data(),PreCut.Data());
			VarCutData = Form("((%s %s %f && %s) && (%s))",Var[j].Data(),Direction[j].Data(),CutValue,VarPtRange[j].Data(),PreCut.Data());

			cout << "VarCutData = " << VarCutData.Data() << endl;


			VarCutMC = Form("((%s) && Bgen == 23333)",VarCutData.Data());

			cout << "VarCutMC = " << VarCutMC.Data() << endl;


			HisData = new TH1D(Form("%s_%d_DataHis",VarName[j].Data(),i),Form("%s_%d_DataHis",VarName[j].Data(),i),NBMassBin,BMassMin,BMassMax);
			HisMC = new TH1D(Form("%s_%d_MCHis",VarName[j].Data(),i),Form("%s_%d_MCHis",Var[j].Data(),i),NBMassBin,BMassMin,BMassMax);	


			cout << "MC REALLY CUT = " << (TCut(BptWeight.Data())*TCut(CentWeight.Data())*TCut(PVzWeight.Data())*TCut(weightfunctionreco.Data()))*(TCut(VarCutMC.Data())) << endl;

			nt->Project(Form("%s_%d_DataHis",VarName[j].Data(),i),"Bmass",VarCutData.Data());
			ntMC->Project(Form("%s_%d_MCHis",VarName[j].Data(),i),"Bmass",(TCut(BptWeight.Data())*TCut(CentWeight.Data())*TCut(PVzWeight.Data())*TCut(weightfunctionreco.Data()))*(TCut(VarCutMC.Data())));



			cout << "Data Integral " << HisData->Integral() << endl;
			cout << "MC Integral " << HisMC->Integral() << endl;
			HisData->SetTitle("");
			HisData->SetBinErrorOption(TH1::kPoisson);
			HisMC->SetTitle("");
			fData = fit(c, cMC, HisData, HisMC, ptMin, ptMax, 0, isPbPb, total, CentMinBin, CentMaxBin, "1");



			YieldData = fData->Integral(BMassMin,BMassMax)/binwidthmass;
			YieldErrData = fData->Integral(BMassMin,BMassMax)/binwidthmass*fData->GetParError(0)/fData->GetParameter(0);

			cout << "YieldData = " << YieldData << endl;
			cout << "YieldErrData = " << YieldErrData << endl;

			TLatex* tex;
			TLatex* tex2;
			TLatex* tex3;

			tex = new TLatex(0.25,0.80,Form("%s %s %.2f",Var[j].Data(),Direction[j].Data(),CutValue));
			tex->SetNDC();
			tex->SetTextFont(42);
			tex->SetTextSize(0.045);
			tex->SetLineWidth(2);
			tex->Draw("SAME");

			tex2 = new TLatex(0.25,0.70,Form("Data Yield = %.1f ",YieldData));
			tex2->SetNDC();
			tex2->SetTextFont(42);
			tex2->SetTextSize(0.045);
			tex2->SetLineWidth(2);
			tex2->Draw("SAME");

			tex3 = new TLatex(0.25,0.60,Form("Data Yield Error = %.1f",YieldErrData));
			tex3->SetNDC();
			tex3->SetTextFont(42);
			tex3->SetTextSize(0.045);
			tex3->SetLineWidth(2);
			tex3->Draw("SAME");



			c->SaveAs(Form("Plots/Fits/Data_%s_%d.png",VarName[j].Data(),i));
			c->SaveAs(Form("Plots/Fits/Data_%s_%d.pdf",VarName[j].Data(),i));




			//fMC = fit(c, cMC, HisMC, HisMC, ptMin, ptMax, 1, isPbPb, total, CentMinBin, CentMaxBin, "1");

			//		YieldMC = fMC->Integral(BMassMin,BMassMax)/binwidthmass;
			//		YieldErrMC = fMC->Integral(BMassMin,BMassMax)/binwidthmass*fMC->GetParError(0)/fMC->GetParameter(0);

			YieldMC = HisMC->Integral();
			YieldErrMC = TMath::Sqrt(YieldMC);
	
			/*
			tex = new TLatex(0.25,0.80,Form("%s %s %f",Var[j].Data(),Direction[j].Data(),CutValue));
			tex->SetNDC();
			tex->SetTextFont(42);
			tex->SetTextSize(0.045);
			tex->SetLineWidth(2);
			tex->Draw("SAME");

			tex2 = new TLatex(0.25,0.70,Form("MC Yield = %f  -  Data Yield Error = %f",YieldData));
			tex2->SetNDC();
			tex2->SetTextFont(42);
			tex2->SetTextSize(0.045);
			tex2->SetLineWidth(2);
			tex2->Draw("SAME");

			tex3 = new TLatex(0.25,0.60,Form("MC Yield Error = %f",YieldErrData));
			tex3->SetNDC();
			tex3->SetTextFont(42);
			tex3->SetTextSize(0.045);
			tex3->SetLineWidth(2);
			tex3->Draw("SAME");
			*/




		//	c->SaveAs(Form("Plots/Fits/MC_%s_%d.png",VarName[j].Data(),i));
		//	c->SaveAs(Form("Plots/Fits/MC_%s_%d.pdf",VarName[j].Data(),i));


			//		YieldMC = HisMC->Integral();
			//		YieldErrMC = TMath::Sqrt(YieldMC);	



			DataRatio = YieldData/PreCutYieldData;
			DataRatioErr = DataRatio * sqrt((YieldErrData/YieldData)*(YieldErrData/YieldData) + (PreCutYieldErrData/PreCutYieldData)*(PreCutYieldErrData/PreCutYieldData));

			MCRatio = YieldMC/PreCutYieldMC;
			MCRatioErr = MCRatio * sqrt((YieldErrMC/YieldMC)*(YieldErrMC/YieldMC) + (PreCutYieldErrMC/PreCutYieldMC)*(PreCutYieldErrMC/PreCutYieldMC));

			doubleratio = DataRatio/MCRatio;
			doubleratioErr = doubleratio * sqrt((MCRatioErr/MCRatio)*(MCRatioErr/MCRatio) + (DataRatioErr/DataRatio)*(DataRatioErr/DataRatio));


			cout << "DataRatio = " << DataRatio << "  MCRatio = " << MCRatio << endl;


			cout << "YieldData = " << YieldData << "  PreCutYieldData = " << PreCutYieldData << " YieldMC =  " << YieldMC << endl;

			if(YieldData > 0 && YieldMC > 0){
				cout << "doubleratio = " << doubleratio << "  doubleratioErr = " << doubleratioErr << endl;
				out1 << alpha	<<	 " & " << DataRatio << " $\\pm$ " << DataRatioErr <<  " & " << MCRatio <<  " $\\pm$ " << MCRatioErr << " & "  << doubleratio << " $\\pm$ " << doubleratioErr << endl;

				CutValueXBin = ResultHis->GetXaxis()->FindBin(CutValue);

				ResultHis->SetBinContent(CutValueXBin,doubleratio);
				ResultHis->SetBinError(CutValueXBin,doubleratioErr);
			}

			CurrentMax[j] = abs(doubleratio - 1);
			if(CurrentMax[j] > MaxDeviation[j]) MaxDeviation[j] = CurrentMax[j];
			
			cout << "j = " << j  << "   CurrentMax = " << CurrentMax[j]  << "   MaxDeviation = " << MaxDeviation[j] << endl; 

		}
		cResults->cd();
		ResultHis->SetMarkerStyle(20);
		ResultHis->SetMarkerColor(kBlack);
		ResultHis->SetMarkerSize(2);


		TF1 *func = new TF1("func","[0] + [1] * x");
		ResultHis->Fit(func);

		double par0 = func->GetParameter(0);
		double par0Err = func->GetParError(0);

		double par1 = func->GetParameter(1);
		double par1Err = func->GetParError(1);


		cout <<  Var[j].Data() << "  " << " y - intercept = " << par0 << "  y - intercept Error = " << par0Err << "   " << "slope = " << par1 <<  " slope error = " << par1Err << endl;

		fout <<  Var[j].Data() << "  " << " y - intercept = " << par0 << "  y - intercept Error = " << par0Err << "   " << "slope = " << par1 <<  " slope error = " << par1Err << endl;
		ResultHis->Draw("ep");


		TLine *l10 = new TLine(WorkingPoint[j],gPad->GetUymin(),WorkingPoint[j],gPad->GetUymax());
		l10->SetLineStyle(2);
		l10->SetLineWidth(2);
		l10->SetLineColor(kBlue);
		l10->Draw("SAME");



		TLine *unityline = new TLine(VarHisMin[j],1,VarHisMax[j],1);
		unityline->SetLineStyle(2);
		unityline->SetLineWidth(2);
		unityline->SetLineColor(kGreen);
		unityline->Draw("SAME");



		cResults->SaveAs(Form("Plots/CutVar%s.png",VarName[j].Data()));
		cResults->SaveAs(Form("Plots/CutVar%s.pdf",VarName[j].Data()));
		ResultHis->Write();

	}


	CutVarOut->Close();


	for(int i = 0; i < NVar; i++){

		cout << "i = " << i << "  Maximum Deviation = " <<	MaxDeviation[i]  << endl;

	}

}



int main(int argc, char *argv[])
{
	if((argc !=5))
	{
		std::cout << "Wrong number of inputs" << std::endl;
		return 1;
	}

	if(argc == 5)
		CutVariation(argv[1],argv[2],argv[3],argv[4]);
	return 0;
}


