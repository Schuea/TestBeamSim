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

	TH1F * EnergyHisto_Photons = new TH1F("E_Photons","Energy distribution of photons existing behind converter",40,0,6.5);
	EnergyHisto_Photons->GetXaxis()->SetTitle("Energy/GeV");
	EnergyHisto_Photons->GetXaxis()->CenterTitle();
	TH1F * EnergyHisto_Electrons = new TH1F("E_Electrons","Energy distribution of electrons existing behind converter",400,0,7);
	EnergyHisto_Electrons->GetXaxis()->SetTitle("Energy/GeV");
	EnergyHisto_Electrons->GetXaxis()->CenterTitle();
	TH1F * EnergyHisto_Positrons = new TH1F("E_Positrons","Energy distribution of positrons existing behind converter",400,0,7);
	EnergyHisto_Positrons->GetXaxis()->SetTitle("Energy/GeV");
	EnergyHisto_Positrons->GetXaxis()->CenterTitle();


	TH1F * EnergyHisto_AllStatuses = new TH1F("E_All","Energy distribution for all simulation statuses",400,0,1);
	EnergyHisto_AllStatuses->SetLineColor(1);
	EnergyHisto_AllStatuses->GetXaxis()->SetTitle("Energy/GeV");
	EnergyHisto_AllStatuses->GetXaxis()->CenterTitle();
	
	TH1F * EnergyHisto_createdInContinuousProcess = new TH1F("E_createdInContinuousProcess","Energy distribution for particles created in continuous process",400,0,1);
	EnergyHisto_createdInContinuousProcess->SetLineColor(2);
	EnergyHisto_createdInContinuousProcess->GetXaxis()->SetTitle("Energy/GeV");
	EnergyHisto_createdInContinuousProcess->GetXaxis()->CenterTitle();

	TH1F * EnergyHisto_DecayedInTracker = new TH1F("E_DecayedInTracker","Energy distribution for particles decayed in tracker",400,0,1);
	EnergyHisto_DecayedInTracker->SetLineColor(3);
	EnergyHisto_DecayedInTracker->GetXaxis()->SetTitle("Energy/GeV");
	EnergyHisto_DecayedInTracker->GetXaxis()->CenterTitle();

	TH1F * EnergyHisto_Stopped = new TH1F("E_Stopped","Energy distribution for particles stopped in tracker",400,0,1);
	EnergyHisto_Stopped->SetLineColor(4);
	EnergyHisto_Stopped->GetXaxis()->SetTitle("Energy/GeV");
	EnergyHisto_Stopped->GetXaxis()->CenterTitle();

	TH1F * EnergyHisto_hasLeftDetector = new TH1F("E_hasLeftDetector","Energy distribution for particles that left detector",400,0,1);
	EnergyHisto_hasLeftDetector->SetLineColor(7);
	EnergyHisto_hasLeftDetector->GetXaxis()->SetTitle("Energy/GeV");
	EnergyHisto_hasLeftDetector->GetXaxis()->CenterTitle();


	TH1F * EnergyHisto_wo_Parents = new TH1F("E_woParent","Energy distribution for particles without parents",400,0,6.5);
	EnergyHisto_wo_Parents->SetLineColor(2);
	TH1F * EnergyHisto_w_Parents = new TH1F("E_wParent","Energy distribution for particles with parents",400,0,6.5);
	EnergyHisto_w_Parents->SetLineColor(1);
	EnergyHisto_w_Parents->GetXaxis()->SetTitle("Energy/GeV");
	EnergyHisto_w_Parents->GetXaxis()->CenterTitle();

	TH1F * EnergyHisto_ParentElectron = new TH1F("E_ParentElectron","Energy distribution for particles with electrons as parent",400,0,6.5);
	EnergyHisto_ParentElectron->SetLineColor(2);
	EnergyHisto_ParentElectron->GetXaxis()->SetTitle("Energy/GeV");
	EnergyHisto_ParentElectron->GetXaxis()->CenterTitle();

	TH1F * EnergyHisto_ParentPositron = new TH1F("E_ParentPositron","Energy distribution for particles with positrons as parent",400,0,6.5);
	EnergyHisto_ParentPositron->SetLineColor(3);
	EnergyHisto_ParentPositron->GetXaxis()->SetTitle("Energy/GeV");
	EnergyHisto_ParentPositron->GetXaxis()->CenterTitle();

	TH1F * EnergyHisto_ParentPhoton = new TH1F("E_ParentPhoton","Energy distribution for particles with photons as parent",400,0,6.5);
	EnergyHisto_ParentPhoton->SetLineColor(4);
	EnergyHisto_ParentPhoton->GetXaxis()->SetTitle("Energy/GeV");
	EnergyHisto_ParentPhoton->GetXaxis()->CenterTitle();

	TH2F * DeflectionHisto_Photons = new TH2F("Defl_Photons","Deflection of photons existing behind converter",700,-20,20,800,-20,20);
	DeflectionHisto_Photons->GetZaxis()->SetRangeUser(0,200);	
	TH2F * DeflectionHisto_Electrons = new TH2F("Defl_Electrons","Deflection of electrons existing behind converter",700,-20,20,800,-20,20);
	DeflectionHisto_Electrons->GetZaxis()->SetRangeUser(1,100);	
	TH2F * DeflectionHisto_Positrons = new TH2F("Defl_Positrons","Deflection of positrons existing behind converter",700,-20,20,800,-20,20);
	DeflectionHisto_Positrons->GetZaxis()->SetRangeUser(1,100);	
	

	float angle = 0;
	float angleinterval = 1;
	float max_energy = 6.5;
	vector< TH1F* > AngleHisto_Electrons_;
	vector< TCanvas* > Angle_Canvas_;
	vector< TPaveText* > AngleCutBox_;

	for(int i = 1; i < 13; ++i){
		if (angle <= 2){
			angleinterval = 1;
			max_energy = 6.5;
		}

                else if (angle > 2 && angle <= 5){
			angleinterval = 1;
			max_energy = 3;
		}
		else if (angle > 5 && angle <= 10){
			angleinterval = 2;
			max_energy = 2;
		}
		else if (angle > 10 && angle <=30){
			angleinterval = 5;
			max_energy = 1;
		}
		else {
			break;
		}

		cout << "angle = " << angle << ", angleinterval = " << angleinterval << endl;	
	
		stringstream histoname,histotitle,drawtext,cuttext,pavetext,savetext, savetext2;
                histoname << "AngleHisto_Electrons_" << i-1;
		histotitle << "Energy distribution of electrons with photons as parents, for an angle between " << angle  << " and " << angle + angleinterval << " between electron and z-axis";
		drawtext << "Energy >>+ " << histoname.str();
		cuttext << "Particle_ID==11 && Theta >= "<< angle <<" && Theta < " << angle + angleinterval << " && Parent_ID==22";
		pavetext << angle << " degree < Angle < " << angle + angleinterval << " degree";
		savetext << histoname.str() << "_" << angle << "degree_" << angle+angleinterval <<"degree.png";
		savetext2 << histoname.str() << "_" << angle << "degree_" << angle+angleinterval <<"degree.C";

		cout << histoname.str() << endl;
		cout << histotitle.str() << endl;
		cout << drawtext.str() << endl;
		cout << cuttext.str() << endl;
		cout << pavetext.str() << endl;

		TH1F* temphisto = new TH1F(histoname.str().c_str(), histotitle.str().c_str(),700,0,max_energy);
                AngleHisto_Electrons_.push_back(temphisto);
               
	  	TCanvas *tempcanvas = new TCanvas();
		Angle_Canvas_.push_back(tempcanvas);
		Angle_Canvas_.at(i-1)->cd();
		gPad->SetLogy();
	
		AngleHisto_Electrons_.at(i-1)->GetXaxis()->SetTitle("Energy/GeV");
		AngleHisto_Electrons_.at(i-1)->GetXaxis()->CenterTitle();
	
		T->Draw(drawtext.str().c_str(), cuttext.str().c_str());

		TPaveText* tempbox = new TPaveText(.5,.7,.7,.8,"brNDC");
		AngleCutBox_.push_back(tempbox);
		AngleCutBox_.at(i-1)->AddText(pavetext.str().c_str());
		AngleCutBox_.at(i-1)->Draw();

		Angle_Canvas_.at(i-1)->Write();
		Angle_Canvas_.at(i-1)->Print(savetext.str().c_str());
		Angle_Canvas_.at(i-1)->Print(savetext2.str().c_str());
		Angle_Canvas_.at(i-1)->Close();
	
		angle += angleinterval;
        }


	TCanvas * Energy_Canvas = new TCanvas("E_Canvas","Energy distributions");	
	TCanvas * Deflection_Canvas = new TCanvas("Defl_Canvas","Particle deflection");	
	TCanvas * CompareSimStatus_Canvas = new TCanvas("Compare_Canvas","Simulation status");
	TCanvas * Angle_Canvas = new TCanvas("Angle_Canvas","Energy distribution regarding the angle");

	TLegend * ComparisonLegend = new TLegend(0.5,0.5,0.8,0.7);
	ComparisonLegend->SetFillColor(0);
	TLegend * ComparisonLegend2 = new TLegend(0.5,0.5,0.8,0.7);
	ComparisonLegend->SetFillColor(0);
	TLegend * ComparisonLegend3 = new TLegend(0.5,0.5,0.8,0.7);
	ComparisonLegend->SetFillColor(0);

	Energy_Canvas->Divide(2,2);
	Energy_Canvas->cd(1);
	T->Draw("Energy >>+ E_Photons","Particle_ID==22&& 0.1<Energy");
	Energy_Canvas->cd(2);
	T->Draw("Energy >>+ E_Electrons","Particle_ID==11&& 0.1<Energy");
	Energy_Canvas->cd(3);
	T->Draw("Energy >>+ E_Positrons","Particle_ID==-11&& 0.1<Energy");

	stringstream energyprint;
	energyprint << "EnergyDistributions_" << name << ".pdf";
	energyprint2 << "EnergyDistributions_" << name << ".C";
	Energy_Canvas->Write();
	//Energy_Canvas->Print(energyprint.str().c_str());	
	//Energy_Canvas->Print(energyprint2.str().c_str());	
	Energy_Canvas->Close();


	Deflection_Canvas->Divide(2,2);
	Deflection_Canvas->cd(1);
	gStyle->SetNumberContours(999);
	gPad->SetLogz();
	T->Draw("Reflectiony:Reflectionx >>+ Defl_Photons","Particle_ID==22&& 0.1<Energy","colz");
	
	Deflection_Canvas->cd(2);
	gStyle->SetNumberContours(999);
	gPad->SetLogz();
	T->Draw("Reflectiony:Reflectionx >>+ Defl_Electrons","Particle_ID==11&& 0.1<Energy","colz");
	
	Deflection_Canvas->cd(3);
	gStyle->SetNumberContours(999);
	gPad->SetLogz();
	T->Draw("Reflectiony:Reflectionx >>+ Defl_Positrons","Particle_ID==-11&& 0.1<Energy","colz");
	
	stringstream deflectionprint;
	deflectionprint << "DeflectionDistributions_" << name << ".pdf";
	deflectionprint2 << "DeflectionDistributions_" << name << ".C";
	Deflection_Canvas->Write();	
	//Deflection_Canvas->Print(deflectionprint.str().c_str());	
	//Deflection_Canvas->Print(deflectionprint2.str().c_str());	
	Deflection_Canvas->Close();

	CompareSimStatus_Canvas->Divide(2,2);
	CompareSimStatus_Canvas->cd(1);
	gPad->SetLogy();
	T->Draw("Energy >>+ E_All","Particle_ID==11");
	T->Draw("Energy >>+ E_createdInContinuousProcess","Particle_ID==11 && createdInContinuousProcess_Status==1","Same");
	T->Draw("Energy >>+ E_DecayedInTracker","Particle_ID==11 && DecayedInTracker_Status==1","Same");
	T->Draw("Energy >>+ E_Stopped","Particle_ID==11 && Stopped_Status==1","Same");
	T->Draw("Energy >>+ E_hasLeftDetector","Particle_ID==11 && hasLeftDetector_Status==1","Same");
	
	ComparisonLegend->AddEntry(EnergyHisto_AllStatuses,"All statuses","l");
	ComparisonLegend->AddEntry(EnergyHisto_createdInContinuousProcess,"CreatedInContinuousProcess","l");
	ComparisonLegend->AddEntry(EnergyHisto_DecayedInTracker,"DecayedInTracker","l");
	ComparisonLegend->AddEntry(EnergyHisto_Stopped,"Stopped","l");
	ComparisonLegend->AddEntry(EnergyHisto_hasLeftDetector,"HasLeftDetector","l");
	ComparisonLegend->Draw();

	CompareSimStatus_Canvas->cd(2);
	gPad->SetLogy();
	T->Draw("Energy >>+ E_hasLeftDetector","Particle_ID==11 && hasLeftDetector_Status==1");
	
	CompareSimStatus_Canvas->cd(3);
	gPad->SetLogy();
	T->Draw("Energy >>+ E_wParent","Particle_ID==11 && NumberOfParents==1");
	T->Draw("Energy >>+ E_woParent","Particle_ID==11 && NumberOfParents==0","Same");

	ComparisonLegend2->AddEntry(EnergyHisto_wo_Parents,"without parents","l");
	ComparisonLegend2->AddEntry(EnergyHisto_w_Parents,"with parents","l");
	ComparisonLegend2->Draw();

	CompareSimStatus_Canvas->cd(4);
	gPad->SetLogy();

	T->Draw("Energy >>+ E_wParent","Particle_ID==11 && NumberOfParents==1");
	T->Draw("Energy >>+ E_ParentElectron","Particle_ID==11 && Parent_ID==11","Same");
	T->Draw("Energy >>+ E_ParentPositron","Particle_ID==11 && Parent_ID==-11","Same");
	T->Draw("Energy >>+ E_ParentPhoton","Particle_ID==11 && Parent_ID==22 && Angle_initialfinal<10","Same");
	ComparisonLegend3->AddEntry(EnergyHisto_w_Parents,"all particles with parents","l");
	ComparisonLegend3->AddEntry(EnergyHisto_ParentElectron,"with electrons as parents","l");
	ComparisonLegend3->AddEntry(EnergyHisto_ParentPositron,"with positrons as parents","l");
	ComparisonLegend3->AddEntry(EnergyHisto_ParentPhoton,"with photons as parents","l");
	ComparisonLegend3->Draw();

	TPaveText * CutBox = new TPaveText(.5,.7,0.6,0.8,"brNDC");
	CutBox->SetFillColor(0);
	CutBox->SetTextColor(1);

	CutBox->AddText("Angle < 10");
	CutBox->SetLabel("Cut on angle between inital photon and particle");
	CutBox->Draw();

	stringstream comparisonprint;
	comparisonprint << "ComparisonStatuses_" << name << ".pdf";
	comparisonprint2 << "ComparisonStatuses_" << name << ".C";
	//CompareSimStatus_Canvas->Print(comparisonprint.str().c_str());
	//CompareSimStatus_Canvas->Print(comparisonprint2.str().c_str());
	CompareSimStatus_Canvas->Write();
	CompareSimStatus_Canvas->Close();

	output_rootfile->Write();
	input_rootfile->Close();
}
