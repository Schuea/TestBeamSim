#ifndef SIM_PROCESOR_H
#define SIM_PROCESSOR_H

#include "marlin/Processor.h"
#include "TROOT.h"
#include "TFile.h"
#include "TH2F.h"
#include "TTree.h"
#include <fstream>

class Collection_Processor_Interface;
class OutputCollection_Processor_Interface;

class SimProcessor : public marlin::Processor {

  public:

   	virtual marlin::Processor * newProcessor() {return new SimProcessor;}

    	SimProcessor();  // Default constructor, called only once at the beginning,

    	virtual void init (); // Initializing processor

    	virtual void processRunHeader (lcio::LCRunHeader * run);  // Called for every run

    	virtual void processEvent (lcio::LCEvent * evt);  // Called for every event

     	virtual void check (lcio::LCEvent * evt); 

    	virtual void end();  // Called after data processing.

  private:

	void registerInputCollectionProcessor(Collection_Processor_Interface* p);
	void registerOutputCollectionProcessor(OutputCollection_Processor_Interface* p);

	int _iRun;  // Run number
    	int _iEvt;  // Event number

	TFile *File;
	TTree *Tree;
	TCanvas* Map_Canvas;

	std::vector<Collection_Processor_Interface*> input_col_processors;
	std::vector<OutputCollection_Processor_Interface*> output_col_processors;

};

#endif

