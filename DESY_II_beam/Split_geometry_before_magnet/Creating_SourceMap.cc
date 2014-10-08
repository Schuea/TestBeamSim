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
#include "TExec.h"
#include "TPaveStats.h"

#include <cmath>
#include <algorithm>
#include <map>


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

void Creating_Map(string name); 
float* Fill_RebinningArrays(const int nbins, const float min, const float max, const int dimensions);
void GetBinEnergyDistribution(TTree* T, TH2F* histo, const int ParticleID);
void MakeEnergyHistogram(TTree* T, TH2F* histo, const int ParticleID);

int main(int argc,char *argv[]){
	if(argc < 2 || argc > 2){
    		//explain how to use program	
    		cerr << "Type in root filename!" << endl;
    		cerr << "e.g. ./Creating_Map file.root" << endl;
    		terminate();
	}	
	string filename;
	filename = argv[1];

       
	Creating_Map(filename);
}

void Creating_Map(string name){
	TH1::SetDefaultSumw2();

	TFile * input_rootfile = new TFile(name.c_str(),"READ");
	cout << "Inputfile size = " << input_rootfile->GetSize() << endl;
	TTree * T = (TTree*)input_rootfile->Get("Tree");
	cout << "Accessed TTree.." << endl;

	stringstream output_filename;
	output_filename << "CreatingMap_" << name;
	TFile * output_rootfile = new TFile(output_filename.str().c_str(),"RECREATE");


	float *binning_array_x;
	float *binning_array_y;
	binning_array_x=Fill_RebinningArrays(35,-15,15,2);
//	binning_array_x=Fill_RebinningArrays(7,-15,15,2);
	binning_array_y=Fill_RebinningArrays(35,-15,15,2);
//	binning_array_y=Fill_RebinningArrays(7,-15,15,2);

	TH2F * rebinned_Particle_Position = new TH2F("rebinned_Particle_position","Deflection of photons created in fibre",35,binning_array_x,35,binning_array_y);
//	TH2F * rebinned_Particle_Position = new TH2F("rebinned_Particle_position","Deflection of photons created in fibre",7,binning_array_x,7,binning_array_y);
	rebinned_Particle_Position->SetStats(kFALSE);
	rebinned_Particle_Position->SetOption("colz,TEXT");
	rebinned_Particle_Position->GetXaxis()->SetTitle("x-position (mm)");
	rebinned_Particle_Position->GetXaxis()->CenterTitle();
	rebinned_Particle_Position->GetYaxis()->SetTitle("y-position (mm)");
	rebinned_Particle_Position->GetYaxis()->CenterTitle();

	TH2F* EnergyMap_Particles = new TH2F("EnergyMap_Paticles","Deflection of photons created in fibre and their average energy",35,binning_array_x,35,binning_array_y);
//	TH2F* EnergyMap_Particles = new TH2F("EnergyMap_Paticles","Deflection of photons created in fibre and their average energy",7,binning_array_x,7,binning_array_y);
	EnergyMap_Particles->SetStats(kFALSE);
        EnergyMap_Particles->SetOption("TEXT");
        EnergyMap_Particles->GetXaxis()->SetTitle("x-position (mm)");
        EnergyMap_Particles->GetXaxis()->CenterTitle();
        EnergyMap_Particles->GetYaxis()->SetTitle("y-position (mm)");
        EnergyMap_Particles->GetYaxis()->CenterTitle();


	TCanvas* Map_Canvas = new TCanvas("Particles_before_magnet_Maps");
	Map_Canvas->Divide(1,2);
        Map_Canvas->cd(1);
        gPad->SetLogz();
        gPad->DrawFrame(-0.2,-0.2,0.2,0.2);
        TExec *ex1 = new TExec("ex1","gStyle->SetPaintTextFormat(\".0f\");");
        ex1->Draw();
	T->Draw("Reflectiony:Reflectionx >>+ rebinned_Particle_position","Particle_ID==11 && hasLeftDetector_Status==1 && (Reflectionx <15 && Reflectionx >-15 && Reflectiony <15 && Reflectiony >-15 && Theta<0.5)","colz,TEXT");
//	T->Draw("Reflectiony:Reflectionx >>+ rebinned_Particle_position","Particle_ID==11 && hasLeftDetector_Status==1","colz,TEXT");

	Map_Canvas->cd(2);
        gPad->SetTopMargin(0.15);
//	MakeEnergyHistogram(T,EnergyMap_Particles,11);

	string canvasname2=Map_Canvas->GetName();
        Map_Canvas->Write();
        Map_Canvas->Print((canvasname2+".eps").c_str());
        Map_Canvas->Print((canvasname2+".C").c_str());
        Map_Canvas->Close();

	GetBinEnergyDistribution(T,rebinned_Particle_Position,11);

	output_rootfile->Write();
	input_rootfile->Close();

	delete output_rootfile, input_rootfile;
	delete Map_Canvas;
	delete ex1;
	delete[] binning_array_x, binning_array_y;
}
float * Fill_RebinningArrays(const int nbins, const float min, const float max, const int dimensions){
	float *bin_array = new float[nbins+1];
	float *step_size_array = new float[nbins];

	float range = abs(max-min);
	int half_nbins = ceil(float(nbins)/float(2));
	bin_array[0]=min;
	bin_array[nbins]=max;
		
	if (dimensions == 2){	
		for (int i=0; i<half_nbins;i++){
			step_size_array[i]=float(float(range/2.0)/float(pow(2.0,i+1.0)));
			step_size_array[nbins-(i+1)]=step_size_array[i];
		}
		for (int j=1; j<half_nbins;j++){
			bin_array[j]=bin_array[j-1]+step_size_array[j-1];
		}	
		for (int j=nbins-1; j>=half_nbins;j--){
			bin_array[j]=bin_array[j+1]-step_size_array[j];
		}
	}

	if (dimensions == 1){
		for (int i=nbins; i>0;i--){
			step_size_array[nbins-i]=float(float(range)/float(3*i));
		}
		for (int j=1; j<nbins;j++){
			bin_array[j]=bin_array[j-1]+step_size_array[j-1];
		}	
	}

	delete[] step_size_array;
	return bin_array;	
	}

