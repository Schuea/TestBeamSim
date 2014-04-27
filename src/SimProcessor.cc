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

	File = new TFile("SimTEST.root","RECREATE"/*"UPDATE"*/,"TestBeam Simulation");
	Tree = new TTree("Tree","SimHisto_Tree");

	Deflection_Canvas = new TCanvas();
	Map_Canvas = new TCanvas();

	TH1F* Photon_energy_Fibre = new TH1F("PhotonSource","Energy distribution of photons created in Carbon fibre",500,0,6.5);
	TH2F* Photon_Deflection_Fibre = new TH2F("PhotonDeflection","Deflection of photons created in Carbon fibre",500,-30,30,500,-30,30);
	Photon_Deflection_Fibre->SetOption("colz");

	TH1F* Photon_energy_Target = new TH1F("E_Photon","Energy distribution of photons existing after target",500,0,6.5);
	TH1F* Electron_energy_Target = new TH1F("E_Electron","Energy distribution of electrons created in target",500,0,6.5);
	TH1F* Positron_energy_Target= new TH1F("E_Positron","Energy distribution of positrons created in target",500,0,6.5);
	TH2F* Photon_Deflection_Target = new TH2F("DeflTar_Photon","Deflection of photons existing after target",800,-20,20,800,-20,20);
	TH2F* Electron_Deflection_Target = new TH2F("DeflTar_Electron","Deflection of electrons created in target",800,-20,20,800,-20,20);
	TH2F* Positron_Deflection_Target = new TH2F("DeflTar_Positron","Deflection of positrons created in target",800,-20,20,800,-20,20);
	Photon_Deflection_Target->SetOption("colz");
	Photon_Deflection_Target->GetZaxis()->SetRangeUser(0,150);
	Electron_Deflection_Target->SetOption("colz");
	Electron_Deflection_Target->GetZaxis()->SetRangeUser(0,100);
	Positron_Deflection_Target->SetOption("colz");
	Positron_Deflection_Target->GetZaxis()->SetRangeUser(0,100);

  //------MAPS-----
 
	//Rebinning the TH2F histograms:
	float *binning_array_x;
	float *binning_array_y;
	binning_array_x=Fill_RebinningArrays(7,-0.5,0.5,2);
	binning_array_y=Fill_RebinningArrays(7,-0.5,0.5,2);
	
	TH2F* rebinned_Electron_Deflection_Fibre = new TH2F("rebinned_DeflFibre_Electron","Deflection of electrons existing after fibre",7,binning_array_x,7,binning_array_y);
	rebinned_Electron_Deflection_Fibre->SetOption("colz,TEXT");
	rebinned_Electron_Deflection_Fibre->GetXaxis()->SetTitle("x-position/mm");
	rebinned_Electron_Deflection_Fibre->GetXaxis()->CenterTitle();
	rebinned_Electron_Deflection_Fibre->GetYaxis()->SetTitle("y-position/mm");
	rebinned_Electron_Deflection_Fibre->GetYaxis()->CenterTitle();

	rebinned_Photon_Deflection_Fibre = new TH2F("rebinned_DeflFibre_Photon","Deflection of photons created in fibre",7,binning_array_x,7,binning_array_y);
	rebinned_Photon_Deflection_Fibre->SetOption("colz,TEXT");
	rebinned_Photon_Deflection_Fibre->GetXaxis()->SetTitle("x-position/mm");
	rebinned_Photon_Deflection_Fibre->GetXaxis()->CenterTitle();
	rebinned_Photon_Deflection_Fibre->GetYaxis()->SetTitle("y-position/mm");
	rebinned_Photon_Deflection_Fibre->GetYaxis()->CenterTitle();

	EnergyMap_Electron_Deflection_Fibre = new TH2F("EnergyMap_DeflFibre_Electron","Deflection of electrons existing after fibre and their average energy",7,binning_array_x,7,binning_array_y);
	EnergyMap_Electron_Deflection_Fibre->SetOption("box,TEXT");
	EnergyMap_Electron_Deflection_Fibre->GetXaxis()->SetTitle("x-position/mm");
	EnergyMap_Electron_Deflection_Fibre->GetXaxis()->CenterTitle();
	EnergyMap_Electron_Deflection_Fibre->GetYaxis()->SetTitle("y-position/mm");
	EnergyMap_Electron_Deflection_Fibre->GetYaxis()->CenterTitle();

 	EnergyMap_Photon_Deflection_Fibre = new TH2F("EnergyMap_DeflFibre_Photon","Deflection of photons created in fibre and their average energy",7,binning_array_x,7,binning_array_y);
        EnergyMap_Photon_Deflection_Fibre->SetOption("box,TEXT");
	EnergyMap_Photon_Deflection_Fibre->GetXaxis()->SetTitle("x-position/mm");
	EnergyMap_Photon_Deflection_Fibre->GetXaxis()->CenterTitle();
	EnergyMap_Photon_Deflection_Fibre->GetYaxis()->SetTitle("y-position/mm");
	EnergyMap_Photon_Deflection_Fibre->GetYaxis()->CenterTitle();




	registerInputCollectionProcessor(new InputCollectionProcessor_MCParticle_collection(Tree,"MCParticle"));
	registerInputCollectionProcessor(new InputCollectionProcessor_SimCalorimeterHit_collection(Tree,"CarbonFiberHits"));
	registerInputCollectionProcessor(new InputCollectionProcessor_LCGenericObject_collection(Tree,"MCParticleEndPointEnergy"));

	registerOutputCollectionProcessor(new OutputCollectionProcessor_MCParticle_collection("MCParticlePhotonSource"));


	/*
	registerOutputCollection(EVENT::MCParticle, 
                                 "OutputCollectionName", 
				 "Only photons generated by Carbon fibre", 
				 Outputcol1, std::string(""));
	*/

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

	std::cout<<"output_col_processors.push_back(p)->getTypeName()"<<p->getTypeName()<<std::endl;
	std::cout<<"output_col_processors.push_back(p)->getName()"<<p->getName()<<std::endl;
	std::cout<<"output_col_processors.push_back(p)->getDescription()"<<p->getDescription()<<std::endl;
	std::cout<<"output_col_processors.push_back(p)->getCol()"<<p->getCol()<<std::endl;
	std::cout<<"output_col_processors.push_back(p)->getDefaultName()"<<p->getDefaultName()<<std::endl;
}

