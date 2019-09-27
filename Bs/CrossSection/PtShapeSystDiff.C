#include "uti.h"
#include "parametersDiff.h"
#include "TCanvas.h"
using namespace std;

void PtShapeSystDiff(TString plotname,TString outputfile){

	gStyle->SetOptTitle(0);
	gStyle->SetOptStat(0);


	const int NFile = 4;

	TString WeightedFile = "plotPtShapeSyst/FilesDiff/CorrYieldNOMINALPP.root";
	TString UnWeightedFile[NFile]={"plotPtShapeSyst/FilesDiff/CorrYieldVARIATIONPP.root","plotPtShapeSyst/FilesDiff/CorrYieldNOMINALTAMU.root","plotPtShapeSyst/FilesDiff/CorrYieldVARIATIONTAMU.root","plotPtShapeSyst/FilesDiff/CorrYieldTAMUPP.root"}; 

//TString UnWeightedFile[NFile]={"plotPtShapeSyst/Files/CorrYieldVARIATIONPP.root","plotPtShapeSyst/Files/CorrYieldDATACENTRAL.root","plotPtShapeSyst/Files/CorrYieldDATA1PS.root","plotPtShapeSyst/Files/CorrYieldDATA1MS.root"}; 


	TString WeightName[NFile]={"Variation PP","Nominal TAMU","Variation TAMU","20% TAMU + 80% Nominal PP"};
	//TString WeightName[NFile]={"Variation PP","Data-MC Best Fit","Data-MC Slope + 1 Sigma","Data-MC Slope - 1 Sigma"};


TH1D * hEffRatio[NFile];
	for(int i = 0; i < 3; i++){


		TFile *Weighted = new TFile(WeightedFile.Data());
		TFile *UnWeighted = new TFile(UnWeightedFile[i].Data());

		TH1D * hEffWeighted = (TH1D*)  Weighted->Get("CorrYield");
		TH1D * hEffUnWeighted = (TH1D*)  UnWeighted->Get("CorrYield");

		hEffRatio[i] = (TH1D *)	hEffUnWeighted->Clone("hEffRatio");

			

		hEffRatio[i]->Divide(hEffWeighted);
		hEffRatio[i]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
		//	hEffRatio->GetYaxis()->SetTitle("p_{T} Weighted Eff/p_{T} Unweighted Eff");
		hEffRatio[i]->GetYaxis()->SetTitle(Form("p_{T} FONLL %s Weighted Yield/p_{T} Nominal PP Weighted Yield",WeightName[i].Data()));


		hEffRatio[i]->GetYaxis()->SetTitleOffset(1.3);
		hEffRatio[i]->SetTitle("");
		hEffRatio[i]->SetMinimum(0.8);
		hEffRatio[i]->SetMaximum(1.7);
		hEffRatio[i]->Draw("p");
		hEffRatio[i]->SetMarkerColor(i+1);
		hEffRatio[i]->SetLineColor(i+1);
		hEffRatio[i]->SetMarkerStyle(20);




		cout << WeightName[i].Data() << endl;

		for(int j = 0; j < hEffRatio[i]->GetNbinsX();j++){

			cout << "dev from unity = " << abs(1 - hEffRatio[i]->GetBinContent(j+1)) << endl;
		}


		double Ratio;
		double PtShapeSyst = 0;

		for(int j = 1; j < nBins + 1; j++){
			Ratio = abs(hEffWeighted->GetBinContent(j)-hEffUnWeighted->GetBinContent(j))/hEffWeighted->GetBinContent(j);
			PtShapeSyst = PtShapeSyst + Ratio;

			cout << "pt " << ptBins[j-1] <<" - " << ptBins[j] << "   Syst = " << Ratio << endl;
			ofstream foutResults(Form("ResultFile/Ptshape.tex"), ios::app);
			foutResults <<  "pt " << ptBins[j-1] <<" - " << ptBins[j] << "   Syst = " << Ratio << endl;
		}

		double PtShapeSystFinal = TMath::Sqrt(PtShapeSyst)/nBins;

		cout << "Pt Shape Systematics = " <<PtShapeSystFinal << endl; 

	}


	
	
	TLegend* leg = new TLegend(0.12,0.60,0.55,0.92,NULL,"brNDC");
	leg->SetBorderSize(0);
	leg->SetTextSize(0.04);
	leg->SetTextFont(42);
	leg->SetFillStyle(0);



	TFile * fout = new TFile(outputfile.Data(),"RECREATE");
	fout->cd();

	TCanvas * c1 = new TCanvas("c1","c1",600,600);
	c1->cd();

	
	for(int i = 0; i < 3; i++){

		if(i == 0) {
			hEffRatio[i]->Draw();
			TLine *l1 = new TLine(0.0,1,180.0,1);
			l1->SetLineStyle(2);
			l1->SetLineWidth(2);
			l1->Draw();
		}
		if(i > 0) hEffRatio[i]->Draw("epSAME");
	
		leg->AddEntry(hEffRatio[i],Form("%s/Nominal PP",WeightName[i].Data()),"pl");

		hEffRatio[i]->Write();
		
	}

	leg->Draw("SAME");

	c1->SaveAs(plotname.Data());


	//hEffRatio->Write();
	fout->Close();

}

int main(int argc, char *argv[])
{
	if((argc !=3))
	{
		std::cout << "Wrong number of inputs" << std::endl;
		return 1;
	}

	if(argc == 3)
		PtShapeSystDiff(argv[1],argv[2]);
	return 0;
}


