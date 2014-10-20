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

#define _USE_MATH_DEFINES
#include <cmath>  
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>

using namespace std;

void DrawingMacro(string name1, string name2, string IDs, int Particle_ID1, int Particle_ID2, int Particle_ID3, int Particle_ID4, int Particle_ID5, int Particle_ID6); 

int main(int argc,char *argv[]){
	if(argc < 3 || argc > 9){
    		//explain how to use program	
    		if(argc<3) cerr << "More arguments needed: name of BOTH rootfiles (geometry before magnet and magnet) AND up to 6 option(s) which particles are to be drawn!" << endl;
    		if(argc>9) cerr << "Too many arguments: name of BOTH rootfiles (geometry before magnet and magnet) AND up to 6 option(s) which particles are to be drawn!" << endl;
    		cerr << "Possible options for particles: PDG particle ID code(s) OR 'all'" << endl;
    		cerr << "e.g. ./FluxMap file1.root file2.root 11" << endl;
    		cerr << "e.g. ./FluxMap file1.root file2.root 11 -11 22" << endl;
    		cerr << "e.g. ./FluxMap file1.root file2.root all" << endl;
    		exit(1);
	}	
	string Particle_ID_string1, Particle_ID_string2, Particle_ID_string3, Particle_ID_string4, Particle_ID_string5, Particle_ID_string6;
	if (argc >= 4) Particle_ID_string1 = argv[3];
	if (argc >= 5) Particle_ID_string2 = argv[4];
	if (argc >= 6) Particle_ID_string3 = argv[5];
	if (argc >= 7) Particle_ID_string4 = argv[6];
	if (argc >= 8) Particle_ID_string5 = argv[7];
	if (argc = 9)  Particle_ID_string6 = argv[8];

	string filename1, filename2;
	filename1 = argv[1];
	filename2 = argv[2];
	stringstream IDs;

	int Particle_ID1=0;
	int Particle_ID2=0;
	int Particle_ID3=0;
	int Particle_ID4=0;
	int Particle_ID5=0;
	int Particle_ID6=0;
     
	if ( Particle_ID_string1 !=  "all"){
		if (argc >= 4) Particle_ID1= atoi(Particle_ID_string1.c_str());	
		if (argc >= 5) Particle_ID2= atoi(Particle_ID_string2.c_str());	
		if (argc >= 6) Particle_ID3= atoi(Particle_ID_string3.c_str());	
		if (argc >= 7) Particle_ID4= atoi(Particle_ID_string4.c_str());	
		if (argc >= 8) Particle_ID5= atoi(Particle_ID_string5.c_str());	
		if (argc  = 9) Particle_ID6= atoi(Particle_ID_string6.c_str());	
	
		if(Particle_ID1!=0) {
			cout << "Your choice of particles:" << endl;
				    cout << "Particle_ID1 = "<< Particle_ID1 <<endl;
		if(Particle_ID2!=0) cout << "Particle_ID2 = "<< Particle_ID2 <<endl;
		if(Particle_ID3!=0) cout << "Particle_ID3 = "<< Particle_ID3 <<endl;
		if(Particle_ID4!=0) cout << "Particle_ID4 = "<< Particle_ID4 <<endl;
		if(Particle_ID5!=0) cout << "Particle_ID5 = "<< Particle_ID5 <<endl;
		if(Particle_ID6!=0) cout << "Particle_ID6 = "<< Particle_ID6 <<endl;

		IDs << "_" << Particle_ID1 << "_" << Particle_ID2 << "_" << Particle_ID3 << "_" << Particle_ID4 << "_" << Particle_ID5 << "_" << Particle_ID6;
		}
	
		else if(Particle_ID1 == 0 && (Particle_ID2 != 0 || Particle_ID3 != 0 || Particle_ID4 != 0 || Particle_ID5 != 0 || Particle_ID6 != 0)){
			cerr << "Wrong options!" << endl;
			cerr << "If all particles wanted, type 'all' as ONLY draw option!" << endl;
			exit(1);
		}
		
		else {
			cerr << "Error... Try again and observe the possible drawing options!" << endl;
			cerr << "Possible options for particles: PDG particle ID code(s) OR 'all'" << endl;
    			cerr << "e.g. ./FluxMap file1.root file2.root 11" << endl;
    			cerr << "e.g. ./FluxMap file1.root file2.root 11 -11 22" << endl;
    			cerr << "e.g. ./FluxMap file1.root file2.root all" << endl;
			exit(1);
		}
	}
	else if(Particle_ID_string1=="all"){
		cout << "Your choice of particles:" << endl;
		cout << "All particles occurring\n" << endl; 
		IDs << "_all";
	}	
	else {
		cerr << "Unkown error... Try again and observe the possible drawing options!" << endl;
		cerr << "Possible options for particles: PDG particle ID code(s) OR 'all'" << endl;
    		cerr << "e.g. ./FluxMap file1.root file2.root 11" << endl;
    		cerr << "e.g. ./FluxMap file1.root file2.root 11 -11 22" << endl;
    		cerr << "e.g. ./FluxMap file1.root file2.root all" << endl;
		exit(1);
	}
	cout << IDs.str() <<endl;

	DrawingMacro(filename1,filename2,IDs.str(),Particle_ID1, Particle_ID2, Particle_ID3, Particle_ID4, Particle_ID5, Particle_ID6);
}

