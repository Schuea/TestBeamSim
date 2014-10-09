#include "TFile.h"
#include "TBrowser.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1.h"
#include "TF1.h"
#include "TCutG.h"
#include "TH2.h"
#include "TStyle.h"
#include "TPaveText.h"
#include "TLatex.h"
#include "TEllipse.h"
#include "TPaveStats.h"
#include "TProfile.h"
#include "TGraph.h"
#include "TMath.h"
#include "TROOT.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>

using namespace std;

void DrawingMacro(string name, int Particle_ID1, int Particle_ID2, int Particle_ID3, int Particle_ID4, int Particle_ID5, int Particle_ID6); 

int main(int argc,char *argv[]){
	if(argc < 3 || argc > 8){
    		//explain how to use program	
    		if(argc<3) cerr << "More arguments needed: name of rootfile AND up to 6 option(s) which particles are to be drawn!" << endl;
    		if(argc>8) cerr << "Too many arguments: name of rootfile AND up to 6 option(s) which particles are to be drawn!" << endl;
    		cerr << "Possible options for particles: PDG particle ID code(s) OR 'all'" << endl;
    		cerr << "e.g. ./FluxMap file.root 11" << endl;
    		cerr << "e.g. ./FluxMap file.root 11 -11 22" << endl;
    		cerr << "e.g. ./FluxMap file.root all" << endl;
    		exit(1);
	}	
	string filename;
	filename = argv[1];

	string Particle_ID_string1, Particle_ID_string2, Particle_ID_string3, Particle_ID_string4, Particle_ID_string5, Particle_ID_string6;
	if (argc >= 3) Particle_ID_string1 = argv[2];
	if (argc >= 4) Particle_ID_string2 = argv[3];
	if (argc >= 5) Particle_ID_string3 = argv[4];
	if (argc >= 6) Particle_ID_string4 = argv[5];
	if (argc >= 7) Particle_ID_string5 = argv[6];
	if (argc = 8)  Particle_ID_string6 = argv[7];

	int Particle_ID1=0;
	int Particle_ID2=0;
	int Particle_ID3=0;
	int Particle_ID4=0;
	int Particle_ID5=0;
	int Particle_ID6=0;
       
	if (argc >= 3 && Particle_ID_string1 !=  "all"){
		       Particle_ID1= atoi(Particle_ID_string1.c_str());	
	}
	if (argc >= 4) Particle_ID2= atoi(Particle_ID_string2.c_str());	
	if (argc >= 5) Particle_ID3= atoi(Particle_ID_string3.c_str());	
	if (argc >= 6) Particle_ID4= atoi(Particle_ID_string4.c_str());	
	if (argc >= 7) Particle_ID5= atoi(Particle_ID_string5.c_str());	
	if (argc = 8)  Particle_ID6= atoi(Particle_ID_string6.c_str());	

	else if(Particle_ID1 == 0 && (Particle_ID2 != 0 || Particle_ID3 != 0 || Particle_ID4 != 0 || Particle_ID5 != 0 || Particle_ID6 != 0)){
		cerr << "Wrong options!" << endl;
		cerr << "If all particles wanted, type 'all' as ONLY draw option!" << endl;
		exit(1);
	}
	else {
		cerr << "Unkown error... Try again and observe the possible drawing options!" << endl;
		exit(1);
	}
	
	cout << "Your choice of particles:" << endl;
	if(Particle_ID1!=0) {
				    cout << "Particle_ID1 = "<< Particle_ID1 <<endl;
		if(Particle_ID2!=0) cout << "Particle_ID2 = "<< Particle_ID2 <<endl;
		if(Particle_ID3!=0) cout << "Particle_ID3 = "<< Particle_ID3 <<endl;
		if(Particle_ID4!=0) cout << "Particle_ID4 = "<< Particle_ID4 <<endl;
		if(Particle_ID5!=0) cout << "Particle_ID5 = "<< Particle_ID5 <<endl;
		if(Particle_ID6!=0) cout << "Particle_ID6 = "<< Particle_ID6 <<endl;
	}
	else cout << "All particles occurring\n" << endl; 
	
	DrawingMacro(filename,Particle_ID1, Particle_ID2, Particle_ID3, Particle_ID4, Particle_ID5, Particle_ID6);
}

std::pair<float,float> Particle_vector(int step_n, TTree * Tree, int Particle_number, int Particle_ID1, int Particle_ID2, int Particle_ID3, int Particle_ID4, int Particle_ID5, int Particle_ID6, int id, float start, float end, float z_start, float z_end){
	
        if(Particle_ID1 != 0 && id!=Particle_ID1 && id!=Particle_ID2 && id!=Particle_ID3 && id!=Particle_ID4 && id!=Particle_ID5 && id!=Particle_ID6) throw string("Not a particle we are interested in");    

	float t_n = 0;
	float v_zn = 0, v_xn = 0;

	t_n = step_n / sqrt( pow((z_end-z_start),2.0) + pow((end-start),2.0) );
	v_zn = z_start + t_n*(z_end-z_start);
	v_xn = start + t_n*(end-start);

	std::pair<float,float> result(v_zn,v_xn);
	return result;
}

