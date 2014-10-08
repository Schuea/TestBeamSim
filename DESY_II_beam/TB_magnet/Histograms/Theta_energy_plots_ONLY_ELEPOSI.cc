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
#include "TEllipse.h"
#include "TPaveStats.h"
#include "TProfile.h"
#include "TMath.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

using namespace std;

void DrawingMacro(string name); 

int main(int argc,char *argv[]){
	if(argc < 2 || argc > 2){
    		//explain how to use program	
    		cerr << "Type in root filename!" << endl;
    		cerr << "e.g. ./DrawHistograms file.root" << endl;
    		terminate();
	}	
	string filename;
	filename = argv[1];

       
	DrawingMacro(filename);
}

void DrawingMacro(string name){
	TH1::SetDefaultSumw2();

	TFile * input_rootfile = new TFile(name.c_str(),"READ");
	cout << "Inputfile size = " << input_rootfile->GetSize() << endl;
	TTree * T = (TTree*)input_rootfile->Get("Tree");
	cout << "Accessed TTree.." << endl;

	stringstream output_filename;
	output_filename << "theta_enegy_plots_" << name;
	TFile * output_rootfile = new TFile(output_filename.str().c_str(),"RECREATE");

	TH1F * MomentumHisto_ElePosi = new TH1F("p_ElectronsPositrons","Momentum distribution of electrons and positrons existing behind magnet",80,0,6.5);
	MomentumHisto_ElePosi->SetLineColor(kRed-6);
	MomentumHisto_ElePosi->GetXaxis()->SetTitle("Energy (GeV)");
	MomentumHisto_ElePosi->GetXaxis()->CenterTitle();

	TH1F * MomentumHisto_Protons = new TH1F("p_ProtonsNeutrons","Momentum distribution of protons, neutrons and photons existing behind magnet",93,0,6.5);
	MomentumHisto_Protons->SetLineColor(4);
	MomentumHisto_Protons->GetXaxis()->SetTitle("Energy (GeV)");
	MomentumHisto_Protons->GetXaxis()->CenterTitle();
	TH1F * MomentumHisto_Photons = new TH1F("p_Photons","Momentum distribution of protons, neutrons and photons existing behind magnet",93,0,6.5);
	MomentumHisto_Photons->SetLineColor(kGreen+2);
	MomentumHisto_Photons->GetXaxis()->SetTitle("Energy (GeV)");
	MomentumHisto_Photons->GetXaxis()->CenterTitle();

	TH1F * AngleHisto_Prot = new TH1F("AngleHisto_Prot","Angular distribution of protons and #pi^{+/-} behind magnet",70,0,180);
	AngleHisto_Prot->SetLineColor(4);
	AngleHisto_Prot->GetXaxis()->SetTitle("#theta (degree)");
	AngleHisto_Prot->GetXaxis()->CenterTitle();
	TH1F * AngleHisto_Pions = new TH1F("AngleHisto_Pions","Angular distribution of protons and #pi^{+/-} behind magnet",70,0,180);
	AngleHisto_Pions->SetLineColor(6);
	AngleHisto_Pions->GetXaxis()->SetTitle("#theta (degree)");
	AngleHisto_Pions->GetXaxis()->CenterTitle();

	float energy = 1.0;
	float energyinterval = 1.0;
	float max_angle = 6.0;
	float lower_limit = 0.996;
	vector< TH1F* > AngleHisto_ElePosi_;
	vector< TH1F* > cosTheta_histo_ElePosi_;

	vector< TCanvas* > Angle_Canvas_;
	vector< TPaveText* > AngleCutBox_;

	for(int i = 1; i < 8; ++i){
		if (energy <= 2.0){
			max_angle = 6.0;
			lower_limit = 0.996;
		}
                else if (energy > 2.0 && energy <=5.0 ){
			max_angle = 3.0;
			lower_limit = 0.9995;
		}
                else if (energy > 5.0 && energy < 7.0){
			max_angle = 2.0;
			energyinterval = 0.3;
			lower_limit = 0.9998;
		}

		stringstream histoname1_angle;
		histoname1_angle << "AngleHisto_ElePosi_" << i;
		stringstream histoname1_cosTheta;
		histoname1_cosTheta << "cosTheta_histo_ElePosi_" << i;

		stringstream histotitle1_angle;
		histotitle1_angle << "Angular distribution of electrons and positrons for an energy between " << energy -0.1 << " and " << energy + 0.1 << " GeV";
		stringstream histotitle1_cosTheta;
		histotitle1_cosTheta << "cos(#theta)-distribution of electrons and positrons for an energy between " << energy -0.1 << " and " << energy + 0.1 << " GeV";

		stringstream drawtext1_angle;
		drawtext1_angle << "Theta >>+ " << histoname1_angle.str();
		stringstream drawtext1_cosTheta;
		drawtext1_cosTheta << "cos(TMath::DegToRad()*Theta) >>+ " << histoname1_cosTheta.str();
	
		stringstream cuttext1;
		cuttext1 << "(Particle_ID==11 || Particle_ID==-11) && Energy > "<< energy-0.1 <<" && Energy < " << energy+0.1 << " && hasLeftDetector_Status==1";
	
		stringstream pavetext;
		pavetext << energy-0.1 << " GeV < Energy < " << energy+0.1 << " GeV";
		stringstream savetext1_angle, savetext2_angle;
		savetext1_angle << "DeflectionAngle_Magnet_" << energy << "GeV.eps";
		savetext2_angle << "DeflectionAngle_Magnet_" << energy << "GeV.C";

		TH1F* temphisto1_angle = new TH1F(histoname1_angle.str().c_str(), histotitle1_angle.str().c_str(),80,0,max_angle);
                AngleHisto_ElePosi_.push_back(temphisto1_angle);
	  	AngleHisto_ElePosi_.at(i-1)->SetLineColor(kRed-6);
		AngleHisto_ElePosi_.at(i-1)->GetXaxis()->SetTitle("#theta (degree)");
		AngleHisto_ElePosi_.at(i-1)->GetXaxis()->CenterTitle();
       
	       	TCanvas *tempcanvas_angle = new TCanvas();
		Angle_Canvas_.push_back(tempcanvas_angle);

                //---cos(theta) distributions:	
		TH1F* temphisto1_cosTheta = new TH1F(histoname1_cosTheta.str().c_str(), histotitle1_cosTheta.str().c_str(),100,lower_limit,1);
                cosTheta_histo_ElePosi_.push_back(temphisto1_cosTheta);
	  	cosTheta_histo_ElePosi_.at(i-1)->SetLineColor(kRed-6);
		cosTheta_histo_ElePosi_.at(i-1)->GetXaxis()->SetTitle("cos(#theta)");
		cosTheta_histo_ElePosi_.at(i-1)->GetXaxis()->CenterTitle();
       
		Angle_Canvas_.at(i-1)->Divide(1,2);
		Angle_Canvas_.at(i-1)->cd(1);
		gStyle->SetOptStat(11);
	
		T->Draw(drawtext1_angle.str().c_str(), cuttext1.str().c_str());
		gPad->Update();
        	TPaveStats *st1 = (TPaveStats*)AngleHisto_ElePosi_.at(i-1)->FindObject("stats");
        	double statboxsize = st1->GetY2NDC() - st1->GetY1NDC();
        	st1->SetTextColor(kRed-6);
        	st1->SetY1NDC(0.8);
        	st1->SetY2NDC(0.8+statboxsize);	

		TPaveText* tempbox = new TPaveText(.4,.86,.6,.93,"brNDC");
		AngleCutBox_.push_back(tempbox);
		AngleCutBox_.at(i-1)->AddText(pavetext.str().c_str());
		AngleCutBox_.at(i-1)->Draw();

		Angle_Canvas_.at(i-1)->cd(2);
		gStyle->SetOptStat(11);
	
		T->Draw(drawtext1_cosTheta.str().c_str(), cuttext1.str().c_str());
		gPad->Update();
        	TPaveStats *st1_cosTheta = (TPaveStats*)cosTheta_histo_ElePosi_.at(i-1)->FindObject("stats");
        	double statboxsize_cosTheta = st1_cosTheta->GetY2NDC() - st1_cosTheta->GetY1NDC();
        	st1_cosTheta->SetTextColor(kRed-6);
        	st1_cosTheta->SetY1NDC(0.8);
        	st1_cosTheta->SetY2NDC(0.8+statboxsize_cosTheta);	

		AngleCutBox_.at(i-1)->Draw();

		Angle_Canvas_.at(i-1)->Write();
		Angle_Canvas_.at(i-1)->Print(savetext1_angle.str().c_str());
		Angle_Canvas_.at(i-1)->Print(savetext2_angle.str().c_str());
		Angle_Canvas_.at(i-1)->Close();

		energy += energyinterval;
 		delete temphisto1_angle;
 		delete temphisto1_cosTheta;
		delete tempcanvas_angle;
		delete tempbox;
 }


	TCanvas * E_Canvas_ElePhotProt = new TCanvas("E_Canvas_ElePhotProt");	

	TLegend * E_Legend = new TLegend(0.6,0.63,0.7,0.73);
	E_Legend->SetFillColor(0);
	TLegend * ComparisonLegend = new TLegend(0.6,0.6,0.7,0.7);
	ComparisonLegend->SetFillColor(0);

	E_Canvas_ElePhotProt->Divide(2,2);
	E_Canvas_ElePhotProt->cd(1);
	T->Draw("Momentum >>+ p_ElectronsPositrons","(Particle_ID==11 || Particle_ID==-11) && hasLeftDetector_Status==1 && Reflectionx>-33 && Reflectionx<33 && Reflectiony>-33 && Reflectiony<33");
	gPad->Update();
       	TPaveStats *st1_ElePosi = (TPaveStats*)MomentumHisto_ElePosi->FindObject("stats");
       	st1_ElePosi->SetTextColor(kRed-6);

	E_Canvas_ElePhotProt->cd(2);
	gPad->SetLogy();
	T->Draw("Momentum >>+ p_Photons","Particle_ID==22 && hasLeftDetector_Status==1 && Reflectionx>-33 && Reflectionx<33 && Reflectiony>-33 && Reflectiony<33");
	MomentumHisto_Photons->SetMinimum(0.1);
	MomentumHisto_Photons->SetMaximum(10000500);
	gPad->Update();
       	TPaveStats *st1_PhotProt = (TPaveStats*)MomentumHisto_Photons->FindObject("stats");
       	double statboxsize_PhotProt = st1_PhotProt->GetY2NDC() - st1_PhotProt->GetY1NDC();
       	st1_PhotProt->SetTextColor(kGreen+2);
       	st1_PhotProt->SetY1NDC(0.8);
       	st1_PhotProt->SetY2NDC(0.8+statboxsize_PhotProt);	

	T->Draw("Momentum >>+ p_ProtonsNeutrons","(Particle_ID==2212 || Particle_ID==2112)&& hasLeftDetector_Status==1 && Reflectionx>-33 && Reflectionx<33 && Reflectiony>-33 && Reflectiony<33","SAMES");
	MomentumHisto_Protons->SetMinimum(0.1);
	MomentumHisto_Protons->SetMaximum(10000500);
	gPad->Update();
       	TPaveStats *st2_PhotProt = (TPaveStats*)MomentumHisto_Protons->FindObject("stats");
       	st2_PhotProt->SetTextColor(4);
      	st2_PhotProt->SetY2NDC(st1_PhotProt->GetY1NDC()); //new x start position
 	st2_PhotProt->SetY1NDC(st2_PhotProt->GetY2NDC()-statboxsize_PhotProt); //new x end position

	E_Legend->AddEntry(MomentumHisto_Photons,"#gamma");
	E_Legend->AddEntry(MomentumHisto_Protons,"p, n");
	E_Legend->Draw();
	
	E_Canvas_ElePhotProt->cd(3);
	gPad->SetLogy();
	T->Draw("Theta >>+ AngleHisto_Prot","Particle_ID==2212");
	gPad->Update();
       	TPaveStats *st1_ProtPions = (TPaveStats*)AngleHisto_Prot->FindObject("stats");
       	double statboxsize_ProtPions = st1_ProtPions->GetY2NDC() - st1_ProtPions->GetY1NDC();
       	st1_ProtPions->SetTextColor(4);
       	st1_ProtPions->SetY1NDC(0.8);
       	st1_ProtPions->SetY2NDC(0.8+statboxsize_ProtPions);	

	T->Draw("Theta >>+ AngleHisto_Pions","Particle_ID==211 || Particle_ID==-211","SAMES");
	gPad->Update();
       	TPaveStats *st2_ProtPions = (TPaveStats*)AngleHisto_Pions->FindObject("stats");
       	st2_ProtPions->SetTextColor(6);
      	st2_ProtPions->SetY2NDC(st1_ProtPions->GetY1NDC()); //new x start position
 	st2_ProtPions->SetY1NDC(st2_ProtPions->GetY2NDC()-statboxsize_ProtPions); //new x end position

	ComparisonLegend->AddEntry(AngleHisto_Prot,"p");
	ComparisonLegend->AddEntry(AngleHisto_Pions,"#pi^{+}, #pi");
	ComparisonLegend->Draw();

	string canvasname1 = E_Canvas_ElePhotProt->GetName();
	E_Canvas_ElePhotProt->Write();
	E_Canvas_ElePhotProt->Print((canvasname1+".eps").c_str());	
	E_Canvas_ElePhotProt->Print((canvasname1+".C").c_str());	
	E_Canvas_ElePhotProt->Close();

	
	output_rootfile->Write();
	input_rootfile->Close();
}