void SimProcessor::processRunHeader( LCRunHeader* run) { 
	streamlog_out(DEBUG) << "Running processRunHeader" << endl;

    	_iRun++ ;
} //end processRunHeader


void SimProcessor::processEvent( LCEvent * evt ) {   
	streamlog_out(DEBUG0) << "Running processEvent" << endl;
	
	//Process single collections in specific functions

	/*const std::vector<std::string>* CollectionNames = evt->getCollectionNames();	
	for(size_t i=0; i<CollectionNames->size(); ++i){
		std::cout<<CollectionNames->at(i)<<std::endl;
	}

 	for( StringVec::const_iterator it = CollectionNames->begin();it != CollectionNames->end() ; it++ ){  

	}*/

	//for(size_t i=0; i<input_col_processors.size(); ++i){
		//input_col_processors.at(0/*only MCParticle*/)->processCurrentEvent_InputCollection(evt); //in Collection_Processor.h
		//streamlog_out(MESSAGE1) << "Process InputCollection #"<< i <<" for current event #" << evt->getEventNumber()  << endl;
	//}
	input_col_processors.at(0/*only MCParticle*/)->processCurrentEvent_InputCollection(evt); //in Collection_Processor.h
/*
	for(size_t j=0; j<output_col_processors.size(); ++j){
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

	TCut* PhotonID_cut = new TCut("Particle_ID==22");
	TCut* ElectronID_cut = new TCut("Particle_ID==11");
	TCut* PositronID_cut = new TCut("Particle_ID==-11");
	TCut* cut_on_Energy0 = new TCut("");
	TCut* cut_on_Energy1 = new TCut("0.1<Energy");
	TCut* cut_on_Energy2 = new TCut("0.0<Energy && Energy<0.1");
	TCut* cut_on_Energy3 = new TCut("0.1<Energy && Energy<1.0");
	TCut* cut_on_Energy4 = new TCut("1.0<Energy");
/*
	Tree->Draw("Energy >>+ PhotonSource",(*PhotonID_cut) &&(* cut_on_Energy1));
	Tree->Draw("Reflectiony:Reflectionx >>+ PhotonDeflection",(*PhotonID_cut) &&(* cut_on_Energy2), "colz");
*/
/*	
	Deflection_Canvas->Divide(2,2);
	Deflection_Canvas->cd(1);
	gStyle->SetNumberContours(999);
	gPad->SetLogz();
	Tree->Draw("Reflectiony:Reflectionx >>+ DeflTar_Photon",(*PhotonID_cut)&&(*cut_on_Energy0),"colz");
	//TH2F* Photon_Defl_Tar_log = (TH2F*)Deflection_Canvas->GetPad(1)->GetPrimitive("DeflTar_Photon");
	//Photon_Defl_Tar_log->Write();
	
	Deflection_Canvas->cd(2);
	gStyle->SetNumberContours(999);
	gPad->SetLogz();
	Tree->Draw("Reflectiony:Reflectionx >>+ DeflTar_Electron",(*ElectronID_cut)&&(*cut_on_Energy0), "colz");
	
	Deflection_Canvas->cd(3);
	gStyle->SetNumberContours(999);
	gPad->SetLogz();
	Tree->Draw("Reflectiony:Reflectionx >>+ DeflTar_Positron",(*PositronID_cut)&&(*cut_on_Energy0), "colz");
	

	Deflection_Canvas->Write();
	Deflection_Canvas->Close();
	
	Tree->Draw("Energy >>+ E_Photon",(*PhotonID_cut)&&(*cut_on_Energy0));
	Tree->Draw("Energy >>+ E_Electron",(*ElectronID_cut)&&(*cut_on_Energy0));
	Tree->Draw("Energy >>+ E_Positron",(*PositronID_cut)&&(*cut_on_Energy0));
*/

	Map_Canvas->Divide(1,2);
	Map_Canvas->cd(1);
	gPad->DrawFrame(-0.5,-0.5,0.5,0.5);
	TExec *ex1 = new TExec("ex1","gStyle->SetPaintTextFormat(\".0f\");");
	ex1->Draw();
//	Tree->Draw("Reflectiony:Reflectionx >>+ rebinned_DeflFibre_Electron",(*ElectronID_cut)&&(*cut_on_Energy0), "colz,TEXT");
	
//	Map_Canvas->cd(2);
//	gPad->DrawFrame(-20,-20,20,20);
//	TExec *ex2 = new TExec("ex2","gStyle->SetPaintTextFormat(\".0f\");");
//	ex2->Draw();
	Tree->Draw("Reflectiony:Reflectionx >>+ rebinned_DeflFibre_Photon",(*PhotonID_cut)&&(*cut_on_Energy0), "colz,TEXT");

 
//	Map_Canvas->cd(3);
//	gPad->DrawFrame(-20,-20,20,20);
//	TExec *ex3 = new TExec("ex3","gStyle->SetPaintTextFormat(\".4f\");");
//	ex3->Draw();
//	MakeEnergyHistogram(Tree,EnergyMap_Electron_Deflection_Fibre,11);

	Map_Canvas->cd(2);
	gPad->SetTopMargin(0.15);
//	gPad->DrawFrame(-0.5,-0.5,0.5,0.5);
//	TExec *ex2 = new TExec("ex2","gStyle->SetPaintTextFormat(\".4f\");");
//	ex2->Draw();
	MakeEnergyHistogram(Tree,EnergyMap_Photon_Deflection_Fibre,22);

	Map_Canvas->Write();


//	GetBinEnergyDistribution(Tree, rebinned_Photon_Deflection_Fibre, 22);

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







//---------------------------------------------------------------------------------------------------------------
//USED FUNCTIONS:

float * SimProcessor::Fill_RebinningArrays(const int nbins, const float min, const float max, const int dimensions){
	float *bin_array = new float[nbins+1];
	float *step_size_array = new float[nbins];

	float range = abs(max-min);
	int half_nbins = ceil(float(nbins)/float(2));
	bin_array[0]=min;
	bin_array[nbins]=max;
		
	streamlog_out(DEBUG4)<<"Fill_RebinningArrays: range= "<<range<<std::endl;
	if (dimensions == 2){	
		for (int i=0; i<half_nbins;i++){
			step_size_array[i]=float(float(range/2.0)/float(pow(2.0,i+1.0)));
			step_size_array[nbins-(i+1)]=step_size_array[i];
		}
		for (int j=1; j<half_nbins;j++){
			bin_array[j]=bin_array[j-1]+step_size_array[j-1];
		}	
		for (int j=nbins-1; j>=half_nbins;j--){
			bin_array[j]=bin_array[j+1]-step_size_array[j];
		}
	}

	if (dimensions == 1){
		for (int i=nbins; i>0;i--){
			step_size_array[nbins-i]=float(float(range)/float(3*i));
		}
		for (int j=1; j<nbins;j++){
			bin_array[j]=bin_array[j-1]+step_size_array[j-1];
		}	
	}

	streamlog_out(DEBUG4)<<"Fill_RebinningArrays_array: bin_array[0]= "<<bin_array[0]<<std::endl;
	for (int k=0;k<nbins;k++){
	streamlog_out(DEBUG4)<<"Fill_RebinningArrays_array: step_size_array["<<k<<"]= "<<step_size_array[k]<<std::endl;
	streamlog_out(DEBUG4)<<"Fill_RebinningArrays_array: bin_array["<<k+1<<"]= "<<bin_array[k+1]<<std::endl;
	}
	return bin_array;	
}

float GetMean(vector<float> vec){
	float mean(0);
	for(int i = 0; i < vec.size(); ++i){
		mean += vec.at(i);
	}
	mean /= float(vec.size());
	if(vec.size() == 0) mean = 0;
	return mean;
}

void SimProcessor::MakeEnergyHistogram(TTree *Tree, TH2F *histo, const int ParticleID){
	int id,event_id=0;
	float energy,x,y,z;
	id=0;
	energy=0;
	x=0;
	y=0;
	z=0;
	int binno=0;
	
	Tree->SetBranchAddress("Event_ID",&event_id);
	Tree->SetBranchAddress("Particle_ID",&id);
	Tree->SetBranchAddress("Energy",&energy);
	Tree->SetBranchAddress("Reflectionx",&x);
	Tree->SetBranchAddress("Reflectiony",&y);
	Tree->SetBranchAddress("Reflectionz",&z);

	int entries = Tree->GetEntries();
	std::map<int, vector< float > > tempmap;

	for(int i = 0; i < entries; ++i){
		Tree->GetEntry(i);

		if (id!=ParticleID) continue;	
		if (histo->GetXaxis()->GetXmin()<=x && x<=histo->GetXaxis()->GetXmax() && histo->GetYaxis()->GetXmin()<=y && y<=histo->GetYaxis()->GetXmax()){
			binno = histo->FindBin(x,y);
			tempmap[binno].push_back(energy);
		}
	}
	int totalbin = (histo->GetNbinsX()+2)*(histo->GetNbinsY()+2);//+2 due to overflow bins
	for(int i = 0; i < totalbin; ++i){
		float mean = GetMean(tempmap[i]);
		int xbin,ybin,zbin;
		xbin=0;
		ybin=0;
		zbin=0;
		streamlog_out(DEBUG4) << "Average Energy in Bin " << i << " = " << mean << endl;
		histo->GetBinXYZ(i,xbin,ybin,zbin);
		streamlog_out(DEBUG4) << "xbin(" << i << ") = " << xbin << endl;
		streamlog_out(DEBUG4) << "ybin(" << i << ") = " << ybin << endl;
		histo->Fill(histo->GetXaxis()->GetBinCenter(xbin),histo->GetYaxis()->GetBinCenter(ybin),mean);
	}
	histo->Draw("box,TEXT SAME");
}

void SimProcessor::GetBinEnergyDistribution(TTree* Tree, TH2F* histo, const int ParticleID){

	int id;
	float energy,x,y;
	id=0;
	energy=0;
	x=0;
	y=0;
	
	int bin_number=0;
	std::map<int, vector< float > > deflection_map;
	
	Tree->SetBranchAddress("Particle_ID",&id);
	Tree->SetBranchAddress("Energy",&energy);
	Tree->SetBranchAddress("Reflectionx",&x);
	Tree->SetBranchAddress("Reflectiony",&y);

	int entries = Tree->GetEntries();


	for(int i = 0; i < entries; ++i){
		Tree->GetEntry(i);

		if (id!=ParticleID) continue;	
		bin_number = histo->FindBin(x,y);
		deflection_map[bin_number].push_back(energy);
	}

	int totalbins = (histo->GetNbinsX()+2)*(histo->GetNbinsY()+2);//+2 overflowbins per axis
	
	int overflowbins[]={0,1,2,3,4,5,6,7,8,9,17,18,26,27,35,36,44,45,53,54,62,63,71,72,73,74,75,76,77,78,79,80};
		
	int normalbin_counter=1;
	TH1F** BinEnergyDistri_ = new TH1F*[totalbins];	

	float * binning_array;	
		binning_array = Fill_RebinningArrays(20,0,7,1);

	Hvalues_for_macros.open ("Hvalues_for_macros.txt");
	Hvalues_for_macros << "Bin number in Map" << " " << "x" << " " << "y" << " " << "Bin number in distribution histo" << " " << "Events" << " " << "Energy" << endl;

	for(int i = 0; i < totalbins; ++i){
		int xbin,ybin,zbin;
		histo->GetBinXYZ(i,xbin,ybin,zbin);

		stringstream histoname,histotitle;
		histoname << "BinEnergyDistri_[" << i << "]";
		histotitle << "Energy distribution of photons in Bin " << normalbin_counter << " of histogram \"rebinned_DeflFibre_Photon\"";
		
		//bool isoverflowbin =  std::find(std::begin(overflowbins),std::end( overflowbins), i) != std::end(overflowbins);
		bool isoverflowbin =  std::find(overflowbins,overflowbins+32, i) != overflowbins+32;
		if (isoverflowbin) continue;

		BinEnergyDistri_[i] = new TH1F(histoname.str().c_str(), histotitle.str().c_str(),20,binning_array); 
		BinEnergyDistri_[i]->GetXaxis()->SetTitle("Energy/GeV");
		BinEnergyDistri_[i]->GetXaxis()->CenterTitle();

		for(int j=0; j<deflection_map[i].size(); j++){
			BinEnergyDistri_[i]->Fill(deflection_map[i].at(j));
		}
		BinEnergyDistri_[i]->Draw("L");
		for (int k=1; k <= BinEnergyDistri_[i]->GetNbinsX();k++){
			Hvalues_for_macros << normalbin_counter  << "\t" << histo->GetXaxis()->GetBinCenter(xbin) << "\t" << histo->GetYaxis()->GetBinCenter(ybin) << "\t" << k << "\t" << BinEnergyDistri_[i]->GetBinContent(k) << "\t" << BinEnergyDistri_[i]->GetBinCenter(k) << endl;
		}
		normalbin_counter++;
	}
	Hvalues_for_macros.close();
}


//-------------------------------------------------------------------------------------------------------------------------


