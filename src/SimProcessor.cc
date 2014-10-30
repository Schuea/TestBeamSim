#include "SimProcessor.h"
#include "Collection_Processor.h"
#include "OutputCollection_Processor.h"

#include "marlin/Processor.h"
#include <EVENT/MCParticle.h>
#include <EVENT/SimCalorimeterHit.h>
#include <EVENT/LCGenericObject.h>
#include <EVENT/LCEvent.h>
#include <EVENT/LCCollection.h>

#include <IMPL/LCCollectionVec.h>

#include <sstream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <map>

#include "TPaveStats.h"
#include "TExec.h"
#include "TStyle.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TBranch.h"
#include "TPad.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TCut.h"
#ifdef USE_MARLIN
// streamlog include
#include "streamlog/streamlog.h"
#endif

using namespace lcio;
using namespace marlin;
using namespace std;


//SIMPROCESSOR -------------------------------------------------------------------------------------------------

SimProcessor aSimProcessor;

SimProcessor::SimProcessor() : Processor("SimProcessor") {
	streamlog_out(DEBUG4) << "Running constructor" << endl;
	// Processor description 
  	// _description = "...";
	gStyle->SetNumberContours(999);
}

void SimProcessor::init() { 
	streamlog_out(DEBUG4) << "Running init" << endl;

	File = new TFile("Sim.root","RECREATE"/*"UPDATE"*/,"TestBeam Simulation");
	Tree_MCParticle = new TTree("Tree_MCP","TTree for MCParticles");
	Tree_EnterMagnet = new TTree("Tree_EnterMagnet","TTree for SimCalorimeterHit");
	Tree_LeaveMagnet = new TTree("Tree_LeaveMagnet","TTree for SimCalorimeterHit");

	registerInputCollectionProcessor(new InputCollectionProcessor_MCParticle_collection(Tree_MCParticle,"InputCollectionName","MCParticle"));
	registerInputCollectionProcessor(new InputCollectionProcessor_SimCalorimeterHit_collection(Tree_EnterMagnet,"InputCollectionName2","EnterMagnetPositions"));
	registerInputCollectionProcessor(new InputCollectionProcessor_SimCalorimeterHit_collection(Tree_LeaveMagnet,"InputCollectionName3","LeaveMagnetPositions"));
//	registerInputCollectionProcessor(new InputCollectionProcessor_LCGenericObject_collection(Tree,"InputCollectionName4","MCParticleEndPointEnergy"));

	registerOutputCollectionProcessor(new OutputCollectionProcessor_MCParticle_collection("MCParticlePhotonSource"));

  	printParameters() ;
    	_iRun = 0 ;
    	_iEvt = 0 ;
}

void SimProcessor::registerInputCollectionProcessor(Collection_Processor_Interface* p){
streamlog_out(DEBUG0) << "Line " << __LINE__ << endl;
	registerInputCollection(p->getTypeName(), 
				p->getName(), 
				p->getDescription(),
				p->getCol(),
				p->getDefaultName());

	input_col_processors.push_back(p);
	streamlog_out(DEBUG4)<<"col_processors.back()->getCol()= "<<input_col_processors.back()->getCol()<<std::endl;
}

void SimProcessor::registerOutputCollectionProcessor(OutputCollection_Processor_Interface* p){
streamlog_out(DEBUG0) << "Line " << __LINE__ << endl;
	registerOutputCollection(p->getTypeName(), 
				p->getName(), 
				p->getDescription(),
				p->getCol(),
				p->getDefaultName());

	output_col_processors.push_back(p);

}

void SimProcessor::processRunHeader( LCRunHeader* run) { 
	streamlog_out(DEBUG) << "Running processRunHeader" << endl;

    	_iRun++ ;
} //end processRunHeader


void SimProcessor::processEvent( LCEvent * evt ) {   
	streamlog_out(DEBUG0) << "Running processEvent" << endl;
	
	//Process single collections in specific functions
	const vector<string> *names = evt->getCollectionNames();
	for(size_t i=0; i<input_col_processors.size(); ++i){
		for(int j = 0; j < names->size(); ++j){
	//		cout << input_col_processors.at(i)->getCol() << " = " <<  names->at(j) << endl;
			if(input_col_processors.at(i)->getCol() == names->at(j)) {
				input_col_processors.at(i)->processCurrentEvent_InputCollection(evt); //in Collection_Processor.h
				streamlog_out(MESSAGE1) << "Process InputCollection #"<< i <<" for current event #" << evt->getEventNumber()  << endl;
			}
		}
	}

/*	for(size_t j=0; j<output_col_processors.size(); ++j){
streamlog_out(DEBUG0) << "Line " << __LINE__ << endl;
		output_col_processors.at(j)->processCurrentEvent_OutputCollection(evt); //in Collection_Processor.h
		streamlog_out(DEBUG4) << "Process OutputCollection #"<< j <<" for current event #" << evt->getEventNumber()  << endl;
	}
*/

    	//-- note: this will not be printed if compiled w/o MARLINstreamlog_out(DEBUG=1 !
    	streamlog_out(DEBUG) << "   processing event: " << evt->getEventNumber() 
      	  << "   in run:  " << evt->getRunNumber() << std::endl ;

    	_iEvt ++ ;
}


void SimProcessor::check( LCEvent * evt ) { 
    // nothing to check here - could be used to fill checkplots in reconstruction processor
}// end check()


void SimProcessor::end(){ 
streamlog_out(DEBUG0) << "Line " << __LINE__ << endl;


	File->Write();
	File->Close();

    	std::cout << "SimProcessor::end()  " << name() 
  	<< " processed " << _iEvt << " events in " << _iRun << " runs "
    	<< std::endl ;

	while(!input_col_processors.empty()){
		streamlog_out(DEBUG4) << "while(!input_col_processors.empty()){"<<std::endl;
		delete input_col_processors.back();
		input_col_processors.pop_back();
	}

}//end end()
//END SIMPROCESSOR ---------------------------------------------------------------------------------------------
