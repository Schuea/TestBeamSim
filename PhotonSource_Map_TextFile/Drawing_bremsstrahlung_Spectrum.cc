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
	bremsstrahlungSpectrum->GetXaxis()->SetTitle("Energy(GeV)");
	bremsstrahlungSpectrum->GetXaxis()->CenterTitle();
	TH1F * bremsstrahlungSpectrum_Cut = new TH1F("Spectrum_Cut","Energy distribution of bremsstrahlung photon source with cut on #theta",50,0,7);
	bremsstrahlungSpectrum_Cut->SetLineColor(4);
	bremsstrahlungSpectrum_Cut->GetXaxis()->SetTitle("Energy(GeV)");
	bremsstrahlungSpectrum_Cut->GetXaxis()->CenterTitle();

	float *binning_array_x;
	float *binning_array_y;
	binning_array_x=Fill_RebinningArrays(7,-0.2,0.2,2);
	binning_array_y=Fill_RebinningArrays(7,-0.2,0.2,2);

	TH2F * rebinned_Photon_Deflection_Fibre = new TH2F("rebinned_DeflFibre_Photon","Deflection of photons created in fibre",7,binning_array_x,7,binning_array_y);
	rebinned_Photon_Deflection_Fibre->SetStats(kFALSE);
	rebinned_Photon_Deflection_Fibre->SetOption("colz,TEXT");
	rebinned_Photon_Deflection_Fibre->GetXaxis()->SetTitle("x-position/mm");
	rebinned_Photon_Deflection_Fibre->GetXaxis()->CenterTitle();
	rebinned_Photon_Deflection_Fibre->GetYaxis()->SetTitle("y-position/mm");
	rebinned_Photon_Deflection_Fibre->GetYaxis()->CenterTitle();

	TH2F* EnergyMap_Photon_Deflection_Fibre = new TH2F("EnergyMap_DeflFibre_Photon","Deflection of photons created in fibre and their average energy",7,binning_array_x,7,binning_array_y);
	EnergyMap_Photon_Deflection_Fibre->SetStats(kFALSE);
        EnergyMap_Photon_Deflection_Fibre->SetOption("box,TEXT");
        EnergyMap_Photon_Deflection_Fibre->GetXaxis()->SetTitle("x-position (mm)");
        EnergyMap_Photon_Deflection_Fibre->GetXaxis()->CenterTitle();
        EnergyMap_Photon_Deflection_Fibre->GetYaxis()->SetTitle("y-position (mm)");
        EnergyMap_Photon_Deflection_Fibre->GetYaxis()->CenterTitle();

	TLegend* E_Legend = new TLegend(0.4,0.5,0.75,0.7);
	E_Legend->SetFillColor(0);

	TCanvas * CompareSpectrums = new TCanvas("Compare_bremsstrahlungSpectrums");
	TPad *subpad1, *subpad2;
  	subpad1 = new TPad("subpad1"," ",0,0.3,1,1);
  	subpad1->SetBottomMargin(0.);
  	subpad1->Draw();
  	subpad2 = new TPad("subpad2"," ",0,0,1,0.3);
  	subpad2->SetBottomMargin(0.30);
  	subpad2->SetTopMargin(0.);
  	subpad2->Draw();

  	subpad1->cd();
	gPad->SetLogy();
	T->Draw("Energy >>+ Spectrum","Particle_ID==22");
 //	bremsstrahlungSpectrum->Scale(1.0/(bremsstrahlungSpectrum->Integral()));
	gPad->Update();
 	TPaveStats *st1 = (TPaveStats*)bremsstrahlungSpectrum->FindObject("stats");
  	double statboxsize = st1->GetY2NDC() - st1->GetY1NDC();
 	st1->SetTextColor(2);
  	st1->SetY1NDC(0.8);
  	st1->SetY2NDC(0.8+statboxsize);	

	T->Draw("Energy >>+ Spectrum_Cut","Particle_ID==22 && Theta<0.2865","SAMES");
