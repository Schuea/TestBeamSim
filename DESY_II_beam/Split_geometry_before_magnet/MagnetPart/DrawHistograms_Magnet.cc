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

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

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
//	TH1::SetDefaultSumw2();

	TFile * input_rootfile = new TFile(name.c_str(),"READ");
	cout << "Inputfile size = " << input_rootfile->GetSize() << endl;
	TTree * T = (TTree*)input_rootfile->Get("Tree");
	cout << "Accessed TTree.." << endl;

	stringstream output_filename;
	output_filename << "histograms_" << name;
	TFile * output_rootfile = new TFile(output_filename.str().c_str(),"RECREATE");

	float energy = 1.0;
	float energyinterval = 1.0;
	float energyrange = 0.02;
	float max_angle = 6.0;
	int nbinsx = 300;
	vector< TH1F* > AngleHisto_ElePosiPions_;
	vector< TCanvas* > Angle_Canvas_;
	vector< TPaveText* > AngleCutBox_;

	for(int i = 1; i < 8; ++i){
		if (energy == 2.0){
			max_angle = 6.0;
			nbinsx = 200;
			energyrange = 0.02;
		}
                else if (energy > 2.0 && energy <=5.0 ){
			max_angle = 3.0;
			nbinsx = 200;
			energyrange = 0.04;
		}
                else if (energy > 5.0 && energy < 7.0){
			max_angle = 2.0;
			nbinsx = 50;
			energyrange = 0.05;
			energyinterval = 0.3;
		}

		stringstream histoname1;
		stringstream histotitle1;
		stringstream drawtext1;
		stringstream cuttext1;
		stringstream histotitle,pavetext;
		stringstream savetext1, savetext2;
                histoname1 << "AngleHisto_ElePosiPions_" << i;
		histotitle1 << "Angular distribution of e^{-} and e^{+}, for an energy between " << energy -energyrange << " and " << energy + energyrange;
		drawtext1 << "Theta >>+ " << histoname1.str();
		cuttext1 << "(Particle_ID==11 || Particle_ID==-11) && Energy > "<< energy-energyrange <<" && Energy < " << energy+energyrange << " && hasLeftDetector_Status==1";
		pavetext << energy-energyrange << " GeV < Energy < " << energy+energyrange << " GeV";
		savetext1 << "DeflectionAngle_Magnet_" << energy << "GeV.eps";
		savetext2 << "DeflectionAngle_Magnet_" << energy << "GeV.C";

		TH1F* temphisto1 = new TH1F(histoname1.str().c_str(), histotitle1.str().c_str(),300,0,max_angle);
                AngleHisto_ElePosiPions_.push_back(temphisto1);
//cout << __LINE__<<endl;              
	  	AngleHisto_ElePosiPions_.at(i-1)->SetLineColor(2);
		AngleHisto_ElePosiPions_.at(i-1)->GetXaxis()->SetTitle("#theta (degree)");
		AngleHisto_ElePosiPions_.at(i-1)->GetXaxis()->CenterTitle();
	
		TCanvas *tempcanvas = new TCanvas();
		Angle_Canvas_.push_back(tempcanvas);
	
		Angle_Canvas_.at(i-1)->cd();
//		gStyle->SetOptStat(11);
	
		T->Draw(drawtext1.str().c_str(), cuttext1.str().c_str());
		gPad->Update();
        	TPaveStats *st1 = (TPaveStats*)AngleHisto_ElePosiPions_.at(i-1)->FindObject("stats");
        	double statboxsize = st1->GetY2NDC() - st1->GetY1NDC();
        	st1->SetTextColor(2);
        	st1->SetY1NDC(0.75);
        	st1->SetY2NDC(0.75+statboxsize);	

		TPaveText* tempbox = new TPaveText(.4,.89,.6,.94,"brNDC");
		AngleCutBox_.push_back(tempbox);
		AngleCutBox_.at(i-1)->AddText(pavetext.str().c_str());
		AngleCutBox_.at(i-1)->Draw();

		Angle_Canvas_.at(i-1)->Write();
		Angle_Canvas_.at(i-1)->Print(savetext1.str().c_str());
		Angle_Canvas_.at(i-1)->Print(savetext2.str().c_str());
		Angle_Canvas_.at(i-1)->Close();
	
		energy += energyinterval;
        }

	output_rootfile->Write();
	input_rootfile->Close();
}
