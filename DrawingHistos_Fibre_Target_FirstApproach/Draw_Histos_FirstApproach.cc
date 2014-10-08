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
#include "TPaveStats.h"

#include <cmath>
#include <algorithm>
#include <map>


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

void FirstApproach(string name);

int main(int argc,char *argv[]){
        if(argc < 2 || argc > 2){
		cerr << "Type in root filename!" << endl;
                cerr << "e.g. ./DrawHistograms file.root" << endl;
                terminate();
        }
        string filename;
        filename = argv[1];

        FirstApproach(filename);
}

void FirstApproach(string name){
        TH1::SetDefaultSumw2();

        TFile * input_rootfile = new TFile(name.c_str(),"READ");
        cout << "Inputfile size = " << input_rootfile->GetSize() << endl;
        TTree * Tree = (TTree*)input_rootfile->Get("Tree");
        cout << "Accessed TTree.." << endl;

        stringstream output_filename;
        output_filename << "Histos_FirstApproach_" << name;
        TFile * output_rootfile = new TFile(output_filename.str().c_str(),"RECREATE");

//ONLY FIBRE:  ------------------	
	TH1F* Photon_energy_Fibre = new TH1F("PhotonSource","Energy distribution of photons created in Carbon fibre",500,0,6.5);
	Photon_energy_Fibre->GetXaxis()->SetTitle("Energy (GeV)");
	Photon_energy_Fibre->GetXaxis()->CenterTitle();
	TH2F* Photon_Deflection_Fibre = new TH2F("PhotonDeflection","Deflection of photons created in Carbon fibre",500,-30,30,500,-30,30);
	Photon_Deflection_Fibre->SetOption("colz");
	Photon_Deflection_Fibre->GetXaxis()->SetTitle("x (mm)");
	Photon_Deflection_Fibre->GetXaxis()->CenterTitle();
	Photon_Deflection_Fibre->GetYaxis()->SetTitle("y (mm)");
	Photon_Deflection_Fibre->GetYaxis()->CenterTitle();
	TH2F* Photon_Deflection_Fibre2 = new TH2F("PhotonDeflection2","Deflection of photons created in Carbon fibre",500,-30,30,500,-30,30);
	Photon_Deflection_Fibre2->SetOption("colz");
	Photon_Deflection_Fibre2->GetXaxis()->SetTitle("x (mm)");
	Photon_Deflection_Fibre2->GetXaxis()->CenterTitle();
	Photon_Deflection_Fibre2->GetYaxis()->SetTitle("y (mm)");
	Photon_Deflection_Fibre2->GetYaxis()->CenterTitle();
	TH2F* Photon_Deflection_Fibre3 = new TH2F("PhotonDeflection3","Deflection of photons created in Carbon fibre",500,-30,30,500,-30,30);
	Photon_Deflection_Fibre3->SetOption("colz");
	Photon_Deflection_Fibre3->GetXaxis()->SetTitle("x (mm)");
	Photon_Deflection_Fibre3->GetXaxis()->CenterTitle();
	Photon_Deflection_Fibre3->GetYaxis()->SetTitle("y (mm)");
	Photon_Deflection_Fibre3->GetYaxis()->CenterTitle();

	TCanvas* FibreEnergy_Canvas = new TCanvas("Fibre_Energy");
	TCanvas* FibreDeflection_Canvas = new TCanvas("Fibre_Deflection");
	
	FibreEnergy_Canvas->cd();
	gPad->SetLogy();
	Tree->Draw("Energy >>+ PhotonSource","Particle_ID==22");

	string Fibre_Canvas1=FibreEnergy_Canvas->GetName();
	FibreEnergy_Canvas->Write();
	FibreEnergy_Canvas->Print((Fibre_Canvas1+".eps").c_str());
	FibreEnergy_Canvas->Print((Fibre_Canvas1+".C").c_str());
	FibreEnergy_Canvas->Close();

	FibreDeflection_Canvas->Divide(2,2);
	FibreDeflection_Canvas->cd(1);
	gPad->SetLogz();
	gStyle->SetOptStat(11);
	Tree->Draw("Reflectiony:Reflectionx >>+ PhotonDeflection","Particle_ID==22", "colz");
	gPad->Update();
  	TPaveStats *Fibre_st1 = (TPaveStats*)Photon_Deflection_Fibre->FindObject("stats");
  	double statboxsize = Fibre_st1->GetX2NDC() - Fibre_st1->GetX1NDC();
  	Fibre_st1->SetX1NDC(0.7);
  	Fibre_st1->SetX2NDC(0.7+statboxsize);

	FibreDeflection_Canvas->cd(2);
	gPad->SetLogz();
	gStyle->SetOptStat(11);
	Tree->Draw("Reflectiony:Reflectionx >>+ PhotonDeflection2","Particle_ID==22 && 0.1<Energy && Energy<1.0", "colz");
	gPad->Update();
  	TPaveStats *Fibre_st2 = (TPaveStats*)Photon_Deflection_Fibre->FindObject("stats");
  	double statboxsize2 = Fibre_st2->GetX2NDC() - Fibre_st2->GetX1NDC();
  	Fibre_st2->SetX1NDC(0.7);
  	Fibre_st2->SetX2NDC(0.7+statboxsize2);

	FibreDeflection_Canvas->cd(3);
	gPad->SetLogz();
	gStyle->SetOptStat(11);
	Tree->Draw("Reflectiony:Reflectionx >>+ PhotonDeflection3","Particle_ID==22 && Energy>1.0", "colz");
	gPad->Update();
  	TPaveStats *Fibre_st3 = (TPaveStats*)Photon_Deflection_Fibre->FindObject("stats");
  	double statboxsize3 = Fibre_st3->GetX2NDC() - Fibre_st3->GetX1NDC();
  	Fibre_st3->SetX1NDC(0.7);
  	Fibre_st3->SetX2NDC(0.7+statboxsize3);

	string Fibre_Canvas2=FibreDeflection_Canvas->GetName();
	FibreDeflection_Canvas->Write();
	FibreDeflection_Canvas->Print((Fibre_Canvas2+".eps").c_str());
	FibreDeflection_Canvas->Print((Fibre_Canvas2+".C").c_str());
	FibreDeflection_Canvas->Close();

//FIBRE AND CONVERTER:  ------------------
	TH1F* Photon_Energy_Target = new TH1F("E_Photon","Energy distribution of photons existing after target",500,0,7);
	Photon_Energy_Target->GetXaxis()->SetTitle("Energy (GeV)");
	Photon_Energy_Target->GetXaxis()->CenterTitle();
	TH1F* Electron_Energy_Target = new TH1F("E_Electron","Energy distribution of electrons created in target",500,0,7);
	Electron_Energy_Target->GetXaxis()->SetTitle("Energy (GeV)");
	Electron_Energy_Target->GetXaxis()->CenterTitle();
	TH1F* Positron_Energy_Target= new TH1F("E_Positron","Energy distribution of positrons created in target",500,0,7);
	Positron_Energy_Target->GetXaxis()->SetTitle("Energy (GeV)");
	Positron_Energy_Target->GetXaxis()->CenterTitle();

	TH2F* Photon_Deflection_Target = new TH2F("DeflTar_Photon","Deflection of photons existing after target",800,-20,20,800,-20,20);
	Photon_Deflection_Target->SetOption("colz");
	Photon_Deflection_Target->GetZaxis()->SetRangeUser(50,200);
	Photon_Deflection_Target->GetXaxis()->SetTitle("x-position (mm)");
	Photon_Deflection_Target->GetXaxis()->CenterTitle();
	Photon_Deflection_Target->GetYaxis()->SetTitle("y-position (mm)");
	Photon_Deflection_Target->GetYaxis()->CenterTitle();
	TH2F* Electron_Deflection_Target = new TH2F("DeflTar_Electron","Deflection of electrons created in target",800,-6,6,800,-6,6);
	Electron_Deflection_Target->SetOption("colz");
	Electron_Deflection_Target->GetZaxis()->SetRangeUser(5,200);
	Electron_Deflection_Target->GetXaxis()->SetTitle("x-position (mm)");
	Electron_Deflection_Target->GetXaxis()->CenterTitle();
	Electron_Deflection_Target->GetYaxis()->SetTitle("y-position (mm)");
	Electron_Deflection_Target->GetYaxis()->CenterTitle();
	TH2F* Positron_Deflection_Target = new TH2F("DeflTar_Positron","Deflection of positrons created in target",800,-6,6,800,-6,6);
	Positron_Deflection_Target->SetOption("colz");
	Positron_Deflection_Target->GetZaxis()->SetRangeUser(2,400);
	Positron_Deflection_Target->GetXaxis()->SetTitle("x-position (mm)");
	Positron_Deflection_Target->GetXaxis()->CenterTitle();
	Positron_Deflection_Target->GetYaxis()->SetTitle("y-position (mm)");
	Positron_Deflection_Target->GetYaxis()->CenterTitle();

	TH2F* Photon_Deflection_Target_Cut = new TH2F("DeflTar_Photon_Cut","Deflection of photons existing after target",700,-4,4,700,-4,4);
	Photon_Deflection_Target_Cut->SetOption("colz");
	Photon_Deflection_Target_Cut->GetZaxis()->SetRangeUser(5,120);
	Photon_Deflection_Target_Cut->GetXaxis()->SetTitle("x-position (mm)");
	Photon_Deflection_Target_Cut->GetXaxis()->CenterTitle();
	Photon_Deflection_Target_Cut->GetYaxis()->SetTitle("y-position (mm)");
	Photon_Deflection_Target_Cut->GetYaxis()->CenterTitle();
	TH2F* Electron_Deflection_Target_Cut = new TH2F("DeflTar_Electron_Cut","Deflection of electrons created in target",400,-1,1,400,-1,1);
	Electron_Deflection_Target_Cut->SetOption("colz");
	Electron_Deflection_Target_Cut->GetZaxis()->SetRangeUser(2,150);
	Electron_Deflection_Target_Cut->GetXaxis()->SetTitle("x-position (mm)");
	Electron_Deflection_Target_Cut->GetXaxis()->CenterTitle();
	Electron_Deflection_Target_Cut->GetYaxis()->SetTitle("y-position (mm)");
	Electron_Deflection_Target_Cut->GetYaxis()->CenterTitle();
	TH2F* Positron_Deflection_Target_Cut = new TH2F("DeflTar_Positron_Cut","Deflection of positrons created in target",150,-1,1,150,-1,1);
	Positron_Deflection_Target_Cut->SetOption("colz");
	Positron_Deflection_Target_Cut->GetZaxis()->SetRangeUser(0,45);
	Positron_Deflection_Target_Cut->GetXaxis()->SetTitle("x-position (mm)");
	Positron_Deflection_Target_Cut->GetXaxis()->CenterTitle();
	Positron_Deflection_Target_Cut->GetYaxis()->SetTitle("y-position (mm)");
	Positron_Deflection_Target_Cut->GetYaxis()->CenterTitle();

	TCanvas* TargetDeflection_Canvas = new TCanvas("Target_Deflection_Photons");
	TCanvas* TargetDeflection_Canvas2 = new TCanvas("Target_Deflection_Electrons");
	TCanvas* TargetDeflection_Canvas3 = new TCanvas("Target_Deflection_Positrons");
       	TCanvas* TargetDeflection_Canvas_Cut = new TCanvas("Target_Deflection_Cut_Photons");
	TCanvas* TargetDeflection_Canvas_Cut2 = new TCanvas("Target_Deflection_Cut_Electrons");
	TCanvas* TargetDeflection_Canvas_Cut3 = new TCanvas("Target_Deflection_Cut_Positrons");
	TCanvas* TargetEnergy_Canvas = new TCanvas("Target_Energy_Photons");
	TCanvas* TargetEnergy_Canvas2 = new TCanvas("Target_Energy_Electrons");
	TCanvas* TargetEnergy_Canvas3 = new TCanvas("Target_Energy_Positrons");
	TCanvas* TargetEnergy_Canvas_Cut = new TCanvas("Target_Energy_Cut_Photons");
	TCanvas* TargetEnergy_Canvas_Cut2 = new TCanvas("Target_Energy_Cut_Electrons");
	TCanvas* TargetEnergy_Canvas_Cut3 = new TCanvas("Target_Energy_Cut_Positrons");

	TargetDeflection_Canvas->cd();
        gStyle->SetNumberContours(999);
        gPad->SetLogz();
        Tree->Draw("Reflectiony:Reflectionx >>+ DeflTar_Photon","Particle_ID==22","colz");
        gPad->Update();
        TPaveStats *st1 = (TPaveStats*)Photon_Deflection_Target->FindObject("stats");
        double Deflection_statboxsize1 = st1->GetX2NDC() - st1->GetX1NDC();
        st1->SetX1NDC(0.7);
        st1->SetX2NDC(0.7+Deflection_statboxsize1);
	string canvasname=TargetDeflection_Canvas->GetName();
        TargetDeflection_Canvas->Write();
        TargetDeflection_Canvas->Print((canvasname+".eps").c_str());
        TargetDeflection_Canvas->Print((canvasname+".C").c_str());
        TargetDeflection_Canvas->Close();

	TargetDeflection_Canvas2->cd();
        gStyle->SetNumberContours(999);
        gPad->SetLogz();
        Tree->Draw("Reflectiony:Reflectionx >>+ DeflTar_Electron","Particle_ID==11","colz");
        gPad->Update();
        TPaveStats *st2 = (TPaveStats*)Electron_Deflection_Target->FindObject("stats");
        double Deflection_statboxsize2 = st2->GetX2NDC() - st2->GetX1NDC();
        st2->SetX1NDC(0.7);
        st2->SetX2NDC(0.7+Deflection_statboxsize2);
	string canvasname2=TargetDeflection_Canvas2->GetName();
        TargetDeflection_Canvas2->Write();
        TargetDeflection_Canvas2->Print((canvasname2+".eps").c_str());
        TargetDeflection_Canvas2->Print((canvasname2+".C").c_str());
        TargetDeflection_Canvas2->Close();

	TargetDeflection_Canvas3->cd();
        gStyle->SetNumberContours(999);
        gPad->SetLogz();
        Tree->Draw("Reflectiony:Reflectionx >>+ DeflTar_Positron","Particle_ID==-11","colz");
        gPad->Update();
        TPaveStats *st3 = (TPaveStats*)Positron_Deflection_Target->FindObject("stats");
        double Deflection_statboxsize3 = st3->GetX2NDC() - st3->GetX1NDC();
        st3->SetX1NDC(0.7);
        st3->SetX2NDC(0.7+Deflection_statboxsize3);
	string canvasname3=TargetDeflection_Canvas3->GetName();
        TargetDeflection_Canvas3->Write();
        TargetDeflection_Canvas3->Print((canvasname3+".eps").c_str());
        TargetDeflection_Canvas3->Print((canvasname3+".C").c_str());
        TargetDeflection_Canvas3->Close();

	TargetDeflection_Canvas_Cut->cd();
        gStyle->SetNumberContours(999);
        gPad->SetLogz();
        Tree->Draw("Reflectiony:Reflectionx >>+ DeflTar_Photon_Cut","Particle_ID==22 && Energy > 1.0","colz");
        gPad->Update();
        TPaveStats *st4 = (TPaveStats*)Photon_Deflection_Target_Cut->FindObject("stats");
        double Deflection_statboxsize4 = st4->GetX2NDC() - st4->GetX1NDC();
        st4->SetX1NDC(0.7);
        st4->SetX2NDC(0.7+Deflection_statboxsize4);
	string canvasname_Cut=TargetDeflection_Canvas_Cut->GetName();
        TargetDeflection_Canvas_Cut->Write();
        TargetDeflection_Canvas_Cut->Print((canvasname_Cut+".eps").c_str());
        TargetDeflection_Canvas_Cut->Print((canvasname_Cut+".C").c_str());
        TargetDeflection_Canvas_Cut->Close();

	TargetDeflection_Canvas_Cut2->cd();
        gStyle->SetNumberContours(999);
        gPad->SetLogz();
        Tree->Draw("Reflectiony:Reflectionx >>+ DeflTar_Electron_Cut","Particle_ID==11 && Energy > 1.0","colz");
        gPad->Update();
        TPaveStats *st5 = (TPaveStats*)Electron_Deflection_Target_Cut->FindObject("stats");
        double Deflection_statboxsize5 = st5->GetX2NDC() - st5->GetX1NDC();
        st5->SetX1NDC(0.7);
        st5->SetX2NDC(0.7+Deflection_statboxsize5);
	string canvasname_Cut2=TargetDeflection_Canvas_Cut2->GetName();
        TargetDeflection_Canvas_Cut2->Write();
        TargetDeflection_Canvas_Cut2->Print((canvasname_Cut2+".eps").c_str());
        TargetDeflection_Canvas_Cut2->Print((canvasname_Cut2+".C").c_str());
        TargetDeflection_Canvas_Cut2->Close();

	TargetDeflection_Canvas_Cut3->cd();
        gStyle->SetNumberContours(999);
        gPad->SetLogz();
        Tree->Draw("Reflectiony:Reflectionx >>+ DeflTar_Positron_Cut","Particle_ID==-11 && Energy > 1.0","colz");
        gPad->Update();
        TPaveStats *st6 = (TPaveStats*)Positron_Deflection_Target_Cut->FindObject("stats");
        double Deflection_statboxsize6 = st6->GetX2NDC() - st6->GetX1NDC();
        st6->SetX1NDC(0.7);
        st6->SetX2NDC(0.7+Deflection_statboxsize6);
	string canvasname_Cut3=TargetDeflection_Canvas_Cut3->GetName();
        TargetDeflection_Canvas_Cut3->Write();
        TargetDeflection_Canvas_Cut3->Print((canvasname_Cut3+".eps").c_str());
        TargetDeflection_Canvas_Cut3->Print((canvasname_Cut3+".C").c_str());
        TargetDeflection_Canvas_Cut3->Close();


	TargetEnergy_Canvas->cd();
        gPad->SetLogy();
        Tree->Draw("Energy >>+ E_Photon","Particle_ID==22","colz");
        gPad->Update();
        TPaveStats *Energy_st1 = (TPaveStats*)Photon_Energy_Target->FindObject("stats");
        double Energy_statboxsize1 = Energy_st1->GetX2NDC() - Energy_st1->GetX1NDC();
        Energy_st1->SetX1NDC(0.7);
        Energy_st1->SetX2NDC(0.7+Energy_statboxsize1);
	string canvasname_Energy=TargetEnergy_Canvas->GetName();
        TargetEnergy_Canvas->Write();
        TargetEnergy_Canvas->Print((canvasname_Energy+".eps").c_str());
        TargetEnergy_Canvas->Print((canvasname_Energy+".C").c_str());
        TargetEnergy_Canvas->Close();

	TargetEnergy_Canvas2->cd();
        gPad->SetLogy();
        Tree->Draw("Energy >>+ E_Electron","Particle_ID==11","colz");
        gPad->Update();
        TPaveStats *Energy_st2 = (TPaveStats*)Electron_Energy_Target->FindObject("stats");
        double Energy_statboxsize2 = Energy_st2->GetX2NDC() - Energy_st2->GetX1NDC();
        Energy_st2->SetX1NDC(0.7);
        Energy_st2->SetX2NDC(0.7+Energy_statboxsize2);
	string canvasname_Energy2=TargetEnergy_Canvas2->GetName();
        TargetEnergy_Canvas2->Write();
        TargetEnergy_Canvas2->Print((canvasname_Energy2+".eps").c_str());
        TargetEnergy_Canvas2->Print((canvasname_Energy2+".C").c_str());
        TargetEnergy_Canvas2->Close();

	TargetEnergy_Canvas3->cd();
        gPad->SetLogy();
        Tree->Draw("Energy >>+ E_Positron","Particle_ID==-11","colz");
        gPad->Update();
        TPaveStats *Energy_st3 = (TPaveStats*)Positron_Energy_Target->FindObject("stats");
        double Energy_statboxsize3 = Energy_st3->GetX2NDC() - Energy_st3->GetX1NDC();
        Energy_st3->SetX1NDC(0.7);
        Energy_st3->SetX2NDC(0.7+Energy_statboxsize3);
	string canvasname_Energy3=TargetEnergy_Canvas3->GetName();
        TargetEnergy_Canvas3->Write();
        TargetEnergy_Canvas3->Print((canvasname_Energy3+".eps").c_str());
        TargetEnergy_Canvas3->Print((canvasname_Energy3+".C").c_str());
        TargetEnergy_Canvas3->Close();

	output_rootfile->Write();
	input_rootfile->Close();
}
