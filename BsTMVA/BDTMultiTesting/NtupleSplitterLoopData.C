#include "uti.h"
#include "parameters.h"

using namespace std;
//#include "TMVA_BDT_PbPb_15_50_varStage1.class.C"
#define MAX_XB       10000
void NtupleSplitterLoopData(TString infname, int ki= 0, int kf = 4)
{
	TFile* inf = new TFile(infname,"READ");

	TString label = "Resplit";



	const int NTrees = 4;
	TTree* t[NTrees];
	TString Names[NTrees]={"Bfinder/ntphi","hltanalysis/HltTree","hiEvtAnalyzer/HiTree","skimanalysis/HltTree"};
	TString FolderName[NTrees]={"Bfinder","hltanalysis","hiEvtAnalyzer","skimanalysis"};
	TTree* t_new[NTrees];


	for(int k = ki; k < kf; k++){

		cout << "Working on k Tree = " << k  << endl;

		t[k] = (TTree*)inf->Get(Names[k].Data());


		TFile * outf;



		int NEvent = t[k]->GetEntries();
		int NFile = 0;
		for(int i=0;i< NEvent;i++)
		{

			t[k]->GetEntry(i);



			if(i%3000==0 || i == NEvent - 1){
				if(NFile > 0){
					outf->Write();
					outf->Close();
				}
				std::cout<<std::setiosflags(std::ios::left)<<"  [ \033[1;36m"<<std::setw(10)<<i<<"\033[0m"<<" / "<<std::setw(10)<<t[k]->GetEntries()<<" ] "<<"\033[1;36m"<<Form("%.0f",100.*i/t[k]->GetEntries())<<"%\033[0m"<<"\r"<<std::flush;
				NFile = NFile + 1;
				outf = new TFile(Form("outputfiles%s/output_%d_%d.root",label.Data(),NFile,k),"recreate");
				outf->cd();


				outf->mkdir(FolderName[k].Data());

				outf->cd(FolderName[k].Data());
				t_new[k] = t[k]->CloneTree(0);

			}

			t_new[k]->Fill();

		}

	}
}


