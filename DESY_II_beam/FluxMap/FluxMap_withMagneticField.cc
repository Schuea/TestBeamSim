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

void DrawingMacro(string name, string IDs, int Particle_ID1, int Particle_ID2, int Particle_ID3, int Particle_ID4, int Particle_ID5, int Particle_ID6); 

int main(int argc,char *argv[]){
	if(argc < 3 || argc > 8){
		//explain how to use program	
		if(argc<3) cerr << "More arguments needed: name of the rootfile AND up to 6 option(s) which particles are to be drawn!" << endl;
		if(argc>8) cerr << "Too many arguments: name of the rootfiles AND up to 6 option(s) which particles are to be drawn!" << endl;
		cerr << "Possible options for particles: PDG particle ID code(s) OR 'all'" << endl;
		cerr << "e.g. ./FluxMap file.root 11" << endl;
		cerr << "e.g. ./FluxMap file.root 11 -11 22" << endl;
		cerr << "e.g. ./FluxMap file.root all" << endl;
		exit(1);
	}	
	string Particle_ID_string1, Particle_ID_string2, Particle_ID_string3, Particle_ID_string4, Particle_ID_string5, Particle_ID_string6;
	if (argc >= 3) Particle_ID_string1 = argv[2];
	if (argc >= 4) Particle_ID_string2 = argv[3];
	if (argc >= 5) Particle_ID_string3 = argv[4];
	if (argc >= 6) Particle_ID_string4 = argv[5];
	if (argc >= 7) Particle_ID_string5 = argv[6];
	if (argc = 8)  Particle_ID_string6 = argv[7];

	string filename;
	filename = argv[1];
	stringstream IDs;

	int Particle_ID1=0;
	int Particle_ID2=0;
	int Particle_ID3=0;
	int Particle_ID4=0;
	int Particle_ID5=0;
	int Particle_ID6=0;

	if ( Particle_ID_string1 !=  "all"){
		if (argc >= 3) Particle_ID1= atoi(Particle_ID_string1.c_str());	
		if (argc >= 4) Particle_ID2= atoi(Particle_ID_string2.c_str());	
		if (argc >= 5) Particle_ID3= atoi(Particle_ID_string3.c_str());	
		if (argc >= 6) Particle_ID4= atoi(Particle_ID_string4.c_str());	
		if (argc >= 7) Particle_ID5= atoi(Particle_ID_string5.c_str());	
		if (argc  = 8) Particle_ID6= atoi(Particle_ID_string6.c_str());	

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
			cerr << "e.g. ./FluxMap file.root 11" << endl;
			cerr << "e.g. ./FluxMap file.root 11 -11 22" << endl;
			cerr << "e.g. ./FluxMap file.root all" << endl;
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
		cerr << "e.g. ./FluxMap file.root 11" << endl;
		cerr << "e.g. ./FluxMap file.root 11 -11 22" << endl;
		cerr << "e.g. ./FluxMap file.root all" << endl;
		exit(1);
	}

	DrawingMacro(filename,IDs.str(),Particle_ID1, Particle_ID2, Particle_ID3, Particle_ID4, Particle_ID5, Particle_ID6);
}

float scalar_product(float a[2], float b[2]){
		float result = 0;
		for (int d=0; d<2; d++) {
			result += a[d]*b[d];
		}
		return result;
}


float norm(float a[2]){
		float result = 0;
		float tmp=0;
		for (int d=0; d<2; d++) {
			tmp += a[d]*a[d];
		}
		result = sqrt(tmp);
		return result;
}

std::pair<float,float> Particle_vector(float step_n, TTree * Tree, int Particle_ID1, int Particle_ID2, int Particle_ID3, int Particle_ID4, int Particle_ID5, int Particle_ID6, int id, float start, float end, float z_start, float z_end){

	if(Particle_ID1 != 0 && id!=Particle_ID1 && id!=Particle_ID2 && id!=Particle_ID3 && id!=Particle_ID4 && id!=Particle_ID5 && id!=Particle_ID6) throw string("Not a particle we are interested in");    

	float t_n = 0;
	float v_zn = 0, v_xn = 0;

	t_n = step_n / sqrt( pow((z_end-z_start),2.0) + pow((end-start),2.0) );
	v_zn = z_start + t_n*(z_end-z_start);
	v_xn = start + t_n*(end-start);

	std::pair<float,float> result(v_zn,v_xn);
	return result;
}