float GetMean(vector<float> vec){
	float mean(0);
	for(int i = 0; i < vec.size(); ++i){
		mean += vec.at(i);
	}
	mean /= float(vec.size());
	if(vec.size() == 0) mean = 0;
	return mean;
}

void MakeEnergyHistogram(TTree *T, TH2F *histo, const int ParticleID){ 
        int id,event_id=0; 
        float energy,x,y,z,theta; 
        id=0; 
        energy=0; 
        x=0; 
        y=0; 
        z=0; 
	theta=0;
	bool hasLeftDetector_Status=0;
        int binno=0; 
         
        T->SetBranchAddress("Event_ID",&event_id); 
        T->SetBranchAddress("Particle_ID",&id); 
        T->SetBranchAddress("Energy",&energy); 
        T->SetBranchAddress("Reflectionx",&x); 
        T->SetBranchAddress("Reflectiony",&y); 
        T->SetBranchAddress("Reflectionz",&z); 
	T->SetBranchAddress("Theta",&theta);
	T->SetBranchAddress("hasLeftDetector_Status",&hasLeftDetector_Status);
 
        int entries = T->GetEntries(); 
        std::map<int, vector< float > > tempmap; 
 
        for(int i = 0; i < entries; ++i){ 
                T->GetEntry(i); 
 
                //if (id!=ParticleID) continue;    
                if (id!=ParticleID || (x>15 && x<-15 && y>15 && y<-15 && theta>0.5) || hasLeftDetector_Status!=1) continue;    
                if (histo->GetXaxis()->GetXmin()<=x && x<=histo->GetXaxis()->GetXmax() && histo->GetYaxis()->GetXmin()<=y && y<=histo->GetYaxis()->GetXmax()){ 
                        binno = histo->FindBin(x,y); 
                        tempmap[binno].push_back(energy); 
                } 
        } 
        int totalbin = (histo->GetNbinsX()+2)*(histo->GetNbinsY()+2);//+2 due to overflow bins 
        for(int i = 0; i < totalbin; ++i){ 
                float mean = GetMean(tempmap[i]); 
                int xbin,ybin,zbin; 
                xbin=0; 
                ybin=0; 
                zbin=0; 
                histo->GetBinXYZ(i,xbin,ybin,zbin); 
                histo->Fill(histo->GetXaxis()->GetBinCenter(xbin),histo->GetYaxis()->GetBinCenter(ybin),mean); 
        } 
        histo->Draw("TEXT SAME"); 
}


