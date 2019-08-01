#include "fitBpdf.h"

int _nBins = nBins;
double *_ptBins = ptBins;
void fitBpdf(int usePbPb = 0, int fitOnSaved = 0, TString inputdata = "", TString inputmc = "", TString _varExp = "", TString trgselection = "",  TString cut = "", TString cutmcgen = "", int isMC = 0, Double_t luminosity = 1., int doweight = 0, TString collsyst = "", TString outputfile = "", TString outplotf = "", TString npfit = "", int doDataCor = 0, Float_t centmin = 0., Float_t centmax = 100.)
{
	collisionsystem=collsyst;
	TString varExp = _varExp;
	if(_varExp == "Bpt750"){
		_nBins = nBins750;
		_ptBins = ptBins750;
		varExp = "Bpt";
	}
	if(_varExp == "Bpt750_acc"){
		_nBins = nBins750_acc;
		_ptBins = ptBins750_acc;
		varExp = "Bpt";
	}
	if(_varExp == "abs(By)"){
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

	if (!(usePbPb==1||usePbPb==0)) std::cout<<"ERROR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!, you are using a non valid isPbPb option"<<std::endl;
	bool isPbPb=(bool)(usePbPb);

	if(!isPbPb)
	{
		seldata = Form("%s&&%s",trgselection.Data(),cut.Data());
		selmc = Form("%s",cut.Data());
		selmcgen = Form("%s",cutmcgen.Data());
	}
	else
	{
		seldata = Form("%s&&%s&&hiBin>=%f&&hiBin<=%f",trgselection.Data(),cut.Data(),hiBinMin,hiBinMax);
		selmc = Form("%s&&hiBin>=%f&&hiBin<=%f",cut.Data(),hiBinMin,hiBinMax);
		selmcgen = Form("%s&&hiBin>=%f&&hiBin<=%f",cutmcgen.Data(),hiBinMin,hiBinMax);
	}

	TFile *foutCheck = new TFile("CheckPlots.root","RECREATE");




	gStyle->SetTextSize(0.05);
	gStyle->SetTextFont(42);
	gStyle->SetPadRightMargin(cRightMargin);
	gStyle->SetPadLeftMargin(cLeftMargin);
	gStyle->SetPadTopMargin(cTopMargin);
	gStyle->SetPadBottomMargin(cBottomMargin);
	gStyle->SetTitleX(.0f);

	TFile* inf = new TFile(inputdata.Data());
	TFile* infMC = new TFile(inputmc.Data());

	TH1D* h;
	TH1D* hMCSignal;
	TH1D* hpull;

	TTree* nt;
	TTree* ntMC;
	TTree* ntGen;

	cout << "Pass 1" << endl;
	if(fitOnSaved == 0){
		nt = (TTree*)inf->Get("Bfinder/ntphi");
		nt->AddFriend("hltanalysis/HltTree");
		nt->AddFriend("hiEvtAnalyzer/HiTree");
		nt->AddFriend("skimanalysis/HltTree");
		nt->AddFriend("BDT_pt_15_20");
		nt->AddFriend("BDT_pt_5_10");
		nt->AddFriend("BDT_pt_10_15");	
		nt->AddFriend("BDT_pt_20_50");		
		/*
		   nt->AddFriend("BDT_pt_15_20");
		   nt->AddFriend("BDT_pt_7_15");
		   nt->AddFriend("BDT_pt_5_7");
		   nt->AddFriend("BDT_pt_20_50");	
		   */
		//	nt->AddFriend("BDT");	

		/*
		   ntGen = (TTree*)infMC->Get("ntGen");
		   ntGen->AddFriend("ntHlt");
		   ntGen->AddFriend("ntHi");

		   cout << "Pass 2" << endl;

		   ntMC = (TTree*)infMC->Get("ntphi");


		   ntMC->AddFriend("ntHlt");
		   ntMC->AddFriend("ntHi");
		   ntMC->AddFriend("ntSkim");
		   ntMC->AddFriend("mvaTree");
		   ntMC->AddFriend("ntGen");
		   */
		ntGen = (TTree*)infMC->Get("Bfinder/ntGen");
		ntGen->AddFriend("hltanalysis/HltTree");
		ntGen->AddFriend("hiEvtAnalyzer/HiTree");


		ntMC = (TTree*)infMC->Get("Bfinder/ntphi");
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
		   ntMC->AddFriend("BDT_pt_5_7");
		   ntMC->AddFriend("BDT_pt_20_50");	
		   */
		//	ntMC->AddFriend("BDT");
		ntMC->AddFriend("Bfinder/ntGen");
	}

	cout << "Pass 3" << endl;


	double ptmin;
	double ptmax;
	const int NPDF = 9;


	TString FitMethodName[NPDF]={"default","linear","2nd","Expo","Single","Triple","increase","decrease","fixmean"};
	TString OutPlotName[NPDF]={"plotFits/pdfofficial/DMass_defaultnew","plotFits/pdfofficial/DMass_poly1","plotFits/pdfofficial/DMass_poly2","plotFits/pdfofficial/DMass_expo2","plotFits/pdfofficial/DMass_SingleGaus","plotFits/pdfofficial/DMass_TripleGaus","plotFits/pdfofficial/DMass_increasewid","plotFits/pdfofficial/DMass_decreasewid","plotFits/pdfofficial/DMass_fixmean"};

	TString foutName[NPDF]={Form("outYield/defaultnew_%s_cent_%.0f_%.0f.dat",collsyst.Data(),centmin,centmax),Form("outYield/poly1_%s_cent_%.0f_%.0f.dat",collsyst.Data(),centmin,centmax),Form("outYield/poly2_%s_cent_%.0f_%.0f.dat",collsyst.Data(),centmin,centmax),Form("outYield/expo2_%s_cent_%.0f_%.0f.dat",collsyst.Data(),centmin,centmax),Form("outYield/SingleGaus_%s_cent_%.0f_%.0f.dat",collsyst.Data(),centmin,centmax),Form("outYield/TripleGaus_%s_cent_%.0f_%.0f.dat",collsyst.Data(),centmin,centmax),Form("outYield/increasewid_%s_cent_%.0f_%.0f.dat",collsyst.Data(),centmin,centmax),Form("outYield/decreasewid_%s_cent_%.0f_%.0f.dat",collsyst.Data(),centmin,centmax),Form("outYield/fixmean_%s_cent_%.0f_%.0f.dat",collsyst.Data(),centmin,centmax)};




	TString falphaName[NPDF]={Form("outAlpha/defaultnew_%s_cent_%.0f_%.0f.dat",collsyst.Data(),centmin,centmax),Form("outAlpha/poly1_%s_cent_%.0f_%.0f.dat",collsyst.Data(),centmin,centmax),Form("outAlpha/poly2_%s_cent_%.0f_%.0f.dat",collsyst.Data(),centmin,centmax),Form("outAlpha/expo2_%s_cent_%.0f_%.0f.dat",collsyst.Data(),centmin,centmax),Form("outAlpha/SingleGaus_%s_cent_%.0f_%.0f.dat",collsyst.Data(),centmin,centmax),Form("outAlpha/TripleGaus_%s_cent_%.0f_%.0f.dat",collsyst.Data(),centmin,centmax),Form("outAlpha/increasewid_%s_cent_%.0f_%.0f.dat",collsyst.Data(),centmin,centmax),Form("outAlpha/decreasewid_%s_cent_%.0f_%.0f.dat",collsyst.Data(),centmin,centmax),Form("outAlpha/fixmean_%s_cent_%.0f_%.0f.dat",collsyst.Data(),centmin,centmax)};
	int FitOpt1[NPDF] = {0,0,0,0,1,2,3,4,5};
	int FitOpt2[NPDF] = {0,1,2,3,0,0,0,0,0};

	TF1 *total;
	TString outputf;
	outputf = Form("%s",outputfile.Data());
	TFile* outf = new TFile(outputf.Data(),"recreate");
	outf->cd();

	TH1D* hPt = new TH1D("hPt","",_nBins,_ptBins);
	TH1D* hPtMC = new TH1D("hPtMC","",_nBins,_ptBins);
	TH1D* hPtGen = new TH1D("hPtGen","",_nBins,_ptBins);
	TH1D* hMean = new TH1D("hMean","",_nBins,_ptBins);                       
	TH1D* hSigmaGaus1 = new TH1D("hSigmaGaus1","",_nBins,_ptBins); 
	TH1D* hSigmaGaus2 = new TH1D("hSigmaGaus2","",_nBins,_ptBins); 

	weightgen = weightgen_pp;
	weightmc  = weightmc_pp;
	if(usePbPb){
		weightgen = weightgen_PbPb;
		//weightmc = weightmc_PbPb;
		if(doweight == 0 ) weightmc = "1"; 
		if(doweight == 1) weightmc = "pthatweight";
	}

	TString _prefix = "";
	TString _isMC = "data";
	if(isMC) _isMC = "mcAsData";
	TString _isPbPb = "pp";
	if(usePbPb) _isPbPb = "PbPb";
	TString _postfix = "";
	if(weightdata!="1") _postfix = "_EFFCOR";
	/*
	   const int NCentBin = 2;
	   int CentMinBin[NCentBin] ={0,60};
	   int CentMaxBin[NCentBin] ={60,200};
	   */
	const int NCentBin = 1;
	int CentMinBin[NCentBin] ={0};
	int CentMaxBin[NCentBin] ={180};

	cout << "Pass 4" << endl;

	int doPhi = 1;
	int doStat = 1;


	for(int k = 0; k < NPDF; k++){

		ofstream fout(foutName[k].Data());
		ofstream falpha(falphaName[k].Data());


		for(int i=0;i<_nBins;i++)
		{
			_count++;
			for(int j = 0; j < NCentBin; j++){		
				TCanvas* c= new TCanvas(Form("c%d",_count),"",600,600);
				TCanvas* cMC= new TCanvas(Form("cMC%d",_count),"",600,600);


				if(fitOnSaved == 1){
					drawOpt = 1;
					h = (TH1D*)inf->Get(Form("h%d",_count));
					hMCSignal = (TH1D*)inf->Get(Form("hMCSignal%d",_count));
				}
				h->SetBinErrorOption(TH1::kPoisson);


				//TF1* f = fit(c, cMC, h, hMCSignal, _ptBins[i], _ptBins[i+1], isMC, isPbPb, total, centmin, centmax, npfit);
				TF1* f = fit(c, cMC, h, hMCSignal, _ptBins[i], _ptBins[i+1], isMC, isPbPb, total, CentMinBin[j], CentMaxBin[j], npfit, FitOpt1[k],FitOpt2[k]);

				cout << "Pass 5" << endl;

				double yield = f->Integral(minhisto,maxhisto)/binwidthmass;
				double yieldErr = f->Integral(minhisto,maxhisto)/binwidthmass*f->GetParError(0)/f->GetParameter(0);
				printf("yield: %f, yieldErr: %f\n", yield, yieldErr);
				yieldErr = yieldErr*_ErrCor;
				if(fitOnSaved == 0){
					TH1D* htest = new TH1D(Form("htest%d",_count),"",nbinsmasshisto,minhisto,maxhisto);
					TString sideband = "(abs(Bmass-5.367)>0.2&&abs(Bmass-5.367)<0.3";
					nt->Project(Form("htest%d",_count),"Bmass",Form("%s&&%s&&%s>%f&&%s<%f)*(1/%s)",sideband.Data(),seldata.Data(),varExp.Data(),_ptBins[i],varExp.Data(),_ptBins[i+1],weightdata.Data()));
					std::cout<<"yield bkg sideband: "<<htest->GetEntries()<<std::endl;
				}
				cout << "Pass 6" << endl;

				hPt->SetBinContent(i+1,yield/(_ptBins[i+1]-_ptBins[i]));
				hPt->SetBinError(i+1,yieldErr/(_ptBins[i+1]-_ptBins[i]));
				hMean->SetBinContent(i+1,f->GetParameter(1));
				hMean->SetBinError(i+1,f->GetParError(1));  

				TLatex* tex;
				tex = new TLatex(0.35,0.85,Form("%.0f < p_{T} < %.0f GeV/c, %d < Cent < %d ",_ptBins[i],_ptBins[i+1],CentMinBin[j]/2,CentMaxBin[j]/2));
				if(varExp=="abs(By)") tex = new TLatex(0.55,0.85,Form("%.1f < y < %.1f",_ptBins[i],_ptBins[i+1]));
				tex->SetNDC();
				tex->SetTextFont(42);
				tex->SetTextSize(0.045);
				tex->SetLineWidth(2);
				tex->Draw();

				tex = new TLatex(0.75,0.80,"|y| < 2.4");
				if(varExp=="abs(By)") tex = new TLatex(0.75,0.80,"7 < p_{T} < 50 GeV.c");
				tex->SetNDC();
				tex->SetTextFont(42);
				tex->SetTextSize(0.045);
				tex->SetLineWidth(2);
				tex->Draw();


	//			c->SaveAs(Form("%s%s/%s_%s_%d_%d%s.pdf",outplotf.Data(),_prefix.Data(),_isMC.Data(),_isPbPb.Data(),_count,j,_postfix.Data()));
	//			cMC->SaveAs(Form("%s%s/%s_%s_%d_%d%s.pdf",outplotf.Data(),_prefix.Data(),"mc",_isPbPb.Data(),_count,j,_postfix.Data()));
	

				c->SaveAs(Form("%s_%s_cent_%.0f_%.0f_pt_%.0f_%.0f.pdf",OutPlotName[j].Data(),collsyst.Data(),centMin ,centMax,ptBins[i],ptBins[i+1]));
				
				cout << "Pass 7" << endl;

				TCanvas* cpull= new TCanvas(Form("cpull%d",_count),"",600,600);
				cpull->cd();
				TGraphAsymmErrors* pullgraph = new TGraphAsymmErrors();
				pullgraph->SetTitle("");
				pullgraph->SetMaximum(5);
				pullgraph->SetMinimum(-5);
				pullgraph->SetMarkerSize(1.55); pullgraph->SetMarkerStyle(20); pullgraph->SetLineColor(1); pullgraph->SetLineWidth(4);
				double binerr;
				for(int b = 0; b < h->GetNbinsX(); b++){
					binerr = h->GetBinContent(b+1) > total->Eval(h->GetBinCenter(b+1)) ? h->GetBinErrorLow(b+1) : h->GetBinErrorUp(b+1);
					pullgraph->SetPoint(b,h->GetBinCenter(b+1),(h->GetBinContent(b+1)-total->Eval(h->GetBinCenter(b+1)))/binerr);
					//pullgraph->SetPointEYlow(b,h->GetBinErrorLow(b+1)/binerr);
					//pullgraph->SetPointEYhigh(b,h->GetBinErrorUp(b+1)/binerr);
					pullgraph->SetPointEYlow(b,1);
					pullgraph->SetPointEYhigh(b,1);
				}
				TLine* line = new TLine(5., 0., 6., 0.);
				line->SetLineStyle(9);
				line->SetLineWidth(4);
				line->SetLineColor(kGreen+1);
				line->Draw();
				pullgraph->Draw();
			//	cpull->SaveAs(Form("%s%s/%s_%s_%d_%d%s_pull.pdf",outplotf.Data(),_prefix.Data(),_isMC.Data(),_isPbPb.Data(),_count,j,_postfix.Data()));
			}
		}  

	}



	hMean->Write();
	hPt->Write();
	if(fitOnSaved == 1){
		outf->Close();	
		return;
	}

	ntMC->Project("hPtMC",varExp.Data(),TCut(weightmc)*(TCut(selmc.Data())&&"(Bgen==23333)"));
	divideBinWidth(hPtMC);
	ntGen->Project("hPtGen","Gpt",TCut(weightgen)*(TCut(selmcgen.Data())));
	divideBinWidth(hPtGen);

	TCanvas* cPt =  new TCanvas("cPt","",600,600);
	cPt->SetLogy();
	hPt->SetXTitle("B_{s} p_{T} (GeV/c)");
	hPt->SetYTitle("Uncorrected dN(B_{s})/dp_{T}");
	hPt->Draw();
	if(isMC==1)
	{
		hPtMC->Draw("same hist");
		TLegend* legPt = myLegend(0.55,0.80,0.90,0.94);
		legPt->AddEntry(hPt,"Signal extraction","pl");
		legPt->AddEntry(hPtMC,"Matched reco","lf");
		legPt->Draw("same");  
	}
	hPtMC->Sumw2();
	TH1D* hEff = (TH1D*)hPtMC->Clone("hEff");
	hEff->SetTitle(";B_{s} p_{T} (GeV/c);Efficiency");
	hEff->Sumw2();
	hEff->Divide(hPtGen);
	TCanvas* cEff = new TCanvas("cEff","",600,600);
	hEff->Draw();

	TH1D* hPtCor = (TH1D*)hPt->Clone("hPtCor");
	hPtCor->SetTitle(";B_{s} p_{T} (GeV/c);Corrected dN(B_{s})/dp_{T}");
	hPtCor->Divide(hEff);
	TCanvas* cPtCor=  new TCanvas("cCorResult","",600,600);
	cPtCor->SetLogy();
	hPtCor->Draw();
	if(isMC==1)
	{
		hPtGen->Draw("same hist");
		TLegend* legPtCor = myLegend(0.55,0.80,0.90,0.94);
		legPtCor->AddEntry(hPtCor,"Corrected signal","pl");
		legPtCor->AddEntry(hPtGen,"Generated B_{s}","lf");
		legPtCor->Draw("same");  
	}

	TH1D* hPtSigma= (TH1D*)hPtCor->Clone("hPtSigma");
	hPtSigma->SetTitle(";B_{s} p_{T} (GeV/c);d#sigma(B_{s})/dp_{T} (pb/GeV)");
	hPtSigma->Scale(1./(2*luminosity*BRchain));
	TCanvas* cPtSigma=  new TCanvas("cPtSigma","",600,600);
	cPtSigma->SetLogy();
	hPtSigma->Draw();

	hPtMC->Write();
	hPtGen->Write();
	hEff->Write();
	hPtCor->Write();
	hPtSigma->Write();
	outf->Close();
}

int main(int argc, char *argv[])
{
	if(argc==19)
	{
		//fitB(atoi(argv[1]), atoi(argv[2]), argv[3], argv[4], argv[5], argv[6], argv[7], argv[8], argv[9], atoi(argv[10]), atof(argv[11]), atoi(argv[12]), argv[13], argv[14], argv[15], argv[16], atoi(argv[17]), atof(argv[18]), atof(argv[19]));
		fitBpdf(atoi(argv[1]), atoi(argv[2]), argv[3], argv[4], argv[5], argv[6], argv[7], argv[8], atoi(argv[9]), atof(argv[10]), atoi(argv[11]), argv[12], argv[13], argv[14], argv[15], atoi(argv[16]), atof(argv[17]), atof(argv[18]));

	}
	else
	{
		std::cout << "Wrong number of inputs" << std::endl;
		return 1;
	}
	return 0;
}
