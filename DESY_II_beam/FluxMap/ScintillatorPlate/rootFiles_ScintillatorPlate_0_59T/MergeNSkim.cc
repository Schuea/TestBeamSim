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
	TChain * rootchain_MCP = new TChain("Tree_MCP");
	TChain * rootchain_EnterMagnet = new TChain("Tree_EnterMagnet");
	TChain * rootchain_LeaveMagnet = new TChain("Tree_LeaveMagnet");

	for (int i=1; i<=44087; i++){
		stringstream filename;
		filename << "ScintillatorPlate_Converter_MAGNET_0_59T_wo_Collimator_" << i << ".root";
		rootchain_MCP->Add(filename.str().c_str());
		rootchain_EnterMagnet->Add(filename.str().c_str());
		rootchain_LeaveMagnet->Add(filename.str().c_str());
		if (i%100==0) cout << "Adding files..." << endl;
	}
	rootchain_MCP->Merge("allfiles_scintillator_TB_magnet_B0_59T_MCP.root");
	rootchain_EnterMagnet->Merge("allfiles_scintillator_TB_magnet_B0_59T_EnterMagnet.root");
	rootchain_LeaveMagnet->Merge("allfiles_scintillator_TB_magnet_B0_59T_LeaveMagnet.root");

	int particle_id = 0;
	int hit_particle_id = 0;

	rootchain_MCP->SetBranchAddress("Particle_ID",&particle_id);
	rootchain_EnterMagnet->SetBranchAddress("HitParticle_ID",&hit_particle_id);
	rootchain_LeaveMagnet->SetBranchAddress("HitParticle_ID",&hit_particle_id);

	TFile * skimfile = new TFile("allfiles_scintillator_TB_magnet_B0_59T_skimmed.root","RECREATE");
	rootchain_MCP->LoadTree(0);
	TTree * newtree_MCP = (TTree*)rootchain_MCP->GetTree()->CloneTree(0);

	for (int k=0; k<rootchain_MCP->GetEntries(); k++){
		rootchain_MCP->GetEntry(k);
		if (particle_id > 10000000) continue;
		newtree_MCP->Fill();
		if (k%500==0) cout << "Skimming file..." << endl;
	}
	rootchain_EnterMagnet->LoadTree(0);
	TTree * newtree_EnterMagnet = (TTree*)rootchain_EnterMagnet->GetTree()->CloneTree(0);

	for (int k=0; k<rootchain_EnterMagnet->GetEntries(); k++){
		rootchain_EnterMagnet->GetEntry(k);
		if (particle_id > 10000000) continue;
		newtree_EnterMagnet->Fill();
		if (k%500==0) cout << "Skimming file..." << endl;
	}
	rootchain_LeaveMagnet->LoadTree(0);
	TTree * newtree_LeaveMagnet = (TTree*)rootchain_LeaveMagnet->GetTree()->CloneTree(0);

	for (int k=0; k<rootchain_LeaveMagnet->GetEntries(); k++){
		rootchain_LeaveMagnet->GetEntry(k);
		if (particle_id > 10000000) continue;
		newtree_LeaveMagnet->Fill();
		if (k%500==0) cout << "Skimming file..." << endl;
	}

	skimfile->cd();
	newtree_MCP->Write();
	newtree_EnterMagnet->Write();
	newtree_LeaveMagnet->Write();
	skimfile->Close();

	delete rootchain_MCP,rootchain_EnterMagnet,rootchain_LeaveMagnet;
	delete skimfile;
}