void GetBinEnergyDistribution(TTree* T, TH2F* histo, const int ParticleID){

	int id;
	float energy,x,y,theta;
	id=0;
	energy=0;
	x=0;
	y=0;
	theta=0;
	bool hasLeftDetector_Status=0;
	
	int bin_number=0;
	std::map<int, vector< float > > deflection_map;
	
	T->SetBranchAddress("Particle_ID",&id);
	T->SetBranchAddress("Energy",&energy);
	T->SetBranchAddress("Reflectionx",&x);
	T->SetBranchAddress("Reflectiony",&y);
	T->SetBranchAddress("Theta",&theta);
	T->SetBranchAddress("hasLeftDetector_Status",&hasLeftDetector_Status);

	int entries = T->GetEntries();


	for(int i = 0; i < entries; ++i){
		T->GetEntry(i);

                //if (id!=ParticleID) continue;    
                //if (id!=ParticleID || (x>5 && x<-5 && y>5 && y<-5 && theta>0.5)) continue;    
                if (id!=ParticleID || (x>15 && x<-15 && y>15 && y<-15 && theta>0.5) || hasLeftDetector_Status!=1) continue;    
		bin_number = histo->FindBin(x,y);
		deflection_map[bin_number].push_back(energy);
	}
	
	int NbinsX = histo->GetNbinsX();
	int NbinsY = histo->GetNbinsY();
	int totalbins = (NbinsX+2)*(NbinsY+2);//+2 overflowbins per axis
	
	std::vector < int > overflowbins;
	int z(0);
	for (int i=0; i<totalbins; ++i){
		if ((i <= NbinsX+1) || (i % (NbinsX+2)==0) || (i % (NbinsX+2)==(NbinsX+1)) || (i >= (NbinsX+2)*(NbinsY+1))){
			overflowbins.push_back(i);
			++z;
		}
	}
	
	int normalbin_counter=1;
	TH1F** BinEnergyDistri_ = new TH1F*[totalbins];	
	ofstream Hvalues_for_macros;
	Hvalues_for_macros.open ("Hvalues_for_macros_Split_geometry_before_magnet_ELECTRONS.txt");
	Hvalues_for_macros << "Bin number in Map" << " " << "x" << " " << "y" << " " << "Bin number in distribution histo" << " " << "Events" << " " << "Energy" << endl;

	for(int i = 0; i < totalbins; ++i){
		int xbin,ybin,zbin;
		histo->GetBinXYZ(i,xbin,ybin,zbin);

		stringstream histoname,histotitle;
		histoname << "BinEnergyDistri_[" << i << "]";
		histotitle << "Energy distribution of photons in Bin " << normalbin_counter << " of histogram \"rebinned_Particle_position\"";
		
		bool isoverflowbin = (std::find(overflowbins.begin(),overflowbins.end(), i) != overflowbins.end());
		if (isoverflowbin) continue;

		BinEnergyDistri_[i] = new TH1F(histoname.str().c_str(), histotitle.str().c_str(),100,0,6.5); 
		BinEnergyDistri_[i]->GetXaxis()->SetTitle("Energy (GeV)");
		BinEnergyDistri_[i]->GetXaxis()->CenterTitle();

		for(int j=0; j<deflection_map[i].size(); j++){
			BinEnergyDistri_[i]->Fill(deflection_map[i].at(j));
		}
		BinEnergyDistri_[i]->Draw("L");
		for (int k=1; k <= BinEnergyDistri_[i]->GetNbinsX();k++){
			Hvalues_for_macros << normalbin_counter  << "\t" << histo->GetXaxis()->GetBinCenter(xbin) << "\t" << histo->GetYaxis()->GetBinCenter(ybin) << "\t" << k << "\t" << BinEnergyDistri_[i]->GetBinContent(k) << "\t" << BinEnergyDistri_[i]->GetBinCenter(k) << endl;
		}
		normalbin_counter++;
		delete BinEnergyDistri_[i];
	}
	Hvalues_for_macros.close();
	delete[] BinEnergyDistri_;
}

