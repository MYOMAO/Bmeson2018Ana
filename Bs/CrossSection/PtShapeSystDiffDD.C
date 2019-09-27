#include "uti.h"
#include "parametersDiff.h"
#include "TCanvas.h"
using namespace std;

void PtShapeSystDiffDD(TString plotname,TString outputfile){

	gStyle->SetOptTitle(0);
	gStyle->SetOptStat(0);


	const int NFile = 6;

	TString WeightedFile = "plotPtShapeSyst/FilesDiff/CorrYieldLLOG.root";
	TString UnWeightedFile[NFile]={"plotPtShapeSyst/FilesDiff/CorrYieldLINEAR.root","plotPtShapeSyst/FilesDiff/CorrYieldQUADRATIC.root","plotPtShapeSyst/FilesDiff/CorrYieldLINVERSE.root","plotPtShapeSyst/FilesDiff/CorrYieldLSQRT.root","plotPtShapeSyst/FilesDiff/CorrYieldNOMINALPP.root","plotPtShapeSyst/FilesDiff/CorrYieldVARIATIONPP.root"}; 

//TString UnWeightedFile[NFile]={"plotPtShapeSyst/Files/CorrYieldVARIATIONPP.root","plotPtShapeSyst/Files/CorrYieldDATACENTRAL.root","plotPtShapeSyst/Files/CorrYieldDATA1PS.root","plotPtShapeSyst/Files/CorrYieldDATA1MS.root"}; 


//	TString WeightName[NFile]={"Data Driven: Linear Fits","Data Driven: Quadratic Fits","Data Driven: Linear + Inverse Fits","Data Driven: Linear + SQRT Fits"};
	//TString WeightName[NFile]={"Variation PP","Data-MC Best Fit","Data-MC Slope + 1 Sigma","Data-MC Slope - 1 Sigma"};

	TString WeightName[NFile]={"Data Driven: Linear Fits","Data Driven: Quadratic Fits","Data Driven: Linear + Inverse Fits","Data Driven: Linear + SQRT Fits","Nominal PP Shape","Variation PP Shape"};



	TH1D * hEffRatio[NFile];

    TLatex* texChi = new TLatex(0.00,0.96, "Nominal For Bpt Reweighting: Data - Driven Linear + LOG Fits");
	texChi->SetNDC();
	texChi->SetTextAlign(12);
	texChi->SetTextSize(0.04);
	texChi->SetTextFont(42);
	texChi->SetTextColor(7);

	//int color[NFile]={2,3,4,6};
	int color[NFile]={2,3,4,6,1,33};

	for(int i = 0; i < NFile; i++){


		TFile *Weighted = new TFile(WeightedFile.Data());
		TFile *UnWeighted = new TFile(UnWeightedFile[i].Data());

		TH1D * hEffWeighted = (TH1D*)  Weighted->Get("CorrYield");
		TH1D * hEffUnWeighted = (TH1D*)  UnWeighted->Get("CorrYield");

		hEffRatio[i] = (TH1D *)	hEffUnWeighted->Clone("hEffRatio");

			

		hEffRatio[i]->Divide(hEffWeighted);
		hEffRatio[i]->GetXaxis()->SetTitle("hiBin");
		//	hEffRatio->GetYaxis()->SetTitle("p_{T} Weighted Eff/p_{T} Unweighted Eff");
		hEffRatio[i]->GetYaxis()->SetTitle("Corrected Yield Ratio");
		hEffRatio[i]->GetYaxis()->CenterTitle();
		hEffRatio[i]->GetXaxis()->CenterTitle();


		hEffRatio[i]->GetYaxis()->SetTitleOffset(1.3);
		hEffRatio[i]->SetTitle("");
		hEffRatio[i]->SetMinimum(0.7);
		hEffRatio[i]->SetMaximum(1.7);
		hEffRatio[i]->Draw("p");
		hEffRatio[i]->SetMarkerColor(color[i]);
		hEffRatio[i]->SetLineColor(color[i]);
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



	
	TLegend* leg = new TLegend(0.09,0.60,0.55,0.87,NULL,"brNDC");
	leg->SetBorderSize(0);
	leg->SetTextSize(0.04);
	leg->SetTextFont(42);
	leg->SetFillStyle(0);



	TFile * fout = new TFile(outputfile.Data(),"RECREATE");
	fout->cd();

	TCanvas * c1 = new TCanvas("c1","c1",600,600);
	c1->cd();

	
	for(int i = 0; i < NFile; i++){

		if(i == 0) {
			hEffRatio[i]->Draw();
			TLine *l1 = new TLine(0.0,1,180.0,1);
			l1->SetLineStyle(2);
			l1->SetLineWidth(2);
			l1->SetLineColor(7);
			l1->Draw();
		}
		if(i > 0) hEffRatio[i]->Draw("pSAME");

		leg->AddEntry(hEffRatio[i],Form("%s/Nominal",WeightName[i].Data()),"pl");

		hEffRatio[i]->Write();
		
	}

	leg->Draw("SAME");
	texChi->Draw("SAME");
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
		PtShapeSystDiffDD(argv[1],argv[2]);
	return 0;
}


