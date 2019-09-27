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

void FitEff(){

	const int NCent =3;
	
	int CentMin[NCent]={0,0,30};
	int CentMax[NCent]={30,90,90};

	for(int i = 0; i < NCent; i++){

	TString FileName = Form("EffFine_%d_%d.root",CentMin[i],CentMax[i]);

	TFile * fin = new TFile(FileName.Data());

	TH1D * hEffFine = (TH1D *) fin->Get("hEffFine");


	TF1 * f = new TF1("f1","1/(1/[4]+TMath::Exp(-[0]*(x-[1]))+TMath::Exp(-[2]*(x-[3])))",5,50);

	f->SetParLimits(0,1,2);
	f->SetParLimits(2,0,1);
	f->SetParLimits(1,10,50);
	f->SetParLimits(3,10,50);
	f->SetParLimits(4,0.1,0.3);


	hEffFine->Fit(f);


	TCanvas * c = new TCanvas("c","c",600,600);
	

	c->cd();

	TLegend* leg = new TLegend(0.09,0.60,0.55,0.87,NULL,"brNDC");
	leg->SetBorderSize(0);
	leg->SetTextSize(0.04);
	leg->SetTextFont(42);
	leg->SetFillStyle(0);
	leg->AddEntry(hEffFine,"Efficiency vs B_{s} p_{T}","pl");
	leg->AddEntry(f,"Fit Function","pl");

	hEffFine->GetXaxis()->SetTitle("B_{s} p_T (GeV/c)");
	hEffFine->GetYaxis()->SetTitle("Acc x Eff");
	hEffFine->GetYaxis()->SetTitleOffset(1.5);
	hEffFine->GetXaxis()->CenterTitle();
	hEffFine->GetYaxis()->CenterTitle();


	hEffFine->Draw("ep");
	f->Draw("SAME");
	leg->Draw("SAME");
	

	double Chi2 = f->GetChisquare(); 
	int NDF = f->GetNDF();
	double Chi2NDF= Chi2/NDF; 

	TLatex* texCms = new TLatex(0.39,0.30 ,Form("#chi^{2}/ndf = %.2f/%d = %.2f",Chi2,NDF,Chi2NDF));
	texCms->SetNDC();
	texCms->SetTextAlign(13);
	texCms->SetTextSize(0.04);
	texCms->SetTextFont(42);
	texCms->Draw("SAME");


	c->SaveAs(Form("EffFits_%d_%d.pdf",CentMin[i],CentMax[i]));
	c->SaveAs(Form("EffFits_%d_%d.pdf",CentMin[i],CentMax[i]));



	double p0 = f->GetParameter(0);
	double p1 = f->GetParameter(1);
	double p2 = f->GetParameter(2);
	double p3 = f->GetParameter(3);
	double p4 = f->GetParameter(4);



	TString Func = Form("1/(1/%f+TMath::Exp(-%f*(Bpt-%f))+TMath::Exp(-%f*(Bpt-%f)))",p4,p0,p1,p2,p3);


	cout << "i = " << i << "   Fit Func = " << Func.Data() << endl;


	TH1D * hEffFineInv = (TH1D *) fin->Get("hEffFineInv");
	hEffFineInv->Draw("ep");
	TF1 * f2 = new TF1("f2", Form("(1/%f+TMath::Exp(-%f*(x-%f))+TMath::Exp(-%f*(x-%f)))",p4,p0,p1,p2,p3),5,50);

	f2->Draw("SAME");

	c->SetLogy();

	c->SaveAs("InvComp.png");

	}


}
