#include "TFile.h"
#include "TBrowser.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TTree.h"
#include "TChain.h"
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

void bremsstrahlungSpectrum(string name1,string name2,string name3,string name4,string name5,string name6,string name7,string name8,string name9,string name10); 
float* Fill_RebinningArrays(const int nbins, const float min, const float max, const int dimensions);
void GetBinEnergyDistribution(TChain* rootchain, TH2F* histo, const int ParticleID);
void MakeEnergyHistogram(TChain* rootchain, TH2F* histo, const int ParticleID);

int main(int argc,char *argv[]){
	if(argc < 11 || argc > 11){
    		//explain how to use program	
    		cerr << "Type in root filename!" << endl;
    		cerr << "e.g. ./DrawHistograms file.root" << endl;
    		terminate();
	}	
	string filename1,filename2,filename3,filename4,filename5,filename6,filename7,filename8,filename9,filename10;
	filename1 = argv[1];
	filename2 = argv[2];
	filename3 = argv[3];
	filename4 = argv[4];
	filename5 = argv[5];
	filename6 = argv[6];
	filename7 = argv[7];
	filename8 = argv[8];
	filename9 = argv[9];
	filename10 = argv[10];

       
	bremsstrahlungSpectrum(filename1,filename2,filename3,filename4,filename5,filename6,filename7,filename8,filename9,filename10);
}

