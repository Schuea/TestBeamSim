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

void bremsstrahlungSpectrum(string name); 
float* Fill_RebinningArrays(const int nbins, const float min, const float max, const int dimensions);
void GetBinEnergyDistribution(TTree* T, TH2F* histo, const int ParticleID);
void MakeEnergyHistogram(TTree* T, TH2F* histo, const int ParticleID);

int main(int argc,char *argv[]){
	if(argc < 2 || argc > 2){
    		//explain how to use program	
    		cerr << "Type in root filename!" << endl;
    		cerr << "e.g. ./DrawHistograms file.root" << endl;
    		terminate();
	}	
	string filename;
	filename = argv[1];

       
	bremsstrahlungSpectrum(filename);
}

void bremsstrahlungSpectrum(string name){
	TH1::SetDefaultSumw2();

	TFile * input_rootfile = new TFile(name.c_str(),"READ");
	cout << "Inputfile size = " << input_rootfile->GetSize() << endl;
	TTree * T = (TTree*)input_rootfile->Get("Tree");
	cout << "Accessed TTree.." << endl;

	stringstream output_filename;
	output_filename << "bremsstrahlungSpectrums_" << name;
	TFile * output_rootfile = new TFile(output_filename.str().c_str(),"RECREATE");

	TH1F * bremsstrahlungSpectrum = new TH1F("Spectrum","Energy distribution of bremsstrahlung photon source",50,0,7);
	bremsstrahlungSpectrum->SetLineColor(2);
	bremsstrahlungSpectrum->GetXaxis()->SetTitle("Energy (GeV)");
	bremsstrahlungSpectrum->GetXaxis()->CenterTitle();

	float *binning_array_x;
	float *binning_array_y;
	binning_array_x=Fill_RebinningArrays(21/*7*/,-0.1,0.1,2);
	binning_array_y=Fill_RebinningArrays(21/*7*/,-0.1,0.1,2);

	TH2F * rebinned_Photon_Deflection_Fibre = new TH2F("rebinned_DeflFibre_Photon","Deflection of photons created in fibre",21/*7*/,binning_array_x,21/*7*/,binning_array_y);
	rebinned_Photon_Deflection_Fibre->SetStats(kFALSE);
	rebinned_Photon_Deflection_Fibre->SetOption("colz,TEXT");
	rebinned_Photon_Deflection_Fibre->GetXaxis()->SetTitle("x (mm)");
	rebinned_Photon_Deflection_Fibre->GetXaxis()->CenterTitle();
	rebinned_Photon_Deflection_Fibre->GetYaxis()->SetTitle("y (mm)");
	rebinned_Photon_Deflection_Fibre->GetYaxis()->CenterTitle();

	TH2F* EnergyMap_Photon_Deflection_Fibre = new TH2F("EnergyMap_DeflFibre_Photon","Deflection of photons created in fibre and their average energy",21/*7*/,binning_array_x,21/*7*/,binning_array_y);
	EnergyMap_Photon_Deflection_Fibre->SetStats(kFALSE);
        EnergyMap_Photon_Deflection_Fibre->SetOption("box,TEXT");
        EnergyMap_Photon_Deflection_Fibre->GetXaxis()->SetTitle("x (mm)");
        EnergyMap_Photon_Deflection_Fibre->GetXaxis()->CenterTitle();
        EnergyMap_Photon_Deflection_Fibre->GetYaxis()->SetTitle("y (mm)");
        EnergyMap_Photon_Deflection_Fibre->GetYaxis()->CenterTitle();


	TCanvas * Spectrum_Canvas = new TCanvas("bremsstrahlungSpectrum");

	Spectrum_Canvas->cd();
	gPad->SetLogy();
	T->Draw("Energy >>+ Spectrum","Particle_ID==22");
	gPad->Update();
 	TPaveStats *st1 = (TPaveStats*)bremsstrahlungSpectrum->FindObject("stats");
  	double statboxsize = st1->GetY2NDC() - st1->GetY1NDC();
 	st1->SetTextColor(2);
  	st1->SetY1NDC(0.8);
  	st1->SetY2NDC(0.8+statboxsize);	

	string canvasname=Spectrum_Canvas->GetName();
	Spectrum_Canvas->Write();
	Spectrum_Canvas->Print((canvasname+".eps").c_str());
	Spectrum_Canvas->Print((canvasname+".C").c_str());
	Spectrum_Canvas->Close();



	TCanvas* Map_Canvas = new TCanvas("PhotonSource_Maps");

	Map_Canvas->Divide(1,2);
        Map_Canvas->cd(1);
        gPad->SetLogz();
        gPad->DrawFrame(-0.1,-0.1,0.1,0.1);
        TExec *ex1 = new TExec("ex1","gStyle->SetPaintTextFormat(\".0f\");");
        ex1->Draw();
	T->Draw("Reflectiony:Reflectionx >>+ rebinned_DeflFibre_Photon","Particle_ID==22","colz,TEXT");
	Map_Canvas->cd(2);
        gPad->SetTopMargin(0.15);
//	MakeEnergyHistogram(T,EnergyMap_Photon_Deflection_Fibre,22);

	string canvasname2=Map_Canvas->GetName();
        Map_Canvas->Write();
        Map_Canvas->Print((canvasname2+".eps").c_str());
        Map_Canvas->Print((canvasname2+".C").c_str());
        Map_Canvas->Close();

	GetBinEnergyDistribution(T,rebinned_Photon_Deflection_Fibre,22);

	output_rootfile->Write();
	input_rootfile->Close();

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
			cout << "bin_array[" << j << "] =" << bin_array[j] << endl;
		}	
		for (int j=nbins-1; j>=half_nbins;j--){
			bin_array[j]=bin_array[j+1]-step_size_array[j];
			cout << "bin_array[" << j << "] =" << bin_array[j] << endl;
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
        int binno=0; 
         
        T->SetBranchAddress("Event_ID",&event_id); 
        T->SetBranchAddress("Particle_ID",&id); 
        T->SetBranchAddress("Energy",&energy); 
        T->SetBranchAddress("Reflectionx",&x); 
        T->SetBranchAddress("Reflectiony",&y); 
        T->SetBranchAddress("Reflectionz",&z); 
	T->SetBranchAddress("Theta",&theta);
 
        int entries = T->GetEntries(); 
        std::map<int, vector< float > > tempmap; 
 
        for(int i = 0; i < entries; ++i){ 
                T->GetEntry(i); 
 
                if (id!=ParticleID) continue;    
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
        histo->Draw("box,TEXT SAME"); 
}


void GetBinEnergyDistribution(TTree* T, TH2F* histo, const int ParticleID){

	int id;
	float energy,x,y,theta;
	id=0;
	energy=0;
	x=0;
	y=0;
	theta=0;
	
	int bin_number=0;
	std::map<int, vector< float > > deflection_map;
	
	T->SetBranchAddress("Particle_ID",&id);
	T->SetBranchAddress("Energy",&energy);
	T->SetBranchAddress("Reflectionx",&x);
	T->SetBranchAddress("Reflectiony",&y);
	T->SetBranchAddress("Theta",&theta);

	int entries = T->GetEntries();


	for(int i = 0; i < entries; ++i){
		T->GetEntry(i);

		if (id!=ParticleID) continue;	
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
	Hvalues_for_macros.open ("Hvalues_for_DESY_II_beam_macros.txt");
	Hvalues_for_macros << "Bin number in Map" << " " << "x" << " " << "y" << " " << "Bin number in distribution histo" << " " << "Events" << " " << "Energy" << endl;

	for(int i = 0; i < totalbins; ++i){
		int xbin,ybin,zbin;
		histo->GetBinXYZ(i,xbin,ybin,zbin);

		stringstream histoname,histotitle;
		histoname << "BinEnergyDistri_[" << i << "]";
		histotitle << "Energy distribution of photons in Bin " << normalbin_counter << " of histogram \"rebinned_DeflFibre_Photon\"";
		
		bool isoverflowbin = (std::find(overflowbins.begin(),overflowbins.end(), i) != overflowbins.end());
		if (isoverflowbin) continue;

		BinEnergyDistri_[i] = new TH1F(histoname.str().c_str(), histotitle.str().c_str(),100,0,7); 
		BinEnergyDistri_[i]->GetXaxis()->SetTitle("Energy/GeV");
		BinEnergyDistri_[i]->GetXaxis()->CenterTitle();

		for(int j=0; j<deflection_map[i].size(); j++){
			BinEnergyDistri_[i]->Fill(deflection_map[i].at(j));
		}
		BinEnergyDistri_[i]->Draw("L");
		for (int k=1; k <= BinEnergyDistri_[i]->GetNbinsX();k++){
			Hvalues_for_macros << normalbin_counter  << "\t" << histo->GetXaxis()->GetBinCenter(xbin) << "\t" << histo->GetYaxis()->GetBinCenter(ybin) << "\t" << k << "\t" << BinEnergyDistri_[i]->GetBinContent(k) << "\t" << BinEnergyDistri_[i]->GetBinCenter(k) << endl;
		}
		normalbin_counter++;
	}
	Hvalues_for_macros.close();
}