//	bremsstrahlungSpectrum_Cut->Scale(1.0/(bremsstrahlungSpectrum_Cut->Integral()));
	gPad->Update();
  	TPaveStats *st2 = (TPaveStats*)bremsstrahlungSpectrum_Cut->FindObject("stats");
  	st2->SetY2NDC(st1->GetY1NDC()); //new x start position
  	st2->SetY1NDC(st2->GetY2NDC()-statboxsize); //new x end position
  	st2->SetTextColor(4);
	E_Legend->AddEntry(bremsstrahlungSpectrum,"Bremsstrahlung spectrum","l");	
	E_Legend->AddEntry(bremsstrahlungSpectrum_Cut,"Bremsstrahlung spectrum for #theta<0.2865^{#circ}","l");	
	E_Legend->Draw();

	subpad2->cd();
  	TH1F* E_Diff;
	string E_DiffName;
    	E_DiffName =  "EnergyDistribution_Difference";
      	E_Diff = new TH1F(E_DiffName.c_str(),E_DiffName.c_str(), bremsstrahlungSpectrum_Cut->GetNbinsX(), bremsstrahlungSpectrum_Cut->GetXaxis()->GetXmin(), bremsstrahlungSpectrum_Cut->GetXaxis()->GetXmax());
    	E_Diff->Divide(bremsstrahlungSpectrum,bremsstrahlungSpectrum_Cut,1,1,"b");
    	E_Diff->SetLineColor(bremsstrahlungSpectrum->GetLineColor());
    	E_Diff->SetLineWidth(1);
    	E_Diff->SetMarkerStyle(bremsstrahlungSpectrum_Cut->GetMarkerStyle());
    	E_Diff->SetMarkerColor(bremsstrahlungSpectrum_Cut->GetMarkerColor());
    	E_Diff->SetMarkerSize(.8);
    	E_Diff->Draw("EP");	

	TLine *line1 = new TLine(bremsstrahlungSpectrum_Cut->GetBinLowEdge(1),1,bremsstrahlungSpectrum_Cut->GetBinLowEdge(bremsstrahlungSpectrum_Cut->GetNbinsX()+1),1);
  	line1->SetLineWidth(2);
  	line1->SetLineColor(kBlack);
  	line1->Draw();
  	E_Diff->GetXaxis()->SetTitle(bremsstrahlungSpectrum_Cut->GetXaxis()->GetTitle());
  	E_Diff->GetYaxis()->SetTitle("Ratio");
  	E_Diff->GetXaxis()->SetTitleSize(0.17);
  	E_Diff->GetYaxis()->SetTitleSize(0.15);
  	E_Diff->GetXaxis()->SetTitleOffset(0.80);
  	E_Diff->GetYaxis()->SetTitleOffset(0.3);
  	E_Diff->SetLabelSize(0.13,"X");
  	E_Diff->SetLabelSize(0.07,"Y");
  	E_Diff->SetTitle("");
  	E_Diff->GetXaxis()->SetLabelOffset(0.010);
  	E_Diff->GetYaxis()->SetLabelOffset(0.015);
  	E_Diff->SetStats(kFALSE);
  	E_Diff->SetDirectory(0);
    	E_Diff->SetMinimum(0.997);    
	E_Diff->SetMaximum(1.003);

	string canvasname=CompareSpectrums->GetName();
	CompareSpectrums->Write();
	CompareSpectrums->Print((canvasname+".eps").c_str());
	CompareSpectrums->Print((canvasname+".C").c_str());
	CompareSpectrums->Close();

	TCanvas* Map_Canvas = new TCanvas("PhotonSource_Maps");
	Map_Canvas->Divide(1,2);
        Map_Canvas->cd(1);
        gPad->SetLogz();
        gPad->DrawFrame(-0.2,-0.2,0.2,0.2);
        TExec *ex1 = new TExec("ex1","gStyle->SetPaintTextFormat(\".0f\");");
        ex1->Draw();
	T->Draw("Reflectiony:Reflectionx >>+ rebinned_DeflFibre_Photon","Particle_ID==22","colz,TEXT");
	Map_Canvas->cd(2);
        gPad->SetTopMargin(0.15);
	MakeEnergyHistogram(T,EnergyMap_Photon_Deflection_Fibre,22);

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
 
                if (id!=ParticleID || theta>0.2865) continue;    
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

		if (id!=ParticleID || theta>0.2865) continue;	
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
	Hvalues_for_macros.open ("Hvalues_for_macros_AngleCut.txt");
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

