// @(#)root/tmva $Id$
/**********************************************************************************
 * Project   : TMVA - a ROOT-integrated toolkit for multivariate data analysis    *
 * Package   : TMVA                                                               *
 * Root Macro: TMVAClassification                                                 *
 *                                                                                *
 * This macro provides examples for the training and testing of the               *
 * TMVA classifiers.                                                              *
 *                                                                                *
 * As input data is used a toy-MC sample consisting of four Gaussian-distributed  *
 * and linearly correlated input variables.                                       *
 *                                                                                *
 * The methods to be used can be switched on and off by means of booleans, or     *
 * via the prompt command, for example:                                           *
 *                                                                                *
 *    root -l ./TMVAClassification.C\(\"Fisher,Likelihood\"\)                     *
 *                                                                                *
 * (note that the backslashes are mandatory)                                      *
 * If no method given, a default set of classifiers is used.                      *
 *                                                                                *
 * The output file "TMVA.root" can be analysed with the use of dedicated          *
 * macros (simply say: root -l <macro.C>), which can be conveniently              *
 * invoked through a GUI that will appear at the end of the run of this macro.    *
 * Launch the GUI via the command:                                                *
 *                                                                                *
 *    root -l ./TMVAGui.C                                                         *
 *                                                                                *
 **********************************************************************************/
#include "../includes/TMVAClassification.h"
//#include "Config.cxx"
#include "Config.h"

