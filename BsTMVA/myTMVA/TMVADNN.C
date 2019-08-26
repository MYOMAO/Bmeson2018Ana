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
 #include "TMVA/DataLoader.h"

void TMVADNN(TString inputSname, TString inputBname,
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
    Use["DNN"]             = 1;     // Deep Neural Network
   Use["DNN_GPU"]         = 0; // CUDA-accelerated DNN training.
  Use["DNN_CPU"]         = 0; // Multi-core accelerated DNN.	


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

	 TMVA::DataLoader *dataloader=new TMVA::DataLoader("dataset");

	TMVA::Factory *factory = new TMVA::Factory( "TMVADNN", outputFile,
			"!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification" );
	(TMVA::gConfig().GetVariablePlotting()).fNbinsXOfROCCurve = 100;


  

	dataloader->AddVariable("Dchi2cl");//>
	dataloader->AddVariable("DsvpvDistance*TMath::Sin(Dalpha)");
	dataloader->AddVariable("DsvpvDistance/DsvpvDisErr");//>
	dataloader->AddVariable("TMath::Cos(Dtrk1thetastar)");          
	dataloader->AddVariable("Dtrk1Pt");
	dataloader->AddVariable("Dtrk2Pt");
	dataloader->AddVariable("Dtrk1D0");
	dataloader->AddVariable("Dtrk2D0");
	dataloader->AddVariable("DsvpvDistance/(TMath::Sqrt((Dpt*TMath::CosH(Dy)*Dpt*TMath::CosH(Dy)+Dmass*Dmass*TMath::SinH(Dy)*TMath::SinH(Dy)))/Dmass)");
	
	TFile *inputS = new TFile(inputSname.Data());
	TFile *inputB = new TFile(inputBname.Data());

	std::cout << "--- TMVAClassification       : Using input file: " << inputS->GetName() << " & "<< inputB->GetName() <<std::endl;


	TTree* background = (TTree*)inputB->Get("ntDkpi");

	TTree* signal = (TTree*)inputS->Get("ntDkpi");
	signal->AddFriend("ntGen");

	//global event weights per tree (see below for setting event-wise weights)
	double signalWeight     = 1.0;
	double backgroundWeight = 1.0;

	// You can add an arbitrary number of signal or background trees
	dataloader->AddSignalTree    ( signal,     signalWeight     );
	dataloader->AddBackgroundTree( background, backgroundWeight );

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

	TString cuts = isPbPb?Form("(%s)&&Dpt>%f&&Dpt<%f&&hiBin>=0&&hiBin<=200",mycuts.Data(),ptmin,ptmax):Form("(%s)&&Dpt>%f&&Dpt<%f",mycuts.Data(),ptmin,ptmax); 
	TString cutb = isPbPb?Form("(%s)&&Dpt>%f&&Dpt<%f&&hiBin>=0&&hiBin<=200",mycutb.Data(),ptmin,ptmax):Form("(%s)&&Dpt>%f&&Dpt<%f",mycutb.Data(),ptmin,ptmax);

	TCut mycutS = (TCut)cuts;
	TCut mycutB = (TCut)cutb;
//	factory->PrepareTrainingAndTestTree( mycutS, mycutB, "nTrain_Signal=0:nTrain_Background=0:SplitMode=Random:NormMode=NumEvents:!V" );
    dataloader->PrepareTrainingAndTestTree( mycutS, mycutB,
                                           "nTrain_Signal=1000:nTrain_Background=1000:SplitMode=Random:NormMode=NumEvents:!V" );
	// ---- Book MVA methods
	//
	// Please lookup the various method configuration options in the corresponding cxx files, eg:
	// src/MethoCuts.cxx, etc, or here: http://tmva.sourceforge.net/optionRef.html
	// it is possible to preset ranges in the option string in which the cut optimisation should be done:
	// "...:CutRangeMin[2]=-1:CutRangeMax[2]=1"...", where [2] is the third input variable

	 if (Use["DNN"])
      {
         // General layout.
         TString layoutString ("Layout=TANH|128,TANH|128,TANH|128,LINEAR");
   
         // Training strategies.
         TString training0("LearningRate=1e-1,Momentum=0.9,Repetitions=1,"
                           "ConvergenceSteps=20,BatchSize=256,TestRepetitions=10,"
                           "WeightDecay=1e-4,Regularization=L2,"
                           "DropConfig=0.0+0.5+0.5+0.5, Multithreading=True");
         TString training1("LearningRate=1e-2,Momentum=0.9,Repetitions=1,"
                           "ConvergenceSteps=20,BatchSize=256,TestRepetitions=10,"
                           "WeightDecay=1e-4,Regularization=L2,"
                           "DropConfig=0.0+0.0+0.0+0.0, Multithreading=True");
         TString training2("LearningRate=1e-3,Momentum=0.0,Repetitions=1,"
                           "ConvergenceSteps=20,BatchSize=256,TestRepetitions=10,"
                           "WeightDecay=1e-4,Regularization=L2,"
                           "DropConfig=0.0+0.0+0.0+0.0, Multithreading=True");
         TString trainingStrategyString ("TrainingStrategy=");
         trainingStrategyString += training0 + "|" + training1 + "|" + training2;
   
         // General Options.
         TString dnnOptions ("!H:V:ErrorStrategy=CROSSENTROPY:VarTransform=N:"
                             "WeightInitialization=XAVIERUNIFORM");
         dnnOptions.Append (":"); dnnOptions.Append (layoutString);
         dnnOptions.Append (":"); dnnOptions.Append (trainingStrategyString);
   
         // Standard implementation, no dependencies.
         TString stdOptions = dnnOptions + ":Architecture=STANDARD";
         factory->BookMethod(dataloader, TMVA::Types::kDNN, "DNN", stdOptions);
   
         // Cuda implementation.
         if (Use["DNN_GPU"]) {
            TString gpuOptions = dnnOptions + ":Architecture=GPU";
            factory->BookMethod(dataloader, TMVA::Types::kDNN, "DNN GPU", gpuOptions);
         }
         // Multi-core CPU implementation.
         if (Use["DNN_CPU"]) {
            TString cpuOptions = dnnOptions + ":Architecture=CPU";
            factory->BookMethod(dataloader, TMVA::Types::kDNN, "DNN CPU", cpuOptions);
         }
       }
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
		TMVADNN(argv[1],argv[2],argv[3],argv[4],argv[5],atof(argv[6]),atof(argv[7]),argv[8]);
		return 0;
	}
	else if(argc==8)
	{
		TMVADNN(argv[1],argv[2],argv[3],argv[4],argv[5],atof(argv[6]),atof(argv[7]));
		return 0;
	}
	else
	{
		std::cout<<"  Error: invalid argument number - TMVADNN()"<<std::endl;
		return 1;
	}
}
