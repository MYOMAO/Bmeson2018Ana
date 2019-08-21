#include "uti.h"
#include "parameters.h"
using namespace std;


int _nBins = nBinsCent;
double *_ptBins = ptBinsCent;

void PtShapeSyst(TString WeightedFile, TString UnWeightedFile,  TString plotname, TString outputfile){

	TFile *Weighted = new TFile(WeightedFile.Data());
	TFile *UnWeighted = new TFile(UnWeightedFile.Data());

	TH1D * hEffWeighted = (TH1D*)  Weighted->Get("hEff");
	TH1D * hEffUnWeighted = (TH1D*)  UnWeighted->Get("hEff");
	
	TH1D * hEffRatio = (TH1D *)	hEffWeighted->Clone("hEffRatio");
	TCanvas *c = new TCanvas("c","c",600,600);
	c->cd();


	hEffRatio->Divide(hEffUnWeighted);
	hEffRatio->GetXaxis()->SetTitle("p_{T} (GeV/c)");
	hEffRatio->GetYaxis()->SetTitle("p_{T} NLO Weighted Eff/p_{T} FONLL Weighted Eff");
	hEffRatio->GetYaxis()->SetTitleOffset(1.3);
	hEffRatio->SetTitle("");
	hEffRatio->SetMinimum(0.8);
	hEffRatio->SetMaximum(1.2);

	hEffRatio->Draw("p");



	TLine *l1 = new TLine(0.0,1,180.0,1);
	l1->SetLineStyle(2);
	l1->SetLineWidth(2);
	l1->Draw();


	for(int i = 0; i < hEffRatio->GetNbinsX();i++){

		cout << "dev from unity = " << abs(1 - hEffRatio->GetBinContent(i+1)) << endl;
	}

	c->SaveAs(plotname.Data());
	double Ratio;
	double PtShapeSyst = 0;

	for(int i = 1; i < _nBins + 1; i++){
		Ratio = abs(hEffWeighted->GetBinContent(i)-hEffUnWeighted->GetBinContent(i))/hEffWeighted->GetBinContent(i);
		PtShapeSyst = PtShapeSyst + Ratio;
		
		cout << "Cent " << ptBins[i-1] <<" - " << ptBins[i] << "   Syst = " << Ratio << endl;
		ofstream foutResults(Form("ResultFile/Ptshape.tex"), ios::app);
		foutResults <<  "Cent " << _ptBins[i-1] <<" - " << _ptBins[i] << "   Syst = " << Ratio << endl;
	}

	double PtShapeSystFinal = TMath::Sqrt(PtShapeSyst)/nBins;

	cout << "Pt Shape Systematics in Centrality = " <<PtShapeSystFinal << endl; 


	TFile * fout = new TFile(outputfile.Data(),"RECREATE");
	fout->cd();
	hEffRatio->Write();
	fout->Close();

}

int main(int argc, char *argv[])
{
	if((argc !=5))
	{
		std::cout << "Wrong number of inputs" << std::endl;
		return 1;
	}

	if(argc == 5)
		PtShapeSyst(argv[1],argv[2],argv[3],argv[4]);
	return 0;
}


