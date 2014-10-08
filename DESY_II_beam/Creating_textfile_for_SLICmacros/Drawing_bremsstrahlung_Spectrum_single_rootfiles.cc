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

void bremsstrahlungSpectrum(string name1,string name2,string name3,string name4,string name5,string name6,string name7,string name8,string name9,string name10); 
float* Fill_RebinningArrays(const int nbins, const float min, const float max, const int dimensions);
void GetBinEnergyDistribution(TTree* T1,TTree* T2,TTree* T3,TTree* T4,TTree* T5,TTree* T6,TTree* T7,TTree* T8,TTree* T9,TTree* T10, TH2F* histo, const int ParticleID);
void MakeEnergyHistogram(TTree* T1,TTree* T2,TTree* T3,TTree* T4,TTree* T5,TTree* T6,TTree* T7,TTree* T8,TTree* T9,TTree* T10, TH2F* histo, const int ParticleID);

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

	TFile * input_rootfile1 = new TFile(name1.c_str(),"READ");
	cout << "Inputfile 1 size = " << input_rootfile1->GetSize() << endl;
	TTree * T1 = (TTree*)input_rootfile1->Get("Tree");
		cout << "Accessed TTree.." << endl;
	TFile * input_rootfile2 = new TFile(name2.c_str(),"READ");
	cout << "Inputfile 2 size = " << input_rootfile2->GetSize() << endl;
	TTree * T2 = (TTree*)input_rootfile2->Get("Tree");
		cout << "Accessed TTree.." << endl;
	TFile * input_rootfile3 = new TFile(name3.c_str(),"READ");
	cout << "Inputfile 3 size = " << input_rootfile3->GetSize() << endl;
	TTree * T3 = (TTree*)input_rootfile3->Get("Tree");
		cout << "Accessed TTree.." << endl;
	TFile * input_rootfile4 = new TFile(name4.c_str(),"READ");
	cout << "Inputfile 4 size = " << input_rootfile4->GetSize() << endl;
	TTree * T4 = (TTree*)input_rootfile4->Get("Tree");
		cout << "Accessed TTree.." << endl;
	TFile * input_rootfile5 = new TFile(name5.c_str(),"READ");
	cout << "Inputfile 5 size = " << input_rootfile5->GetSize() << endl;
	TTree * T5 = (TTree*)input_rootfile5->Get("Tree");
		cout << "Accessed TTree.." << endl;
	TFile * input_rootfile6 = new TFile(name6.c_str(),"READ");
	cout << "Inputfile 6 size = " << input_rootfile6->GetSize() << endl;
	TTree * T6 = (TTree*)input_rootfile6->Get("Tree");
		cout << "Accessed TTree.." << endl;
	TFile * input_rootfile7 = new TFile(name7.c_str(),"READ");
	cout << "Inputfile 7 size = " << input_rootfile7->GetSize() << endl;
	TTree * T7 = (TTree*)input_rootfile7->Get("Tree");
		cout << "Accessed TTree.." << endl;
	TFile * input_rootfile8 = new TFile(name8.c_str(),"READ");
	cout << "Inputfile 8 size = " << input_rootfile8->GetSize() << endl;
	TTree * T8 = (TTree*)input_rootfile8->Get("Tree");
		cout << "Accessed TTree.." << endl;
	TFile * input_rootfile9 = new TFile(name9.c_str(),"READ");
	cout << "Inputfile 9 size = " << input_rootfile9->GetSize() << endl;
	TTree * T9 = (TTree*)input_rootfile9->Get("Tree");
		cout << "Accessed TTree.." << endl;
	TFile * input_rootfile10 = new TFile(name10.c_str(),"READ");
	cout << "Inputfile 10 size = " << input_rootfile10->GetSize() << endl;
	TTree * T10 = (TTree*)input_rootfile10->Get("Tree");
		cout << "Accessed TTree.." << endl;

	stringstream output_filename;
	output_filename << "bremsstrahlungSpectrums_Fibre.root";
	TFile * output_rootfile = new TFile(output_filename.str().c_str(),"RECREATE");

	TH1F * bremsstrahlungSpectrum = new TH1F("Photon_spectrum","Energy distribution of bremsstrahlung photon source",150,0,7);
	bremsstrahlungSpectrum->SetLineColor(2);
	bremsstrahlungSpectrum->GetXaxis()->SetTitle("Energy (GeV)");
	bremsstrahlungSpectrum->GetXaxis()->CenterTitle();
	
	TH2F * Photon_Fluxmap = new TH2F("Photon_map","Position of bremsstrahlung photons",100,-0.05,0.05,100,-0.05,0.05);
	Photon_Fluxmap->GetXaxis()->SetTitle("x (mm)");
	Photon_Fluxmap->GetXaxis()->CenterTitle();
	Photon_Fluxmap->GetYaxis()->SetTitle("y (mm)");
	Photon_Fluxmap->GetYaxis()->CenterTitle();

	float *binning_array_x;
	float *binning_array_y;
	binning_array_x=Fill_RebinningArrays(/*21*/7,-0.002,0.002,2);
	binning_array_y=Fill_RebinningArrays(/*21*/7,-0.002,0.002,2);

	TH2F * rebinned_Photon_Deflection_Fibre = new TH2F("rebinned_DeflFibre_Photon","Deflection of photons created in fibre",/*21*/7,binning_array_x,/*21*/7,binning_array_y);
	rebinned_Photon_Deflection_Fibre->SetStats(kFALSE);
	rebinned_Photon_Deflection_Fibre->SetOption("colz,TEXT");
	rebinned_Photon_Deflection_Fibre->GetXaxis()->SetTitle("x (mm)");
	rebinned_Photon_Deflection_Fibre->GetXaxis()->CenterTitle();
	rebinned_Photon_Deflection_Fibre->GetYaxis()->SetTitle("y (mm)");
	rebinned_Photon_Deflection_Fibre->GetYaxis()->CenterTitle();

	TH2F* EnergyMap_Photon_Deflection_Fibre = new TH2F("EnergyMap_DeflFibre_Photon","Deflection of photons created in fibre and their average energy",/*21*/7,binning_array_x,/*21*/7,binning_array_y);
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
	T1->Draw("Energy >>+ Photon_spectrum","Particle_ID==22 && hasLeftDetector_Status==1");
	T2->Draw("Energy >>+ Photon_spectrum","Particle_ID==22 && hasLeftDetector_Status==1","SAME");
	T3->Draw("Energy >>+ Photon_spectrum","Particle_ID==22 && hasLeftDetector_Status==1","SAME");
	T4->Draw("Energy >>+ Photon_spectrum","Particle_ID==22 && hasLeftDetector_Status==1","SAME");
	T5->Draw("Energy >>+ Photon_spectrum","Particle_ID==22 && hasLeftDetector_Status==1","SAME");
	T6->Draw("Energy >>+ Photon_spectrum","Particle_ID==22 && hasLeftDetector_Status==1","SAME");
	T7->Draw("Energy >>+ Photon_spectrum","Particle_ID==22 && hasLeftDetector_Status==1","SAME");
	T8->Draw("Energy >>+ Photon_spectrum","Particle_ID==22 && hasLeftDetector_Status==1","SAME");
	T9->Draw("Energy >>+ Photon_spectrum","Particle_ID==22 && hasLeftDetector_Status==1","SAME");
	T10->Draw("Energy >>+ Photon_spectrum","Particle_ID==22 && hasLeftDetector_Status==1","SAME");
	gPad->Update();
 	TPaveStats *st1 = (TPaveStats*)bremsstrahlungSpectrum->FindObject("stats");
  	double statboxsize = st1->GetY2NDC() - st1->GetY1NDC();
 	st1->SetTextColor(2);
  	st1->SetY1NDC(0.8);
  	st1->SetY2NDC(0.8+statboxsize);	

	Spectrum_Canvas->cd(2);
	gPad->SetLogz();
	gStyle->SetNumberContours(999);
	gStyle->SetOptStat(11);
	T1->Draw("Reflectiony:Reflectionx >>+ Photon_map","Particle_ID==22 && hasLeftDetector_Status==1","colz");
	T2->Draw("Reflectiony:Reflectionx >>+ Photon_map","Particle_ID==22 && hasLeftDetector_Status==1","colz,SAME");
	T3->Draw("Reflectiony:Reflectionx >>+ Photon_map","Particle_ID==22 && hasLeftDetector_Status==1","colz,SAME");
	T4->Draw("Reflectiony:Reflectionx >>+ Photon_map","Particle_ID==22 && hasLeftDetector_Status==1","colz,SAME");
	T5->Draw("Reflectiony:Reflectionx >>+ Photon_map","Particle_ID==22 && hasLeftDetector_Status==1","colz,SAME");
	T6->Draw("Reflectiony:Reflectionx >>+ Photon_map","Particle_ID==22 && hasLeftDetector_Status==1","colz,SAME");
	T7->Draw("Reflectiony:Reflectionx >>+ Photon_map","Particle_ID==22 && hasLeftDetector_Status==1","colz,SAME");
	T8->Draw("Reflectiony:Reflectionx >>+ Photon_map","Particle_ID==22 && hasLeftDetector_Status==1","colz,SAME");
	T9->Draw("Reflectiony:Reflectionx >>+ Photon_map","Particle_ID==22 && hasLeftDetector_Status==1","colz,SAME");
	T10->Draw("Reflectiony:Reflectionx >>+ Photon_map","Particle_ID==22 && hasLeftDetector_Status==1","colz,SAME");

	string canvasname=Spectrum_Canvas->GetName();
	Spectrum_Canvas->Write();
	Spectrum_Canvas->Print((canvasname+".eps").c_str());
	Spectrum_Canvas->Print((canvasname+".C").c_str());
	Spectrum_Canvas->Close();



	TCanvas* Map_Canvas = new TCanvas("PhotonSource_Maps");

	Map_Canvas->Divide(1,2);
        Map_Canvas->cd(1);
        gPad->SetLogz();
        gPad->DrawFrame(-0.002,-0.002,0.002,0.002);
        TExec *ex1 = new TExec("ex1","gStyle->SetPaintTextFormat(\".0f\");");
        ex1->Draw();
	T1->Draw("Reflectiony:Reflectionx >>+ rebinned_DeflFibre_Photon","Particle_ID==22 && hasLeftDetector_Status==1","colz,TEXT");
	T2->Draw("Reflectiony:Reflectionx >>+ rebinned_DeflFibre_Photon","Particle_ID==22 && hasLeftDetector_Status==1","SAME,colz,TEXT");
	T3->Draw("Reflectiony:Reflectionx >>+ rebinned_DeflFibre_Photon","Particle_ID==22 && hasLeftDetector_Status==1","SAME,colz,TEXT");
	T4->Draw("Reflectiony:Reflectionx >>+ rebinned_DeflFibre_Photon","Particle_ID==22 && hasLeftDetector_Status==1","SAME,colz,TEXT");
	T5->Draw("Reflectiony:Reflectionx >>+ rebinned_DeflFibre_Photon","Particle_ID==22 && hasLeftDetector_Status==1","SAME,colz,TEXT");
	T6->Draw("Reflectiony:Reflectionx >>+ rebinned_DeflFibre_Photon","Particle_ID==22 && hasLeftDetector_Status==1","SAME,colz,TEXT");
	T7->Draw("Reflectiony:Reflectionx >>+ rebinned_DeflFibre_Photon","Particle_ID==22 && hasLeftDetector_Status==1","SAME,colz,TEXT");
	T8->Draw("Reflectiony:Reflectionx >>+ rebinned_DeflFibre_Photon","Particle_ID==22 && hasLeftDetector_Status==1","SAME,colz,TEXT");
	T9->Draw("Reflectiony:Reflectionx >>+ rebinned_DeflFibre_Photon","Particle_ID==22 && hasLeftDetector_Status==1","SAME,colz,TEXT");
	T10->Draw("Reflectiony:Reflectionx >>+ rebinned_DeflFibre_Photon","Particle_ID==22 && hasLeftDetector_Status==1","SAME,colz,TEXT");
	Map_Canvas->cd(2);
        gPad->SetTopMargin(0.15);
	MakeEnergyHistogram(T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,EnergyMap_Photon_Deflection_Fibre,22);

	string canvasname2=Map_Canvas->GetName();
        Map_Canvas->Write();
        Map_Canvas->Print((canvasname2+".eps").c_str());
        Map_Canvas->Print((canvasname2+".C").c_str());
        Map_Canvas->Close();

