#include "TFile.h"
#include "TBrowser.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TStyle.h"
#include "TPaveText.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

void DrawingMacro(string outputname,string name1, string name2); 

int main(int argc,char *argv[]){
	if(argc < 4 || argc > 4){
    		//explain how to use program	
    		cerr << "Type in name of output file AND both root filenames, first w/o air, second w air!" << endl;
    		cerr << "e.g. ./DrawHistograms outputfilename.root file_w/o_air.root file_air.root" << endl;
    		terminate();
	}	
	string filename1, filename2, outputfilename;
	outputfilename = argv[1];
	filename1 = argv[2];
	filename2 = argv[3];
       
	DrawingMacro(outputfilename,filename1,filename2);
}

void DrawingMacro(string outputname,string name1, string name2){

	TFile * input_rootfile1 = new TFile(name1.c_str(),"READ");
	cout << "Size of input file 1 = " << input_rootfile1->GetSize() << endl;
	TFile * input_rootfile2 = new TFile(name2.c_str(),"READ");
	cout << "Size of input file 2 = " << input_rootfile2->GetSize() << endl;
	TTree * T1;
	input_rootfile1->GetObject("Tree",T1);
	TTree * T2; 
	input_rootfile2->GetObject("Tree",T2);
	cout << "Accessed TTrees.." << endl;

	TFile * output_rootfile = new TFile(outputname.c_str(),"RECREATE");

	TH1::SetDefaultSumw2();

	TH1F * EnergyHisto_All_AIR = new TH1F("E_All_AIR","Normalised energy distribution of all particles, passage through 1 cubic meter air",100,0,7);
	EnergyHisto_All_AIR->SetLineColor(4);
	EnergyHisto_All_AIR->GetXaxis()->SetTitle("Energy/GeV");
	EnergyHisto_All_AIR->GetXaxis()->CenterTitle();
	TH1F * EnergyHisto_All = new TH1F("E_All","Normalised energy distribution of all particles, passage w/o air",100,0,7);
	EnergyHisto_All->SetLineColor(2);
	EnergyHisto_All->GetXaxis()->SetTitle("Energy/GeV");
	EnergyHisto_All->GetXaxis()->CenterTitle();

	TH1F * EnergyHisto_AIR_Photons = new TH1F("E_AIR_Photons","Energy distribution for photons in air",100,0,7);
	EnergyHisto_AIR_Photons->SetLineColor(1);
	EnergyHisto_AIR_Photons->GetXaxis()->SetTitle("Energy/GeV");
	EnergyHisto_AIR_Photons->GetXaxis()->CenterTitle();
	TH1F * EnergyHisto_DecayedInTracker_AIR_Photons = new TH1F("E_DecayedInTracker_AIR_Photons","Energy distribution for particles decayed",100,0,7);
	EnergyHisto_DecayedInTracker_AIR_Photons->SetLineColor(4);
	EnergyHisto_DecayedInTracker_AIR_Photons->GetXaxis()->SetTitle("Energy/GeV");
	EnergyHisto_DecayedInTracker_AIR_Photons->GetXaxis()->CenterTitle();

	TH1F * EnergyHisto_AIR_Protons = new TH1F("E_AIR_Protons","Energy distribution for protons in air",50,0,4);
	EnergyHisto_AIR_Protons->SetLineColor(1);
	EnergyHisto_AIR_Protons->GetXaxis()->SetTitle("Energy/GeV");
	EnergyHisto_AIR_Protons->GetXaxis()->CenterTitle();
	TH1F * EnergyHisto_hasLeftDetector_AIR_Protons = new TH1F("E_hasLeftDetector_AIR_Protons","Energy distribution for protons leaving world",50,0,4);
	EnergyHisto_hasLeftDetector_AIR_Protons->SetLineColor(3);
	EnergyHisto_hasLeftDetector_AIR_Protons->GetXaxis()->SetTitle("Energy/GeV");
	EnergyHisto_hasLeftDetector_AIR_Protons->GetXaxis()->CenterTitle();
	TH1F * EnergyHisto_Stopped_AIR_Protons = new TH1F("E_Stopped_AIR_Protons","Energy distribution for protons stopped",50,0,4);
	EnergyHisto_Stopped_AIR_Protons->SetLineColor(2);
	EnergyHisto_Stopped_AIR_Protons->GetXaxis()->SetTitle("Energy/GeV");
	EnergyHisto_Stopped_AIR_Protons->GetXaxis()->CenterTitle();

	TH1F * EnergyHisto_AIR_Muons = new TH1F("E_AIR_Muons","Energy distribution for muons in air",30,0,1);
	EnergyHisto_AIR_Muons->SetLineColor(1);
	EnergyHisto_AIR_Muons->GetXaxis()->SetTitle("Energy/GeV");
	EnergyHisto_AIR_Muons->GetXaxis()->CenterTitle();
	TH1F * EnergyHisto_DecayedInTracker_AIR_Muons = new TH1F("E_DecayedInTracker_AIR_Muons","Energy distribution for muons decayed",30,0,1);
	EnergyHisto_DecayedInTracker_AIR_Muons->SetLineColor(4);
	EnergyHisto_DecayedInTracker_AIR_Muons->GetXaxis()->SetTitle("Energy/GeV");
	EnergyHisto_DecayedInTracker_AIR_Muons->GetXaxis()->CenterTitle();
	TH1F * EnergyHisto_hasLeftDetector_AIR_Muons = new TH1F("E_hasLeftDetector_AIR_Muons","Energy distribution for muons leaving world",30,0,1);
	EnergyHisto_hasLeftDetector_AIR_Muons->SetLineColor(3);
	EnergyHisto_hasLeftDetector_AIR_Muons->GetXaxis()->SetTitle("Energy/GeV");
	EnergyHisto_hasLeftDetector_AIR_Muons->GetXaxis()->CenterTitle();

	TH1F * EnergyHisto_AIR_Electrons = new TH1F("E_AIR_Electrons","Energy distribution for electrons in air",200,0,7);
	EnergyHisto_AIR_Electrons->SetLineColor(1);
	EnergyHisto_AIR_Electrons->GetXaxis()->SetTitle("Energy/GeV");
	EnergyHisto_AIR_Electrons->GetXaxis()->CenterTitle();
	TH1F * EnergyHisto_Stopped_AIR_Electrons = new TH1F("E_Stopped_AIR_Electrons","Energy distribution for electrons stopped",200,0,7);
	EnergyHisto_Stopped_AIR_Electrons->SetLineColor(2);
	EnergyHisto_Stopped_AIR_Electrons->GetXaxis()->SetTitle("Energy/GeV");
	EnergyHisto_Stopped_AIR_Electrons->GetXaxis()->CenterTitle();

	TH2F * PositionMap_Photons = new TH2F("Position_Photons","Position of photons at z=2380cm",500,-7.5,7.5,200,-7.5,7.5);
	PositionMap_Photons->GetXaxis()->SetTitle("x/cm");
	PositionMap_Photons->GetXaxis()->CenterTitle();
	PositionMap_Photons->GetYaxis()->SetTitle("y/cm");
	PositionMap_Photons->GetYaxis()->CenterTitle();
	TH2F * PositionMap_Electrons = new TH2F("Position_Electrons","Position of electrons at z=2380cm",500,-7.5,7.5,200,-7.5,7.5);
	PositionMap_Electrons->GetXaxis()->SetTitle("x/cm");
	PositionMap_Electrons->GetXaxis()->CenterTitle();
	PositionMap_Electrons->GetYaxis()->SetTitle("y/cm");
	PositionMap_Electrons->GetYaxis()->CenterTitle();
	TH2F * PositionMap_Protons = new TH2F("Position_Protons","Position of protons at z=2380cm",500,-7.5,7.5,200,-7.5,7.5);
	PositionMap_Protons->GetXaxis()->SetTitle("x/cm");
	PositionMap_Protons->GetXaxis()->CenterTitle();
	PositionMap_Protons->GetYaxis()->SetTitle("y/cm");
	PositionMap_Protons->GetYaxis()->CenterTitle();

	TH2F * EnergyAngle = new TH2F("Energy_Angle","Energy as a function of the angle",700,0,180,700,0,7);
	EnergyAngle->GetXaxis()->SetTitle("Theta/degree");
	EnergyAngle->GetXaxis()->CenterTitle();
	EnergyAngle->GetYaxis()->SetTitle("Energy/GeV");
	EnergyAngle->GetYaxis()->CenterTitle();
	TH2F * EnergyAngle_AIR = new TH2F("Energy_Angle_AIR","Energy as a function of the angle",700,0,180,700,0,7);
	EnergyAngle_AIR->GetXaxis()->SetTitle("Theta/degree");
	EnergyAngle_AIR->GetXaxis()->CenterTitle();
	EnergyAngle_AIR->GetYaxis()->SetTitle("Energy/GeV");
	EnergyAngle_AIR->GetYaxis()->CenterTitle();

	TH1F * AngleDistribution_AIR_All = new TH1F("Angle_AIR_All","Normalised angular distribution of particles",10,0,40);
	AngleDistribution_AIR_All->SetLineColor(4);
	AngleDistribution_AIR_All->GetXaxis()->SetTitle("Theta/degree");
	AngleDistribution_AIR_All->GetXaxis()->CenterTitle();
	TH1F * AngleDistribution_All= new TH1F("Angle_All","Normalised angular distribution of particles",10,0,40);
	AngleDistribution_All->SetLineColor(2);
	AngleDistribution_All->GetXaxis()->SetTitle("Theta/degree");
	AngleDistribution_All->GetXaxis()->CenterTitle();
	TH1F * AngleDistribution_All_Subtraction;

	TCanvas * Energy_Canvas = new TCanvas("E_Canvas","Energy distributions");	
	TCanvas * CompareEnergy_Canvas = new TCanvas("CompareE_Canvas","Energy distributions");	
	TCanvas * CompareAngle_Canvas = new TCanvas("CompareAngle_Canvas","Angular distribution");
	TCanvas * EnergyAngle_Canvas = new TCanvas("EnergyAngle_Canvas","Energy as function of angle");
	TCanvas * Position_Canvas = new TCanvas("Position_Canvas","Particle position");

	TLegend * Legend1 = new TLegend(0.5,0.6,0.8,0.8);
	Legend1->SetFillColor(0);
	TLegend * Legend2 = new TLegend(0.5,0.6,0.8,0.8);
	Legend2->SetFillColor(0);
	TLegend * Legend3 = new TLegend(0.4,0.6,0.7,0.8);
	Legend3->SetFillColor(0);
	TLegend * Legend4 = new TLegend(0.4,0.6,0.7,0.8);
	Legend4->SetFillColor(0);
	TLegend * Legend5 = new TLegend(0.4,0.6,0.7,0.8);
	Legend5->SetFillColor(0);
	TLegend * Legend6 = new TLegend(0.4,0.6,0.7,0.8);
	Legend6->SetFillColor(0);
	TLegend * Legend7 = new TLegend(0.5,0.6,0.8,0.8);
	Legend7->SetFillColor(0);

	Energy_Canvas->cd();
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
	EnergyHisto_All->SetStats(kFALSE);
	EnergyHisto_All_AIR->SetStats(kFALSE);
	T2->Draw("Energy >>+ E_All_AIR");
	EnergyHisto_All_AIR->Scale(1.0/(EnergyHisto_All_AIR->Integral()));
	T1->Draw("Energy >>+ E_All","","same");
	EnergyHisto_All->Scale(1.0/(EnergyHisto_All->Integral()));
	Legend2->AddEntry(EnergyHisto_All,"All particles, w/o air","l");
	Legend2->AddEntry(EnergyHisto_All_AIR,"All particles, in air","l");
	Legend2->Draw();

      	subpad2->cd();
  	TH1F* E_Diff;

	string E_DiffName;
    	E_DiffName =  "EnergyDistribution_Difference";

      	E_Diff = new TH1F(E_DiffName.c_str(),E_DiffName.c_str(), EnergyHisto_All->GetNbinsX(), EnergyHisto_All->GetXaxis()->GetXmin(), EnergyHisto_All->GetXaxis()->GetXmax());
    	E_Diff->Divide(EnergyHisto_All_AIR,EnergyHisto_All,1,1,"b");
    	E_Diff->SetLineColor(EnergyHisto_All_AIR->GetLineColor());
    	E_Diff->SetLineWidth(1);
    	E_Diff->SetMarkerStyle(EnergyHisto_All->GetMarkerStyle());
    	E_Diff->SetMarkerColor(EnergyHisto_All->GetMarkerColor());
    	E_Diff->SetMarkerSize(.8);
    	E_Diff->Draw("EP");	

	TLine *line1 = new TLine(EnergyHisto_All->GetBinLowEdge(1),1,EnergyHisto_All->GetBinLowEdge(EnergyHisto_All->GetNbinsX()+1),1);
  	line1->SetLineWidth(2);
  	line1->SetLineColor(kBlack);
  	line1->Draw();
  	E_Diff->GetXaxis()->SetTitle(EnergyHisto_All->GetXaxis()->GetTitle());
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
    	E_Diff->SetMinimum(0.97);    
	E_Diff->SetMaximum(1.03);

	string canvasname1=Energy_Canvas->GetName();
	Energy_Canvas->Write();
	Energy_Canvas->Print((canvasname1+".eps").c_str());	
	Energy_Canvas->Print((canvasname1+".C").c_str());	
	Energy_Canvas->Close();

	CompareEnergy_Canvas->Divide(2,2);
	CompareEnergy_Canvas->cd(1);
	gPad->SetLogy();
	EnergyHisto_AIR_Photons->SetStats(kFALSE);
	EnergyHisto_DecayedInTracker_AIR_Photons->SetStats(kFALSE);
	T2->Draw("Energy >>+ E_AIR_Photons","Particle_ID==22");
	T2->Draw("Energy >>+ E_DecayedInTracker_AIR_Photons","Particle_ID==22 && DecayedInTracker_Status==1","same");
	Legend3->AddEntry(EnergyHisto_AIR_Photons,"All photons, in air","l");
	Legend3->AddEntry(EnergyHisto_DecayedInTracker_AIR_Photons,"Decayed photons, in air","l");
	Legend3->Draw();
	CompareEnergy_Canvas->cd(2);
	gPad->SetLogy();
	EnergyHisto_AIR_Muons->SetStats(kFALSE);
	EnergyHisto_DecayedInTracker_AIR_Muons->SetStats(kFALSE);
	EnergyHisto_hasLeftDetector_AIR_Muons->SetStats(kFALSE);
	T2->Draw("Energy >>+ E_AIR_Muons","Particle_ID==13");		
	T2->Draw("Energy >>+ E_DecayedInTracker_AIR_Muons","Particle_ID==13 && DecayedInTracker_Status==1","same");
	T2->Draw("Energy >>+ E_hasLeftDetector_AIR_Muons","Particle_ID==13 && hasLeftDetector_Status==1","same");
	Legend4->AddEntry(EnergyHisto_AIR_Muons,"All muons, in air","l");
	Legend4->AddEntry(EnergyHisto_DecayedInTracker_AIR_Muons,"Decayed muons, in air","l");
	Legend4->AddEntry(EnergyHisto_hasLeftDetector_AIR_Muons,"Muons leaving world, in air","l");
	Legend4->Draw();
	CompareEnergy_Canvas->cd(3);
	gPad->SetLogy();
	EnergyHisto_AIR_Electrons->SetStats(kFALSE);
	EnergyHisto_Stopped_AIR_Electrons->SetStats(kFALSE);
	T2->Draw("Energy >>+ E_AIR_Electrons","Particle_ID==11");
	T2->Draw("Energy >>+ E_Stopped_AIR_Electrons","Particle_ID==11 && Stopped_Status==1","same");
	Legend5->AddEntry(EnergyHisto_AIR_Electrons,"All electrons, in air","l");
	Legend5->AddEntry(EnergyHisto_Stopped_AIR_Electrons,"Stopped electrons, in air","l");
	Legend5->Draw();
	CompareEnergy_Canvas->cd(4);
	gPad->SetLogy();
	EnergyHisto_AIR_Protons->SetStats(kFALSE);
	EnergyHisto_hasLeftDetector_AIR_Protons->SetStats(kFALSE);
	EnergyHisto_Stopped_AIR_Protons->SetStats(kFALSE);
	T2->Draw("Energy >>+ E_AIR_Protons","Particle_ID==2212");
	T2->Draw("Energy >>+ E_hasLeftDetector_AIR_Protons","Particle_ID==2212 && hasLeftDetector_Status==1","same");
	T2->Draw("Energy >>+ E_Stopped_AIR_Protons","Particle_ID==2212 && Stopped_Status==1","same");
	Legend6->AddEntry(EnergyHisto_AIR_Protons,"All protons, in air","l");
	Legend6->AddEntry(EnergyHisto_hasLeftDetector_AIR_Protons,"Protons leaving world, in air","l");
	Legend6->AddEntry(EnergyHisto_Stopped_AIR_Protons,"Stopped protons, in air","l");
	Legend6->Draw();

	string canvasname2=CompareEnergy_Canvas->GetName();
	CompareEnergy_Canvas->Write();
	CompareEnergy_Canvas->Print((canvasname2+".eps").c_str());	
	CompareEnergy_Canvas->Print((canvasname2+".C").c_str());	
	CompareEnergy_Canvas->Close();

	Position_Canvas->Divide(2,2);
	Position_Canvas->cd(1);
	gPad->SetLogz();
	gStyle->SetNumberContours(999);
	T2->Draw("Reflectiony:Reflectionx >>+ Position_Photons","Particle_ID==22","colz");
	TLine * l1 = new TLine(-2.5,-2.5,2.5,-2.5);
	l1->SetLineWidth(2);
	TLine * l2 = new TLine(-2.5,-2.5,-2.5,2.5);
	l2->SetLineWidth(2);
	TLine * l3 = new TLine(2.5,-2.5,2.5,2.5);
	l3->SetLineWidth(2);
	TLine * l4 = new TLine(-2.5,2.5,2.5,2.5);
	l4->SetLineWidth(2);
	l1->Draw();
	l2->Draw();
	l3->Draw();
	l4->Draw();
	Position_Canvas->cd(2);
	gPad->SetLogz();
	gStyle->SetNumberContours(999);
	T2->Draw("Reflectiony:Reflectionx >>+ Position_Electrons","Particle_ID==11","colz");
	l1->Draw();
	l2->Draw();
	l3->Draw();
	l4->Draw();
	Position_Canvas->cd(3);
	gStyle->SetNumberContours(999);
	T2->Draw("Reflectiony:Reflectionx >>+ Position_Protons","Particle_ID==2212","colz");
	l1->Draw();
	l2->Draw();
	l3->Draw();
	l4->Draw();

	string canvasname3=Position_Canvas->GetName();
	Position_Canvas->Write();
	Position_Canvas->Print((canvasname3+".eps").c_str());	
	Position_Canvas->Print((canvasname3+".C").c_str());	
	Position_Canvas->Close();

	EnergyAngle_Canvas->cd();
	T1->Draw("Energy:Theta >>+ Energy_Angle");
	EnergyAngle->SetStats(kFALSE);
	EnergyAngle->SetMarkerStyle(6);
	EnergyAngle->SetMarkerColor(2);
//	EnergyAngle->SetMarkerSize(0.2);
	EnergyAngle->Draw();
	gPad->Update();
	T2->Draw("Energy:Theta >>+ Energy_Angle_AIR");
	EnergyAngle_AIR->SetStats(kFALSE);
	EnergyAngle_AIR->SetMarkerStyle(7);
	EnergyAngle_AIR->SetMarkerColor(4);
//	EnergyAngle->SetMarkerSize(0.6);
	EnergyAngle->Draw("same");
	
	Legend7->AddEntry(EnergyAngle,"All particles, w/o air","p");		
	Legend7->AddEntry(EnergyAngle_AIR,"All particles, in air","p");		
	Legend7->Draw();		

	string canvasname4=EnergyAngle_Canvas->GetName();
	EnergyAngle_Canvas->Write();
	EnergyAngle_Canvas->Print((canvasname4+".eps").c_str());	
	EnergyAngle_Canvas->Print((canvasname4+".C").c_str());	
	EnergyAngle_Canvas->Close();

/*	
	float energy = 0;
	float energyinterval = 1;
	float max_angle = 180;
	vector< TH1F* > AngleHisto_;
	vector< TH1F* > AngleHisto_AIR;
	vector< TH1F* > AngleHisto_Diff;
	vector< TCanvas* > Angle_Canvas_;
	vector< TPaveText* > AngleCutBox_;

	for(int i = 1; i < 8; ++i){
		if (energy < 1){
			max_angle=180;
		}
                else if (energy >= 1 && energy < 2){
			max_angle=180;
		}
		else if (energy >= 2 && energy < 3){
			max_angle=90;
		}
		else if (energy >= 3 && energy < 4){
			max_angle=90;
		}
		else if (energy >= 4 && energy < 5){
			max_angle=20;
		}
		else if (energy >= 5 && energy < 6){
			max_angle=20;
		}
		else if (energy >= 6 && energy < 7){
			max_angle=10;
		}	
		else {
			break;
		}
		cout << "energy = " << energy << endl;
		cout << i << ": max_angle = " << max_angle << endl;
		stringstream histoname,histoname_AIR,histotitle,histotitle_AIR,drawtext,drawtext_AIR,cuttext,pavetext;
                histoname << "AngleHisto_" << i-1;
                histoname_AIR << "AngleHisto_AIR" << i-1;
		histotitle << "Angular distribution of all particles without air, for an energy between " << energy  << " and " << energy + energyinterval << " GeV";
		histotitle_AIR << "Angular distribution of all particles in air, for an energy between " << energy  << " and " << energy + energyinterval << " GeV";
		drawtext << "Theta >>+ " << histoname.str();
		drawtext_AIR << "Theta >>+ " << histoname_AIR.str();
		cuttext << "Energy >= "<< energy <<" && Energy < " << energy + energyinterval;
		pavetext << energy << " GeV < Energy < " << energy + energyinterval << " GeV";

		TH1F* temphisto = new TH1F(histoname.str().c_str(), histotitle.str().c_str(),100,0,max_angle);
		TH1F* temphisto_AIR = new TH1F(histoname_AIR.str().c_str(), histotitle_AIR.str().c_str(),100,0,max_angle);
                AngleHisto_.push_back(temphisto);
                AngleHisto_AIR.push_back(temphisto_AIR);
               
	  	TCanvas *tempcanvas = new TCanvas();
		Angle_Canvas_.push_back(tempcanvas);

		Angle_Canvas_.at(i-1)->cd();
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
		AngleHisto_.at(i-1)->SetLineColor(2);
		AngleHisto_.at(i-1)->GetXaxis()->SetTitle("Theta/degree");
		AngleHisto_.at(i-1)->GetXaxis()->CenterTitle();
		AngleHisto_AIR.at(i-1)->SetLineColor(4);
		AngleHisto_AIR.at(i-1)->GetXaxis()->SetTitle("Theta/degree");
		AngleHisto_AIR.at(i-1)->GetXaxis()->CenterTitle();
	
		AngleHisto_.at(i-1)->SetStats(kFALSE);
		AngleHisto_AIR.at(i-1)->SetStats(kFALSE);
		
		T1->Draw(drawtext.str().c_str(), cuttext.str().c_str());
//		cout << "Integral = " << AngleHisto_.at(i-1)->Integral() << endl;
//		AngleHisto_.at(i-1)->Scale(1.0/(AngleHisto_.at(i-1)->Integral()));
		T2->Draw(drawtext_AIR.str().c_str(), cuttext.str().c_str(),"same");
//		cout << "Integral_AIR = " << AngleHisto_AIR.at(i-1)->Integral() << endl;
//		AngleHisto_AIR.at(i-1)->Scale(1.0/(AngleHisto_AIR.at(i-1)->Integral()));
	
		TPaveText* tempbox = new TPaveText(.5,.7,.7,.8,"brNDC");
		AngleCutBox_.push_back(tempbox);
		AngleCutBox_.at(i-1)->AddText(pavetext.str().c_str());
		AngleCutBox_.at(i-1)->Draw();

		subpad2->cd();
		string hDivName;
    		hDivName =  "AngleDistribution_Difference";

      		TH1F * temphisto_diff = new TH1F(hDivName.c_str(),hDivName.c_str(), AngleHisto_.at(i-1)->GetNbinsX(), AngleHisto_.at(i-1)->GetXaxis()->GetXmin(), AngleHisto_.at(i-1)->GetXaxis()->GetXmax());
		AngleHisto_Diff.push_back(temphisto_diff);
	    	AngleHisto_Diff.at(i-1)->Divide(AngleHisto_AIR.at(i-1),AngleHisto_.at(i-1),1,1,"b");
	    	AngleHisto_Diff.at(i-1)->SetLineColor(AngleHisto_.at(i-1)->GetLineColor());
	    	AngleHisto_Diff.at(i-1)->SetLineWidth(1);
	    	AngleHisto_Diff.at(i-1)->SetMarkerStyle(AngleHisto_.at(i-1)->GetMarkerStyle());
	    	AngleHisto_Diff.at(i-1)->SetMarkerColor(AngleHisto_.at(i-1)->GetMarkerColor());
	    	AngleHisto_Diff.at(i-1)->SetMarkerSize(.8);
	    	AngleHisto_Diff.at(i-1)->Draw("EP");	

		TLine *line = new TLine(AngleHisto_.at(i-1)->GetBinLowEdge(1),1,AngleHisto_.at(i-1)->GetBinLowEdge(AngleHisto_.at(i-1)->GetNbinsX()+1),1);
	  	line->SetLineWidth(2);
	  	line->SetLineColor(kBlack);
	  	line->Draw();
 		AngleHisto_Diff.at(i-1)->GetXaxis()->SetTitle(AngleHisto_.at(i-1)->GetXaxis()->GetTitle());
	  	AngleHisto_Diff.at(i-1)->GetYaxis()->SetTitle("Ratio");
	  	AngleHisto_Diff.at(i-1)->GetXaxis()->SetTitleSize(0.17);
	  	AngleHisto_Diff.at(i-1)->GetYaxis()->SetTitleSize(0.15);
	  	AngleHisto_Diff.at(i-1)->GetXaxis()->SetTitleOffset(0.80);
	  	AngleHisto_Diff.at(i-1)->GetYaxis()->SetTitleOffset(0.3);
  		AngleHisto_Diff.at(i-1)->SetLabelSize(0.13,"X");
	  	AngleHisto_Diff.at(i-1)->SetLabelSize(0.07,"Y");
	  	AngleHisto_Diff.at(i-1)->SetTitle("");
  		AngleHisto_Diff.at(i-1)->GetXaxis()->SetLabelOffset(0.010);
	  	AngleHisto_Diff.at(i-1)->GetYaxis()->SetLabelOffset(0.015);
	  	AngleHisto_Diff.at(i-1)->SetStats(kFALSE);
	  	AngleHisto_Diff.at(i-1)->SetDirectory(0);
	    	AngleHisto_Diff.at(i-1)->SetMinimum(0.9);    
		AngleHisto_Diff.at(i-1)->SetMaximum(1.1);

		stringstream Angle_canvasname;
		Angle_canvasname << "Angle_Canvas_"<< i-1;
		Angle_Canvas_.at(i-1)->Write();
		Angle_Canvas_.at(i-1)->Print((Angle_canvasname.str() + ".eps").c_str());
		Angle_Canvas_.at(i-1)->Print((Angle_canvasname.str() + ".C").c_str());
		Angle_Canvas_.at(i-1)->Close();
	
		energy += energyinterval;
		
		delete temphisto, temphisto_AIR, tempcanvas, temphisto_diff, tempbox, line, subpad1, subpad2;		
        }
*/	CompareAngle_Canvas->cd();
	TPad *m_subpad1, *m_subpad2;

  	m_subpad1 = new TPad("m_subpad1"," ",0,0.3,1,1);
  	m_subpad1->SetBottomMargin(0.);
  	m_subpad1->Draw();
  	m_subpad2 = new TPad("m_subpad2"," ",0,0,1,0.3);
  	m_subpad2->SetBottomMargin(0.30);
  	m_subpad2->SetTopMargin(0.);
  	m_subpad2->Draw();

  	m_subpad1->cd();
	gPad->SetLogy();
	AngleDistribution_All->SetStats(kFALSE);
	AngleDistribution_AIR_All->SetStats(kFALSE);
	T1->Draw("Theta >>+ Angle_All");
	AngleDistribution_All->Scale(1.0/(AngleDistribution_All->Integral()));
	T2->Draw("Theta >>+ Angle_AIR_All","","same");
	AngleDistribution_AIR_All->Scale(1.0/(AngleDistribution_AIR_All->Integral()));
	Legend1->AddEntry(AngleDistribution_All,"All particles, w/o air","l");
	Legend1->AddEntry(AngleDistribution_AIR_All,"All particles, in air","l");
	Legend1->Draw();

      	m_subpad2->cd();
  	TH1F* hDiv;

	string hDivName;
    	hDivName =  "AngleDistribution_Difference";

      	hDiv = new TH1F(hDivName.c_str(),hDivName.c_str(), AngleDistribution_All->GetNbinsX(), AngleDistribution_All->GetXaxis()->GetXmin(), AngleDistribution_All->GetXaxis()->GetXmax());
    	hDiv->Divide(AngleDistribution_AIR_All,AngleDistribution_All,1,1,"b");
    	hDiv->SetLineColor(AngleDistribution_All->GetLineColor());
    	hDiv->SetLineWidth(1);
    	hDiv->SetMarkerStyle(AngleDistribution_All->GetMarkerStyle());
    	hDiv->SetMarkerColor(AngleDistribution_All->GetMarkerColor());
    	hDiv->SetMarkerSize(.8);
    	hDiv->Draw("EP");	

	TLine *line = new TLine(AngleDistribution_All->GetBinLowEdge(1),1,AngleDistribution_All->GetBinLowEdge(AngleDistribution_All->GetNbinsX()+1),1);
  	line->SetLineWidth(2);
  	line->SetLineColor(kBlack);
  	line->Draw();
  	hDiv->GetXaxis()->SetTitle(AngleDistribution_All->GetXaxis()->GetTitle());
  	hDiv->GetYaxis()->SetTitle("Ratio");
  	hDiv->GetXaxis()->SetTitleSize(0.17);
  	hDiv->GetYaxis()->SetTitleSize(0.15);
  	hDiv->GetXaxis()->SetTitleOffset(0.80);
  	hDiv->GetYaxis()->SetTitleOffset(0.3);
  	hDiv->SetLabelSize(0.13,"X");
  	hDiv->SetLabelSize(0.07,"Y");
  	hDiv->SetTitle("");
  	hDiv->GetXaxis()->SetLabelOffset(0.010);
  	hDiv->GetYaxis()->SetLabelOffset(0.015);
  	hDiv->SetStats(kFALSE);
  	hDiv->SetDirectory(0);
    	hDiv->SetMinimum(0.9);    
	hDiv->SetMaximum(1.1);

	string canvasname5=CompareAngle_Canvas->GetName();
	CompareAngle_Canvas->Print((canvasname5+".eps").c_str());
	CompareAngle_Canvas->Print((canvasname5+".C").c_str());
	CompareAngle_Canvas->Write();
	CompareAngle_Canvas->Close();

	output_rootfile->Write();
	input_rootfile1->Close();
	input_rootfile2->Close();
}
