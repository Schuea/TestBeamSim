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

void DrawingMacro(vector< string > names); 

int main(int argc,char *argv[]){
	if(argc < 21 || argc > 21){
    		//explain how to use program	
    		cerr << "Type in root filenames!" << endl;
    		cerr << "e.g. ./DrawHistograms file1.root file2.root" << endl;
    		terminate();
	}	
	vector < string > names;
	for(int i = 1; i < 21; ++i){
		names.push_back(argv[i]);
	}
	       
	DrawingMacro(names);
}

void DrawingMacro(vector <string> names){
	TH1::SetDefaultSumw2();
	vector < TTree * > vectree;
	vector < TFile * > filevec;
	cout << "size = " << names.size() << endl;
	for(int i = 0; i < names.size(); ++i){
		cout << "i = " << i << endl;
		TFile * input_rootfile = new TFile(names.at(i).c_str(),"READ");
		filevec.push_back(input_rootfile);
		cout << "Inputfile " << i << " size = " << input_rootfile->GetSize() << endl;
		TTree * T1 = (TTree*)input_rootfile->Get("Tree_MCP");
		vectree.push_back(T1);
		cout << "Accessed TTree.." << endl;
	}

cout<<__LINE__<< " TTrees registered and stored in vectree. Rootfiles stored in filevec." <<endl;

	stringstream output_filename;
	output_filename << "histograms_DESY_II_beam";
	TFile * output_rootfile = new TFile(output_filename.str().c_str(),"RECREATE");

	TH1D * EnergyHisto_DESY_II_beam = new TH1D("E_DESY_II_beam","Energy distribution of electrons in the DESY-II beam bunch",200,6.2,6.4);
	EnergyHisto_DESY_II_beam->SetLineColor(4);
	EnergyHisto_DESY_II_beam->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_DESY_II_beam->GetXaxis()->CenterTitle();

	TH2D * beam_shape = new TH2D("beam_shape","DESY-II beam shape",1100,-10,10,1100,-10,10);
	beam_shape->GetXaxis()->SetTitle("x (mm)");
	beam_shape->GetXaxis()->CenterTitle();
	beam_shape->GetYaxis()->SetTitle("y (mm)");
	beam_shape->GetYaxis()->CenterTitle();

	TCanvas * Position_Canvas = new TCanvas("beam_Position_Canvas_better");
	TCanvas * Energy_Canvas = new TCanvas("beam_Energy_Canvas_better");

	Energy_Canvas->cd();
	vectree.at(0)->Draw("Energy >>+ E_DESY_II_beam","Particle_ID==11");
	for(int i = 1; i < vectree.size(); ++i){
		vectree.at(i)->Draw("Energy >>+ E_DESY_II_beam","Particle_ID==11","SAME");
	}
cout<<__LINE__<< " Energy filled into histogramm from all TTrees." << endl;
	TF1 *gausfit = new TF1("gausfit","gaus");
	EnergyHisto_DESY_II_beam->Fit(gausfit,"U");
cout<<__LINE__<< " Fit applied to energy histogramm." << endl;

	string canvasname=Energy_Canvas->GetName();
	Energy_Canvas->Write();
	Energy_Canvas->Print((canvasname+".eps").c_str());
	Energy_Canvas->Print((canvasname+".C").c_str());
	Energy_Canvas->Close();
cout<<__LINE__<< " Energy canvas written, printed and closed." << endl;


	Position_Canvas->cd();
	TGaxis::SetMaxDigits(2);
	vectree.at(0)->Draw("Reflectiony:Reflectionx >>+ beam_shape","Particle_ID==11","colz");
	for(int i = 1; i < vectree.size(); ++i){
		vectree.at(i)->Draw("Reflectiony:Reflectionx >>+ beam_shape","Particle_ID==11","colz,SAME");
	}
cout<<__LINE__<< " y over x plotted in position plot for all TTrees." << endl;
	gStyle->SetNumberContours(99);
	gPad->SetLogz();
	gROOT->ForceStyle();
cout<<__LINE__<< " Logarithmic colour z-axis." << endl;

	string canvasname2=Position_Canvas->GetName();
	Position_Canvas->Write();
	Position_Canvas->Print((canvasname2+".eps").c_str());
	Position_Canvas->Print((canvasname2+".C").c_str());
	Position_Canvas->Close();
cout<<__LINE__<< " Position canvas written, printed and closed." << endl;

	output_rootfile->Write();
cout<<__LINE__<< " Output rootfile written." << endl;
	for(int i = 0; i < filevec.size(); ++i){
		filevec.at(i)->Close();
		delete filevec.at(i);
	}
	delete output_rootfile;
	delete Position_Canvas, Energy_Canvas;
}

