#include "TFile.h"
#include "TBrowser.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1.h"
#include "TH2.h"
#include "TStyle.h"
#include "TPaveText.h"
#include "TLatex.h"
#include "TEllipse.h"
#include "TPaveStats.h"
#include "TProfile.h"
#include "TMath.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

#define _USE_MATH_DEFINES

using namespace std;

void DrawingMacro(string name); 

int main(int argc,char *argv[]){
	if(argc < 2 || argc > 2){
    		//explain how to use program	
    		cerr << "Type in root filename!" << endl;
    		cerr << "e.g. ./DrawHistograms file.root" << endl;
    		terminate();
	}	
	string filename;
	filename = argv[1];

       
	DrawingMacro(filename);
}

void DrawingMacro(string name){
	TH1::SetDefaultSumw2();

	TFile * input_rootfile = new TFile(name.c_str(),"READ");
	cout << "Inputfile size = " << input_rootfile->GetSize() << endl;

	int id1,id2;
	float energy1;
	double x1,x2,y1,y2,z1,z2;

	TTree * T1 = (TTree*)input_rootfile->Get("Tree_TrackerPlane1");
	T1->SetBranchAddress("LCIO_ID",&id1);
	T1->SetBranchAddress("HitPosition_x",&x1);
	T1->SetBranchAddress("HitPosition_y",&y1);
	T1->SetBranchAddress("HitPosition_z",&z1);
	T1->SetBranchAddress("HitEnergy",&energy1);

	TTree * T2 = (TTree*)input_rootfile->Get("Tree_TrackerPlane2");
	T2->SetBranchAddress("LCIO_ID",&id2);
	T2->SetBranchAddress("HitPosition_x",&x2);
	T2->SetBranchAddress("HitPosition_y",&y2);
	T2->SetBranchAddress("HitPosition_z",&z2);
	cout << "Accessed TTrees.." << endl;

	stringstream output_filename;
	output_filename << "theta_plots_" << name;
	TFile * output_rootfile = new TFile(output_filename.str().c_str(),"RECREATE");
	
	////////////////////PHILLS CODE//////////////////////////////
	TTree *OutputTree = new TTree("Output","Output");
	float angle(0);
	OutputTree->Branch("id",&id1,'I');
	OutputTree->Branch("energy",&energy1,'F');
	OutputTree->Branch("angle",&angle,'F');
	TH1F *deflectionangle = new TH1F("deflectionangle","Deflection angle of particles in magnetic field;#theta;Count",50,0,7);
	long int t1entries = T1->GetEntries();
	long int t2entries = T2->GetEntries();
	vector< int > knownID;
	for(long int i = 0; i < t1entries; ++i){
		T1->GetEntry(i);
		for(long int j = 0; j < t2entries; ++j){
			T2->GetEntry(j);
			if(id1 == id2){
				bool alreadyKnown = false;
				for(int k = 0; k < knownID.size(); ++k){
					if(id1 == knownID.at(k)){
						alreadyKnown = true;
						break;
					}
				}
				if(!alreadyKnown){
					angle = 360./(2.*M_PI)*acos((z2-z1)/(sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)+(z2-z1)*(z2-z1))));
					deflectionangle->Fill(angle);
					knownID.push_back(id1);
					OutputTree->Fill();
				}
			}
		}
	}
	
	////////////////////////////////////////////////////////////

	float energy = 1.0;
	float energyinterval = 1.0;
	float energyrange = 0.02;
	float max_angle = 6.0;
	float lower_limit = 0.996;
	vector< TH1F* > AngleHisto_ElePosi_;

	vector< TCanvas* > Angle_Canvas_;
	vector< TPaveText* > AngleCutBox_;
