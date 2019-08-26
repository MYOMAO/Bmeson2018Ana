#include "uti.h"
#include "parameters.h"

using namespace std;
//#include "TMVA_BDT_PbPb_15_50_varStage1.class.C"
#define MAX_XB       10000
void NtupleSplitterMC(TString infname)
{
	TFile* inf = new TFile(infname,"READ");

	TString label = "OfficialMCAll";





	TTree* t = (TTree*)inf->Get("Bfinder/ntphi");


	TFile * outf;


	TTree* t_new;

	int NEvent = t->GetEntries();
	int NFile = 0;
	for(int i=0;i< NEvent;i++)
	{

		t->GetEntry(i);





		if(i%3000==0 || i  ==  NEvent - 1){
			if(NFile > 0){
				outf->Write();
				outf->Close();
			}
			std::cout<<std::setiosflags(std::ios::left)<<"  [ \033[1;36m"<<std::setw(10)<<i<<"\033[0m"<<" / "<<std::setw(10)<<t->GetEntries()<<" ] "<<"\033[1;36m"<<Form("%.0f",100.*i/t->GetEntries())<<"%\033[0m"<<"\r"<<std::flush;
			NFile = NFile + 1;
			outf = new TFile(Form("outputfiles%s/output_%d.root",label.Data(),NFile),"recreate");
			outf->cd();

			t_new = t->CloneTree(0);





		}

		t_new->Fill();

	}

}