void TMVAClassification(TString inputSname, TString inputBname,
		TString collisionsyst,
		TString mycuts, TString mycutb,
		Float_t ptmin, Float_t ptmax,
		TString myMethodList="")
{
	Bool_t isPbPb = collisionsyst=="PbPb"?true:false;

	// The explicit loading of the shared libTMVA is done in TMVAlogon.C, defined in .rootrc
	// if you use your private .rootrc, or run from a different directory, please copy the
	// corresponding lines from .rootrc

	// methods to be processed can be given as an argument; use format:
	//
	// mylinux~> root -l TMVAClassification.C\(\"myMethod1,myMethod2,myMethod3\"\)
	//
	// if you like to use a method via the plugin mechanism, we recommend using
	//
	// mylinux~> root -l TMVAClassification.C\(\"P_myMethod\"\)
	// (an example is given for using the BDT as plugin (see below),
	// but of course the real application is when you write your own
	// method based)

	//---------------------------------------------------------------
	// This loads the library
	TMVA::Tools::Instance();

	// to get access to the GUI and all tmva macros
	TString tmva_dir(TString(gRootDir) + "/tmva");
	if(gSystem->Getenv("TMVASYS"))
		tmva_dir = TString(gSystem->Getenv("TMVASYS"));
	gROOT->SetMacroPath(tmva_dir + "/test/:" + gROOT->GetMacroPath() );
	//gROOT->ProcessLine(".L TMVAGui.C");


	// Default MVA methods to be trained + tested
	std::map<std::string,int> Use;

	// --- Cut optimisation
	Use["Cuts"]            = 0;
	Use["CutsD"]           = 0;
	Use["CutsPCA"]         = 0;
	Use["CutsGA"]          = 0;
	Use["CutsSA"]          = 0;
	// 
	// --- 1-dimensional likelihood ("naive Bayes estimator")
	Use["Likelihood"]      = 0;
	Use["LikelihoodD"]     = 0; // the "D" extension indicates decorrelated input variables (see option strings)
	Use["LikelihoodPCA"]   = 0; // the "PCA" extension indicates PCA-transformed input variables (see option strings)
	Use["LikelihoodKDE"]   = 0;
	Use["LikelihoodMIX"]   = 0;
	//
	// --- Mutidimensional likelihood and Nearest-Neighbour methods
	Use["PDERS"]           = 0;
	Use["PDERSD"]          = 0;
	Use["PDERSPCA"]        = 0;
	Use["PDEFoam"]         = 0;
	Use["PDEFoamBoost"]    = 0; // uses generalised MVA method boosting
	Use["KNN"]             = 0; // k-nearest neighbour method
	//
	// --- Linear Discriminant Analysis
	Use["LD"]              = 0; // Linear Discriminant identical to Fisher
	Use["Fisher"]          = 0;
	Use["FisherG"]         = 0;
	Use["BoostedFisher"]   = 0; // uses generalised MVA method boosting
	Use["HMatrix"]         = 0;
	//
	// --- Function Discriminant analysis
	Use["FDA_GA"]          = 0; // minimisation of user-defined function using Genetics Algorithm
	Use["FDA_SA"]          = 0;
	Use["FDA_MC"]          = 0;
	Use["FDA_MT"]          = 0;
	Use["FDA_GAMT"]        = 0;
	Use["FDA_MCMT"]        = 0;
	//
	// --- Neural Networks (all are feed-forward Multilayer Perceptrons)
	Use["MLP"]             = 0; // Recommended ANN
	Use["MLPBFGS"]         = 0; // Recommended ANN with optional training method
	Use["MLPBNN"]          = 0; // Recommended ANN with BFGS training method and bayesian regulator
	Use["CFMlpANN"]        = 0; // Depreciated ANN from ALEPH
	Use["TMlpANN"]         = 0; // ROOT's own ANN
	// --- Support Vector Machine 
	Use["SVM"]             = 0;
	// 
	// --- Boosted Decision Trees
	Use["BDT"]             = 0;  // uses Adaptive Boost
	Use["BDTG"]            = 1; // uses Gradient Boost
	Use["BDTB"]            = 0; // uses Bagging
	Use["BDTD"]            = 0; // decorrelation + Adaptive Boost
	Use["BDTF"]            = 0; // allow usage of fisher discriminant for node splitting 
	// 
	// --- Friedman's RuleFit method, ie, an optimised series of cuts ("rules")
	Use["RuleFit"]         = 0;
	// ---------------------------------------------------------------

	std::cout << std::endl;
	std::cout << "==> Start TMVAClassification" << std::endl;

	// Select methods (don't look at this code - not of interest)
	if(myMethodList!= "")
	{
		for(std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) it->second = 0;
		std::vector<TString> mlist = TMVA::gTools().SplitString( myMethodList, ',' );
		for(unsigned i=0; i<mlist.size(); i++)
		{
			std::string regMethod(mlist[i]);
			if (Use.find(regMethod)==Use.end())
			{
				std::cout << "Method \"" << regMethod << "\" not known in TMVA under this name. Choose among the following:" << std::endl;
				for(std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) std::cout << it->first << " ";
				std::cout << std::endl;
				return;
			}
			Use[regMethod] = 1;
		}
	}

	// --------------------------------------------------------------------------------------------------

	// --- Here the preparation phase begins

	// Create a ROOT output file where TMVA will store ntuples, histograms, etc.
	TString outfileName("ROOT/TMVA.root");
	TFile* outputFile = TFile::Open(outfileName,"RECREATE");

	// Create the factory object. Later you can choose the methods
	// whose performance you'd like to investigate. The factory is 
	// the only TMVA object you have to interact with
	//
	// The first argument is the base of the name of all the
	// weightfiles in the directory weight/
	//
	// The second argument is the output file for the training results
	// All TMVA output can be suppressed by removing the "!" (not) in
	// front of the "Silent" argument in the option string
	TMVA::Factory *factory = new TMVA::Factory( "TMVAClassification", outputFile,
			"!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification" );
	(TMVA::gConfig().GetVariablePlotting()).fNbinsXOfROCCurve = 100;
	// If you wish to modify default settings
	// (please check "src/Config.h" to see all available global options)
	//    (TMVA::gConfig().GetVariablePlotting()).fTimesRMS = 8.0;
	//    (TMVA::gConfig().GetIONames()).fWeightFileDir = "myWeightDirectory";

	// Define the input variables that shall be used for the MVA training
	// note that you may also use variable expressions, such as: "3*var1/var2*abs(var3)"
	// [all types of expressions that can also be parsed by TTree::Draw( "expression" )]

	//factory->AddVariable("Dalpha");//>
	//factory->AddVariable("DsvpvDistance/DsvpvDisErr");
	/*
	factory->AddVariable("Dchi2cl");//>
	factory->AddVariable("DsvpvDistance*TMath::Sin(Dalpha)");

	factory->AddVariable("DsvpvDistance/DsvpvDisErr");//>
	factory->AddVariable("TMath::Cos(Dtrk1thetastar)");          
	factory->AddVariable("Dtrk1Pt");
	factory->AddVariable("Dtrk2Pt");
	*/
	/*
	   factory->AddVariable("Dtrk1Pt");
	   factory->AddVariable("Dtrk2Pt");
	   factory->AddVariable("Dtrk1P");
	   factory->AddVariable("Dtrk2P");
	   */
	//	  factory->AddVariable("(Dtrk1Pt*TMath::Cos(Dtrk1Phi)*Dtrk2Pt*TMath::Cos(Dtrk2Phi)+Dtrk1Pt*TMath::Sin(Dtrk1Phi)*Dtrk2Pt*TMath::Sin(Dtrk2Phi)+(Dtrk1P**2-Dtrk1Pt**2)**0.5*(Dtrk2P**2-Dtrk2Pt**2)**0.5)/(Dtrk1P*Dtrk2P)");
	//factory->AddVariable("abs((Dtrk1Pt + Dtrk2Pt)/Dpt-2)/abs((Dtrk1Pt + Dtrk2Pt)/Dpt-3)");

	//		factory->AddVariable("abs(((Dtrk1P/(Dtrk1P**2+Dtrk1MassHypo**2)**0.5)**2*(log((Dtrk1P/(Dtrk1P**2+Dtrk1MassHypo**2)**0.5)**2/(1-(Dtrk1P/(Dtrk1P**2+Dtrk1MassHypo**2)**0.5)**2))-((Dtrk1P/(Dtrk1P**2+Dtrk1MassHypo**2)**0.5)**2)))/(-35.0/12.0*Dtrk1dedx+10.25833)-1)");
	//	      factory->AddVariable("abs(((Dtrk2P/(Dtrk2P**2+Dtrk2MassHypo**2)**0.5)**2*(log((Dtrk2P/(Dtrk2P**2+Dtrk1MassHypo**2)**0.5)**2/(1-(Dtrk2P/(Dtrk2P**2+Dtrk1MassHypo**2)**0.5)**2))-((Dtrk2P/(Dtrk2P**2+Dtrk2MassHypo**2)**0.5)**2)))/(-35.0/12.0*Dtrk2dedx+10.25833)-1)");
	/*	
		factory->AddVariable("Dtrk1Y");
		factory->AddVariable("Dtrk2Y");
		factory->AddVariable("Dtrk1Eta");
		factory->AddVariable("Dtrk2Eta");
		*/
	
	/*
	factory->AddVariable("abs(Dtrk1D0)/abs(Dtrk1D0Err)");
	factory->AddVariable("abs(Dtrk2D0)/abs(Dtrk2D0Err)");
	
	     factory->AddVariable("abs(Dtrk1Dz)/abs(Dtrk1DzErr)");
        factory->AddVariable("abs(Dtrk2Dz)/abs(Dtrk2DzErr)");
	*/
	//factory->AddVariable("DsvpvDistance/(TMath::Sqrt((Dpt*TMath::CosH(Dy)*Dpt*TMath::CosH(Dy)+Dmass*Dmass*TMath::SinH(Dy)*TMath::SinH(Dy)))/Dmass)");
	//  factory->AddVariable("(Dtrk1dedx)/(DsvpvDistance/DsvpvDisErr)");
	// factory->AddVariable("(Dtrk2dedx)/(DsvpvDistance/DsvpvDisErr)");
	//   factory->AddVariable("(Dtrk1P)/(DsvpvDistance/DsvpvDisErr)");
	//   factory->AddVariable("(Dtrk2P)/(DsvpvDistance/DsvpvDisErr)");



	//   factory->AddVariable("TMath::Cos(Dtrk1thetastar)");
	//  factory->AddVariable("Dtrk1Pt");
	//   factory->AddVariable("Dtrk2Pt");
	/* 
	   factory->AddVariable("Dtrk1Y");
	   factory->AddVariable("Dtrk2Y");
	   factory->AddVariable("Dtrk1Eta");
	   factory->AddVariable("Dtrk2Eta");

	   factory->AddVariable("Dtrk1D0");
	   factory->AddVariable("Dtrk2D0");


	   factory->AddVariable("Dtrk1dedx");
	   factory->AddVariable("Dtrk2dedx");
	   factory->AddVariable("Dtrk1P");
	   factory->AddVariable("Dtrk2P");

	   factory->AddVariable("Dd0");
	   factory->AddVariable("Dd0Err");
	   factory->AddVariable("Dphi");
	   factory->AddVariable("Dchi2ndf");
	   factory->AddVariable("Dtrk1Phi");
	   factory->AddVariable("Dtrk2Phi");
	   */ 
	/*	
		factory->AddVariable("Dtrk1dedx");
		factory->AddVariable("Dtrk2dedx");
		factory->AddVariable("Dtrk1P");
		factory->AddVariable("Dtrk2P");
		*/	
	//factory->AddVariable("Dalpha");
	//factory->AddVariable("(Dtrk2Pt-Dtrk1Pt)/(Dtrk2Pt+Dtrk1Pt)");
	//factory->AddVariable("TMath::Cos(Dtrk1thetastar)");

	//factory->AddVariable("Dtrk1Phi-Dtrk2Phi");
	//		factory->AddVariable("Dtrk1Eta-Dtrk2Eta");

	//factory->AddVariable("TMath::Cos(Dtrk1thetastar)");
	//factory->AddVariable("Dtrk1Pt");
	//factory->AddVariable("Dtrk2Pt");
	/*
	   factory->AddVariable("Dtrk1dedx");
	   factory->AddVariable("Dtrk2dedx");
	   factory->AddVariable("Dtrk1P");
	   factory->AddVariable("Dtrk2P");
	   */
	//	factory->AddVariable("Dmass");

	//factory->AddVariable("MLP_pt_0_1");
	//factory->AddVariable("MLP_PID_pt_0_1");
	//factory->AddVariable("MLP_Track_pt_0_1");

	/*
	   factory->AddVariable("Dtrk1Eta");
	   factory->AddVariable("Dtrk2Eta");
	   factory->AddVariable("Dtrk1P");
	   factory->AddVariable("Dtrk2P");
	   */
	//	factory->AddVariable("MVA");


	/*
	   factory->AddVariable("Dy");
	   */
	/*
	   factory->AddVariable("Dchi2cl");//>
	   factory->AddVariable("DsvpvDistance/DsvpvDisErr");//>
	   factory->AddVariable("Dalpha");
	//factory->AddVariable("DsvpvDistance*TMath::Sin(Dalpha)");

	factory->AddVariable("Dtrk1Pt");
	factory->AddVariable("Dtrk2Pt");
	*/
	// You can add so-called "Spectator variables", which are not used in the MVA training,
	// but will appear in the final "TestTree" produced by TMVA. This TestTree will contain the
	// input variables, the response values of all trained MVAs, and the spectator variables
	//factory->AddSpectator( "spec1 := var1*2",  "Spectator 1", "units", 'F' );
	//factory->AddSpectator( "spec2 := var1*3",  "Spectator 2", "units", 'F' );

	// Read training and test data
	// (it is also possible to use ASCII format as input -> see TMVA Users Guide)
	//TString fname = "./tmva_class_example.root";

	//if (gSystem->AccessPathName( fname ))  // file does not exist in local directory
	//   gSystem->Exec("wget http://root.cern.ch/files/tmva_class_example.root");
	

	//factory->AddVariable("abs(Btrk1Eta)");
	//factory->AddVariable("abs(Btrk2Eta)");

	factory->AddVariable("Btrk1Pt");
	factory->AddVariable("Btrk2Pt");
        factory->AddVariable("Btrk1Eta");
        factory->AddVariable("Btrk2Eta");
        factory->AddVariable("Btrk1Dxy/Btrk1D0Err");
        factory->AddVariable("Btrk2Dxy/Btrk2D0Err");
	factory->AddVariable("abs(Btktkmass-1.019455)");
	factory->AddVariable("BsvpvDistance/BsvpvDisErr");//>
	factory->AddVariable("Balpha");
	factory->AddVariable("Bd0/Bd0Err"); 
	factory->AddVariable("cos(Bdtheta)");   
        factory->AddVariable("Bchi2cl")
//	factory->AddVariable("(Btrk1Phi-Bmumuphi)*(Btrk1Phi-Bmumuphi)+(Btrk1Eta-Bmumueta)*(Btrk1Eta-Bmumueta)");
//	factory->AddVariable("(Btrk2Phi-Bmumuphi)*(Btrk2Phi-Bmumuphi)+(Btrk2Eta-Bmumueta)*(Btrk2Eta-Bmumueta)");
//	factory->AddVariable("Bmass - Btktkmass - Bmumumass");
//	factory->AddVariable("TMath::Cos(Bdtheta)");          

//	factory->AddVariable("abs(Btrk1D0)/abs(Btrk1D0Err)");
//	factory->AddVariable("abs(Btrk2D0)/abs(Btrk2D0Err)");
//	factory->AddVariable("abs(Btrk1Dz)");
//	factory->AddVariable("abs(Btrk2Dz)");
 //       factory->AddVariable("Bchi2cl");

	TFile *inputS = new TFile(inputSname.Data());
	TFile *inputB = new TFile(inputBname.Data());

	std::cout << "--- TMVAClassification       : Using input file: " << inputS->GetName() << " & "<< inputB->GetName() <<std::endl;

	// TTree* signal = (TTree*)inputB->Get("ntphi");
	// TTree* background = (TTree*)inputB->Get("ntphi");
	 TTree* background = (TTree*)inputB->Get("Bfinder/ntphi");
	 TTree* signal = (TTree*)inputS->Get("Bfinder/ntphi");
	
//	TTree* background = (TTree*)inputB->Get("Bfinder/ntphi");
	/*
	   background->AddFriend("ntHlt");
	   background->AddFriend("ntHi");
	   background->AddFriend("ntSkim");
	   background->AddFriend("ntGen");
	   */

	//	background->AddFriend("Dchi2cl");
	//	background->AddFriend("MLP_Track_pt_0_1");

	//	background->AddFriend("MLP_PID_pt_0_1");
	//	background->AddFriend("MLP_pt_0_1");
	//	background->AddFriend("MLP_Track_pt_0_1");

//	TTree* signal = (TTree*)inputS->Get("Bfinder/ntphi");
	/*
	   signal->AddFriend("ntHlt");
	   signal->AddFriend("ntHi");
	   signal->AddFriend("ntSkim");
	   */
	signal->AddFriend("Bfinder/ntGen");
	//signal->AddFriend("BDTG_pt_0_1");
	signal->AddFriend("hiEvtAnalyzer/HiTree");

	//background->AddFriend("DsvpvDistance/DsvpvDisErr");
	//	background->AddFriend("Dchi2cl");


	//	signal->AddFriend("MLP_PID_pt_0_1");
	//	signal->AddFriend("MLP_pt_0_1");
	//	signal->AddFriend("MLP_Track_pt_0_1");


	// --- Register the training and test trees

	//global event weights per tree (see below for setting event-wise weights)
	double signalWeight     = 1.0;
	double backgroundWeight = 1.0;

	// You can add an arbitrary number of signal or background trees
	factory->AddSignalTree    ( signal,     signalWeight     );
	factory->AddBackgroundTree( background, backgroundWeight );

	//factory->SetSignalWeightExpression    ("pthatweight");

	// To give different trees for training and testing, do as follows:
	//    factory->AddSignalTree( signalTrainingTree, signalTrainWeight, "Training" );
	//    factory->AddSignalTree( signalTestTree,     signalTestWeight,  "Test" );

	// Use the following code instead of the above two or four lines to add signal and background
	// training and test events "by hand"
	// NOTE that in this case one should not give expressions (such as "var1+var2") in the input
	//      variable definition, but simply compute the expression before adding the event
	//
	//     // --- begin ----------------------------------------------------------
	//     std::vector<double> vars( 4 ); // vector has size of number of input variables
	//     float  treevars[4], weight;
	//     
	//     // Signal
	//     for (Uint ivar=0; ivar<4; ivar++) signal->SetBranchAddress( Form( "var%i", ivar+1 ), &(treevars[ivar]) );
	//     for (Uint i=0; i<signal->GetEntries(); i++) {
	//        signal->GetEntry(i);
	//        for (Uint ivar=0; ivar<4; ivar++) vars[ivar] = treevars[ivar];
	//        // add training and test events; here: first half is training, second is testing
	//        // note that the weight can also be event-wise
	//        if (i < signal->GetEntries()/2.0) factory->AddSignalTrainingEvent( vars, signalWeight );
	//        else                              factory->AddSignalTestEvent    ( vars, signalWeight );
	//     }
	//   
	//     // Background (has event weights)
	//     background->SetBranchAddress( "weight", &weight );
	//     for (Uint ivar=0; ivar<4; ivar++) background->SetBranchAddress( Form( "var%i", ivar+1 ), &(treevars[ivar]) );
	//     for (Uint i=0; i<background->GetEntries(); i++) {/export/d00/scratch/zzshi/CMSSW_7_5_8_patch3/Merge/ppHigh/MC/pp/NonPrompt/NonPromptpp.root
	//        background->GetEntry(i);
	//        for (Uint ivar=0; ivar<4; ivar++) vars[ivar] = treevars[ivar];
	//        // add training and test events; here: first half is training, second is testing
	//        // note that the weight can also be event-wise
	//        if (i < background->GetEntries()/2) factory->AddBackgroundTrainingEvent( vars, backgroundWeight*weight );
	//        else                                factory->AddBackgroundTestEvent    ( vars, backgroundWeight*weight );
	//     }
	// --- end ------------------------------------------------------------
	//
	// --- end of tree registration 

	// Set individual event weights (the variables must exist in the original TTree)
	//    for signal    : factory->SetSignalWeightExpression    ("weight1*weight2");
	//    for background: factory->SetBackgroundWeightExpression("weight1*weight2");
	//factory->SetBackgroundWeightExpression( "weight" );

	// Apply additional cuts on the signal and background samples (can be different)

	// Tell the factory how to use the training and testing events
	//
	// If no numbers of events are given, half of the events in the tree are used 
	// for training, and the other half for testing:
	//    factory->PrepareTrainingAndTestTree( mycut, "SplitMode=random:!V" );
	// To also specify the number of testing events, use:
	//    factory->PrepareTrainingAndTestTree( mycut,
	//                                         "NSigTrain=3000:NBkgTrain=3000:NSigTest=3000:NBkgTest=3000:SplitMode=Random:!V" );

	TString cuts = isPbPb?Form("(%s)&&Bpt>%f&&Bpt<%f&&hiBin>=0&&hiBin<=200",mycuts.Data(),ptmin,ptmax):Form("(%s)&&Bpt>%f&&Bpt<%f",mycuts.Data(),ptmin,ptmax); 
	TString cutb = isPbPb?Form("(%s)&&Bpt>%f&&Bpt<%f&&hiBin>=0&&hiBin<=200",mycutb.Data(),ptmin,ptmax):Form("(%s)&&Bpt>%f&&Bpt<%f",mycutb.Data(),ptmin,ptmax);

	TCut mycutS = (TCut)cuts;
	TCut mycutB = (TCut)cutb;
	factory->PrepareTrainingAndTestTree( mycutS, mycutB, "nTrain_Signal=0:nTrain_Background=0:SplitMode=Random:NormMode=NumEvents:!V" );

	// ---- Book MVA methods
	//
	// Please lookup the various method configuration options in the corresponding cxx files, eg:
	// src/MethoCuts.cxx, etc, or here: http://tmva.sourceforge.net/optionRef.html
	// it is possible to preset ranges in the option string in which the cut optimisation should be done:
	// "...:CutRangeMin[2]=-1:CutRangeMax[2]=1"...", where [2] is the third input variable

	// Cut optimisation
	if (Use["Cuts"])
		factory->BookMethod( TMVA::Types::kCuts, "Cuts",
				"!H:!V:FitMethod=MC:EffSel:VarProp[0]=FMax:VarProp[1]=FMax:VarProp[2]=FMax:VarProp[3]=FMax" );
	//                           "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart" );

	if (Use["CutsD"])
		factory->BookMethod( TMVA::Types::kCuts, "CutsD",
				"!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart:VarTransform=Decorrelate" );

	if (Use["CutsPCA"])
		factory->BookMethod( TMVA::Types::kCuts, "CutsPCA",
				"!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart:VarTransform=PCA:VarProp[0]=FMax:VarProp[1]=FMax:VarProp[2]=FMax:VarProp[3]=FMax" );

	if (Use["CutsGA"])
		factory->BookMethod( TMVA::Types::kCuts, "CutsGA",
				"H:!V:FitMethod=GA:EffSel:Steps=5:Cycles=3:PopSize=400:SC_steps=10:SC_rate=5:SC_factor=0.95:VarProp[0]=FMax:VarProp[1]=FMax:VarProp[2]=FMax:VarProp[3]=FMax:VarProp[4]=FMin");
	//"H:!V:FitMethod=GA:EffSel:Steps=30:Cycles=3:PopSize=400:SC_steps=10:SC_rate=5:SC_factor=0.95:VarProp[0]=FMax:VarProp[1]=FMax:VarProp[2]=FMax");

	if (Use["CutsSA"])
		factory->BookMethod( TMVA::Types::kCuts, "CutsSA",
				//				                         "!H:!V:FitMethod=SA:EffSel:MaxCalls=150000:KernelTemp=IncAdaptive:InitialTemp=1e+6:MinTemp=1e-6:Eps=1e-10:UseDefaultScale:VarProp[0]=FMax:VarProp[1]=FMax:VarProp[2]=FMin" );
				//	   "!H:!V:FitMethod=SA:EffSel:MaxCalls=150000:KernelTemp=IncAdaptive:InitialTemp=1e+6:MinTemp=1e-6:Eps=1e-10:UseDefaultScale:VarProp[0]=FMax:VarProp[1]=FMax");

			"!H:!V:FitMethod=SA:EffSel:MaxCalls=200000:KernelTemp=IncAdaptive:InitialTemp=1e+6:MinTemp=1e-6:Eps=1e-10:UseDefaultScale:VarProp[0]=FMax:VarProp[1]=FMin:VarProp[2]=FMax:VarProp[3]=FMax:VarProp[4]=FMax::VarProp[5]=FMin::VarProp[6]=FMin:VarProp[7]=FMin:CutRangeMin[0]=0.05:CutRangeMax[0]=1.00:CutRangeMin[1]=0.0:CutRangeMax[1]=0.04:CutRangeMin[2]=0.0:CutRangeMax[2]=10:CutRangeMin[5]=-1:CutRangeMax[5]=1:CutRangeMin[6]=0:CutRangeMax[6]=10:CutRangeMin[7]=0:CutRangeMax[7]=10" );
	//"!H:!V:FitMethod=SA:EffSel:MaxCalls=170000:KernelTemp=IncAdaptive:InitialTemp=1e+6:MinTemp=1e-6:Eps=1e-10:UseDefaultScale:VarProp[0]=FMax:VarProp[1]=FMax" );

	//	"!H:!V:FitMethod=SA:EffSel:MaxCalls=170000:KernelTemp=IncAdaptive:InitialTemp=1e+6:MinTemp=1e-6:Eps=1e-10:VarProp=FSmart:CutRangeMin[0]=0.05:CutRangeMax[]=1.00:CutRangeMin[]=0.00" );
	// Likelihood ("naive Bayes estimator")
	if (Use["Likelihood"])
		factory->BookMethod( TMVA::Types::kLikelihood, "Likelihood",
				"H:!V:TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmoothBkg[1]=10:NSmooth=1:NAvEvtPerBin=50" );

	// Decorrelated likelihood
	if (Use["LikelihoodD"])
		factory->BookMethod( TMVA::Types::kLikelihood, "LikelihoodD",
				"!H:!V:TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmooth=5:NAvEvtPerBin=50:VarTransform=Decorrelate" );

	// PCA-transformed likelihood
	if (Use["LikelihoodPCA"])
		factory->BookMethod( TMVA::Types::kLikelihood, "LikelihoodPCA",
				"!H:!V:!TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmooth=5:NAvEvtPerBin=50:VarTransform=PCA" ); 

	// Use a kernel density estimator to approximate the PDFs
	if (Use["LikelihoodKDE"])
		factory->BookMethod( TMVA::Types::kLikelihood, "LikelihoodKDE",
				"!H:!V:!TransformOutput:PDFInterpol=KDE:KDEtype=Gauss:KDEiter=Adaptive:KDEFineFactor=0.3:KDEborder=None:NAvEvtPerBin=50" ); 

	// Use a variable-dependent mix of splines and kernel density estimator
	if (Use["LikelihoodMIX"])
		factory->BookMethod( TMVA::Types::kLikelihood, "LikelihoodMIX",
				"!H:!V:!TransformOutput:PDFInterpolSig[0]=KDE:PDFInterpolBkg[0]=KDE:PDFInterpolSig[1]=KDE:PDFInterpolBkg[1]=KDE:PDFInterpolSig[2]=Spline2:PDFInterpolBkg[2]=Spline2:PDFInterpolSig[3]=Spline2:PDFInterpolBkg[3]=Spline2:KDEtype=Gauss:KDEiter=Nonadaptive:KDEborder=None:NAvEvtPerBin=50" ); 

	// Test the multi-dimensional probability density estimator
	// here are the options strings for the MinMax and RMS methods, respectively:
	//      "!H:!V:VolumeRangeMode=MinMax:DeltaFrac=0.2:KernelEstimator=Gauss:GaussSigma=0.3" );
	//      "!H:!V:VolumeRangeMode=RMS:DeltaFrac=3:KernelEstimator=Gauss:GaussSigma=0.3" );
	if (Use["PDERS"])
		factory->BookMethod( TMVA::Types::kPDERS, "PDERS",
				"!H:!V:NormTree=T:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600" );

	if (Use["PDERSD"])
		factory->BookMethod( TMVA::Types::kPDERS, "PDERSD",
				"!H:!V:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600:VarTransform=Decorrelate" );

	if (Use["PDERSPCA"])
		factory->BookMethod( TMVA::Types::kPDERS, "PDERSPCA",
				"!H:!V:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600:VarTransform=PCA" );

	// Multi-dimensional likelihood estimator using self-adapting phase-space binning
	if (Use["PDEFoam"])
		factory->BookMethod( TMVA::Types::kPDEFoam, "PDEFoam",
				"!H:!V:SigBgSeparate=F:TailCut=0.001:VolFrac=0.0666:nActiveCells=500:nSampl=2000:nBin=5:Nmin=100:Kernel=None:Compress=T" );

	if (Use["PDEFoamBoost"])
		factory->BookMethod( TMVA::Types::kPDEFoam, "PDEFoamBoost",
				"!H:!V:Boost_Num=30:Boost_Transform=linear:SigBgSeparate=F:MaxDepth=4:UseYesNoCell=T:DTLogic=MisClassificationError:FillFoamWithOrigWeights=F:TailCut=0:nActiveCells=500:nBin=20:Nmin=400:Kernel=None:Compress=T" );

	// K-Nearest Neighbour classifier (KNN)
	if (Use["KNN"])
		factory->BookMethod( TMVA::Types::kKNN, "KNN",
				"H:nkNN=20:ScaleFrac=0.8:SigmaFact=1.0:Kernel=Gaus:UseKernel=F:UseWeight=T:!Trim" );

	// H-Matrix (chi2-squared) method
	if (Use["HMatrix"])
		factory->BookMethod( TMVA::Types::kHMatrix, "HMatrix", "!H:!V:VarTransform=None" );

	// Linear discriminant (same as Fisher discriminant)
	if (Use["LD"])
		factory->BookMethod( TMVA::Types::kLD, "LD", "H:!V:VarTransform=None:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10" );

	// Fisher discriminant (same as LD)
	if (Use["Fisher"])
		factory->BookMethod( TMVA::Types::kFisher, "Fisher", "H:!V:Fisher:VarTransform=None:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10" );

	// Fisher with Gauss-transformed input variables
	if (Use["FisherG"])
		factory->BookMethod( TMVA::Types::kFisher, "FisherG", "H:!V:VarTransform=Gauss" );

	// Composite classifier: ensemble (tree) of boosted Fisher classifiers
	if (Use["BoostedFisher"])
		factory->BookMethod( TMVA::Types::kFisher, "BoostedFisher", 
				"H:!V:Boost_Num=20:Boost_Transform=log:Boost_Type=AdaBoost:Boost_AdaBoostBeta=0.2:!Boost_DetailedMonitoring" );

	// Function discrimination analysis (FDA) -- test of various fitters - the recommended one is Minuit (or GA or SA)
	if (Use["FDA_MC"])
		factory->BookMethod( TMVA::Types::kFDA, "FDA_MC",
				"H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=MC:SampleSize=100000:Sigma=0.1" );

	if (Use["FDA_GA"]) // can also use Simulated Annealing (SA) algorithm (see Cuts_SA options])
		factory->BookMethod( TMVA::Types::kFDA, "FDA_GA",
				"H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=GA:PopSize=300:Cycles=3:Steps=20:Trim=True:SaveBestGen=1" );

	if (Use["FDA_SA"]) // can also use Simulated Annealing (SA) algorithm (see Cuts_SA options])
		factory->BookMethod( TMVA::Types::kFDA, "FDA_SA",
				"H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=SA:MaxCalls=15000:KernelTemp=IncAdaptive:InitialTemp=1e+6:MinTemp=1e-6:Eps=1e-10:UseDefaultScale" );

	if (Use["FDA_MT"])
		factory->BookMethod( TMVA::Types::kFDA, "FDA_MT",
				"H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=2:UseImprove:UseMinos:SetBatch" );

	if (Use["FDA_GAMT"])
		factory->BookMethod( TMVA::Types::kFDA, "FDA_GAMT",
				"H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=GA:Converger=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=0:!UseImprove:!UseMinos:SetBatch:Cycles=1:PopSize=5:Steps=5:Trim" );

	if (Use["FDA_MCMT"])
		factory->BookMethod( TMVA::Types::kFDA, "FDA_MCMT",
				"H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=MC:Converger=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=0:!UseImprove:!UseMinos:SetBatch:SampleSize=20" );

	// TMVA ANN: MLP (recommended ANN) -- all ANNs in TMVA are Multilayer Perceptrons
	if (Use["MLP"])
		factory->BookMethod( TMVA::Types::kMLP, "MLP", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=10000:HiddenLayers=N+15:TestRate=8:!UseRegulator" );

	if (Use["MLPBFGS"])
		factory->BookMethod( TMVA::Types::kMLP, "MLPBFGS", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=5000:HiddenLayers=N+10:TestRate=5:TrainingMethod=BFGS:!UseRegulator" );

	if (Use["MLPBNN"])
		factory->BookMethod( TMVA::Types::kMLP, "MLPBNN", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=10000:HiddenLayers=N+16:TestRate=8:TrainingMethod=BFGS:UseRegulator" ); // BFGS training with bayesian regulators

	// CF(Clermont-Ferrand)ANN
	if (Use["CFMlpANN"])
		factory->BookMethod( TMVA::Types::kCFMlpANN, "CFMlpANN", "!H:!V:NCycles=10000:HiddenLayers=N+18,N"  ); // n_cycles:#nodes:#nodes:...  

	// Tmlp(Root)ANN
	if (Use["TMlpANN"])
		factory->BookMethod( TMVA::Types::kTMlpANN, "TMlpANN", "!H:!V:NCycles=200:HiddenLayers=N+1,N:LearningMethod=BFGS:ValidationFraction=0.4"  ); // n_cycles:#nodes:#nodes:...

	// Support Vector Machine
	if (Use["SVM"])
		factory->BookMethod( TMVA::Types::kSVM, "SVM", "Gamma=0.25:Tol=0.001:VarTransform=Norm" );

	// Boosted Decision Trees
	if (Use["BDTG"]) // Gradient Boost
		factory->BookMethod( TMVA::Types::kBDT, "BDTG",
				"!H:!V:NTrees=1300:MinNodeSize=5.0%:BoostType=Grad:Shrinkage=1.0:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=35000:MaxDepth=3" );

	if (Use["BDT"])  // Adaptive Boost
		factory->BookMethod( TMVA::Types::kBDT, "BDT",
				"!H:!V:NTrees=250:MinNodeSize=5.0%:MaxDepth=3:BoostType=RealAdaBoost:AdaBoostBeta=0.7:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=10000" );

	if (Use["BDTB"]) // Bagging
		factory->BookMethod( TMVA::Types::kBDT, "BDTB",
				"!H:!V:NTrees=300:BoostType=Bagging:SeparationType=GiniIndex:nCuts=50" );

	if (Use["BDTD"]) // Decorrelation + Adaptive Boost
		factory->BookMethod( TMVA::Types::kBDT, "BDTD",
				"!H:!V:NTrees=300:MinNodeSize=5%:MaxDepth=4:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=50:VarTransform=Decorrelate" );

	if (Use["BDTF"])  // Allow Using Fisher discriminant in node splitting for (strong) linearly correlated variables
		factory->BookMethod( TMVA::Types::kBDT, "BDTMitFisher",
				"!H:!V:NTrees=200:MinNodeSize=2.5%:UseFisherCuts:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.8:SeparationType=GiniIndex:nCuts=10000" );

	// RuleFit -- TMVA implementation of Friedman's method
	if (Use["RuleFit"])
		factory->BookMethod( TMVA::Types::kRuleFit, "RuleFit",
				"H:!V:RuleFitModule=RFTMVA:Model=ModRuleLinear:MinImp=0.001:RuleMinDist=0.001:NTrees=20:fEventsMin=0.01:fEventsMax=0.5:GDTau=-1.0:GDTauPrec=0.01:GDStep=0.01:GDNSteps=10000:GDErrScale=1.02" );

	// For an example of the category classifier usage, see: TMVAClassificationCategory

	// --------------------------------------------------------------------------------------------------

	// ---- Now you can optimize the setting (configuration) of the MVAs using the set of training events

	// ---- STILL EXPERIMENTAL and only implemented for BDT's ! 
	// factory->OptimizeAllMethods("SigEffAt001","Scan");
	// factory->OptimizeAllMethods("ROCIntegral","FitGA");

	// --------------------------------------------------------------------------------------------------

	// ---- Now you can tell the factory to train, test, and evaluate the MVAs

	// Train MVAs using the set of training events
	factory->TrainAllMethods();

	// ---- Evaluate all MVAs using the set of test events
	factory->TestAllMethods();

	// ----- Evaluate and compare performance of all configured MVAs
	factory->EvaluateAllMethods();

	// --------------------------------------------------------------

	// Binning (Step Size) //
	/*
	   TMVA::MethodCuts* cuts = dynamic_cast<TMVA::MethodCuts*>(factory->GetMethod("Cuts"));
	   Double_t epsilon = 0.0001;
	   for (Double_t eff=0.0001; eff<0.01; eff += 0.0001) {
	   cuts->PrintCuts( eff+epsilon );
	   }
	   */

	// Save the output
	outputFile->Close();

	std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
	std::cout << "==> TMVAClassification is done!" << std::endl;

	delete factory;

	// Launch the GUI for the root macros
	if(!gROOT->IsBatch()) TMVAGui(outfileName);
}

int main(int argc, char* argv[])
{
	if(argc==9)
	{
		TMVAClassification(argv[1],argv[2],argv[3],argv[4],argv[5],atof(argv[6]),atof(argv[7]),argv[8]);
		return 0;
	}
	else if(argc==8)
	{
		TMVAClassification(argv[1],argv[2],argv[3],argv[4],argv[5],atof(argv[6]),atof(argv[7]));
		return 0;
	}
	else
	{
		std::cout<<"  Error: invalid argument number - TMVAClassification()"<<std::endl;
		return 1;
	}
}
