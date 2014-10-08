#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TBranch.h"
#include <sstream>
#include <iostream>

using namespace std;

#define PDGID_GAMMA 22
#define PDGID_POSITRON -11
#define PDGID_ELECTRON 11

int main(){
	TChain * rootchain = new TChain("Tree");

	for (int i=1; i<=19590; i++){
		stringstream filename;
		filename << "Sim_2380cm_air_2G_" << i << ".root";
		rootchain->Add(filename.str().c_str());
		if (i%100==0) cout << "Merging files..." << endl;
	}
	rootchain->Merge("allfiles_2380cm_air_2G.root");

	bool hasLeft_status;
	int particle_id;
	int event_id;
	float energy, x, y;
	hasLeft_status = 0;
	particle_id = 0;
	event_id = 0;
	energy = 0;
	x = 0;
	y = 0;

	rootchain->SetBranchAddress("Particle_ID",&particle_id);
	rootchain->SetBranchAddress("Event_ID",&event_id);
	rootchain->SetBranchAddress("hasLeftDetector_Status",&hasLeft_status);
	rootchain->SetBranchAddress("Energy",&energy);
	rootchain->SetBranchAddress("Reflectionx",&x);
	rootchain->SetBranchAddress("Reflectiony",&y);

	TFile * skimfile = new TFile("allfiles_2380cm_air_skimmed_2G.root","RECREATE");
	rootchain->LoadTree(0);
	TTree * newtree = (TTree*)rootchain->GetTree()->CloneTree(0);

	for (int k=0; k<rootchain->GetEntries(); k++){
		rootchain->GetEntry(k);
	//	if (particle_id==PDGID_GAMMA && hasLeft_status==true) continue;
		if (particle_id > 10000000) continue;
		newtree->Fill();
		if (k%10000==0) cout << "Skimming file..." << endl;
	}

	skimfile->cd();
	newtree->Write();
	skimfile->Close();
}