//	GetBinEnergyDistribution(T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,rebinned_Photon_Deflection_Fibre,22);

	output_rootfile->Write();
	input_rootfile1->Close();
	input_rootfile2->Close();
	input_rootfile3->Close();
	input_rootfile4->Close();
	input_rootfile5->Close();
	input_rootfile6->Close();
	input_rootfile7->Close();
	input_rootfile8->Close();
	input_rootfile9->Close();
	input_rootfile10->Close();

	delete output_rootfile, input_rootfile1,input_rootfile2,input_rootfile3,input_rootfile4,input_rootfile5,input_rootfile6,input_rootfile7,input_rootfile8,input_rootfile9,input_rootfile10;
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

void MakeEnergyHistogram(TTree *T1,TTree* T2,TTree* T3,TTree* T4,TTree* T5,TTree* T6,TTree* T7,TTree* T8,TTree* T9,TTree* T10, TH2F *histo, const int ParticleID){ 
        int id1,id2,id3,id4,id5,id6,id7,id8,id9,id10=0; 
	float energy1,energy2,energy3,energy4,energy5,energy6,energy7,energy8,energy9,energy10=0; 
	float x1,x2,x3,x4,x5,x6,x7,x8,x9,x10=0; 
	float y1,y2,y3,y4,y5,y6,y7,y8,y9,y10=0; 
        int binno=0; 
         
T1->SetBranchAddress("Particle_ID",&id1); 
        T1->SetBranchAddress("Energy",&energy1); 
        T1->SetBranchAddress("Reflectionx",&x1); 
        T1->SetBranchAddress("Reflectiony",&y1); 
T2->SetBranchAddress("Particle_ID",&id2); 
        T2->SetBranchAddress("Energy",&energy2); 
        T2->SetBranchAddress("Reflectionx",&x2); 
        T2->SetBranchAddress("Reflectiony",&y2); 
T3->SetBranchAddress("Particle_ID",&id3); 
        T3->SetBranchAddress("Energy",&energy3); 
        T3->SetBranchAddress("Reflectionx",&x3); 
        T3->SetBranchAddress("Reflectiony",&y3); 
T4->SetBranchAddress("Particle_ID",&id4); 
        T4->SetBranchAddress("Energy",&energy4); 
        T4->SetBranchAddress("Reflectionx",&x4); 
        T4->SetBranchAddress("Reflectiony",&y4); 
T5->SetBranchAddress("Particle_ID",&id5); 
        T5->SetBranchAddress("Energy",&energy5); 
        T5->SetBranchAddress("Reflectionx",&x5); 
        T5->SetBranchAddress("Reflectiony",&y5); 
T6->SetBranchAddress("Particle_ID",&id6); 
        T6->SetBranchAddress("Energy",&energy6); 
        T6->SetBranchAddress("Reflectionx",&x6); 
        T6->SetBranchAddress("Reflectiony",&y6); 
T7->SetBranchAddress("Particle_ID",&id7); 
        T7->SetBranchAddress("Energy",&energy7); 
        T7->SetBranchAddress("Reflectionx",&x7); 
        T7->SetBranchAddress("Reflectiony",&y7); 
T8->SetBranchAddress("Particle_ID",&id8); 
        T8->SetBranchAddress("Energy",&energy8); 
        T8->SetBranchAddress("Reflectionx",&x8); 
        T8->SetBranchAddress("Reflectiony",&y8); 
T9->SetBranchAddress("Particle_ID",&id9); 
        T9->SetBranchAddress("Energy",&energy9); 
        T9->SetBranchAddress("Reflectionx",&x9); 
        T9->SetBranchAddress("Reflectiony",&y9); 
T10->SetBranchAddress("Particle_ID",&id10); 
        T10->SetBranchAddress("Energy",&energy10); 
        T10->SetBranchAddress("Reflectionx",&x10); 
        T10->SetBranchAddress("Reflectiony",&y10); 

        int entries1 = T1->GetEntries(); 
        int entries2 = T2->GetEntries(); 
        int entries3 = T3->GetEntries(); 
        int entries4 = T4->GetEntries(); 
        int entries5 = T5->GetEntries(); 
        int entries6 = T6->GetEntries(); 
        int entries7 = T7->GetEntries(); 
        int entries8 = T8->GetEntries(); 
        int entries9 = T9->GetEntries(); 
        int entries10 = T10->GetEntries(); 
        std::map<int, vector< float > > tempmap; 
 
        for(int i = 0; i < entries1; ++i){ 
                T1->GetEntry(i); 
                if (id1!=ParticleID) continue;    
                if (histo->GetXaxis()->GetXmin()<=x1 && x1<=histo->GetXaxis()->GetXmax() && histo->GetYaxis()->GetXmin()<=y1 && y1<=histo->GetYaxis()->GetXmax()){ 
                        binno = histo->FindBin(x1,y1); 
                        tempmap[binno].push_back(energy1); 
                } 
        }
        for(int i = 0; i < entries2; ++i){ 
                T2->GetEntry(i); 
                if (id2!=ParticleID) continue;    
                if (histo->GetXaxis()->GetXmin()<=x2 && x2<=histo->GetXaxis()->GetXmax() && histo->GetYaxis()->GetXmin()<=y2 && y2<=histo->GetYaxis()->GetXmax()){ 
                        binno = histo->FindBin(x2,y2); 
                        tempmap[binno].push_back(energy2); 
                } 
        }
        for(int i = 0; i < entries3; ++i){ 
                T3->GetEntry(i); 
                if (id3!=ParticleID) continue;    
                if (histo->GetXaxis()->GetXmin()<=x3 && x3<=histo->GetXaxis()->GetXmax() && histo->GetYaxis()->GetXmin()<=y3 && y3<=histo->GetYaxis()->GetXmax()){ 
                        binno = histo->FindBin(x3,y3); 
                        tempmap[binno].push_back(energy3); 
                } 
        }
        for(int i = 0; i < entries4; ++i){ 
                T4->GetEntry(i); 
                if (id4!=ParticleID) continue;    
                if (histo->GetXaxis()->GetXmin()<=x4 && x4<=histo->GetXaxis()->GetXmax() && histo->GetYaxis()->GetXmin()<=y4 && y4<=histo->GetYaxis()->GetXmax()){ 
                        binno = histo->FindBin(x4,y4); 
                        tempmap[binno].push_back(energy4); 
                } 
        }
        for(int i = 0; i < entries5; ++i){ 
                T5->GetEntry(i); 
                if (id5!=ParticleID) continue;    
                if (histo->GetXaxis()->GetXmin()<=x5 && x5<=histo->GetXaxis()->GetXmax() && histo->GetYaxis()->GetXmin()<=y5 && y5<=histo->GetYaxis()->GetXmax()){ 
                        binno = histo->FindBin(x5,y5); 
                        tempmap[binno].push_back(energy5); 
                } 
        }
        for(int i = 0; i < entries6; ++i){ 
                T6->GetEntry(i); 
                if (id6!=ParticleID) continue;    
                if (histo->GetXaxis()->GetXmin()<=x6 && x6<=histo->GetXaxis()->GetXmax() && histo->GetYaxis()->GetXmin()<=y6 && y6<=histo->GetYaxis()->GetXmax()){ 
                        binno = histo->FindBin(x6,y6); 
                        tempmap[binno].push_back(energy6); 
                } 
        }
        for(int i = 0; i < entries7; ++i){ 
                T7->GetEntry(i); 
                if (id7!=ParticleID) continue;    
                if (histo->GetXaxis()->GetXmin()<=x7 && x7<=histo->GetXaxis()->GetXmax() && histo->GetYaxis()->GetXmin()<=y7 && y7<=histo->GetYaxis()->GetXmax()){ 
                        binno = histo->FindBin(x7,y7); 
                        tempmap[binno].push_back(energy7); 
                } 
        }
        for(int i = 0; i < entries8; ++i){ 
                T8->GetEntry(i); 
                if (id8!=ParticleID) continue;    
                if (histo->GetXaxis()->GetXmin()<=x8 && x8<=histo->GetXaxis()->GetXmax() && histo->GetYaxis()->GetXmin()<=y8 && y8<=histo->GetYaxis()->GetXmax()){ 
                        binno = histo->FindBin(x8,y8); 
                        tempmap[binno].push_back(energy8); 
                } 
        }
        for(int i = 0; i < entries9; ++i){ 
                T9->GetEntry(i); 
                if (id9!=ParticleID) continue;    
                if (histo->GetXaxis()->GetXmin()<=x9 && x9<=histo->GetXaxis()->GetXmax() && histo->GetYaxis()->GetXmin()<=y9 && y9<=histo->GetYaxis()->GetXmax()){ 
                        binno = histo->FindBin(x9,y9); 
                        tempmap[binno].push_back(energy9); 
                } 
        }
        for(int i = 0; i < entries10; ++i){ 
                T10->GetEntry(i); 
                if (id10!=ParticleID) continue;    
                if (histo->GetXaxis()->GetXmin()<=x10 && x10<=histo->GetXaxis()->GetXmax() && histo->GetYaxis()->GetXmin()<=y10 && y10<=histo->GetYaxis()->GetXmax()){ 
                        binno = histo->FindBin(x10,y10); 
                        tempmap[binno].push_back(energy10); 
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


void GetBinEnergyDistribution(TTree* T1,TTree* T2,TTree* T3,TTree* T4,TTree* T5,TTree* T6,TTree* T7,TTree* T8,TTree* T9,TTree* T10, TH2F* histo, const int ParticleID){

 	int id1,id2,id3,id4,id5,id6,id7,id8,id9,id10=0; 
	float energy1,energy2,energy3,energy4,energy5,energy6,energy7,energy8,energy9,energy10=0; 
	float x1,x2,x3,x4,x5,x6,x7,x8,x9,x10=0; 
	float y1,y2,y3,y4,y5,y6,y7,y8,y9,y10=0; 
        int binno=0; 
         
T1->SetBranchAddress("Particle_ID",&id1); 
        T1->SetBranchAddress("Energy",&energy1); 
        T1->SetBranchAddress("Reflectionx",&x1); 
        T1->SetBranchAddress("Reflectiony",&y1); 
T2->SetBranchAddress("Particle_ID",&id2); 
        T2->SetBranchAddress("Energy",&energy2); 
        T2->SetBranchAddress("Reflectionx",&x2); 
        T2->SetBranchAddress("Reflectiony",&y2); 
T3->SetBranchAddress("Particle_ID",&id3); 
        T3->SetBranchAddress("Energy",&energy3); 
        T3->SetBranchAddress("Reflectionx",&x3); 
        T3->SetBranchAddress("Reflectiony",&y3); 
T4->SetBranchAddress("Particle_ID",&id4); 
        T4->SetBranchAddress("Energy",&energy4); 
        T4->SetBranchAddress("Reflectionx",&x4); 
        T4->SetBranchAddress("Reflectiony",&y4); 
T5->SetBranchAddress("Particle_ID",&id5); 
        T5->SetBranchAddress("Energy",&energy5); 
        T5->SetBranchAddress("Reflectionx",&x5); 
        T5->SetBranchAddress("Reflectiony",&y5); 
T6->SetBranchAddress("Particle_ID",&id6); 
        T6->SetBranchAddress("Energy",&energy6); 
        T6->SetBranchAddress("Reflectionx",&x6); 
        T6->SetBranchAddress("Reflectiony",&y6); 
T7->SetBranchAddress("Particle_ID",&id7); 
        T7->SetBranchAddress("Energy",&energy7); 
        T7->SetBranchAddress("Reflectionx",&x7); 
        T7->SetBranchAddress("Reflectiony",&y7); 
T8->SetBranchAddress("Particle_ID",&id8); 
        T8->SetBranchAddress("Energy",&energy8); 
        T8->SetBranchAddress("Reflectionx",&x8); 
        T8->SetBranchAddress("Reflectiony",&y8); 
T9->SetBranchAddress("Particle_ID",&id9); 
        T9->SetBranchAddress("Energy",&energy9); 
        T9->SetBranchAddress("Reflectionx",&x9); 
        T9->SetBranchAddress("Reflectiony",&y9); 
T10->SetBranchAddress("Particle_ID",&id10); 
        T10->SetBranchAddress("Energy",&energy10); 
        T10->SetBranchAddress("Reflectionx",&x10); 
        T10->SetBranchAddress("Reflectiony",&y10); 

        int entries1 = T1->GetEntries(); 
        int entries2 = T2->GetEntries(); 
        int entries3 = T3->GetEntries(); 
        int entries4 = T4->GetEntries(); 
        int entries5 = T5->GetEntries(); 
        int entries6 = T6->GetEntries(); 
        int entries7 = T7->GetEntries(); 
        int entries8 = T8->GetEntries(); 
        int entries9 = T9->GetEntries(); 
        int entries10 = T10->GetEntries(); 

	
	std::map<int, vector< float > > tempmap;
	
        for(int i = 0; i < entries1; ++i){ 
                T1->GetEntry(i); 
                if (id1!=ParticleID) continue;    
                if (histo->GetXaxis()->GetXmin()<=x1 && x1<=histo->GetXaxis()->GetXmax() && histo->GetYaxis()->GetXmin()<=y1 && y1<=histo->GetYaxis()->GetXmax()){ 
                        binno = histo->FindBin(x1,y1); 
                        tempmap[binno].push_back(energy1); 
                } 
        }
        for(int i = 0; i < entries2; ++i){ 
                T2->GetEntry(i); 
                if (id2!=ParticleID) continue;    
                if (histo->GetXaxis()->GetXmin()<=x2 && x2<=histo->GetXaxis()->GetXmax() && histo->GetYaxis()->GetXmin()<=y2 && y2<=histo->GetYaxis()->GetXmax()){ 
                        binno = histo->FindBin(x2,y2); 
                        tempmap[binno].push_back(energy2); 
                } 
        }
        for(int i = 0; i < entries3; ++i){ 
                T3->GetEntry(i); 
                if (id3!=ParticleID) continue;    
                if (histo->GetXaxis()->GetXmin()<=x3 && x3<=histo->GetXaxis()->GetXmax() && histo->GetYaxis()->GetXmin()<=y3 && y3<=histo->GetYaxis()->GetXmax()){ 
                        binno = histo->FindBin(x3,y3); 
                        tempmap[binno].push_back(energy3); 
                } 
        }
        for(int i = 0; i < entries4; ++i){ 
                T4->GetEntry(i); 
                if (id4!=ParticleID) continue;    
                if (histo->GetXaxis()->GetXmin()<=x4 && x4<=histo->GetXaxis()->GetXmax() && histo->GetYaxis()->GetXmin()<=y4 && y4<=histo->GetYaxis()->GetXmax()){ 
                        binno = histo->FindBin(x4,y4); 
                        tempmap[binno].push_back(energy4); 
                } 
        }
        for(int i = 0; i < entries5; ++i){ 
                T5->GetEntry(i); 
                if (id5!=ParticleID) continue;    
                if (histo->GetXaxis()->GetXmin()<=x5 && x5<=histo->GetXaxis()->GetXmax() && histo->GetYaxis()->GetXmin()<=y5 && y5<=histo->GetYaxis()->GetXmax()){ 
                        binno = histo->FindBin(x5,y5); 
                        tempmap[binno].push_back(energy5); 
                } 
        }
        for(int i = 0; i < entries6; ++i){ 
                T6->GetEntry(i); 
                if (id6!=ParticleID) continue;    
                if (histo->GetXaxis()->GetXmin()<=x6 && x6<=histo->GetXaxis()->GetXmax() && histo->GetYaxis()->GetXmin()<=y6 && y6<=histo->GetYaxis()->GetXmax()){ 
                        binno = histo->FindBin(x6,y6); 
                        tempmap[binno].push_back(energy6); 
                } 
        }
        for(int i = 0; i < entries7; ++i){ 
                T7->GetEntry(i); 
                if (id7!=ParticleID) continue;    
                if (histo->GetXaxis()->GetXmin()<=x7 && x7<=histo->GetXaxis()->GetXmax() && histo->GetYaxis()->GetXmin()<=y7 && y7<=histo->GetYaxis()->GetXmax()){ 
                        binno = histo->FindBin(x7,y7); 
                        tempmap[binno].push_back(energy7); 
                } 
        }
        for(int i = 0; i < entries8; ++i){ 
                T8->GetEntry(i); 
                if (id8!=ParticleID) continue;    
                if (histo->GetXaxis()->GetXmin()<=x8 && x8<=histo->GetXaxis()->GetXmax() && histo->GetYaxis()->GetXmin()<=y8 && y8<=histo->GetYaxis()->GetXmax()){ 
                        binno = histo->FindBin(x8,y8); 
                        tempmap[binno].push_back(energy8); 
                } 
        }
        for(int i = 0; i < entries9; ++i){ 
                T9->GetEntry(i); 
                if (id9!=ParticleID) continue;    
                if (histo->GetXaxis()->GetXmin()<=x9 && x9<=histo->GetXaxis()->GetXmax() && histo->GetYaxis()->GetXmin()<=y9 && y9<=histo->GetYaxis()->GetXmax()){ 
                        binno = histo->FindBin(x9,y9); 
                        tempmap[binno].push_back(energy9); 
                } 
        }
        for(int i = 0; i < entries10; ++i){ 
                T10->GetEntry(i); 
                if (id10!=ParticleID) continue;    
                if (histo->GetXaxis()->GetXmin()<=x10 && x10<=histo->GetXaxis()->GetXmax() && histo->GetYaxis()->GetXmin()<=y10 && y10<=histo->GetYaxis()->GetXmax()){ 
                        binno = histo->FindBin(x10,y10); 
                        tempmap[binno].push_back(energy10); 
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