void DrawingMacro(string name, int Particle_ID1, int Particle_ID2, int Particle_ID3, int Particle_ID4, int Particle_ID5, int Particle_ID6){

	TFile * input_rootfile = new TFile(name.c_str(),"READ");
	cout << "Inputfile size = " << input_rootfile->GetSize() << endl;
	TTree * Tree = (TTree*)input_rootfile->Get("Tree");
	cout << "Accessed TTree.." << endl;

	stringstream output_filename;
	output_filename << "fluxmap_" << name;
	TFile * output_rootfile = new TFile(output_filename.str().c_str(),"RECREATE");

	const int TB_line_length = 24900; //length of the TB line (in mm) is also the number of x-bins -> resolution 1 mm

	TH2F * FluxMap_xz = new TH2F("FluxMap_xz","Flux map x vs. z of particles along the TB line",TB_line_length,0,TB_line_length,3000,-1500,1500);
	FluxMap_xz->GetXaxis()->SetTitle("z (mm)");
	FluxMap_xz->GetXaxis()->CenterTitle();
	FluxMap_xz->GetYaxis()->SetTitle("x (mm)");
	FluxMap_xz->GetYaxis()->CenterTitle();

	TH2F * FluxMap_yz = new TH2F("FluxMap_yz","Flux map y vs. z of particles along the TB line",TB_line_length,0,TB_line_length,3000,-1500,1500);
	FluxMap_yz->GetXaxis()->SetTitle("z (mm)");
	FluxMap_yz->GetXaxis()->CenterTitle();
	FluxMap_yz->GetYaxis()->SetTitle("y (mm)");
	FluxMap_yz->GetYaxis()->CenterTitle();


	TCanvas* fluxmap_xz_Canvas = new TCanvas("FluxMap_xz_Canvas");
	TCanvas* fluxmap_yz_Canvas = new TCanvas("FluxMap_yz_Canvas");

	int xz_prev_stored_binno = 0;
	int yz_prev_stored_binno = 0;
        int entries = Tree->GetEntries(); 

 	int   id = 0; 
        float x_start=0,y_start=0,z_start = 0; 
	float x_end=0,y_end=0,z_end = 0; 

	Tree->SetBranchAddress("Particle_ID",&id); 
	Tree->SetBranchAddress("Vertexx",&x_start); 
	Tree->SetBranchAddress("Reflectionx",&x_end); 
	Tree->SetBranchAddress("Vertexy",&y_start); 
	Tree->SetBranchAddress("Reflectiony",&y_end); 
	Tree->SetBranchAddress("Vertexz",&z_start);
	Tree->SetBranchAddress("Reflectionz",&z_end);       

	for(int Particle_number = 0; Particle_number < 10000/*entries*/; ++Particle_number){ 
		Tree->GetEntry(Particle_number); 

		for(int step_n = 1; step_n <= TB_line_length; ++step_n){
			std::pair<float,float> xz_vector_point_n;
			std::pair<float,float> yz_vector_point_n;

			try{
				xz_vector_point_n = Particle_vector(step_n, Tree, Particle_number, Particle_ID1, Particle_ID2, Particle_ID3, Particle_ID4, Particle_ID5, Particle_ID6, id, x_start, x_end, z_start, z_end); 
				yz_vector_point_n = Particle_vector(step_n, Tree, Particle_number, Particle_ID1, Particle_ID2, Particle_ID3, Particle_ID4, Particle_ID5, Particle_ID6, id, y_start, y_end, z_start, z_end); 
			} 
			catch(string error){
				break;
			} 

			float xz_x_n = 0;
			float xz_y_n = 0;
			xz_x_n = xz_vector_point_n.first;
			xz_y_n = xz_vector_point_n.second;

			float yz_x_n = 0;
			float yz_y_n = 0;
			yz_x_n = yz_vector_point_n.first;
			yz_y_n = yz_vector_point_n.second;
		
			int xz_bin_number = 0;
			int yz_bin_number = 0;
			xz_bin_number = FluxMap_xz->FindBin(xz_x_n,xz_y_n,0);
			yz_bin_number = FluxMap_yz->FindBin(yz_x_n,yz_y_n,0);

			if(xz_bin_number != xz_prev_stored_binno) FluxMap_xz->Fill(xz_x_n,xz_y_n);
			if(yz_bin_number != yz_prev_stored_binno) FluxMap_yz->Fill(yz_x_n,yz_y_n);
	
			xz_prev_stored_binno = xz_bin_number;
			yz_prev_stored_binno = yz_bin_number;
		}
		
	}

	fluxmap_xz_Canvas->cd();
	
	gROOT->ForceStyle();

	gStyle->SetOptStat(11);
	gPad->SetLogz();
	FluxMap_xz->SetContour(999);
	
	FluxMap_xz->Draw("colz");

	string canvasname=fluxmap_xz_Canvas->GetName();
	fluxmap_xz_Canvas->Write();
	fluxmap_xz_Canvas->Print((canvasname+".eps").c_str());
	fluxmap_xz_Canvas->Print((canvasname+".pdf").c_str());
	fluxmap_xz_Canvas->Print((canvasname+".C").c_str());
	fluxmap_xz_Canvas->Close();

	fluxmap_yz_Canvas->cd();
	gStyle->SetOptStat(11);
	gPad->SetLogz();
	FluxMap_yz->SetContour(999);
	
	FluxMap_yz->Draw("colz");

	string canvasname2=fluxmap_yz_Canvas->GetName();
	fluxmap_yz_Canvas->Write();
	fluxmap_yz_Canvas->Print((canvasname2+".eps").c_str());
	fluxmap_yz_Canvas->Print((canvasname2+".pdf").c_str());
	fluxmap_yz_Canvas->Print((canvasname2+".C").c_str());
	fluxmap_yz_Canvas->Close();

	output_rootfile->Write();
	input_rootfile->Close();

	delete output_rootfile, input_rootfile;
	delete fluxmap_xz_Canvas;
	delete fluxmap_yz_Canvas;
}