void bremsstrahlungSpectrum(string name1,string name2,string name3,string name4,string name5,string name6,string name7,string name8,string name9,string name10){
	TH1::SetDefaultSumw2();

	TChain * rootchain = new TChain("Tree");
        rootchain->SetMaxTreeSize(250000000000);

        rootchain->Add(name1.c_str());
        rootchain->Add(name2.c_str());
        rootchain->Add(name3.c_str());
        rootchain->Add(name4.c_str());
        rootchain->Add(name5.c_str());
        rootchain->Add(name6.c_str());
        rootchain->Add(name7.c_str());
        rootchain->Add(name8.c_str());
        rootchain->Add(name9.c_str());
        rootchain->Add(name10.c_str());

 	cout << "rootchain->GetEntries(Particle_ID==22);" << rootchain->GetEntries("Particle_ID==22") << endl;
 	cout << "rootchain->GetEntries(Particle_ID==22 && hasLeftDetector_Status==1);" << rootchain->GetEntries("Particle_ID==22 && hasLeftDetector_Status==1") << endl;
	
	stringstream output_filename;
	output_filename << "bremsstrahlungSpectrum_Fibre.root";
	TFile * output_rootfile = new TFile(output_filename.str().c_str(),"RECREATE");

	TH1F * bremsstrahlungSpectrum = new TH1F("Photon_spectrum","Energy distribution of bremsstrahlung photon source",100,0,7);
	bremsstrahlungSpectrum->SetLineColor(2);
	bremsstrahlungSpectrum->GetXaxis()->SetTitle("Energy (GeV)");
	bremsstrahlungSpectrum->GetXaxis()->CenterTitle();
	
	TH2F * Photon_Fluxmap = new TH2F("Photon_map","Position of bremsstrahlung photons",100,-0.1,0.1,100,-0.1,0.1);
	Photon_Fluxmap->GetXaxis()->SetTitle("x (mm)");
	Photon_Fluxmap->GetXaxis()->CenterTitle();
	Photon_Fluxmap->GetYaxis()->SetTitle("y (mm)");
	Photon_Fluxmap->GetYaxis()->CenterTitle();

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

	Spectrum_Canvas->Divide(1,2);
	Spectrum_Canvas->cd(1);
	gPad->SetLogy();
	rootchain->Draw("Energy >>+ Photon_spectrum","Particle_ID==22");
	gPad->Update();
 	TPaveStats *st1 = (TPaveStats*)bremsstrahlungSpectrum->FindObject("stats");
  	double statboxsize = st1->GetY2NDC() - st1->GetY1NDC();
 	st1->SetTextColor(2);
  	st1->SetY1NDC(0.8);
  	st1->SetY2NDC(0.8+statboxsize);	

	Spectrum_Canvas->cd(2);
	gPad->SetLogz();
	gStyle->SetNumberContours(99999);
	gStyle->SetOptStat(11);
	rootchain->Draw("Reflectiony:Reflectionx >>+ Photon_map","Particle_ID==22","colz");
	

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
	rootchain->Draw("Reflectiony:Reflectionx >>+ rebinned_DeflFibre_Photon","Particle_ID==22","colz,TEXT");
	Map_Canvas->cd(2);
        gPad->SetTopMargin(0.15);
//	MakeEnergyHistogram(rootchain,EnergyMap_Photon_Deflection_Fibre,22);

	string canvasname2=Map_Canvas->GetName();
        Map_Canvas->Write();
        Map_Canvas->Print((canvasname2+".eps").c_str());
        Map_Canvas->Print((canvasname2+".C").c_str());
        Map_Canvas->Close();

//	GetBinEnergyDistribution(rootchain,rebinned_Photon_Deflection_Fibre,22);

	output_rootfile->Write();

	delete output_rootfile;
        delete Spectrum_Canvas,Map_Canvas;
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

void MakeEnergyHistogram(TChain* rootchain, TH2F *histo, const int ParticleID){ 
        int id=0; 
	float energy=0; 
	float x=0; 
	float y=0; 
        int binno=0; 
         
	rootchain->SetBranchAddress("Particle_ID",&id); 
        rootchain->SetBranchAddress("Energy",&energy); 
        rootchain->SetBranchAddress("Reflectionx",&x); 
        rootchain->SetBranchAddress("Reflectiony",&y); 

        int entries = rootchain->GetEntries(); 
	cout << "root-chain->GetEntries() = " << entries << endl;
        std::map<int, vector< float > > tempmap; 
 
        for(int i = 0; i < entries; ++i){ 
                rootchain->GetEntry(i); 
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


void GetBinEnergyDistribution(TChain* rootchain, TH2F* histo, const int ParticleID){
        int id=0; 
	float energy=0; 
	float x=0; 
	float y=0; 
        int binno=0; 
         
	rootchain->SetBranchAddress("Particle_ID",&id); 
        rootchain->SetBranchAddress("Energy",&energy); 
        rootchain->SetBranchAddress("Reflectionx",&x); 
        rootchain->SetBranchAddress("Reflectiony",&y); 

        int entries = rootchain->GetEntries(); 
	cout << "root-chain->GetEntries() = " << entries << endl;
       
	std::map<int, vector< float > > tempmap; 
         
	for(int i = 0; i < entries; ++i){ 
                rootchain->GetEntry(i); 
                if (id!=ParticleID) continue;    
                if (histo->GetXaxis()->GetXmin()<=x && x<=histo->GetXaxis()->GetXmax() && histo->GetYaxis()->GetXmin()<=y && y<=histo->GetYaxis()->GetXmax()){ 
                        binno = histo->FindBin(x,y); 
                        tempmap[binno].push_back(energy); 
                } 
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
		BinEnergyDistri_[i]->GetXaxis()->SetTitle("Energy (GeV)");
		BinEnergyDistri_[i]->GetXaxis()->CenterTitle();

		for(int j=0; j< tempmap[i].size(); j++){
			BinEnergyDistri_[i]->Fill(tempmap[i].at(j));
		}
		BinEnergyDistri_[i]->Draw("L");
		for (int k=1; k <= BinEnergyDistri_[i]->GetNbinsX();k++){
			Hvalues_for_macros << normalbin_counter  << "\t" << histo->GetXaxis()->GetBinCenter(xbin) << "\t" << histo->GetYaxis()->GetBinCenter(ybin) << "\t" << k << "\t" << BinEnergyDistri_[i]->GetBinContent(k) << "\t" << BinEnergyDistri_[i]->GetBinCenter(k) << endl;
		}
		normalbin_counter++;
	}
	Hvalues_for_macros.close();
}

