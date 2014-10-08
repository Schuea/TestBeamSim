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

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

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
	output_filename << "histograms_" << name;
	TFile * output_rootfile = new TFile(output_filename.str().c_str(),"RECREATE");

	TH1F * EnergyHisto_Electrons = new TH1F("E_Electrons","Energy distribution of electrons existing behind magnet",80,0,6.5);
	EnergyHisto_Electrons->SetLineColor(4);
	EnergyHisto_Electrons->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_Electrons->GetXaxis()->CenterTitle();

	TH1F * EnergyHisto_Protons = new TH1F("E_Protons","Energy distribution of protons and photons existing behind magnet",93,0,6.5);
	EnergyHisto_Protons->SetLineColor(4);
	EnergyHisto_Protons->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_Protons->GetXaxis()->CenterTitle();
	TH1F * EnergyHisto_Photons = new TH1F("E_Photons","Energy distribution of protons and photons existing behind magnet",93,0,6.5);
	EnergyHisto_Photons->SetLineColor(2);
	EnergyHisto_Photons->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_Photons->GetXaxis()->CenterTitle();

	TH2F * Position_negCharge = new TH2F("Posi_negCharge","Position of particles with negative charge behind magnet",200,-600,600,200,-600,600);
	Position_negCharge->GetXaxis()->SetTitle("x (mm)");
	Position_negCharge->GetXaxis()->CenterTitle();
	Position_negCharge->GetYaxis()->SetTitle("y (mm)");
	Position_negCharge->GetYaxis()->CenterTitle();
	TH2F * Position_posCharge = new TH2F("Posi_posCharge","Position of particles with positive charge behind magnet",200,-600,600,200,-600,600);
	Position_posCharge->GetXaxis()->SetTitle("x (mm)");
	Position_posCharge->GetXaxis()->CenterTitle();
	Position_posCharge->GetYaxis()->SetTitle("y (mm)");
	Position_posCharge->GetYaxis()->CenterTitle();
	TH2F * Position_noCharge = new TH2F("Posi_noCharge","Position of neutral particles behind magnet",200,-600,600,200,-600,600);
	Position_noCharge->GetXaxis()->SetTitle("x (mm)");
	Position_noCharge->GetXaxis()->CenterTitle();
	Position_noCharge->GetYaxis()->SetTitle("y (mm)");
	Position_noCharge->GetYaxis()->CenterTitle();

	TH1F * AngleHisto_Prot = new TH1F("AngleHisto_Prot","Angular distribution of protons and #pi^{+} behind magnet",70,0,180);
	AngleHisto_Prot->SetLineColor(4);
	AngleHisto_Prot->GetXaxis()->SetTitle("#theta (degree)");
	AngleHisto_Prot->GetXaxis()->CenterTitle();
	TH1F * AngleHisto_Pions = new TH1F("AngleHisto_Pions","Angular distribution of protons and #pi^{+} behind magnet",70,0,180);
	AngleHisto_Pions->SetLineColor(6);
	AngleHisto_Pions->GetXaxis()->SetTitle("#theta (degree)");
	AngleHisto_Pions->GetXaxis()->CenterTitle();

	float energy = 1.0;
	float energyinterval = 1.0;
	float max_angle = 6.0;
	float max_angle2 = 6.0;
	vector< TH1F* > AngleHisto_ElePosiPions_;
	vector< TH1F* > AngleHisto_PhotPionsNeut_;
	vector< TCanvas* > Angle_Canvas_;
	vector< TPaveText* > AngleCutBox_;

	for(int i = 1; i < 8; ++i){
		if (energy <= 2.0){
			max_angle = 6.0;
			max_angle2 = 6.0;
		}
                else if (energy > 2.0 && energy <=5.0 ){
			max_angle = 3.0;
			max_angle2 = 3.0;
		}
                else if (energy > 5.0 && energy < 7.0){
			max_angle = 2.0;
			max_angle2 = 0.5;
			energyinterval = 0.3;
		}

		stringstream histoname1,histoname2;
		stringstream histotitle1,histotitle2;
		stringstream drawtext1,drawtext2;
		stringstream cuttext1,cuttext2;
		stringstream histotitle,pavetext;
		stringstream savetext1, savetext2;
                histoname1 << "AngleHisto_ElePosiPions_" << i;
                histoname2 << "AngleHisto_PhotPionsNeut_" << i;
		histotitle1 << "Angular distribution of e^{-}, e^{+} and #pi^{-}, for an energy between " << energy -0.1 << " and " << energy + 0.1;
		histotitle2 << "Angular distribution of #gamma, #pi^{0} and neutrons, for an energy between " << energy -0.1 << " and " << energy + 0.1;
		drawtext1 << "Theta >>+ " << histoname1.str();
		drawtext2 << "Theta >>+ " << histoname2.str();
		cuttext1 << "(Particle_ID==11 || Particle_ID==-11 || Particle_ID==-211) && Energy > "<< energy-0.1 <<" && Energy < " << energy+0.1 << " && hasLeftDetector_Status==1";
		cuttext2 << "(Particle_ID==22 || Particle_ID==2112 || Particle_ID==111) && Energy > "<< energy-0.1 <<" && Energy < " << energy+0.1 << " && hasLeftDetector_Status==1";
		pavetext << energy-0.1 << " GeV < Energy < " << energy+0.1 << " GeV";
		savetext1 << "DeflectionAngle_Magnet_" << energy << "GeV.eps";
		savetext2 << "DeflectionAngle_Magnet_" << energy << "GeV.C";

		TH1F* temphisto1 = new TH1F(histoname1.str().c_str(), histotitle1.str().c_str(),80,0,max_angle);
		TH1F* temphisto2 = new TH1F(histoname2.str().c_str(), histotitle2.str().c_str(),80,0,max_angle2);
                AngleHisto_ElePosiPions_.push_back(temphisto1);
                AngleHisto_PhotPionsNeut_.push_back(temphisto2);
//cout << __LINE__<<endl;              
	  	AngleHisto_ElePosiPions_.at(i-1)->SetLineColor(2);
		AngleHisto_ElePosiPions_.at(i-1)->GetXaxis()->SetTitle("#theta (degree)");
		AngleHisto_ElePosiPions_.at(i-1)->GetXaxis()->CenterTitle();
		AngleHisto_PhotPionsNeut_.at(i-1)->SetLineColor(4);
		AngleHisto_PhotPionsNeut_.at(i-1)->GetXaxis()->SetTitle("#theta (degree)");
		AngleHisto_PhotPionsNeut_.at(i-1)->GetXaxis()->CenterTitle();
	
		TCanvas *tempcanvas = new TCanvas();
		Angle_Canvas_.push_back(tempcanvas);
		Angle_Canvas_.at(i-1)->Divide(1,2);
		Angle_Canvas_.at(i-1)->cd(1);
		gStyle->SetOptStat(11);
		gPad->SetLogy();
	
		T->Draw(drawtext1.str().c_str(), cuttext1.str().c_str());
		gPad->Update();
        	TPaveStats *st1 = (TPaveStats*)AngleHisto_ElePosiPions_.at(i-1)->FindObject("stats");
        	double statboxsize = st1->GetY2NDC() - st1->GetY1NDC();
        	st1->SetTextColor(2);
        	st1->SetY1NDC(0.8);
        	st1->SetY2NDC(0.8+statboxsize);	

		TPaveText* tempbox = new TPaveText(.4,.81,.6,.88,"brNDC");
		AngleCutBox_.push_back(tempbox);
		AngleCutBox_.at(i-1)->AddText(pavetext.str().c_str());
		AngleCutBox_.at(i-1)->Draw();

		Angle_Canvas_.at(i-1)->cd(2);
		gStyle->SetOptStat(11);
		gPad->SetLogy();

		T->Draw(drawtext2.str().c_str(), cuttext2.str().c_str());
		gPad->Update();
        	TPaveStats *st2 = (TPaveStats*)AngleHisto_PhotPionsNeut_.at(i-1)->FindObject("stats");
        	st2->SetTextColor(4);
         	st2->SetY1NDC(0.8);
        	st2->SetY2NDC(0.8+statboxsize);	
	
		AngleCutBox_.at(i-1)->Draw();
/*
		TLegend * AngleLegend = new TLegend(0.75,0.4,0.85,0.5);
		AngleLegend->SetFillColor(0);
		AngleLegend->AddEntry(AngleHisto_ElePosiPions_.at(i-1),"e^{-}, e^{+}, #pi^{-}");
		AngleLegend->AddEntry(AngleHisto_PhotPionsNeut_.at(i-1),"#gamma, #pi^{0}, n");
//		AngleLegend->AddEntry(AngleHisto_ProtPions_.at(i-1),"p, #pi^{+}");
		AngleLegend->Draw();	
*/
		Angle_Canvas_.at(i-1)->Write();
		Angle_Canvas_.at(i-1)->Print(savetext1.str().c_str());
		Angle_Canvas_.at(i-1)->Print(savetext2.str().c_str());
		Angle_Canvas_.at(i-1)->Close();
	
		energy += energyinterval;
        }


	TCanvas * E_Canvas_ElePhotProt = new TCanvas("E_Canvas_ElePhotProt");	
	TCanvas * Position_Canvas = new TCanvas("deflected_Position_Canvas");

	TLegend * E_Legend = new TLegend(0.6,0.63,0.7,0.73);
	E_Legend->SetFillColor(0);
	TLegend * ComparisonLegend = new TLegend(0.6,0.6,0.7,0.7);
	ComparisonLegend->SetFillColor(0);

	E_Canvas_ElePhotProt->Divide(2,2);
	E_Canvas_ElePhotProt->cd(1);
	gPad->SetLogy();
	T->Draw("Energy >>+ E_Electrons","Particle_ID==11 && hasLeftDetector_Status==1");

	E_Canvas_ElePhotProt->cd(2);
	gPad->SetLogy();
	T->Draw("Energy >>+ E_Photons","Particle_ID==22 && hasLeftDetector_Status==1");
	EnergyHisto_Photons->SetMinimum(0.1);
	EnergyHisto_Photons->SetMaximum(10000500);
	gPad->Update();
       	TPaveStats *st1_PhotProt = (TPaveStats*)EnergyHisto_Photons->FindObject("stats");
       	double statboxsize_PhotProt = st1_PhotProt->GetY2NDC() - st1_PhotProt->GetY1NDC();
       	st1_PhotProt->SetTextColor(2);
       	st1_PhotProt->SetY1NDC(0.8);
       	st1_PhotProt->SetY2NDC(0.8+statboxsize_PhotProt);	

	T->Draw("Energy >>+ E_Protons","Particle_ID==2212 && hasLeftDetector_Status==1","SAMES");
	EnergyHisto_Protons->SetMinimum(0.1);
	EnergyHisto_Protons->SetMaximum(10000500);
	gPad->Update();
       	TPaveStats *st2_PhotProt = (TPaveStats*)EnergyHisto_Protons->FindObject("stats");
       	st2_PhotProt->SetTextColor(4);
      	st2_PhotProt->SetY2NDC(st1_PhotProt->GetY1NDC()); //new x start position
 	st2_PhotProt->SetY1NDC(st2_PhotProt->GetY2NDC()-statboxsize_PhotProt); //new x end position

	E_Legend->AddEntry(EnergyHisto_Photons,"#gamma");
	E_Legend->AddEntry(EnergyHisto_Protons,"p");
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

	T->Draw("Theta >>+ AngleHisto_Pions","Particle_ID==211","SAMES");
	gPad->Update();
       	TPaveStats *st2_ProtPions = (TPaveStats*)AngleHisto_Pions->FindObject("stats");
       	st2_ProtPions->SetTextColor(6);
      	st2_ProtPions->SetY2NDC(st1_ProtPions->GetY1NDC()); //new x start position
 	st2_ProtPions->SetY1NDC(st2_ProtPions->GetY2NDC()-statboxsize_ProtPions); //new x end position

	ComparisonLegend->AddEntry(AngleHisto_Prot,"p");
	ComparisonLegend->AddEntry(AngleHisto_Pions,"#pi^{+}");
	ComparisonLegend->Draw();

	string canvasname1 = E_Canvas_ElePhotProt->GetName();
	E_Canvas_ElePhotProt->Write();
	E_Canvas_ElePhotProt->Print((canvasname1+".eps").c_str());	
	E_Canvas_ElePhotProt->Print((canvasname1+".C").c_str());	
	E_Canvas_ElePhotProt->Close();

	Position_Canvas->Divide(2,2);

	Position_Canvas->cd(1);
	gPad->SetLogz();
	gStyle->SetOptStat(11);
	gStyle->SetNumberContours(999);
	T->Draw("Reflectiony:Reflectionx >>+ Posi_negCharge","Charge<0 && hasLeftDetector_Status==1","colz");	

	TLine * l1 = new TLine(-550,-403,550,-403);
	l1->SetLineWidth(2.5);
	TLine * l2 = new TLine(-550,-403,-550,403);
	l2->SetLineWidth(2.5);
	TLine * l3 = new TLine(-550,403,550,403);
	l3->SetLineWidth(2.5);
	TLine * l4 = new TLine(550,403,550,-403);
	l4->SetLineWidth(2.5);
	l1->Draw();
	l2->Draw();
	l3->Draw();
	l4->Draw();

	TEllipse * el1 = new TEllipse(0,0,127,127);
	el1->SetLineWidth(2.5);
	el1->SetFillStyle(0000);
	el1->Draw();

	Position_Canvas->cd(2);
	gPad->SetLogz();
	gStyle->SetOptStat(11);
	gStyle->SetNumberContours(999);
	T->Draw("Reflectiony:Reflectionx >>+ Posi_posCharge","Charge>0 && hasLeftDetector_Status==1","colz");	
	l1->Draw();
	l2->Draw();
	l3->Draw();
	l4->Draw();
	el1->Draw();

	Position_Canvas->cd(3);
	gPad->SetLogz();
	gStyle->SetOptStat(11);
	gStyle->SetNumberContours(999);
	T->Draw("Reflectiony:Reflectionx >>+ Posi_noCharge","Charge==0 && hasLeftDetector_Status==1","colz");	
	l1->Draw();
	l2->Draw();
	l3->Draw();
	l4->Draw();
	el1->Draw();

	Position_Canvas->cd(4);
//	TProfile * Profile_of_FluxMap_neg = Position_negCharge->ProfileX("",Position_negCharge->FindBin(-5,0,0),Position_negCharge->FindBin(400,0,0),"d");
	TProfile * Profile_of_FluxMap_neg = Position_negCharge->ProfileY();
	Profile_of_FluxMap_neg->Draw();
/*
	TPaveText * CutBox = new TPaveText(.3,.78,0.7,0.87,"brNDC");
	CutBox->SetFillColor(0);
	CutBox->SetTextColor(1);

	CutBox->AddText("Cut on angle between inital photon and particle: Angle < 10");
	CutBox->Draw();
*/
	string canvasname2=Position_Canvas->GetName();
	Position_Canvas->Write();
	Position_Canvas->Print((canvasname2+".eps").c_str());
	Position_Canvas->Print((canvasname2+".C").c_str());
	Position_Canvas->Close();

	output_rootfile->Write();
	input_rootfile->Close();
}
