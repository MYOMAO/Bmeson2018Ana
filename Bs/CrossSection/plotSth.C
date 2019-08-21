#include "fitB.h"
using namespace std;

const int nVar = 2;
string vexp[nVar] = {"Bmumumass", "Btktkmass"};
string vname[nVar] = {"Bmumumass", "Btktkmass"};
string vxaxis[nVar] = {"mumu mass (GeV)", "KK mass (GeV)"};
int vbins[nVar] = {40, 30};
//float vbinmin[nVar] = {2.9, 0.98};
//float vbinmax[nVar] = {3.4, 1.08};
float vbinmin[nVar] = {3.0, 1.00};
float vbinmax[nVar] = {3.2, 1.04};

double rangemin[nVar] = {3.05,1.013};
double rangemax[nVar] = {3.15,1.028};


int _nBins = nBins;
double *_ptBins = ptBins;

//int _nBins = nBinsY;
//double *_ptBins = ptBinsY;


void plotSth(int usePbPb = 0, TString inputdata = "", TString inputmc = "", TString varExp = "", TString trgselection = "",  TString trgselectionmc = "", TString cut = "", TString cutBefore = "", TString cutmcgen = "", int isMC = 0, Double_t luminosity = 1., int doweight = 0, TString collsyst = "", TString outputfile = "", TString outplotf = "", TString npfit = "", int doDataCor = 0, Float_t centmin = 0., Float_t centmax = 100.)
{
	collisionsystem=collsyst;
	if(varExp == "Bpt750"){
		_nBins = nBins750;
		_ptBins = ptBins750;
		varExp = "Bpt";
	}
	if(varExp == "abs(By)"){
		_nBins = nBinsY;
		_ptBins = ptBinsY;
	}
	if(collisionsystem=="ppInc"||collisionsystem=="PbPbInc"){
		_nBins = nBinsInc;
		_ptBins = ptBinsInc;
	}

	hiBinMin = centmin*2;
	hiBinMax = centmax*2;
	centMin = centmin;
	centMax = centmax;

	TString seldataBefore;
	TString selmcBefore;

	if (!(usePbPb==1||usePbPb==0)) std::cout<<"ERROR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!, you are using a non valid isPbPb option"<<std::endl;
	bool isPbPb=(bool)(usePbPb);

	if(!isPbPb)
	{
		seldata = Form("%s&&%s",trgselection.Data(),cut.Data());
		selmc = Form("%s&&%s",trgselectionmc.Data(),cut.Data());
	}
	else
	{
		seldata = Form("%s&&%s&&hiBin>=%f&&hiBin<=%f",trgselection.Data(),cut.Data(),hiBinMin,hiBinMax);
		selmc = Form("%s&&%s&&hiBin>=%f&&hiBin<=%f",trgselectionmc.Data(),cut.Data(),hiBinMin,hiBinMax);

		seldataBefore = Form("%s&&%s&&hiBin>=%f&&hiBin<=%f",trgselection.Data(),cutBefore.Data(),hiBinMin,hiBinMax);
		selmcBefore = Form("%s&&%s&&hiBin>=%f&&hiBin<=%f",trgselectionmc.Data(),cutBefore.Data(),hiBinMin,hiBinMax);


		cout << "Trigger Data = " << trgselection.Data() << endl;
		cout << "Trigger MC = " << trgselectionmc.Data() << endl;

	}
	cout << "Pass 0" << endl; 


	gStyle->SetTextSize(0.05);
	gStyle->SetTextFont(42);
	gStyle->SetPadRightMargin(0.043);
	gStyle->SetPadLeftMargin(0.18);
	gStyle->SetPadTopMargin(0.1);
	gStyle->SetPadBottomMargin(0.145);
	gStyle->SetTitleX(.0f);

	TFile* inf = new TFile(inputdata.Data());
	TFile* infMC = new TFile(inputmc.Data());

	cout << "Pass 1" << endl; 

	cout << "inputmc = " << inputmc.Data() << endl;

	TH1D* h;
	TH1D* hMC;


	TH1D* hBefore;
	TH1D* hMCBefore;



	TTree* nt;
	TTree* ntMC;

	nt = (TTree*)inf->Get("Bfinder/ntphi");
	nt->AddFriend("hltanalysis/HltTree");
	nt->AddFriend("hiEvtAnalyzer/HiTree");
	nt->AddFriend("skimanalysis/HltTree");
	nt->AddFriend("BDT_pt_15_20");
	nt->AddFriend("BDT_pt_5_10");
	nt->AddFriend("BDT_pt_10_15");	
	nt->AddFriend("BDT_pt_20_50");		

	cout << "Pass 2" << endl; 

	//nt->AddFriend("ntHlt");
	//nt->AddFriend("ntHi");
	//nt->AddFriend("ntSkim");
	//nt->AddFriend("BDTStage1_pt15to50");

	ntMC = (TTree*)infMC->Get("Bfinder/ntphi");
	ntMC->AddFriend("hltanalysis/HltTree");
	ntMC->AddFriend("hiEvtAnalyzer/HiTree");
	ntMC->AddFriend("skimanalysis/HltTree");
	ntMC->AddFriend("BDT_pt_15_20");
	ntMC->AddFriend("BDT_pt_5_10");
	ntMC->AddFriend("BDT_pt_10_15");	
	ntMC->AddFriend("BDT_pt_20_50");	
	ntMC->AddFriend("CentWeightTree");	

	cout << "Pass 3" << endl; 

	TF1 *total;
	TString outputf;
	//outputf = Form("%s",outputfile.Data());
	//TFile* outf = new TFile(outputf.Data(),"recreate");
	//outf->cd();

	weightgen = weightgen_pp;
	weightmc  = weightmc_pp;
	if(usePbPb){
		weightgen = weightgen_PbPb;
		weightmc = weightmc_PbPb;
	}

    TString _isMC = "data";
    if(isMC) _isMC = "mc";
    TString _isPbPb = "pp";
    if(usePbPb) _isPbPb = "PbPb";
    TString _postfix = "";
    if(weightdata!="1") _postfix = "_EFFCOR";
//	varExp="abs(By)";
  //  TString _prefix = "By";
	varExp="Bpt";
	TString _prefix = "Bpt";


	TFile * fout = new TFile(Form("ROOTfiles/Phi-J_%s_%s.root",_prefix.Data(),collisionsystem.Data()),"RECREATE");
	fout->cd();

	for(int i=0;i<_nBins;i++)
	{
		TCanvas* c= new TCanvas(Form("c%d",i),"",600,600);
	    TLatex* tex1 = new TLatex(0.518,0.82,Form("%.1f < %s < %.1f",_ptBins[i],varExp.Data(),_ptBins[i+1]));
	    TLatex* tex2 = new TLatex(0.735,0.75,"|y| < 2.4");
	    TLatex* tex3 = new TLatex(0.25, 0.8,"Data");
	    TLatex* tex4 = new TLatex(0.25, 0.8,"MC");
	    TLatex* tex5 = new TLatex(0.25, 0.7,(usePbPb)? "PbPb" : "pp");
		setTex(tex1);
		setTex(tex2);
		setTex(tex3);
		setTex(tex4);
		setTex(tex5);
		tex3->SetTextSize(0.07);
		for(int v = 0; v < nVar; v++){
    
			TF1 * f = new TF1("f","[0]*Gaus(x,[1],[2])/(sqrt(2*3.14159)*[2])",rangemin[v],rangemax[v]);

		
			_count++;
		
			hBefore  = new TH1D(Form("hBefore%d_%d",i,v),  "", vbins[v], vbinmin[v], vbinmax[v]);
			hMCBefore = new TH1D(Form("hMCBefore%d_%d",i,v),"", vbins[v], vbinmin[v], vbinmax[v]);
		
			cout << "Before SET Bro" << endl;


			setHist(hBefore);
		    hBefore->SetXTitle(vxaxis[v].c_str());
			setHist(hMCBefore);
		    hMCBefore->SetXTitle(vxaxis[v].c_str());

			cout << "Before CUT Bro" << endl;


			cout << "MC Projection = " << Form("(Bgen == 23333) && (%s>%f&&%s<%f &&(%s))",varExp.Data(),_ptBins[i],varExp.Data(),_ptBins[i+1],cutBefore.Data()) << endl;
			//cout << "InsideData = " << Form("(%s&&%s>%f&&%s<%f)*(1/%s)", seldata.Data(),varExp.Data(),_ptBins[i],varExp.Data(),_ptBins[i+1],weightdata.Data()) << endl;

			//cout << "InsideMC = " << Form("%s*(%s&&%s>%f&&%s<%f)", weightmc.Data(), Form("%s&&Bgen==23333",selmc.Data()),varExp.Data(),_ptBins[i],varExp.Data(),_ptBins[i+1]) << endl;


			nt->Project(Form("hBefore%d_%d",i,v),     vexp[v].c_str(), Form("(%s&&%s>%f&&%s<%f)*(1/%s)", seldataBefore.Data(),varExp.Data(),_ptBins[i],varExp.Data(),_ptBins[i+1],weightdata.Data()));
			//ntMC->Project(Form("hMC%d",_count), vexp[v].c_str(), Form("%s*(%s&&%s>%f&&%s<%f)", weightmc.Data(), Form("%s&&Bgen==23333",selmc.Data()),varExp.Data(),_ptBins[i],varExp.Data(),_ptBins[i+1]));
			//	ntMC->Project(Form("hMC%d",_count), vexp[v].c_str(), Form("%s*(((%s)&&(%s>%f&&%s<%f))&&(Bgen == 23333))", weightmc.Data(), selmc.Data(),varExp.Data(),_ptBins[i],varExp.Data(),_ptBins[i+1]));
			cout << "Pass Data Projections" << endl;


			ntMC->Project(Form("hMCBefore%d_%d",i,v), vexp[v].c_str(), Form("(Bgen == 23333) && (%s>%f&&%s<%f &&(%s))",varExp.Data(),_ptBins[i],varExp.Data(),_ptBins[i+1],cutBefore.Data()));
			hBefore->SetAxisRange(0,hBefore->GetMaximum()*1.4*1.2,"Y");
			cout << "hBefore Total = " << hBefore->Integral() << endl;
			cout << "hMCBefore Total = " << hMCBefore->Integral() << endl;


			hMCBefore->Sumw2();
			hMCBefore->Scale(1/hMCBefore->Integral());
			hMCBefore->SetAxisRange(0,hMCBefore->GetMaximum()*1.4*1.2,"Y");
	    	
			TLatex* tex;
			

			hBefore->SetMarkerSize(1.55);
			hBefore->SetMarkerStyle(20);
			hBefore->SetMarkerColor(kBlack);
		



			hBefore->Draw("pe");
		    tex1->Draw();
		    tex2->Draw();
		    tex3->Draw();
		    tex5->Draw();
	        c->SaveAs(Form("%s%s/%s_%s_%d_%s_Before.pdf",outplotf.Data(),_prefix.Data(),"data",_isPbPb.Data(),i,vname[v].c_str()));



			hMCBefore->SetMarkerSize(1.55);
			hMCBefore->SetMarkerStyle(20);
			hMCBefore->SetMarkerColor(kBlack);

			hMCBefore->Draw("pe");
		    tex1->Draw();
		    tex2->Draw();
		    tex4->Draw();
		    tex5->Draw();
	        c->SaveAs(Form("%s%s/%s_%s_%d_%s_Before.pdf",outplotf.Data(),_prefix.Data(),"mc",_isPbPb.Data(),i,vname[v].c_str()));



			
			h   = new TH1D(Form("h%d_%d",i,v),  "", vbins[v], vbinmin[v], vbinmax[v]);
			hMC = new TH1D(Form("hMC%d_%d",i,v),"", vbins[v], vbinmin[v], vbinmax[v]);
		
			setHist(h);
		    h->SetXTitle(vxaxis[v].c_str());
			setHist(hMC);
		    hMC->SetXTitle(vxaxis[v].c_str());




			//cout << "InsideData = " << Form("(%s&&%s>%f&&%s<%f)*(1/%s)", seldata.Data(),varExp.Data(),_ptBins[i],varExp.Data(),_ptBins[i+1],weightdata.Data()) << endl;

			//cout << "InsideMC = " << Form("%s*(%s&&%s>%f&&%s<%f)", weightmc.Data(), Form("%s&&Bgen==23333",selmc.Data()),varExp.Data(),_ptBins[i],varExp.Data(),_ptBins[i+1]) << endl;


			nt->Project(Form("h%d_%d",i,v),     vexp[v].c_str(), Form("(%s&&%s>%f&&%s<%f)*(1/%s)", seldata.Data(),varExp.Data(),_ptBins[i],varExp.Data(),_ptBins[i+1],weightdata.Data()));
			//ntMC->Project(Form("hMC%d",_count), vexp[v].c_str(), Form("%s*(%s&&%s>%f&&%s<%f)", weightmc.Data(), Form("%s&&Bgen==23333",selmc.Data()),varExp.Data(),_ptBins[i],varExp.Data(),_ptBins[i+1]));
			//	ntMC->Project(Form("hMC%d",_count), vexp[v].c_str(), Form("%s*(((%s)&&(%s>%f&&%s<%f))&&(Bgen == 23333))", weightmc.Data(), selmc.Data(),varExp.Data(),_ptBins[i],varExp.Data(),_ptBins[i+1]));
			ntMC->Project(Form("hMC%d_%d",i,v), vexp[v].c_str(), Form("(Bgen == 23333) && (%s>%f&&%s<%f &&(%s))",varExp.Data(),_ptBins[i],varExp.Data(),_ptBins[i+1],cut.Data()));
			h->SetAxisRange(0,h->GetMaximum()*1.4*1.2,"Y");
			cout << "h Total = " << h->Integral() << endl;
			cout << "hMC Total = " << hMC->Integral() << endl;

	
			hMC->Sumw2();
			hMC->Scale(1/hMC->Integral());
			hMC->SetAxisRange(0,hMC->GetMaximum()*1.4*1.2,"Y");
	    	
		
			h->SetMarkerSize(1.55);
			h->SetMarkerStyle(20);
			h->SetMarkerColor(kBlack);
		
	//		h->Fit(f,"R");
			
			cout << "vname = " << vname[v] << endl;
			cout << "By = " << _ptBins[i] << "  -  " << _ptBins[i + 1] << "  Width = " << f->GetParameter(2) << "  Witdth Error = " << f->GetParError(2) << endl;



			h->Draw("pe");
		    tex1->Draw();
		    tex2->Draw();
		    tex3->Draw();
		    tex5->Draw();
	        c->SaveAs(Form("%s%s/%s_%s_%d_%s.pdf",outplotf.Data(),_prefix.Data(),"data",_isPbPb.Data(),i,vname[v].c_str()));



			hMC->SetMarkerSize(1.55);
			hMC->SetMarkerStyle(20);
			hMC->SetMarkerColor(kBlack);

			hMC->Draw("pe");
		    tex1->Draw();
		    tex2->Draw();
		    tex4->Draw();
		    tex5->Draw();
	        c->SaveAs(Form("%s%s/%s_%s_%d_%s.pdf",outplotf.Data(),_prefix.Data(),"mc",_isPbPb.Data(),i,vname[v].c_str()));

			h->Write();
			hMC->Write();
		}
	}  
	//outf->Close();
	fout->Close();
}

int main(int argc, char *argv[])
{
	if(argc==20)
	{
		plotSth(atoi(argv[1]), argv[2], argv[3], argv[4], argv[5], argv[6], argv[7], argv[8], argv[9], atoi(argv[10]), atof(argv[11]), atoi(argv[12]), argv[13], argv[14], argv[15], argv[16], atoi(argv[17]), atof(argv[18]), atof(argv[19]));
	}
	else
	{
		std::cout << "Wrong number of inputs" << std::endl;
		return 1;
	}
	return 0;
}
