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

void Remake( TString infile, TString outfile ){

	TFile * fin = new TFile(infile.Data());
	
	TH1D * hNominalPP =  (TH1D *) fin->Get("hNominalPP");
	TH1D * hVariationPP =  (TH1D *) fin->Get("hVariationPP");
	TH1D * hNominalTAMU =  (TH1D *) fin->Get("hNominalTAMU");
	TH1D * hVariationTAMU =  (TH1D *) fin->Get("hVariationTAMU");

	TH1D * hNominalTAMUComb = (TH1D * ) hNominalTAMU->Clone("hNominalTAMUComb");
	hNominalTAMUComb->Add(hNominalPP,4);


	TFile * fout = new TFile(outfile.Data(),"RECREATE");
	fout->cd();
	hNominalPP->Write();
	hVariationPP->Write();
	hNominalTAMU->Write();
	hVariationTAMU->Write();
	hNominalTAMUComb->Write();

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
		Remake(argv[1],argv[2]);
	return 0;
}
