#include "TFile.h"
#include "TBrowser.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1.h"
#include "TF1.h"
#include "TH2.h"
#include "TStyle.h"
#include "TPaveText.h"
#include "TLatex.h"
#include "TEllipse.h"
#include "TPaveStats.h"
#include "TProfile.h"
#include "TGaxis.h"
#include "TROOT.h"

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
	TH1::SetDefaultSumw2();

	TFile * input_rootfile = new TFile(name.c_str(),"READ");
	cout << "Inputfile size = " << input_rootfile->GetSize() << endl;
	TTree * T = (TTree*)input_rootfile->Get("Tree");
	cout << "Accessed TTree.." << endl;

	stringstream output_filename;
	output_filename << "histograms_" << name;
	TFile * output_rootfile = new TFile(output_filename.str().c_str(),"RECREATE");

	TH1F * EnergyHisto_DESY_II_beam = new TH1F("E_DESY_II_beam","Energy distribution of electrons in the DESY II beam bunch (here 10^{7} instead of 10^{10} e^{-})",200,6.2,6.4);
	EnergyHisto_DESY_II_beam->SetLineColor(4);
	EnergyHisto_DESY_II_beam->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_DESY_II_beam->GetXaxis()->CenterTitle();

	TH2F * beam_shape = new TH2F("beam_shape","DESY II beam shape",1000,-0.0005,0.0005,1000,-0.0005,0.0005);
	beam_shape->GetXaxis()->SetTitle("x (mm)");
	beam_shape->GetXaxis()->CenterTitle();
	beam_shape->GetYaxis()->SetTitle("y (mm)");
	beam_shape->GetYaxis()->CenterTitle();

	TCanvas * Position_Canvas = new TCanvas("beam_Position_Canvas");
	TCanvas * Energy_Canvas = new TCanvas("beam_Energy_Canvas");

	Energy_Canvas->cd();
	T->Draw("Energy >>+ E_DESY_II_beam","Particle_ID==11");	
	TF1 *gausfit = new TF1("gausfit","gaus");
	EnergyHisto_DESY_II_beam->Fit(gausfit,"U");

	string canvasname=Energy_Canvas->GetName();
	Energy_Canvas->Write();
	Energy_Canvas->Print((canvasname+".eps").c_str());
	Energy_Canvas->Print((canvasname+".C").c_str());
	Energy_Canvas->Close();


	Position_Canvas->cd();
	TGaxis::SetMaxDigits(2);
	T->Draw("Reflectiony:Reflectionx >>+ beam_shape","Particle_ID==11","colz");	
	gStyle->SetNumberContours(99);
	gPad->SetLogz();
	gROOT->ForceStyle();

	string canvasname2=Position_Canvas->GetName();
	Position_Canvas->Write();
	Position_Canvas->Print((canvasname2+".eps").c_str());
	Position_Canvas->Print((canvasname2+".C").c_str());
	Position_Canvas->Close();

	output_rootfile->Write();
	input_rootfile->Close();

	delete input_rootfile, output_rootfile;
	delete Position_Canvas, Energy_Canvas;
}

