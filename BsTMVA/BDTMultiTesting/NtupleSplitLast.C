#include "uti.h"
#include "parameters.h"


//#include "TMVA_BDT_PbPb_15_50_varStage1.class.C"
#define MAX_XB       10000
void NtupleSplitLast(TString infname, 	int FileLoc = 10)
{
	TFile* inf = new TFile(infname,"READ");

	int EventStep = 3000;


	int Last= EventStep * (FileLoc-1);


	//	int NFile = 2659;



	TTree* t = (TTree*)inf->Get("Bfinder/ntphi");
	//	TTree* t2 = (TTree*)inf->Get("hltanalysis/HltTree");
	//	TTree* t3 = (TTree*)inf->Get("hiEvtAnalyzer/HiTree");
	//	TTree* t4 = (TTree*)inf->Get("skimanalysis/HltTree");



	//	TTree* t2_new = t2->CloneTree(0); 
	//	TTree* t3_new = t3->CloneTree(0); 
	//	TTree* t4_new = t4->CloneTree(0); 

	TFile * outf  = new TFile(Form("outputfilesOfficialMC/output_%d.root",FileLoc),"recreate");


	int NEvent = t->GetEntries();
	outf->cd();
	cout <<  "Last = " << Last << endl;
	TTree* t_new = t->CloneTree(0);


	for(int i=Last;i < Last+3000;i++)
	{

		t->GetEntry(i);

		//		t2->GetEntry(i);
		//		t3->GetEntry(i);
		//		t4->GetEntry(i);

		if(i%20==0){ std::cout<<std::setiosflags(std::ios::left)<<"  [ \033[1;36m"<<std::setw(10)<<i<<"\033[0m"<<" / "<<std::setw(10)<<t->GetEntries()<<" ] "<<"\033[1;36m"<<Form("%.0f",100.*i/t->GetEntries())<<"%\033[0m"<<"\r"<<std::flush;
		}

		t_new->Fill();
		//		t2_new->Fill();
		//		t3_new->Fill();
		//		t4_new->Fill();

	}
	outf->Write();
	outf->Close();
}