std::pair<float,float> Particle_vector(float step_n, TTree * Tree, int Particle_number, int Particle_ID1, int Particle_ID2, int Particle_ID3, int Particle_ID4, int Particle_ID5, int Particle_ID6, int id, float start, float end, float z_start, float z_end){
	
        if(Particle_ID1 != 0 && id!=Particle_ID1 && id!=Particle_ID2 && id!=Particle_ID3 && id!=Particle_ID4 && id!=Particle_ID5 && id!=Particle_ID6) throw string("Not a particle we are interested in");    

	float t_n = 0;
	float v_zn = 0, v_xn = 0;

	t_n = step_n / sqrt( pow((z_end-z_start),2.0) + pow((end-start),2.0) );
	v_zn = z_start + t_n*(z_end-z_start);
	v_xn = start + t_n*(end-start);
/*
cout << "Particle_vector" <<endl;
cout << "v_zn = " << v_zn <<endl; 
cout << "v_xn = " << v_xn <<endl; 
*/
	std::pair<float,float> result(v_zn,v_xn);
	return result;
}

std::pair<float,float> Circular_path(float step, TTree * Tree, int Particle_number, int Particle_ID1, int Particle_ID2, int Particle_ID3, int Particle_ID4, int Particle_ID5, int Particle_ID6, int id, float z_start, float start, float energy, float charge, float Magnetic_field_strength){
	
        if(Particle_ID1 != 0 && id!=Particle_ID1 && id!=Particle_ID2 && id!=Particle_ID3 && id!=Particle_ID4 && id!=Particle_ID5 && id!=Particle_ID6) throw string("Not a particle we are interested in");    
	
	float radius = 0;
	radius = energy/(0.3*abs(charge)*Magnetic_field_strength);

	float sign = 0.0;
	if(charge<0) sign = -1.0;
	if(charge>0) sign =  1.0;

	float phi_n = 0;
	float v_zn = 0, v_xn = 0;

	phi_n = sign*step;
	v_zn = z_start + radius*cos(phi_n);
	v_xn = start + radius*sin(phi_n);

//cout << "Circular_path" <<endl;
//cout << "sign = " << sign <<endl; 
//cout << "radius = " << radius <<endl; 
//cout << "cos("<< phi_n << ") = " <<  cos(phi_n)<< endl; 
//cout << "sin("<< phi_n << ") = " <<  sin(phi_n)<< endl; 
//cout << "z_start = " << z_start <<endl; 
//cout << "start = " << start <<endl; 
//cout << "v_zn = " << v_zn <<endl; 
//cout << "v_xn = " << v_xn <<endl; 

	std::pair<float,float> result(v_zn,v_xn);
	return result;
}

