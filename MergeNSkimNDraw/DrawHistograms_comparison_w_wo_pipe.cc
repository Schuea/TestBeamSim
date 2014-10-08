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
#include "TPaveStats.h"
#include "TEllipse.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

void DrawingMacro(string outputname,string name1, string name2); 

int main(int argc,char *argv[]){
	if(argc < 4 || argc > 4){
    		//explain how to use program	
    		cerr << "Type in name of output file AND both root filenames, first w/o pipe, second with pipe!" << endl;
    		cerr << "e.g. ./DrawHistograms outputfilename.root file_w/o_pipe.root file_pipe.root" << endl;
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

	TH1F * EnergyHisto_All_Pipe = new TH1F("E_All_Pipe","Normalised energy distribution of all photons",100,0,7);
	EnergyHisto_All_Pipe->SetLineColor(4);
	EnergyHisto_All_Pipe->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_All_Pipe->GetXaxis()->CenterTitle();
	TH1F * EnergyHisto_All = new TH1F("E_All","Normalised energy distribution of all photons",100,0,7);
	EnergyHisto_All->SetLineColor(2);
	EnergyHisto_All->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_All->GetXaxis()->CenterTitle();

	TH1F * AngleDistribution_Pipe_All = new TH1F("Angle_Pipe_All","Angular distribution of photons",100,0,0.5);
	AngleDistribution_Pipe_All->SetLineColor(4);
	AngleDistribution_Pipe_All->GetXaxis()->SetTitle("#theta (degree)");
	AngleDistribution_Pipe_All->GetXaxis()->CenterTitle();
	TH1F * AngleDistribution_All= new TH1F("Angle_All","Angular distribution of photons",100,0,0.5);
	AngleDistribution_All->SetLineColor(2);
	AngleDistribution_All->GetXaxis()->SetTitle("#theta (degree)");
	AngleDistribution_All->GetXaxis()->CenterTitle();
	TH1F * AngleDistribution_All_Subtraction;

	TH2F * PositionMap_Photons = new TH2F("Position_Photons","Position of photons at z=4m behind fibre",300,-40,40,300,-40,40);
	PositionMap_Photons->GetXaxis()->SetTitle("x-position (mm)");
	PositionMap_Photons->GetXaxis()->CenterTitle();
	PositionMap_Photons->GetYaxis()->SetTitle("y-position (mm)");
	PositionMap_Photons->GetYaxis()->CenterTitle();

	TCanvas * Energy_Canvas = new TCanvas("E_Canvas","Energy distributions");	
	TCanvas * CompareAngle_Canvas = new TCanvas("CompareAngle_Canvas","Angular distribution");
	TCanvas * Position_Canvas = new TCanvas("Position_Canvas");

	TLegend * Legend1 = new TLegend(0.5,0.6,0.8,0.8);
	Legend1->SetFillColor(0);
	TLegend * Legend2 = new TLegend(0.5,0.6,0.8,0.8);
	Legend2->SetFillColor(0);

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
	EnergyHisto_All_Pipe->SetStats(kFALSE);
	T2->Draw("Energy >>+ E_All_Pipe","Particle_ID==22");
	T1->Draw("Energy >>+ E_All","Particle_ID==22","same");
	Legend2->AddEntry(EnergyHisto_All,"All photons, w/o pipe","l");
	Legend2->AddEntry(EnergyHisto_All_Pipe,"All photons, with pipe","l");
	Legend2->Draw();

      	subpad2->cd();
  	TH1F* E_Diff;

	string E_DiffName;
    	E_DiffName =  "EnergyDistribution_Difference";

      	E_Diff = new TH1F(E_DiffName.c_str(),E_DiffName.c_str(), EnergyHisto_All->GetNbinsX(), EnergyHisto_All->GetXaxis()->GetXmin(), EnergyHisto_All->GetXaxis()->GetXmax());
    	E_Diff->Divide(EnergyHisto_All_Pipe,EnergyHisto_All,1,1,"b");
    	E_Diff->SetLineColor(EnergyHisto_All_Pipe->GetLineColor());
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
    	E_Diff->SetMinimum(0.9);    
	E_Diff->SetMaximum(1.1);

	string canvasname1=Energy_Canvas->GetName();
	Energy_Canvas->Write();
	Energy_Canvas->Print((canvasname1+".eps").c_str());	
	Energy_Canvas->Print((canvasname1+".C").c_str());	
	Energy_Canvas->Close();


	Position_Canvas->cd();
	gPad->SetLogz();
	gStyle->SetOptStat(11);
	gStyle->SetNumberContours(999);
	T2->Draw("Reflectiony:Reflectionx >>+ Position_Photons","Particle_ID==22","colz");
	gPad->Update();
 	TPaveStats *st1 = (TPaveStats*)PositionMap_Photons->FindObject("stats");
  	double statboxsize = st1->GetX2NDC() - st1->GetX1NDC();
  	st1->SetX1NDC(0.7);
  	st1->SetX2NDC(0.7+statboxsize);	

	TEllipse * el1 = new TEllipse(0,0,20,20);
	el1->SetLineWidth(2);
	el1->SetFillStyle(0000);
	el1->Draw();

	string canvasname3=Position_Canvas->GetName();
	Position_Canvas->Write();
	Position_Canvas->Print((canvasname3+".eps").c_str());	
	Position_Canvas->Print((canvasname3+".C").c_str());	
	Position_Canvas->Close();

	CompareAngle_Canvas->cd();
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
	AngleDistribution_Pipe_All->SetStats(kFALSE);
	T1->Draw("Theta >>+ Angle_All","Particle_ID==22");
	T2->Draw("Theta >>+ Angle_Pipe_All","Particle_ID==22","same");
	Legend1->AddEntry(AngleDistribution_All,"All particles, w/o pipe","l");
	Legend1->AddEntry(AngleDistribution_Pipe_All,"All particles, with pipe","l");
	Legend1->Draw();

      	m_subpad2->cd();
  	TH1F* hDiv;

	string hDivName;
    	hDivName =  "AngleDistribution_Difference";

      	hDiv = new TH1F(hDivName.c_str(),hDivName.c_str(), AngleDistribution_All->GetNbinsX(), AngleDistribution_All->GetXaxis()->GetXmin(), AngleDistribution_All->GetXaxis()->GetXmax());
    	hDiv->Divide(AngleDistribution_Pipe_All,AngleDistribution_All,1,1,"b");
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
    	hDiv->SetMinimum(0.6);    
	hDiv->SetMaximum(1.4);

	string canvasname5=CompareAngle_Canvas->GetName();
	CompareAngle_Canvas->Print((canvasname5+".eps").c_str());
	CompareAngle_Canvas->Print((canvasname5+".C").c_str());
	CompareAngle_Canvas->Write();
	CompareAngle_Canvas->Close();

	output_rootfile->Write();
	input_rootfile1->Close();
	input_rootfile2->Close();
}
