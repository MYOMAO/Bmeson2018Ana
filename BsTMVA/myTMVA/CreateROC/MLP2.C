#include "uti.h"
//#include "../header.h"
#include "/afs/cern.ch/work/s/szhaozho/DSignal/Bfinder/Bfinder/Dntuple/TMVA/TMVAD/tutorialTMVABntupleRunII/myTMVA/weights/TMVA_MLP_pp_pt_0p5_1p0.class.C"
//#include "/afs/cern.ch/work/s/szhaozho/DSignal/Bfinder/Bfinder/Dntuple/TMVA/TMVAD/tutorialTMVABntupleRunII/myTMVA/weights/TMVA_MLP_pp_pt_1p0_2p0.class.C"
#define MAX_XB       100000

void MLP2(TString inputname, TString outputname,
		Float_t ptmin, Float_t ptmax, TString mvatype)
{
	void makeoutput(TString infname, TString ofname, TString treename);
	makeoutput(inputname,Form("%s.root",outputname.Data()),Form("%s_pt_%.0f_%.0f",mvatype.Data(),ptmin,ptmax));
}

void makeoutput(TString infname, TString ofname, TString treename)
{
	TFile* inf = new TFile(infname);
	TTree* t = (TTree*)inf->Get("ntDkpi");
	//	t->AddFriend("mvaTree");


	Int_t           Dsize;
	Float_t         Dchi2cl[MAX_XB];   //[Bsize]
	Float_t         DsvpvDistance[MAX_XB];   //[Bsize]
	Float_t         DsvpvDisErr[MAX_XB];   //[Bsize]
	Float_t         Dalpha[MAX_XB];   //[Bsize]   
	Float_t        MVA[MAX_XB];



	//Float_t			 MVA2[MAX_XB];
	//  if(k < 0.5) Double_t        MVA[MAX_XB]; 
	//  if(k > 0.5) Float_t       MVA[MAX_XB]; //[Bsize]   
	Float_t         Dtrk1thetastar[MAX_XB];   //[Bsize]   
	Float_t         Dtrk1Pt[MAX_XB];
	Float_t         Dtrk2Pt[MAX_XB];
	Float_t         Dtrk1Eta[MAX_XB];
	Float_t         Dtrk2Eta[MAX_XB];

	Float_t         Dtrk1P[MAX_XB]; 
	Float_t         Dtrk2P[MAX_XB]; 
	Float_t         Dtrk1dedx[MAX_XB]; 
	Float_t         Dtrk2dedx[MAX_XB]; 

	t->SetBranchAddress("Dsize", &Dsize);
	t->SetBranchAddress("Dalpha", Dalpha);
	t->SetBranchAddress("Dchi2cl", Dchi2cl);
	t->SetBranchAddress("DsvpvDistance", DsvpvDistance);
	t->SetBranchAddress("DsvpvDisErr", DsvpvDisErr);

	//t->SetBranchAddress("MVA", MVA);

	t->SetBranchAddress("Dtrk1thetastar", Dtrk1thetastar);
	t->SetBranchAddress("Dtrk1Pt", Dtrk1Pt);
	t->SetBranchAddress("Dtrk2Pt", Dtrk2Pt);
	t->SetBranchAddress("Dtrk1Eta", Dtrk1Eta);
	t->SetBranchAddress("Dtrk2Eta", Dtrk2Eta);
	t->SetBranchAddress("Dtrk1P", Dtrk1P);
	t->SetBranchAddress("Dtrk2P", Dtrk2P);
	//  t->SetBranchAddress("Dy", Dy);
	t->SetBranchAddress("Dtrk1dedx", Dtrk1dedx);
	t->SetBranchAddress("Dtrk2dedx", Dtrk2dedx);

	std::vector<std::string> theInputVars;
	std::string a1 = "Dalpha";
	std::string a2 = "Dchi2cl";
	std::string a3 = "DsvpvDistance/DsvpvDisErr";
	//   std::string a4 = "MVA";
	std::string a4 = "DsvpvDistance*TMath::Sin(Dalpha)";
	std::string a5 = "TMath::Cos(Dtrk1thetastar)";
	std::string a6 = "Dtrk1Pt";
	std::string a7 = "Dtrk2Pt";  
	std::string a8 = "Dtrk1Eta";
	std::string a9 = "Dtrk2Eta";
	std::string a10 = "Dtrk1P";
	std::string a11 = "Dtrk2P";
	std::string a12 = "Dtrk1dedx";
	std::string a13 = "Dtrk2dedx";
	theInputVars.push_back(a1);
	theInputVars.push_back(a2);
	theInputVars.push_back(a3);
	theInputVars.push_back(a4);
	theInputVars.push_back(a5);
	theInputVars.push_back(a6);

	theInputVars.push_back(a7);
	theInputVars.push_back(a8);
	theInputVars.push_back(a9);
	theInputVars.push_back(a10);
	theInputVars.push_back(a11);
	theInputVars.push_back(a12);
	theInputVars.push_back(a13);
	std::vector<double> inputValues;
	ReadMLP mva(theInputVars);

	TFile* outf = new TFile(ofname,"recreate");
	TTree* mvaTree = new TTree(Form("%s",treename.Data()),"MLP");

	double MLP[MAX_XB];
	mvaTree->Branch("Dsize",&Dsize,"Dsize/I");
	mvaTree->Branch(Form("%s",treename.Data()),MLP,Form("%s[Dsize]/D",treename.Data()));
	std::cout<<std::endl;
	std::cout<<"  Input file: "<<infname<<std::endl;
	std::cout<<"  Calculating MVA values ..."<<std::endl;
	for(int i=0;i<t->GetEntries();i++)
	{
		if(i%10000==0) std::cout<<std::setiosflags(std::ios::left)<<"  [ \033[1;36m"<<std::setw(10)<<i<<"\033[0m"<<" / "<<std::setw(10)<<t->GetEntries()<<" ] "<<"\033[1;36m"<<Form("%.0f",100.*i/t->GetEntries())<<"%\033[0m"<<"\r"<<std::flush;
		t->GetEntry(i);
		for(int j=0;j<Dsize;j++)
		{
			inputValues.clear();
			inputValues.push_back(Dalpha[j]);
			inputValues.push_back(Dchi2cl[j]);
			inputValues.push_back(DsvpvDistance[j]/DsvpvDisErr[j]);
			//inputValues.push_back(MVA[j]);
			inputValues.push_back(DsvpvDistance[j]*TMath::Sin(Dalpha[j]));
			inputValues.push_back(TMath::Cos(Dtrk1thetastar[j]));
			inputValues.push_back(Dtrk1Pt[j]);
			inputValues.push_back(Dtrk2Pt[j]);
			inputValues.push_back(Dtrk1Eta[j]);
			inputValues.push_back(Dtrk2Eta[j]);
			inputValues.push_back(Dtrk1P[j]);
			inputValues.push_back(Dtrk2P[j]);
			inputValues.push_back(Dtrk1dedx[j]);
			inputValues.push_back(Dtrk2dedx[j]);
			MLP[j] = mva.GetMvaValue(inputValues);      
		}
		mvaTree->Fill();
	}
	std::cout<<std::endl<<"  Processed "<<"\033[1;31m"<<t->GetEntries()<<"\033[0m event(s)."<<std::endl;
	outf->cd();
	outf->Write();
	outf->Close();
}

int main(int argc, char* argv[])
{
	if(argc==6)
	{
		MLP2(argv[1],argv[2],atof(argv[3]),atof(argv[4]),argv[5]);
		return 0;
	}
	else
	{
		std::cout<<"  Error: invalid argument number - BDT()"<<std::endl;
		return 1;
	}
}
