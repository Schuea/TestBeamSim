#include "TFile.h"
#include "TBrowser.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TTree.h"
#include "TBranch.h"
#include "TF1.h"
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

void bremsstrahlungSpectrum(vector< string > names); 
float* Fill_RebinningArrays(const int nbins, const float min, const float max, const int dimensions);
void GetBinEnergyDistribution(vector< TTree* > trees, TH2D* histo, const int ParticleID);
void MakeEnergyHistogram(vector< TTree* > trees, TH2D* histo, const int ParticleID);

int main(int argc,char *argv[]){
	if(argc < 21 || argc > 21){
//	if(argc < 2 || argc > 2){
    		//explain how to use program	
    		cerr << "Type in root filename!" << endl;
    		cerr << "e.g. ./DrawHistograms file.root" << endl;
    		terminate();
	}	
	vector < string > names;
	for(int i = 1; i < 21; ++i){
//	for(int i = 1; i < 2; ++i){
	names.push_back(argv[i]);
	}
       
	bremsstrahlungSpectrum(names);
}

void bremsstrahlungSpectrum(vector< string > names){
	TH1::SetDefaultSumw2();
	vector < TTree * > vectree;
	vector < TFile * > filevec;
	for(int i = 0; i < names.size(); ++i){
		TFile * input_rootfile = new TFile(names.at(i).c_str(),"READ");
		filevec.push_back(input_rootfile);
		cout << "Inputfile " << i << " size = " << input_rootfile->GetSize() << endl;
		TTree * T1 = (TTree*)input_rootfile->Get("Tree_MCP");
		vectree.push_back(T1);
		cout << "Accessed TTree.." << endl;
	}
cout<<__LINE__<< " TTrees registered and stored in vectree. Rootfiles stored in filevec." <<endl;

	stringstream output_filename;
	output_filename << "bremsstrahlungSpectrums_Fibre.root";
	TFile * output_rootfile = new TFile(output_filename.str().c_str(),"RECREATE");

	TH1D * bremsstrahlungSpectrum = new TH1D("Photon_spectrum","Energy distribution of bremsstrahlung photon source",150,0,7);
	bremsstrahlungSpectrum->SetLineColor(4);
	bremsstrahlungSpectrum->GetXaxis()->SetTitle("Energy (GeV)");
	bremsstrahlungSpectrum->GetXaxis()->CenterTitle();
/*	
	TH2D * Photon_Fluxmap = new TH2D("Photon_map","Position of bremsstrahlung photons",100,-0.05,0.05,100,-0.05,0.05);
	Photon_Fluxmap->GetXaxis()->SetTitle("x (mm)");
	Photon_Fluxmap->GetXaxis()->CenterTitle();
	Photon_Fluxmap->GetYaxis()->SetTitle("y (mm)");
	Photon_Fluxmap->GetYaxis()->CenterTitle();
*/
	float *binning_array_x;
	float *binning_array_y;
	binning_array_x=Fill_RebinningArrays(21/*7*/,-1,1,2);
	binning_array_y=Fill_RebinningArrays(21/*7*/,-1,1,2);
cout<<__LINE__<< " Function Fill_RebinningArrays called." <<endl;

	TH2D * rebinned_Photon_Deflection_Fibre = new TH2D("rebinned_DeflFibre_Photon","Deflection of photons created in fibre",21/*7*/,binning_array_x,21/*7*/,binning_array_y);
	rebinned_Photon_Deflection_Fibre->SetStats(kFALSE);
	rebinned_Photon_Deflection_Fibre->SetOption("colz,TEXT");
	rebinned_Photon_Deflection_Fibre->GetXaxis()->SetTitle("x (mm)");
	rebinned_Photon_Deflection_Fibre->GetXaxis()->CenterTitle();
	rebinned_Photon_Deflection_Fibre->GetYaxis()->SetTitle("y (mm)");
	rebinned_Photon_Deflection_Fibre->GetYaxis()->CenterTitle();

	TH2D* EnergyMap_Photon_Deflection_Fibre = new TH2D("EnergyMap_DeflFibre_Photon","Deflection of photons created in fibre and their average energy",/*21*/7,binning_array_x,/*21*/7,binning_array_y);
	EnergyMap_Photon_Deflection_Fibre->SetStats(kFALSE);
        EnergyMap_Photon_Deflection_Fibre->SetOption("TEXT");
        EnergyMap_Photon_Deflection_Fibre->GetXaxis()->SetTitle("x (mm)");
        EnergyMap_Photon_Deflection_Fibre->GetXaxis()->CenterTitle();
        EnergyMap_Photon_Deflection_Fibre->GetYaxis()->SetTitle("y (mm)");
        EnergyMap_Photon_Deflection_Fibre->GetYaxis()->CenterTitle();


	TCanvas * Spectrum_Canvas = new TCanvas("bremsstrahlungSpectrum");

//	Spectrum_Canvas->Divide(1,2);
//	Spectrum_Canvas->cd(1);
	Spectrum_Canvas->cd();
	gPad->SetLogy();
	vectree.at(0)->Draw("Energy >>+ Photon_spectrum","Particle_ID==22 && hasLeftDetector_Status==1");
	for(int i = 1; i < vectree.size(); ++i){
		vectree.at(i)->Draw("Energy >>+ Photon_spectrum","Particle_ID==22 && hasLeftDetector_Status==1","SAME");
	}
cout<<__LINE__<< " Energy filled into histogramm from all TTrees." << endl;
	TF1 *One_over_E = new TF1("Bremsstrahlung","[0]-[1]/x+[2]/(x*x)",0,6);
	//TF1 *One_over_E = new TF1("Bremsstrahlung","(0.0025/19.321267)*((4*log([1]/[0]))/3-(4*([1]-[0]))/(3*x)+([1]*[1]-[0]*[0])/(2*x*x))",0,6);
	//One_over_E->SetParLimits(0,0,0.2);
	//One_over_E->SetParLimits(1,6.2,6.4);
	bremsstrahlungSpectrum->Fit(One_over_E,"BR");
cout<<__LINE__<< " Statboxsize manipulated." << endl;

/*
	Spectrum_Canvas->cd(2);
	gPad->SetLogz();
	gStyle->SetNumberContours(999);
	gStyle->SetOptStat(11);
	vectree.at(0)->Draw("Reflectiony:Reflectionx >>+ Photon_map","Particle_ID==22 && hasLeftDetector_Status==1","colz");
	for(int i = 1; i < vectree.size(); ++i){
		vectree.at(i)->Draw("Reflectiony:Reflectionx >>+ Photon_map","Particle_ID==22 && hasLeftDetector_Status==1","colz,SAME");
	}
cout<<__LINE__<< " y over x plotted in position plot for all TTrees." << endl;
*/
	string canvasname=Spectrum_Canvas->GetName();
	Spectrum_Canvas->Write();
	Spectrum_Canvas->Print((canvasname+".eps").c_str());
	Spectrum_Canvas->Print((canvasname+".C").c_str());
	Spectrum_Canvas->Close();
cout<<__LINE__<< " Spectrum canvas written, printed and closed." << endl;



	TCanvas* Map_Canvas = new TCanvas("PhotonSource_Maps");

	Map_Canvas->Divide(1,2);
        Map_Canvas->cd(1);
        gPad->SetLogz();
        gPad->DrawFrame(-1,-1,1,1);
        TExec *ex1 = new TExec("ex1","gStyle->SetPaintTextFormat(\".0f\");");
        ex1->Draw();
	vectree.at(0)->Draw("Reflectiony:Reflectionx >>+ rebinned_DeflFibre_Photon","Particle_ID==22 && hasLeftDetector_Status==1","colz,TEXT");
	for(int i = 1; i < vectree.size(); ++i){
		vectree.at(i)->Draw("Reflectiony:Reflectionx >>+ rebinned_DeflFibre_Photon","Particle_ID==22 && hasLeftDetector_Status==1","SAME,colz,TEXT");
	}
cout<<__LINE__<< " y over x plotted in position plot for all TTrees for map." << endl;
	Map_Canvas->cd(2);
        gPad->SetTopMargin(0.15);
//	MakeEnergyHistogram(vectree,EnergyMap_Photon_Deflection_Fibre,22);
//cout<<__LINE__<< " Function MakeEnergyHistogram called. Energy map printed onto Map canvas." << endl;

	string canvasname2=Map_Canvas->GetName();
        Map_Canvas->Write();
        Map_Canvas->Print((canvasname2+".eps").c_str());
        Map_Canvas->Print((canvasname2+".C").c_str());
        Map_Canvas->Close();
cout<<__LINE__<< " Map canvas written, printed and closed." << endl;

	GetBinEnergyDistribution(vectree,rebinned_Photon_Deflection_Fibre,22);
cout<<__LINE__<< " Function GetBinEnergyDistribution called. Output textfile with energy values written." << endl;

	output_rootfile->Write();
cout<<__LINE__<< " Output rootfile written." << endl;
	for(int i = 0; i < filevec.size(); ++i){
		filevec.at(i)->Close();
		delete filevec.at(i);
	}
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

void MakeEnergyHistogram(vector< TTree* > vectree, TH2D *histo, const int ParticleID){
	vector<int> ids(vectree.size(),0);
	vector<double> energies(vectree.size(),0);
	vector<double> x(vectree.size(),0);
	vector<double> y(vectree.size(),0);
	vector<Long64_t> entries;
        int binno=0; 
        for(int i = 0; i < vectree.size(); ++i){
		vectree.at(i)->SetBranchAddress("Particle_ID",&(ids.at(i)));
		vectree.at(i)->SetBranchAddress("Energy",&(energies.at(i))); 
		vectree.at(i)->SetBranchAddress("Reflectionx",&(x.at(i))); 
		vectree.at(i)->SetBranchAddress("Reflectiony",&(y.at(i)));
		entries.push_back(vectree.at(i)->GetEntries());
	}
cout<<__LINE__<< " In function MakeEnergyHistogram: Branch addresses set." << endl;

        std::map<int, vector< float > > tempmap; 
 
	for(int j = 0; j < entries.size(); ++j){
		for(int i = 0; i < entries.at(j); ++i){ 
			vectree.at(j)->GetEntry(i); 
			if (ids.at(j)!=ParticleID) continue;    
			if (histo->GetXaxis()->GetXmin()<=x.at(j) && x.at(j)<=histo->GetXaxis()->GetXmax() && histo->GetYaxis()->GetXmin()<=y.at(j) && y.at(j)<=histo->GetYaxis()->GetXmax()){ 
				binno = histo->FindBin(x.at(j),y.at(j)); 
				tempmap[binno].push_back(energies.at(j)); 
			} 
		}
	}
cout<<__LINE__<< " In function MakeEnergyHistogram: energies per bin stored in tempmap." << endl;
 
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
cout<<__LINE__<< " In function MakeEnergyHistogram: mean energies per bin filled into histogramm." << endl;
        histo->Draw("box,TEXT SAME"); 
cout<<__LINE__<< " In function MakeEnergyHistogram: histogramm drawn." << endl;
}


void GetBinEnergyDistribution(vector<TTree*> vectree, TH2D* histo, const int ParticleID){

	vector<int> ids(vectree.size(),0);
	vector<double> energies(vectree.size(),0);
	vector<double> x(vectree.size(),0);
	vector<double> y(vectree.size(),0);
	vector<Long64_t> entries;
        int binno=0; 
         
        for(int i = 0; i < vectree.size(); ++i){
		vectree.at(i)->SetBranchAddress("Particle_ID",&(ids.at(i)));
		vectree.at(i)->SetBranchAddress("Energy",&(energies.at(i))); 
		vectree.at(i)->SetBranchAddress("Reflectionx",&(x.at(i))); 
		vectree.at(i)->SetBranchAddress("Reflectiony",&(y.at(i)));
		entries.push_back(vectree.at(i)->GetEntries());
	}
	
	std::map<int, vector< float > > tempmap;
	for(int j = 0; j < entries.size(); ++j){
		for(int i = 0; i < entries.at(j); ++i){ 
			vectree.at(j)->GetEntry(i); 
			if (ids.at(j)!=ParticleID) continue;    
			if (histo->GetXaxis()->GetXmin()<=x.at(j) && x.at(j)<=histo->GetXaxis()->GetXmax() && histo->GetYaxis()->GetXmin()<=y.at(j) && y.at(j)<=histo->GetYaxis()->GetXmax()){ 
				binno = histo->FindBin(x.at(j),y.at(j)); 
				tempmap[binno].push_back(energies.at(j)); 
			} 
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
	TH1D** BinEnergyDistri_ = new TH1D*[totalbins];	
	ofstream Hvalues_for_macros;
	Hvalues_for_macros.open ("Hvalues_for_real_sized_DESY_II_beam_macros.txt");
	Hvalues_for_macros << "Bin number in Map" << " " << "x" << " " << "y" << " " << "Bin number in distribution histo" << " " << "Events" << " " << "Energy" << endl;

	for(int i = 0; i < totalbins; ++i){
		int xbin,ybin,zbin;
		histo->GetBinXYZ(i,xbin,ybin,zbin);

		stringstream histoname,histotitle;
		histoname << "BinEnergyDistri_[" << i << "]";
		histotitle << "Energy distribution of photons in Bin " << normalbin_counter << " of histogram \"rebinned_DeflFibre_Photon\"";
		
		bool isoverflowbin = (std::find(overflowbins.begin(),overflowbins.end(), i) != overflowbins.end());
		if (isoverflowbin) continue;

		BinEnergyDistri_[i] = new TH1D(histoname.str().c_str(), histotitle.str().c_str(),100,0,7); 
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

