#include "TFile.h"
#include "TBrowser.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TTree.h"
#include "TBranch.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TGraph.h"
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

using namespace std;

void DrawingMacro(string name); 
float scalar_product(float a[3], float b[3]);
float norm(float a[3]);

int main(int argc,char *argv[]){
	if(argc < 2 || argc > 7){
    		//explain how to use program	
    		cerr << "Type in the root filename!" << endl;
    		cerr << "e.g. ./DrawHistograms file.root" << endl;
    		terminate();
	}	
	string filename1;
	filename1 = argv[1];

       
	DrawingMacro(filename1);
}

void DrawingMacro(string name1){
	TH1::SetDefaultSumw2();

	TFile * input_rootfile1 = new TFile(name1.c_str(),"READ");
	cout << "Inputfile1 size = " << input_rootfile1->GetSize() << endl;
	TTree * T1 = (TTree*)input_rootfile1->Get("Tree");
	cout << "Accessed TTree.." << endl;

	int   hit_id=0;
	float hit_posi_x=0;	
	float hit_posi_y=0;	
	float hit_posi_z=0;	

	T1->SetBranchAddress("HitParticle_ID",&hit_id);
	T1->SetBranchAddress("HitPosition_x",&hit_posi_x);
	T1->SetBranchAddress("HitPosition_y",&hit_posi_y);
	T1->SetBranchAddress("HitPosition_z",&hit_posi_z);

	float z_start_magnet = 23672.5;
	float z_end_magnet = 24720.0;

	float z_axis[3];
	z_axis[0] = 0.0;
	z_axis[1] = 0.0;
	z_axis[2] = 1.0;
	
	stringstream output_filename;
	output_filename << "Theta_plots_TBmagnet.root";
	TFile * output_rootfile = new TFile(output_filename.str().c_str(),"RECREATE");

	TH1F * DeflectionHisto_ElePosi_0_59T = new TH1F("Theta_ElePosi_0_59T","Deflection of electrons and positrons existing behind magnet with B = 0.59T",80,0,2);
	DeflectionHisto_ElePosi_0_59T->SetLineColor(kRed-6);
	DeflectionHisto_ElePosi_0_59T->GetXaxis()->SetTitle("Theta (degree)");
	DeflectionHisto_ElePosi_0_59T->GetXaxis()->CenterTitle();

	for(int n=0; n<= T1->GetEntries(); ++n){
		T1->GetEntry(n);

		float hit_start_point[3];
		float hit_end_point[3];
		if(hit_posi_z == z_start_magnet) {
			hit_start_point[0] = hit_posi_x;
			hit_start_point[1] = hit_posi_y;
			hit_start_point[2] = hit_posi_z;
			cout << "hit_posi_x = " << hit_posi_x << endl;
			cout << "hit_posi_y = " << hit_posi_y << endl;
			cout << "hit_posi_z = " << hit_posi_z << endl;
		}
		if(hit_posi_z == z_end_magnet) {
			hit_end_point[0] = hit_posi_x;
			hit_end_point[1] = hit_posi_y;
			hit_end_point[2] = hit_posi_z;
			cout << "hit_posi_x = " << hit_posi_x << endl;
			cout << "hit_posi_y = " << hit_posi_y << endl;
			cout << "hit_posi_z = " << hit_posi_z << endl;
		}
		else {
			cerr << "hit_posi_z != z_start/end_magnet" << endl;
			exit(1);
		}

		float particle_course[3];

		for(int d=0; d<3; ++d){
			particle_course[d] = hit_end_point[d] - hit_start_point[d];

			cout << "particle_course["<< d << "] = " << particle_course[d] << endl;
		}
		float theta = 0.0;
		theta = acos(scalar_product(z_axis,particle_course)/(norm(z_axis)*norm(particle_course)));	
		cout << "theta = " << theta << endl;

		DeflectionHisto_ElePosi_0_59T->Fill(theta);
	}
	TCanvas * Theta_Canvas = new TCanvas("Theta_Canvas");	
	Theta_Canvas->cd();
	DeflectionHisto_ElePosi_0_59T->Draw();

	string canvasname1 = Theta_Canvas->GetName();
	Theta_Canvas->Write();
	Theta_Canvas->Print((canvasname1+".eps").c_str());	
	Theta_Canvas->Print((canvasname1+".C").c_str());	
	Theta_Canvas->Close();
 

	output_rootfile->Write();
	input_rootfile1->Close();

	delete Theta_Canvas;
}

float scalar_product(float a[3], float b[3]){
		float result = 0;
		for (int d=0; d<3; d++) {
			result += a[d]*b[d];
		}
		return result;
}


float norm(float a[3]){
		float result = 0;
		float tmp=0;
		for (int d=0; d<3; d++) {
			tmp += a[d]*a[d];
		}
		result = sqrt(tmp);
		return result;
}
