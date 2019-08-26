#ifndef __CINT__
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
#include "../../includes/uti.h"
#include "../header.h"
//#include "/raid5/data/szhaozho/SameSign/myTMVA/dataset/weights/TMVAClassification_DNN_GPU.class.C"

#endif
using namespace std;


#define MAX_XB       20000

void BDTG(TString inputname, TString outputname,
		Float_t ptmin, Float_t ptmax, TString mvatype)
{
	void makeoutput(TString infname, TString ofname, TString treename);
	makeoutput(inputname,Form("%s.root",outputname.Data()),Form("%s_pt_%.0f_%.0f",mvatype.Data(),ptmin,ptmax));
}

void makeoutput(TString infname, TString ofname, TString treename)
{
	TFile* inf = new TFile(infname);
	TTree* t = (TTree*)inf->Get("Bfinder/ntphi");
	//	t->AddFriend("mvaTree");

/*
	Int_t           Dsize;
	Float_t         Dchi2cl[MAX_XB];   //[Bsize]
	Float_t         DsvpvDistance[MAX_XB];   //[Bsize]
	Float_t         DsvpvDisErr[MAX_XB];   //[Bsize]
	Float_t         Dalpha[MAX_XB];   //[Bsize]   
	// Double_t        MVA[MAX_XB];   //[Bsize]   
	//Double_t        MVA[MAX_XB]; 
	Float_t         Dtrk1thetastar[MAX_XB];   //[Bsize]   
	Float_t         Dtrk1Pt[MAX_XB];   //[Bsize]   
	Float_t         Dtrk2Pt[MAX_XB];   //[Bsize]   

	Float_t         Dy[MAX_XB];
	Float_t         Dmass[MAX_XB];
	Float_t         Dpt[MAX_XB];
	Float_t         Dtrk1Eta[MAX_XB];   //[Bsize]   
	Float_t         Dtrk2Eta[MAX_XB];   //[Bsize]   
*/
	Int_t           Bsize;
	Float_t         Bchi2cl[MAX_XB];   //[Bsize]
	Float_t         BsvpvDistance[MAX_XB];   //[Bsize]
	Float_t         BsvpvDisErr[MAX_XB];   //[Bsize]
	Float_t         Balpha[MAX_XB];   //[Bsize]   
	Float_t         Btrk1Pt[MAX_XB];   //[Bsize]   
	Float_t         Btrk2Pt[MAX_XB];   //[Bsize]   
	Float_t         Btrk1Eta[MAX_XB];   //[Bsize]   
	Float_t         Btrk2Eta[MAX_XB];   //[Bsize]   
	Float_t         Btrk1Dxy[MAX_XB];   //[Bsize]   
	Float_t         Btrk2Dxy[MAX_XB];   //[Bsize]   
	Float_t         Btrk1D0Err[MAX_XB];   //[Bsize]   
	Float_t         Btrk2D0Err[MAX_XB];   //[Bsize]   
	Float_t         Btrk1D0[MAX_XB];   //[Bsize]   
	Float_t         Btrk2D0[MAX_XB];   //[Bsize]   
	Float_t         Btrk1Dz[MAX_XB];   //[Bsize]   
	Float_t         Btrk2Dz[MAX_XB];   //[Bsize]   
	//Float_t         Btktkmass[MAX_XB]; 
	Float_t         Bd0[MAX_XB]; 
	Float_t         Bd0Err[MAX_XB]; 
	Float_t         Bdtheta[MAX_XB]; 
	Float_t         Bmu1eta[MAX_XB]; 
	Float_t         Bmu2eta[MAX_XB]; 
	Float_t         Bmu2pt[MAX_XB]; 
	Float_t         Bmu1pt[MAX_XB]; 
        Float_t         Bmumumass[MAX_XB];
	Float_t         Btktkmass[MAX_XB];
        Float_t         Bmass[MAX_XB];
         Float_t         Bmumuphi[MAX_XB];      
	  Float_t         Bmumueta[MAX_XB];
        Float_t         Btrk1Phi[MAX_XB];   //[Bsize]   
        Float_t         Btrk2Phi[MAX_XB];   //[Bsize]   

	t->SetBranchAddress("Bsize", &Bsize);
	t->SetBranchAddress("Balpha", Balpha);
	t->SetBranchAddress("Bchi2cl", Bchi2cl);
	t->SetBranchAddress("BsvpvDistance", BsvpvDistance);
	t->SetBranchAddress("BsvpvDisErr", BsvpvDisErr);
	//  t->SetBranchAddress("MVA", MVA);
	t->SetBranchAddress("Bd0", Bd0);
	t->SetBranchAddress("Bd0Err", Bd0Err);
	t->SetBranchAddress("Btrk1Pt", Btrk1Pt);
	t->SetBranchAddress("Btrk2Pt", Btrk2Pt);
	t->SetBranchAddress("Bdtheta", Bdtheta);
	t->SetBranchAddress("Btktkmass", Btktkmass);
	t->SetBranchAddress("Btrk1Eta", Btrk1Eta);
	t->SetBranchAddress("Btrk2Eta", Btrk2Eta);
	t->SetBranchAddress("Btrk1D0Err", Btrk1D0Err);
	t->SetBranchAddress("Btrk2D0Err", Btrk2D0Err);
	t->SetBranchAddress("Btrk1D0", Btrk1D0);
	t->SetBranchAddress("Btrk2D0", Btrk2D0);
	t->SetBranchAddress("Btrk1Dz", Btrk1Dz);
	t->SetBranchAddress("Btrk2Dz", Btrk2Dz);
	t->SetBranchAddress("Bmu1eta", Bmu1eta);
	t->SetBranchAddress("Bmu2eta", Bmu2eta);
	t->SetBranchAddress("Bmu1pt", Bmu1pt);
	t->SetBranchAddress("Bmu2pt", Bmu2pt);
        t->SetBranchAddress("Bmumumass", Bmumumass);
        t->SetBranchAddress("Btktkmass", Btktkmass);
	        t->SetBranchAddress("Bmass", Bmass);
        t->SetBranchAddress("Bmumuphi", Bmumuphi);
        t->SetBranchAddress("Bmumueta", Bmumueta);
        t->SetBranchAddress("Btrk1Phi", Btrk1Phi);
        t->SetBranchAddress("Btrk2Phi", Btrk2Phi);
/*
	Float_t         Dtrk1dedx[MAX_XB];   //[Bsize]   
	Float_t         Dtrk2dedx[MAX_XB];   //[Bsize]   
	Float_t         Dtrk1P[MAX_XB];   //[Bsize]   
	Float_t         Dtrk2P[MAX_XB];   //[Bsize]   

	Float_t         Dd0[MAX_XB];   //[Bsize]   
	Float_t         Dd0Err[MAX_XB];   //[Bsize]   
	Float_t         Dphi[MAX_XB];   //[Bsize]   
	Float_t         Dchi2ndf[MAX_XB];   //[Bsize]   
	Float_t         Dtrk1Y[MAX_XB];   //[Bsize]   
	Float_t         Dtrk2Y[MAX_XB];   //[Bsize]   
	Float_t         Dtrk1Phi[MAX_XB];   //[Bsize]   
	Float_t         Dtrk2Phi[MAX_XB];   //[Bsize]   

	Float_t        Dtrk1MassHypo[MAX_XB];
	Float_t        Dtrk2MassHypo[MAX_XB];



	Float_t         Dtrk1D0[MAX_XB];   //[Bsize]   
	Float_t         Dtrk2D0[MAX_XB];   //[Bsize]   
	Float_t         Dtrk1D0Err[MAX_XB];   //[Bsize]   
	Float_t         Dtrk2D0Err[MAX_XB];   //[Bsize]   


	Float_t         Dtrk1Dz[MAX_XB];   //[Bsize]   
	Float_t         Dtrk2Dz[MAX_XB];   //[Bsize]   
	Float_t         Dtrk1DzErr[MAX_XB];   //[Bsize]   
	Float_t         Dtrk2DzErr[MAX_XB];   //[Bsize]   
*/


/*
	t->SetBranchAddress("Dsize", &Dsize);
	t->SetBranchAddress("Dalpha", Dalpha);
	t->SetBranchAddress("Dchi2cl", Dchi2cl);
	t->SetBranchAddress("DsvpvDistance", DsvpvDistance);
	t->SetBranchAddress("DsvpvDisErr", DsvpvDisErr);
	//  t->SetBranchAddress("MVA", MVA);
	t->SetBranchAddress("Dtrk1thetastar", Dtrk1thetastar);
	t->SetBranchAddress("Dtrk1Pt", Dtrk1Pt);
	t->SetBranchAddress("Dtrk2Pt", Dtrk2Pt);

	t->SetBranchAddress("Dtrk1D0", Dtrk1D0);
	t->SetBranchAddress("Dtrk2D0", Dtrk2D0);
	t->SetBranchAddress("Dtrk1Eta", Dtrk1Eta);
	t->SetBranchAddress("Dtrk2Eta", Dtrk2Eta);
	t->SetBranchAddress("Dy", Dy);
	t->SetBranchAddress("Dmass", Dmass);
	t->SetBranchAddress("Dpt", Dpt);
	t->SetBranchAddress("Dtrk1D0Err", Dtrk1D0Err);
	t->SetBranchAddress("Dtrk2D0Err", Dtrk2D0Err);

	t->SetBranchAddress("Dtrk1Dz", Dtrk1Dz);
	t->SetBranchAddress("Dtrk2Dz", Dtrk2Dz);
	t->SetBranchAddress("Dtrk1DzErr", Dtrk1DzErr);
	t->SetBranchAddress("Dtrk2DzErr", Dtrk2DzErr);
*/

/*
	t->SetBranchAddress("Dtrk1dedx", Dtrk1dedx);
	t->SetBranchAddress("Dtrk2dedx", Dtrk2dedx);
	t->SetBranchAddress("Dtrk1P", Dtrk1P);
	t->SetBranchAddress("Dtrk2P", Dtrk2P);

	t->SetBranchAddress("Dd0", Dd0);
	t->SetBranchAddress("Dd0Err", Dd0Err);
	t->SetBranchAddress("Dphi", Dphi);
	t->SetBranchAddress("Dchi2ndf", Dchi2ndf);
	t->SetBranchAddress("Dtrk1Y", Dtrk1Y);
	t->SetBranchAddress("Dtrk2Y", Dtrk2Y);
	t->SetBranchAddress("Dtrk1Phi", Dtrk1Phi);
	t->SetBranchAddress("Dtrk2Phi", Dtrk2Phi);
*/









	std::vector<std::string> theInputVars;
	//  std::string a1 = "Dalpha";
/*
	std::string a1 = "Dchi2cl";

	//  std::string a4 = "MVA";
	std::string a2 = "DsvpvDistance*TMath::Sin(Dalpha)";
	std::string a3 = "DsvpvDistance/DsvpvDisErr";

	//	std::string a3 = "DsvpvDistance*TMath::Sin(Dalpha)";
	std::string a4 = "TMath::Cos(Dtrk1thetastar)";
	std::string a5 = "Dtrk1Pt";
	std::string a6 = "Dtrk2Pt";
	//std::string a7 = "Dtrk1D0";
	//std::string a8 = "Dtrk2D0";
	//std::string a7 = "DsvpvDistance/(TMath::Sqrt((Dpt*TMath::CosH(Dy)*Dpt*TMath::CosH(Dy)+Dmass*Dmass*TMath::SinH(Dy)*TMath::SinH(Dy)))/Dmass)";
	std::string a7 = "abs(Dtrk1D0)/abs(Dtrk1D0Err)";
	std::string a8 = "abs(Dtrk2D0)/abs(Dtrk2D0Err)";
    std::string a9 = "abs(Dtrk1Dz)/abs(Dtrk1DzErr)";
    std::string a10 = "abs(Dtrk2D0)/abs(Dtrk2D0Err)";
*/
	//std::string a1 = "abs(Btrk1Eta)";
	//std::string a2 = "abs(Btrk2Eta)";
	std::string a1 = "Btrk1Pt";
	std::string a2 = "Btrk2Pt";
	std::string a3 = "Btrk1Eta";
       std::string a4 = "Btrk2Eta";	
        std::string a5 = "Btrk1Dxy/Btrk1D0Err";
        std::string a6 = "Btrk2Dxy/Btrk2D0Err";
        std::string a7 = "abs(Btktkmass-1.019455)";
       std::string a8 = "BsvpvDistance/BsvpvDisErr";
    std::string a9 = "Balpha";
    std::string a10 = "Bd0/Bd0Err";
       std::string a11 = "cos(Bdtheta)";
       std::string a12 = "Bchi2cl";


/*
   std::string a12 = "abs(Bmu1eta)";
    std::string a13 = "abs(Bmu2eta)";
    std::string a14 = "Bmu2pt";
    std::string a15 = "Bmu1pt";
*/

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

/*
	theInputVars.push_back(a12);
	theInputVars.push_back(a13);
	theInputVars.push_back(a14);
	theInputVars.push_back(a15);
*/
	/*
	   std::string a6="(Dtrk1Pt*TMath::Cos(Dtrk1Phi)*Dtrk2Pt*TMath::Cos(Dtrk2Phi)+Dtrk1Pt*TMath::Sin(Dtrk1Phi)*Dtrk2Pt*TMath::Sin(Dtrk2Phi)+(Dtrk1P**2-Dtrk1Pt**2)**0.5*(Dtrk2P**2-Dtrk2Pt**2)**0.5)/(Dtrk1P*Dtrk2P)";
	//std::string a7="abs((Dtrk1P/(Dtrk1P**2+Dtrk1MassHypo**2)**0.5)**2*(log((Dtrk1P/(Dtrk1P**2+Dtrk1MassHypo**2)**0.5)**2/(1-(Dtrk1P/(Dtrk1P**2+Dtrk1MassHypo**2)**0.5)**2))-((Dtrk1P/(Dtrk1P**2+Dtrk1MassHypo**2)**0.5)**2)))/(-35.0/12.0*Dtrk1dedx+10.25833)";
	//	std::string a8="abs((Dtrk2P/(Dtrk2P**2+Dtrk2MassHypo**2)**0.5)**2*(log((Dtrk2P/(Dtrk2P**2+Dtrk1MassHypo**2)**0.5)**2/(1-(Dtrk2P/(Dtrk2P**2+Dtrk1MassHypo**2)**0.5)**2))-((Dtrk2P/(Dtrk2P**2+SetBranchAddressDtrk2MassHypo**2)**0.5)**2)))/(-35.0/12.0*Dtrk2dedx+10.25833)";
	std::string a7="abs(((Dtrk1P/(Dtrk1P**2+Dtrk1MassHypo**2)**0.5)**2*(log((Dtrk1P/(Dtrk1P**2+Dtrk1MassHypo**2)**0.5)**2/(1-(Dtrk1P/(Dtrk1P**2+Dtrk1MassHypo**2)**0.5)**2))-((Dtrk1P/(Dtrk1P**2+Dtrk1MassHypo**2)**0.5)**2)))/(-35.0/12.0*Dtrk1dedx+10.25833)-1)";
	std::string a8="abs(((Dtrk2P/(Dtrk2P**2+Dtrk2MassHypo**2)**0.5)**2*(log((Dtrk2P/(Dtrk2P**2+Dtrk1MassHypo**2)**0.5)**2/(1-(Dtrk2P/(Dtrk2P**2+Dtrk1MassHypo**2)**0.5)**2))-((Dtrk2P/(Dtrk2P**2+Dtrk2MassHypo**2)**0.5)**2)))/(-35.0/12.0*Dtrk2dedx+10.25833)-1)";
	*/
	/*
	   std::string a17 = "Dd0";
	   std::string a18 = "Dd0Err";
	   std::string a19 = "Dphi";
	   std::string a20 = "Dchi2ndf";


	   std::string a7 = "Dtrk1Y";
	   std::string a8 = "Dtrk2Y";


	   std::string a21 = "Dtrk1Phi";
	   std::string a22 = "Dtrk2Phi";


	   std::string a9 = "Dtrk1Eta";
	   std::string a10 = "Dtrk2Eta";
	   std::string a9 = "Dtrk1D0";
	   std::string a10 = "Dtrk2D0";

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
	   theInputVars.push_back(a14);



	   theInputVars.push_back(a15);
	   theInputVars.push_back(a16);

	   theInputVars.push_back(a17);

	   theInputVars.push_back(a18);

	   theInputVars.push_back(a19);
	   theInputVars.push_back(a20);
	   theInputVars.push_back(a21);
	   theInputVars.push_back(a22);
	   */
	std::vector<double> inputValues;
	ReadBDTG mva(theInputVars);

	TFile* outf = new TFile(ofname,"recreate");
	TTree* mvaTree = new TTree(Form("%s",treename.Data()),"BDTG");

	double BDTG[MAX_XB];
//	mvaTree->Branch("Dsize",&Dsize,"Dsize/I");
	mvaTree->Branch("Bsize",&Bsize,"Bsize/I");


	mvaTree->Branch(Form("%s",treename.Data()),BDTG,Form("%s[Bsize]/D",treename.Data()));
	std::cout<<std::endl;
	std::cout<<"  Input file: "<<infname<<std::endl;
	std::cout<<"  Calculating MVA values ..."<<std::endl;
	for(int i=0;i<t->GetEntries();i++)
	{
		if(i%10000==0) std::cout<<std::setiosflags(std::ios::left)<<"  [ \033[1;36m"<<std::setw(10)<<i<<"\033[0m"<<" / "<<std::setw(10)<<t->GetEntries()<<" ] "<<"\033[1;36m"<<Form("%.0f",100.*i/t->GetEntries())<<"%\033[0m"<<"\r"<<std::flush;
		t->GetEntry(i);
		for(int j=0;j<Bsize;j++)
		{
			inputValues.clear();
			//    inputValues.push_back(Dalpha[j]);
			/*
			inputValues.push_back(Dchi2cl[j]);
			inputValues.push_back(DsvpvDistance[j]*TMath::Sin(Dalpha[j]));
			inputValues.push_back(DsvpvDistance[j]/DsvpvDisErr[j]);
			//		inputValues.push_back(Dchi2cl[j]);
			//		  inputValues.push_back(MVA[j]);
			//		inputValues.push_back(DsvpvDistance[j]*TMath::Sin(Dalpha[j]));
			inputValues.push_back(TMath::Cos(Dtrk1thetastar[j]));
			inputValues.push_back(Dtrk1Pt[j]);
			inputValues.push_back(Dtrk2Pt[j]);
			inputValues.push_back(abs(Dtrk1D0[j])/abs(Dtrk1D0Err[j]));
			inputValues.push_back(abs(Dtrk2D0[j])/abs(Dtrk2D0Err[j]));
			inputValues.push_back(abs(Dtrk1Dz[j])/abs(Dtrk1DzErr[j]));
			inputValues.push_back(abs(Dtrk2Dz[j])/abs(Dtrk2DzErr[j]));
			*/
//			inputValues.push_back(abs(Btrk1Eta[j]));
//			inputValues.push_back(abs(Btrk2Eta[j]));
	
			inputValues.push_back(Btrk1Pt[j]);
			inputValues.push_back(Btrk2Pt[j]);
			          inputValues.push_back(Btrk1Eta[j]);
                        inputValues.push_back(Btrk2Eta[j]);
			   inputValues.push_back(Btrk1Dxy[j]/Btrk1D0Err[j]);
		 inputValues.push_back(Btrk2Dxy[j]/Btrk2D0Err[j]);
			inputValues.push_back(abs(Btktkmass[j]-1.019455));
			inputValues.push_back(BsvpvDistance[j]/BsvpvDisErr[j]);
			inputValues.push_back(Balpha[j]);
			inputValues.push_back(Bd0[j]/Bd0Err[j]);
			inputValues.push_back(cos(Bdtheta[j]));
			inputValues.push_back(Bchi2cl[j]);

			BDTG[j] = mva.GetMvaValue(inputValues);      
		cout << "BDTG = " << BDTG[j] << endl;
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
		BDTG(argv[1],argv[2],atof(argv[3]),atof(argv[4]),argv[5]);
		return 0;
	}
	else
	{
		std::cout<<"  Error: invalid argument number - BDT()"<<std::endl;
		return 1;
	}
}
