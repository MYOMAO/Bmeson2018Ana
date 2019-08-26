#include "uti.h"
#include "parameters.h"

using namespace std;
//#include "TMVA_BDT_PbPb_15_50_varStage1.class.C"
#define MAX_XB       10000
void NtupleSplitter(TString infname)
{
	TFile* inf = new TFile(infname,"READ");

	TString label = "Resplit";





	TTree* t = (TTree*)inf->Get("Bfinder/ntphi");
	TTree* t2 = (TTree*)inf->Get("hltanalysis/HltTree");
	TTree* t3 = (TTree*)inf->Get("hiEvtAnalyzer/HiTree");
	TTree* t4 = (TTree*)inf->Get("skimanalysis/HltTree");



	TFile * outf;


	TTree* t_new;
	TTree* t2_new; 
	TTree* t3_new; 
	TTree* t4_new; 

	int NEvent = t->GetEntries();
	int NFile = 0;
	for(int i=0;i< NEvent;i++)
	{

		t->GetEntry(i);
		t2->GetEntry(i);
		t3->GetEntry(i);
		t4->GetEntry(i);




		if(i%3000==0 || i  ==  NEvent - 1){
			if(NFile > 0){
				outf->Write();
				outf->Close();
			}
			std::cout<<std::setiosflags(std::ios::left)<<"  [ \033[1;36m"<<std::setw(10)<<i<<"\033[0m"<<" / "<<std::setw(10)<<t->GetEntries()<<" ] "<<"\033[1;36m"<<Form("%.0f",100.*i/t->GetEntries())<<"%\033[0m"<<"\r"<<std::flush;
			NFile = NFile + 1;
			outf = new TFile(Form("outputfiles%s/output_%d.root",label.Data(),NFile),"recreate");
			outf->cd();


			outf->mkdir("Bfinder");
			outf->mkdir("hltanalysis");
			outf->mkdir("hiEvtAnalyzer");
			outf->mkdir("skimanalysis");


			outf->cd("Bfinder");
			t_new = t->CloneTree(0);



			outf->cd();
			outf->cd("hltanalysis");
			t2_new = t2->CloneTree(0);

			outf->cd();
			outf->cd("hiEvtAnalyzer");
			t3_new = t3->CloneTree(0);


			outf->cd();
			outf->cd("skimanalysis");
			t4_new = t4->CloneTree(0);



		}

		t_new->Fill();
		t2_new->Fill();
		t3_new->Fill();
		t4_new->Fill();

	}

}