void DrawingMacro(string name1, string name2, string IDs, int Particle_ID1, int Particle_ID2, int Particle_ID3, int Particle_ID4, int Particle_ID5, int Particle_ID6){

	TFile * input_rootfile1 = new TFile(name1.c_str(),"READ");
	cout << "Inputfile 1 size = " << input_rootfile1->GetSize() << endl;
	TTree * Tree1 = (TTree*)input_rootfile1->Get("Tree");
	
	TFile * input_rootfile2 = new TFile(name2.c_str(),"READ");
	cout << "Inputfile 2 size = " << input_rootfile2->GetSize() << endl;
	TTree * Tree2 = (TTree*)input_rootfile2->Get("Tree");

	cout << "Accessed TTrees.." << endl;

	stringstream output_filename;
	output_filename << "fluxmap_withMagneticField" << IDs << ".root";
	TFile * output_rootfile = new TFile(output_filename.str().c_str(),"RECREATE");

	const float Total_TB_line_length = 24727.5; //length of the TB line (in mm) is also the number of x-bins -> resolution 1 mm
	const float TB_line_length_till_magnet = 23672.5; //length of the TB line (in mm) till the magnet is also the number of x-bins -> resolution 1 mm
	const float x_start_magnetic_field = -327.5;
	const float x_end_magnetic_field = 327.5;
	const float y_start_magnetic_field = -185;
	const float y_end_magnetic_field = 185;
	const float z_start_magnetic_field = 7.5;// + 23672.5
	const float z_end_magnetic_field = 104.75;// + 23672.5

	const float B = 0.5; 

	TH2F * FluxMap_xz = new TH2F("FluxMap_xz","Flux map x vs. z of particles along the TB line",Total_TB_line_length,0,Total_TB_line_length,3000,-1500,1500);
	FluxMap_xz->GetXaxis()->SetTitle("z (mm)");
	FluxMap_xz->GetXaxis()->CenterTitle();
	FluxMap_xz->GetYaxis()->SetTitle("x (mm)");
	FluxMap_xz->GetYaxis()->CenterTitle();

	TH2F * FluxMap_yz = new TH2F("FluxMap_yz","Flux map y vs. z of particles along the TB line",Total_TB_line_length,0,Total_TB_line_length,3000,-1500,1500);
	FluxMap_yz->GetXaxis()->SetTitle("z (mm)");
	FluxMap_yz->GetXaxis()->CenterTitle();
	FluxMap_yz->GetYaxis()->SetTitle("y (mm)");
	FluxMap_yz->GetYaxis()->CenterTitle();


	TCanvas* fluxmap_xz_Canvas = new TCanvas("FluxMap_withMagneticField_xz_Canvas");
	TCanvas* fluxmap_yz_Canvas = new TCanvas("FluxMap_withMagneticField_yz_Canvas");

 	int   id1 = 0, id2 = 0;
	float energy2 = 0, charge2 = 0; 
        float x_vertex1 = 0, y_vertex1 = 0, z_vertex1 = 0, x_vertex2 = 0, y_vertex2 = 0, z_vertex2 = 0; 
	float x_end1 = 0, y_end1 = 0, z_end1 = 0, x_end2 = 0, y_end2 = 0, z_end2 = 0; 

	Tree1->SetBranchAddress("Particle_ID",&id1); 
	Tree1->SetBranchAddress("Vertexx",&x_vertex1); 
	Tree1->SetBranchAddress("Reflectionx",&x_end1); 
	Tree1->SetBranchAddress("Vertexy",&y_vertex1); 
	Tree1->SetBranchAddress("Reflectiony",&y_end1); 
	Tree1->SetBranchAddress("Vertexz",&z_vertex1);
	Tree1->SetBranchAddress("Reflectionz",&z_end1);       

	Tree2->SetBranchAddress("Particle_ID",&id2); 
	Tree2->SetBranchAddress("Energy",&energy2); 
	Tree2->SetBranchAddress("Charge",&charge2); 
	Tree2->SetBranchAddress("Vertexx",&x_vertex2); 
	Tree2->SetBranchAddress("Reflectionx",&x_end2); 
	Tree2->SetBranchAddress("Vertexy",&y_vertex2); 
	Tree2->SetBranchAddress("Reflectiony",&y_end2); 
	Tree2->SetBranchAddress("Vertexz",&z_vertex2);
	Tree2->SetBranchAddress("Reflectionz",&z_end2);       

        int entries1 = Tree1->GetEntries(); 
        int entries2 = Tree2->GetEntries(); 
	
	enum{
		OutsideVertex,
		InsideVertex,
		OutsideVertexEntering,
		MagnetInside,
		MagnetLeaving
	};

	int WhichCase = -1;

	bool CircleCompleted(false);

	for(int Particle_number = 0; Particle_number < 10000/*entries1*/; ++Particle_number){ 
		Tree1->GetEntry(Particle_number); 

		int xz_prev_stored_binno = 0;
		int yz_prev_stored_binno = 0;

		for(float step_n = 1; step_n <= TB_line_length_till_magnet; step_n+=1){
			std::pair<float,float> xz_vector_point_n;
			std::pair<float,float> yz_vector_point_n;
	
			float xz_x_n = 0;
			float xz_y_n = 0;
			float yz_x_n = 0;
			float yz_y_n = 0;

			try{
				if (xz_x_n >= 23672.5) break;
				xz_vector_point_n = Particle_vector(step_n, 
								    Tree1, 
								    Particle_number, 
								    Particle_ID1, 
								    Particle_ID2, 
								    Particle_ID3, 
								    Particle_ID4, 
								    Particle_ID5, 
								    Particle_ID6, 
								    id1, 
								    x_vertex1, 
								    x_end1, 
								    z_vertex1, 
								    z_end1); 
				xz_x_n = xz_vector_point_n.first;
				xz_y_n = xz_vector_point_n.second;
			} 
			catch(string error){
				break;
			} 

		
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


	for(int Particle_number = 0; Particle_number < 10000/*entries2*/; ++Particle_number){ 
		Tree2->GetEntry(Particle_number); 

		float xz_start_vector_x = 0;
		float xz_start_vector_y = 0;
		float yz_start_vector_x = 0;
		float yz_start_vector_y = 0;

		float xz_start_circle_x = 0;
		float xz_start_circle_y = 0;
		float yz_start_circle_x = 0;
		float yz_start_circle_y = 0;

		int xz_prev_stored_binno = 0;
		int yz_prev_stored_binno = 0;

		float circle_step_x = 0.01;
		float circle_step_y = 0.01;

		if((z_vertex2 < z_start_magnetic_field || z_vertex2 > z_end_magnetic_field)
		|| (!(z_vertex2 < z_start_magnetic_field || z_vertex2 > z_end_magnetic_field)
		&& x_vertex2 < x_start_magnetic_field || x_vertex2 > x_end_magnetic_field)){
			WhichCase = OutsideVertex;
		}
		else if(x_vertex2 > x_start_magnetic_field 
		&& x_vertex2 < x_end_magnetic_field 
		&& z_vertex2 > z_start_magnetic_field 
		&& z_vertex2 < z_end_magnetic_field){
			WhichCase = InsideVertex;
		}



		for(float step_n = 1; step_n <= Total_TB_line_length - TB_line_length_till_magnet; step_n+=1){
			std::pair<float,float> xz_vector_point_n;
			std::pair<float,float> yz_vector_point_n;
	
			float xz_x_n = 0;
			float xz_y_n = 0;
			float yz_x_n = 0;
			float yz_y_n = 0;

			try{
				switch(WhichCase){
					case OutsideVertex:
						xz_vector_point_n = Particle_vector(step_n, 
										    Tree2, 
										    Particle_number, 
										    Particle_ID1, 
										    Particle_ID2, 
										    Particle_ID3, 
										    Particle_ID4, 
										    Particle_ID5, 
										    Particle_ID6, 
										    id2, 
										    x_vertex2, 
										    x_end2, 
										    z_vertex2, 
										    z_end2); 
						xz_x_n = xz_vector_point_n.first;
						xz_y_n = xz_vector_point_n.second;
						if(xz_y_n > x_start_magnetic_field 
								&& xz_y_n < x_end_magnetic_field 
								&& xz_x_n > z_start_magnetic_field 
								&& xz_x_n < z_end_magnetic_field){
							WhichCase = OutsideVertexEntering;
						}
						break;
				
					case OutsideVertexEntering:
						xz_start_circle_x = xz_x_n;
						xz_start_circle_y = xz_y_n;
						WhichCase = MagnetInside;
						break;
				
					case InsideVertex:
						xz_start_circle_x = z_vertex2;
						xz_start_circle_y = x_vertex2;
						WhichCase = MagnetInside;
						break;
				
					case MagnetInside:
						if(circle_step_x>=2*M_PI){
							CircleCompleted = true;
							break;
						}
						xz_vector_point_n = Circular_path(circle_step_x, 
								Tree2, 
								Particle_number, 
								Particle_ID1, 
								Particle_ID2, 
								Particle_ID3,
								Particle_ID4, 
								Particle_ID5, 
								Particle_ID6, 
								id2, 
								xz_start_circle_x, 
								xz_start_circle_y, 
								energy2, 
								charge2, 
								B); 
						xz_x_n = xz_vector_point_n.first;
						xz_y_n = xz_vector_point_n.second;
			
						circle_step_x+=0.01;
			
						if((xz_x_n < z_start_magnetic_field || xz_x_n > z_end_magnetic_field)
					     || (!(xz_x_n < z_start_magnetic_field || xz_x_n > z_end_magnetic_field)
					     &&    xz_y_n < x_start_magnetic_field || xz_y_n > x_end_magnetic_field)){
							xz_start_vector_x = xz_x_n;
							xz_start_vector_y = xz_y_n;
							WhichCase = MagnetLeaving;
					     	}
						break;
				
					case MagnetLeaving:
						xz_vector_point_n = Particle_vector(step_n, 
										    Tree2, 
										    Particle_number, 
										    Particle_ID1, 
										    Particle_ID2, 
										    Particle_ID3, 
										    Particle_ID4, 
										    Particle_ID5, 
										    Particle_ID6, 
										    id2, 
										    xz_start_vector_y, 
										    x_end2, 
										    xz_start_vector_x, 
										    z_end2); 
						xz_x_n = xz_vector_point_n.first;
						xz_y_n = xz_vector_point_n.second;
						break;
					default:
				
					cerr << "Scenario did not match anything we envisioned..." << endl;
						break;
				};
				if(CircleCompleted) break;
			} 
			catch(string error){
				break;
			} 

		
			int xz_bin_number = 0;
			int yz_bin_number = 0;
			xz_bin_number = FluxMap_xz->FindBin(xz_x_n + 23672.5,xz_y_n,0);
			yz_bin_number = FluxMap_yz->FindBin(yz_x_n + 23672.5,yz_y_n,0);

			if(xz_bin_number != xz_prev_stored_binno) FluxMap_xz->Fill(xz_x_n + 23672.5,xz_y_n);
			if(yz_bin_number != yz_prev_stored_binno) FluxMap_yz->Fill(yz_x_n + 23672.5,yz_y_n);

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
	fluxmap_xz_Canvas->Print((canvasname+IDs+".eps").c_str());
	fluxmap_xz_Canvas->Print((canvasname+IDs+".pdf").c_str());
	fluxmap_xz_Canvas->Print((canvasname+IDs+".C").c_str());
	fluxmap_xz_Canvas->Close();

	fluxmap_yz_Canvas->cd();
	gStyle->SetOptStat(11);
	gPad->SetLogz();
	FluxMap_yz->SetContour(999);
	
	FluxMap_yz->Draw("colz");

	string canvasname2=fluxmap_yz_Canvas->GetName();
	fluxmap_yz_Canvas->Write();
	fluxmap_yz_Canvas->Print((canvasname2+IDs+".eps").c_str());
	fluxmap_yz_Canvas->Print((canvasname2+IDs+".pdf").c_str());
	fluxmap_yz_Canvas->Print((canvasname2+IDs+".C").c_str());
	fluxmap_yz_Canvas->Close();

	output_rootfile->Write();
	input_rootfile1->Close();
	input_rootfile2->Close();

	delete output_rootfile, input_rootfile1, input_rootfile2;
	delete fluxmap_xz_Canvas;
	delete fluxmap_yz_Canvas;
}