std::pair<float,float> Circular_path(float step, TTree * Tree, int Particle_ID1, int Particle_ID2, int Particle_ID3, int Particle_ID4, int Particle_ID5, int Particle_ID6, int id, float z_start, float start, double momentum_z, double momentum, float hit_energy, float hit_charge, float Magnetic_field_strength){

	if(Particle_ID1 != 0 && id!=Particle_ID1 && id!=Particle_ID2 && id!=Particle_ID3 && id!=Particle_ID4 && id!=Particle_ID5 && id!=Particle_ID6) throw string("Not a particle we are interested in");    

	//Calculate incoming angle theta:
	float z_axis[2];
	z_axis[0] = 1.0;
	z_axis[1] = 0.0;

	float incoming_course[2];
	incoming_course[0] = float(momentum_z);
	incoming_course[1] = float(momentum);
	float length = norm(incoming_course);
	float normalized_course[2];
	normalized_course[0] = length*incoming_course[0]/abs(length);
	normalized_course[1] = length*incoming_course[1]/abs(length);

	float theta = 0.0;
	theta = acos(scalar_product(z_axis,normalized_course)/(norm(z_axis)*norm(normalized_course)));	

	//Calculating path:
	float sign = 0.0;
	if(hit_charge<0) sign = -1.0;
	if(hit_charge>0) sign =  1.0;

	float radius = 0;
	radius = hit_energy/(0.3*abs(hit_charge)*Magnetic_field_strength);

	float phi_n = 0;
	phi_n = -sign*step;

	float chord = 0;
	chord = 2*radius*sin(phi_n/2);

	float v_zn = 0, v_xn = 0;
	v_zn = z_start + chord*sin(0.5*(M_PI-phi_n));
	v_xn = start + chord*cos(0.5*(M_PI-phi_n));

	//Matrix transformation:
	float v_zn_transf = cos(theta)*v_zn - sin(theta)*v_xn;
	float v_xn_transf = sin(theta)*v_zn + cos(theta)*v_xn;

	//	v_zn = z_start + radius*cos(phi_n);
	//	v_xn = start + radius*sin(phi_n);

	//cout << "Circular_path" <<endl;
	//cout << "sign = " << sign <<endl; 
	//cout << "radius = " << radius <<endl; 
	//cout << "z_start = " << z_start <<endl; 
	//cout << "start = " << start <<endl; 
	//cout << "v_zn = " << v_zn <<endl; 
	//cout << "v_xn = " << v_xn <<endl; 

	std::pair<float,float> result(v_zn_transf,v_xn_transf);
	return result;
}