/*
	for(int i = 1; i < 8; ++i){
		if (energy <= 2.0){
			max_angle = 6.0;
			lower_limit = 0.996;
			energyrange = 0.02;
		}
                else if (energy > 2.0 && energy <=5.0 ){
			max_angle = 3.0;
			lower_limit = 0.9995;
			energyrange = 0.04;
		}
                else if (energy > 5.0 && energy < 7.0){
			max_angle = 2.0;
			energyinterval = 0.3;
			energyrange = 0.05;
			lower_limit = 0.9998;
		}

		stringstream histoname1_angle;
		histoname1_angle << "AngleHisto_ElePosi_" << i;

		stringstream histotitle1_angle;
		histotitle1_angle << "Angular distribution of electrons and positrons for an energy between " << energy -energyrange << " and " << energy + energyrange << " GeV";

		stringstream drawtext1_angle;
		drawtext1_angle << "360./(2.*3.1415926535)*acos((HitPosition_z-23845.0)/(sqrt((HitPosition_x-HitVertex_x)*(HitPosition_x-HitVertex_x)+(HitPosition_y-HitVertex_y)*(HitPosition_y-HitVertex_y)+(HitPosition_z-23845.0)*(HitPosition_z-23845.0)))) >>+ " << histoname1_angle.str();

		//drawtext1_angle << "360./(2.*3.1415926535)*acos((HitPosition_z-HitVertex_z)/(sqrt((HitPosition_x-HitVertex_x)*(HitPosition_x-HitVertex_x)+(HitPosition_y-HitVertex_y)*(HitPosition_y-HitVertex_y)+(HitPosition_z-HitVertex_z)*(HitPosition_z-HitVertex_z)))) >>+ " << histoname1_angle.str();
	
		stringstream cuttext1;
//		cuttext1 << "(HitParticle_ID==11 || HitParticle_ID==-11) && HitVertex_z>23066.25";
		cuttext1 << "(HitParticle_ID==11 || HitParticle_ID==-11) && HitVertex_z>23066.25 &&HitVertex_z<23071.25 &&  sqrt(HitMomentum_x*HitMomentum_x+HitMomentum_y*HitMomentum_y+HitMomentum_z*HitMomentum_z)> "<< energy-energyrange <<" && sqrt(HitMomentum_x*HitMomentum_x+HitMomentum_y*HitMomentum_y+HitMomentum_z*HitMomentum_z) < " << energy+energyrange;
	
		stringstream pavetext;
		pavetext << energy-energyrange << " GeV < Energy < " << energy+energyrange << " GeV";
		stringstream savetext1_angle, savetext2_angle;
		savetext1_angle << "DeflectionAngle_Magnet_" << energy << "GeV.eps";
		savetext2_angle << "DeflectionAngle_Magnet_" << energy << "GeV.C";

		TH1F* temphisto1_angle = new TH1F(histoname1_angle.str().c_str(), histotitle1_angle.str().c_str(),80,0,max_angle);
                AngleHisto_ElePosi_.push_back(temphisto1_angle);
	  	AngleHisto_ElePosi_.at(i-1)->SetLineColor(kRed-6);
		AngleHisto_ElePosi_.at(i-1)->GetXaxis()->SetTitle("#theta (degree)");
		AngleHisto_ElePosi_.at(i-1)->GetXaxis()->CenterTitle();
       
	       	TCanvas *tempcanvas_angle = new TCanvas();
		Angle_Canvas_.push_back(tempcanvas_angle);

              
		Angle_Canvas_.at(i-1)->cd();
		gStyle->SetOptStat(11);
	
		T->Draw(drawtext1_angle.str().c_str(), cuttext1.str().c_str());
		gPad->Update();
        	TPaveStats *st1 = (TPaveStats*)AngleHisto_ElePosi_.at(i-1)->FindObject("stats");
        	double statboxsize = st1->GetY2NDC() - st1->GetY1NDC();
        	st1->SetTextColor(kRed-6);
        	st1->SetY1NDC(0.8);
        	st1->SetY2NDC(0.8+statboxsize);	

		TPaveText* tempbox = new TPaveText(.4,.86,.6,.93,"brNDC");
		AngleCutBox_.push_back(tempbox);
		AngleCutBox_.at(i-1)->AddText(pavetext.str().c_str());
		AngleCutBox_.at(i-1)->Draw();


		Angle_Canvas_.at(i-1)->Write();
		Angle_Canvas_.at(i-1)->Print(savetext1_angle.str().c_str());
		Angle_Canvas_.at(i-1)->Print(savetext2_angle.str().c_str());
		Angle_Canvas_.at(i-1)->Close();

		energy += energyinterval;
 		delete temphisto1_angle;
		delete tempcanvas_angle;
		delete tempbox;
 	}
 */


	
	output_rootfile->Write();
	input_rootfile->Close();
}
