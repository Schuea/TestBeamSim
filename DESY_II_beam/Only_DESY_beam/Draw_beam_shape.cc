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

void DrawingMacro(string name1, string name2); 

int main(int argc,char *argv[]){
	if(argc < 3 || argc > 3){
    		//explain how to use program	
    		cerr << "Type in root filenames!" << endl;
    		cerr << "e.g. ./DrawHistograms file1.root file2.root" << endl;
    		terminate();
	}	
	string filename1, filename2;
	filename1 = argv[1];
	filename2 = argv[2];

       
	DrawingMacro(filename1,filename2);
}

void DrawingMacro(string name1, string name2){
	TH1::SetDefaultSumw2();

	TFile * input_rootfile1 = new TFile(name1.c_str(),"READ");
	cout << "Inputfile1 size = " << input_rootfile1->GetSize() << endl;
	TTree * T1 = (TTree*)input_rootfile1->Get("Tree_MCP");
	TFile * input_rootfile2 = new TFile(name2.c_str(),"READ");
	cout << "Inputfile2 size = " << input_rootfile2->GetSize() << endl;
	TTree * T2 = (TTree*)input_rootfile2->Get("Tree_MCP");

	cout << "Accessed TTrees.." << endl;

	stringstream output_filename;
	output_filename << "histograms_" << name1;
	TFile * output_rootfile = new TFile(output_filename.str().c_str(),"RECREATE");

	TH1F * EnergyHisto_DESY_II_beam = new TH1F("E_DESY_II_beam","Energy distribution of electrons in the DESY II beam bunch (here 10^{7} instead of 10^{10} e^{-})",200,6.0,6.4);
	EnergyHisto_DESY_II_beam->SetLineColor(4);
	EnergyHisto_DESY_II_beam->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_DESY_II_beam->GetXaxis()->CenterTitle();

	TH2F * beam_shape = new TH2F("beam_shape","DESY II beam shape",1000,-4,4,1000,-4,4);
	beam_shape->GetXaxis()->SetTitle("x (mm)");
	beam_shape->GetXaxis()->CenterTitle();
	beam_shape->GetYaxis()->SetTitle("y (mm)");
	beam_shape->GetYaxis()->CenterTitle();

	TCanvas * Position_Canvas = new TCanvas("beam_Position_Canvas");
	TCanvas * Energy_Canvas = new TCanvas("beam_Energy_Canvas");

	Energy_Canvas->cd();
	T1->Draw("Energy >>+ E_DESY_II_beam","Particle_ID==11");	
	T2->Draw("Energy >>+ E_DESY_II_beam","Particle_ID==11");	
	TF1 *gausfit = new TF1("gausfit","gaus");
	EnergyHisto_DESY_II_beam->Fit(gausfit,"U");

	string canvasname=Energy_Canvas->GetName();
	Energy_Canvas->Write();
	Energy_Canvas->Print((canvasname+name1+".eps").c_str());
	Energy_Canvas->Print((canvasname+name1+".C").c_str());
	Energy_Canvas->Close();


	Position_Canvas->cd();
	TGaxis::SetMaxDigits(2);
	T1->Draw("Reflectiony:Reflectionx >>+ beam_shape","Particle_ID==11","colz");	
	T2->Draw("Reflectiony:Reflectionx >>+ beam_shape","Particle_ID==11","colz");	
	gStyle->SetNumberContours(99);
	gPad->SetLogz();
	gROOT->ForceStyle();

	string canvasname2=Position_Canvas->GetName();
	Position_Canvas->Write();
	Position_Canvas->Print((canvasname2+name1+".eps").c_str());
	Position_Canvas->Print((canvasname2+name1+".C").c_str());
	Position_Canvas->Close();

	output_rootfile->Write();
	input_rootfile1->Close();
	input_rootfile2->Close();

	delete input_rootfile1, input_rootfile2, output_rootfile;
	delete Position_Canvas, Energy_Canvas;
}