void DrawingMacro(string name, string IDs, int Particle_ID1, int Particle_ID2, int Particle_ID3, int Particle_ID4, int Particle_ID5, int Particle_ID6){

	TFile * input_rootfile = new TFile(name.c_str(),"READ");
	cout << "Inputfile  size = " << input_rootfile->GetSize() << endl;
	TTree * Tree_NoScintillator = (TTree*)input_rootfile->Get("Tree_MCP");
	TTree * Tree_BeforeMagnet = (TTree*)input_rootfile->Get("Tree_EnterMagnet");
	TTree * Tree_AfterMagnet = (TTree*)input_rootfile->Get("Tree_LeaveMagnet");

	cout << "Accessed TTrees.." << endl;

	stringstream output_filename;
	output_filename << "fluxmap_withMagneticField" << "_" << name+IDs << ".root";
	TFile * output_rootfile = new TFile(output_filename.str().c_str(),"RECREATE");

	const float Total_TB_line_length = 24728.0; //length of the TB line (in mm) is also the number of x-bins -> resolution 1 mm
	const float x_start_magnetic_field = -327.5;
	const float x_end_magnetic_field = 327.5;
	const float y_start_magnetic_field = -185;
	const float y_end_magnetic_field = 185;
	const float z_start_magnetic_field = 23680;
	const float z_end_magnetic_field = 24720;

	const float B = 0.59; 

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

	int id = 0;
	float x_vertex = 0, y_vertex = 0, z_vertex = 0, x_end = 0, y_end = 0, z_end = 0; 
	int hit_id_before = 0;
	float hit_energy_before = 0, hit_charge_before = 0, hit_posi_x_before = 0, hit_posi_y_before = 0, hit_posi_z_before = 0; 
	double hit_vertex_x_before = 0, hit_vertex_y_before = 0, hit_vertex_z_before = 0, hit_endposi_x_before = 0, hit_endposi_y_before = 0, hit_endposi_z_before = 0; 
	double hit_momentum_x_before = 0, hit_momentum_y_before = 0, hit_momentum_z_before = 0;
	int hit_id_after = 0;
	float hit_energy_after = 0, hit_charge_after = 0, hit_posi_x_after = 0, hit_posi_y_after = 0, hit_posi_z_after = 0; 
	double hit_vertex_x_after = 0, hit_vertex_y_after = 0, hit_vertex_z_after = 0, hit_endposi_x_after = 0, hit_endposi_y_after = 0, hit_endposi_z_after = 0; 

	Tree_NoScintillator->SetBranchAddress("Particle_ID",&id); 
	Tree_NoScintillator->SetBranchAddress("Vertexx",&x_vertex); 
	Tree_NoScintillator->SetBranchAddress("Reflectionx",&x_end); 
	Tree_NoScintillator->SetBranchAddress("Vertexy",&y_vertex); 
	Tree_NoScintillator->SetBranchAddress("Reflectiony",&y_end); 
	Tree_NoScintillator->SetBranchAddress("Vertexz",&z_vertex);
	Tree_NoScintillator->SetBranchAddress("Reflectionz",&z_end);       

	Tree_BeforeMagnet->SetBranchAddress("HitParticle_ID",&hit_id_before);
	Tree_BeforeMagnet->SetBranchAddress("HitCharge",&hit_charge_before); 
	Tree_BeforeMagnet->SetBranchAddress("HitEnergy",&hit_energy_before);

	Tree_BeforeMagnet->SetBranchAddress("HitPosition_x",&hit_posi_x_before);
	Tree_BeforeMagnet->SetBranchAddress("HitPosition_y",&hit_posi_y_before);
	Tree_BeforeMagnet->SetBranchAddress("HitPosition_z",&hit_posi_z_before);
	Tree_BeforeMagnet->SetBranchAddress("HitVertex_x",&hit_vertex_x_before);
	Tree_BeforeMagnet->SetBranchAddress("HitVertex_y",&hit_vertex_y_before);
	Tree_BeforeMagnet->SetBranchAddress("HitVertex_z",&hit_vertex_z_before);
	Tree_BeforeMagnet->SetBranchAddress("HitEndpoint_x",&hit_endposi_x_before);
	Tree_BeforeMagnet->SetBranchAddress("HitEndpoint_y",&hit_endposi_y_before);
	Tree_BeforeMagnet->SetBranchAddress("HitEndpoint_z",&hit_endposi_z_before);
	Tree_BeforeMagnet->SetBranchAddress("HitMomentum_x",&hit_momentum_x_before);
	Tree_BeforeMagnet->SetBranchAddress("HitMomentum_y",&hit_momentum_y_before);
	Tree_BeforeMagnet->SetBranchAddress("HitMomentum_z",&hit_momentum_z_before);

	Tree_AfterMagnet->SetBranchAddress("HitParticle_ID",&hit_id_after);
	Tree_AfterMagnet->SetBranchAddress("HitCharge",&hit_charge_after); 
	Tree_AfterMagnet->SetBranchAddress("HitEnergy",&hit_energy_after);

	Tree_AfterMagnet->SetBranchAddress("HitPosition_x",&hit_posi_x_after);
	Tree_AfterMagnet->SetBranchAddress("HitPosition_y",&hit_posi_y_after);
	Tree_AfterMagnet->SetBranchAddress("HitPosition_z",&hit_posi_z_after);
	Tree_AfterMagnet->SetBranchAddress("HitVertex_x",&hit_vertex_x_after);
	Tree_AfterMagnet->SetBranchAddress("HitVertex_y",&hit_vertex_y_after);
	Tree_AfterMagnet->SetBranchAddress("HitVertex_z",&hit_vertex_z_after);
	Tree_AfterMagnet->SetBranchAddress("HitEndpoint_x",&hit_endposi_x_after);
	Tree_AfterMagnet->SetBranchAddress("HitEndpoint_y",&hit_endposi_y_after);
	Tree_AfterMagnet->SetBranchAddress("HitEndpoint_z",&hit_endposi_z_after);

	int entries_noscintillator = Tree_NoScintillator->GetEntries(); 
	int entries_beforemagnet = Tree_BeforeMagnet->GetEntries(); 
	int entries_aftermagnet = Tree_AfterMagnet->GetEntries(); 

	std::pair<float,float> xz_vector_point_n;
	std::pair<float,float> yz_vector_point_n;


	for(int Particle_number = 0; Particle_number < 10000/*entries_noscintillator*/; ++Particle_number){ 

//		Tree_NoScintillator->GetEntry(Particle_number); 
		Tree_BeforeMagnet->GetEntry(Particle_number); 
	
		float xz_x_n = 0;
		float xz_y_n = 0;
		float yz_x_n = 0;
		float yz_y_n = 0;

		int xz_prev_stored_binno = 0;
		int yz_prev_stored_binno = 0;

		for(float step_n = 1; step_n <= Total_TB_line_length; step_n+=1){
			try{
				if(xz_y_n > x_start_magnetic_field 
						&& xz_y_n < x_end_magnetic_field 
						&& xz_x_n > z_start_magnetic_field){
					//If particle enters the magnetic field then stop
					break;
				}

				xz_vector_point_n = Particle_vector(step_n, 
					//	Tree_NoScintillator, 
					 	Tree_BeforeMagnet, 
						Particle_ID1, 
						Particle_ID2, 
						Particle_ID3, 
						Particle_ID4, 
						Particle_ID5, 
						Particle_ID6, 
					//	id, 
					//	x_vertex, 
					//	x_end, 
					//	z_vertex, 
					//	z_end); 
						hit_id_before, 
						hit_vertex_x_before, 
						hit_posi_x_before, 
						hit_vertex_z_before, 
						hit_posi_z_before); 
			
				xz_x_n = xz_vector_point_n.first;
				xz_y_n = xz_vector_point_n.second;

				int xz_bin_number = 0;
				int yz_bin_number = 0;
				xz_bin_number = FluxMap_xz->FindBin(xz_x_n,xz_y_n,0);
				yz_bin_number = FluxMap_yz->FindBin(yz_x_n,yz_y_n,0);

				if(xz_bin_number != xz_prev_stored_binno) FluxMap_xz->Fill(xz_x_n,xz_y_n);
				if(yz_bin_number != yz_prev_stored_binno) FluxMap_yz->Fill(yz_x_n,yz_y_n);

				xz_prev_stored_binno = xz_bin_number;
				yz_prev_stored_binno = yz_bin_number;
			} catch(...){}
		}

	}
	for(int Particle_number = 0; Particle_number < 10000/*entries_beforemagnet*/; ++Particle_number){ 

		Tree_BeforeMagnet->GetEntry(Particle_number); 
	
		float circle_step_x = 0.005;
		float circle_step_y = 0.005;
		
		float xz_start_circle_x = 0;
		float xz_start_circle_y = 0;
		float yz_start_circle_x = 0;
		float yz_start_circle_y = 0;

		float xz_x_n = 0;
		float xz_y_n = 0;
		float yz_x_n = 0;
		float yz_y_n = 0;
		int xz_prev_stored_binno = 0;
		int yz_prev_stored_binno = 0;

		for(float step_n = 1; step_n <= Total_TB_line_length; step_n+=1){
			if(Particle_number % 100 == 0 && int(step_n) % 1000 == 0) cout << Particle_number << "," << step_n << endl;

			try{

				if(hit_vertex_x_before > x_start_magnetic_field &&
						hit_vertex_z_before > z_start_magnetic_field &&
						hit_vertex_x_before < x_end_magnetic_field &&
						hit_vertex_z_before < z_end_magnetic_field){
					xz_start_circle_x = hit_vertex_z_before;
					xz_start_circle_y = hit_vertex_x_before;
				} else{
					xz_start_circle_x = hit_posi_z_before;
					xz_start_circle_y = hit_posi_x_before;
				}

				if(circle_step_x>=2*M_PI){
					break;
				}
				xz_vector_point_n = Circular_path(circle_step_x, 
						Tree_BeforeMagnet, 
						Particle_ID1, 
						Particle_ID2, 
						Particle_ID3,
						Particle_ID4, 
						Particle_ID5, 
						Particle_ID6, 
						hit_id_before, 
						xz_start_circle_x, 
						xz_start_circle_y,
						hit_momentum_z_before,
						hit_momentum_x_before,
						hit_energy_before, 
						hit_charge_before, 
						B); 
				xz_x_n = xz_vector_point_n.first;
				xz_y_n = xz_vector_point_n.second;

				circle_step_x+=0.005;

				if((xz_x_n < z_start_magnetic_field || xz_x_n > z_end_magnetic_field)
						|| (!(xz_x_n < z_start_magnetic_field || xz_x_n > z_end_magnetic_field)
							&&    xz_y_n < x_start_magnetic_field || xz_y_n > x_end_magnetic_field)){
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
			} catch(...){}
		}
	}
	for(int Particle_number = 0; Particle_number < 10000/*entries_aftermagnet*/; ++Particle_number){ 

		Tree_AfterMagnet->GetEntry(Particle_number); 
		float xz_x_n = 0;
		float xz_y_n = 0;
		float yz_x_n = 0;
		float yz_y_n = 0;

		int xz_prev_stored_binno = 0;
		int yz_prev_stored_binno = 0;
		for(float step_n = 1; step_n <= Total_TB_line_length; step_n+=1){
			if(Particle_number % 100 == 0 && int(step_n) % 1000 == 0) cout << Particle_number << "," << step_n << endl;

				try{

				xz_vector_point_n = Particle_vector(step_n, 
						Tree_AfterMagnet, 
						Particle_ID1, 
						Particle_ID2, 
						Particle_ID3, 
						Particle_ID4, 
						Particle_ID5, 
						Particle_ID6, 
						hit_id_after, 
						hit_posi_x_after, 
						hit_endposi_x_after, 
						hit_posi_z_after, 
						hit_endposi_z_after); 
				xz_x_n = xz_vector_point_n.first;
				xz_y_n = xz_vector_point_n.second;

				int xz_bin_number = 0;
				int yz_bin_number = 0;
				xz_bin_number = FluxMap_xz->FindBin(xz_x_n,xz_y_n,0);
				yz_bin_number = FluxMap_yz->FindBin(yz_x_n,yz_y_n,0);

				if(xz_bin_number != xz_prev_stored_binno) FluxMap_xz->Fill(xz_x_n,xz_y_n);
				if(yz_bin_number != yz_prev_stored_binno) FluxMap_yz->Fill(yz_x_n,yz_y_n);

				xz_prev_stored_binno = xz_bin_number;
				yz_prev_stored_binno = yz_bin_number;
			} catch(...){}
		}

	}
	fluxmap_xz_Canvas->cd();

	gROOT->ForceStyle();

	gStyle->SetOptStat(0);
	gPad->SetLogz();
	FluxMap_xz->SetContour(999);

	FluxMap_xz->Draw("colz");

	string canvasname=fluxmap_xz_Canvas->GetName();
	fluxmap_xz_Canvas->Write();
	fluxmap_xz_Canvas->Print((canvasname+"_"+name+IDs+".eps").c_str());
	fluxmap_xz_Canvas->Print((canvasname+"_"+name+IDs+".pdf").c_str());
	fluxmap_xz_Canvas->Print((canvasname+"_"+name+IDs+".C").c_str());
	fluxmap_xz_Canvas->Close();

	fluxmap_yz_Canvas->cd();
	gStyle->SetOptStat(11);
	gPad->SetLogz();
	FluxMap_yz->SetContour(999);

	FluxMap_yz->Draw("colz");

	string canvasname2=fluxmap_yz_Canvas->GetName();
	fluxmap_yz_Canvas->Write();
	fluxmap_yz_Canvas->Print((canvasname2+"_"+name+IDs+".eps").c_str());
	fluxmap_yz_Canvas->Print((canvasname2+"_"+name+IDs+".pdf").c_str());
	fluxmap_yz_Canvas->Print((canvasname2+"_"+name+IDs+".C").c_str());
	fluxmap_yz_Canvas->Close();

	output_rootfile->Write();
	input_rootfile->Close();

	delete output_rootfile, input_rootfile;
	delete fluxmap_xz_Canvas;
	delete fluxmap_yz_Canvas;
}

