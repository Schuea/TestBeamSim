#include "TFile.h"
#include "TBrowser.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1.h"
#include "TF1.h"
#include "TCutG.h"
#include "TH2.h"
#include "TStyle.h"
#include "TPaveText.h"
#include "TLatex.h"
#include "TEllipse.h"
#include "TPaveStats.h"
#include "TProfile.h"
#include "TGraph.h"
#include "TMath.h"
#include "TROOT.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>

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
	TTree * T = (TTree*)input_rootfile->Get("Tree_TrackerPlane2");
	cout << "Accessed TTree.." << endl;

	stringstream output_filename;
	output_filename << "position_plots_" << name;
	TFile * output_rootfile = new TFile(output_filename.str().c_str(),"RECREATE");

	TH2F * Position_allCharge = new TH2F("Posi_allCharge","Position behind magnet of all charged particles",300,-400,400,300,-300,300);
	Position_allCharge->GetXaxis()->SetTitle("x (mm)");
	Position_allCharge->GetXaxis()->CenterTitle();
	Position_allCharge->GetYaxis()->SetTitle("y (mm)");
	Position_allCharge->GetYaxis()->CenterTitle();
	Position_allCharge->GetZaxis()->SetRangeUser(1,60);
	TH2F * Position_negCharge = new TH2F("Posi_negCharge","Position behind magnet of particles with negative charge",300,-400,400,300,-300,300);
	Position_negCharge->GetXaxis()->SetTitle("x (mm)");
	Position_negCharge->GetXaxis()->CenterTitle();
	Position_negCharge->GetYaxis()->SetTitle("y (mm)");
	Position_negCharge->GetYaxis()->CenterTitle();
	Position_negCharge->GetZaxis()->SetRangeUser(1,60);
	TH2F * Position_posCharge = new TH2F("Posi_posCharge","Position behind magnet of particles with positive charge",300,-400,400,300,-300,300);
	Position_posCharge->GetXaxis()->SetTitle("x (mm)");
	Position_posCharge->GetXaxis()->CenterTitle();
	Position_posCharge->GetYaxis()->SetTitle("y (mm)");
	Position_posCharge->GetYaxis()->CenterTitle();
	Position_posCharge->GetZaxis()->SetRangeUser(1,60);

		
	TCanvas* Posi_allCharge_Canvas_ = new TCanvas("Position_allCharge_Canvas");
	TCanvas* Posi_negCharge_Canvas_ = new TCanvas("Position_negCharge_Canvas");
	TCanvas* Posi_posCharge_Canvas_ = new TCanvas("Position_posCharge_Canvas");
		
	gROOT->ForceStyle();

	Posi_negCharge_Canvas_->cd();
	gPad->SetLogz();
	gStyle->SetOptStat(11);
	gStyle->SetNumberContours(999);
	T->Draw("HitPosition_y:HitPosition_x >>+ Posi_negCharge","HitCharge<0","colz");	
	gPad->Update();
        TPaveStats *st1_pos = (TPaveStats*)Position_negCharge->FindObject("stats");
        double statboxsize_pos_x = st1_pos->GetX2NDC() - st1_pos->GetX1NDC();
        st1_pos->SetX1NDC(0.70);
        st1_pos->SetX2NDC(0.70+statboxsize_pos_x);	
/*
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
*/
	TEllipse * el1 = new TEllipse(0,0,50,50);
	el1->SetLineWidth(2.5);
	el1->SetFillStyle(0000);
	el1->Draw();

	Posi_posCharge_Canvas_->cd();
	gPad->SetLogz();
	gStyle->SetOptStat(11);
	gStyle->SetNumberContours(999);
	T->Draw("HitPosition_y:HitPosition_x >>+ Posi_posCharge","HitCharge>0","colz");	
//	l1->Draw();
//	l2->Draw();
//	l3->Draw();
//	l4->Draw();
	el1->Draw();
	gPad->Update();
        TPaveStats *st2_pos = (TPaveStats*)Position_posCharge->FindObject("stats");
        double statboxsize_pos_x_2 = st2_pos->GetX2NDC() - st2_pos->GetX1NDC();
        st2_pos->SetX1NDC(0.70);
        st2_pos->SetX2NDC(0.70+statboxsize_pos_x_2);	

	Posi_allCharge_Canvas_->cd();
	gPad->SetLogz();
	gStyle->SetOptStat(11);
	gStyle->SetNumberContours(999);
	T->Draw("HitPosition_y:HitPosition_x >>+ Posi_allCharge","","colz");	
//	l1->Draw();
//	l2->Draw();
//	l3->Draw();
//	l4->Draw();
	el1->Draw();
	gPad->Update();
        TPaveStats *st3_pos = (TPaveStats*)Position_allCharge->FindObject("stats");
        double statboxsize_pos_x_3 = st3_pos->GetX2NDC() - st3_pos->GetX1NDC();
        st3_pos->SetX1NDC(0.70);
        st3_pos->SetX2NDC(0.70+statboxsize_pos_x_3);	


	string canvasname=Posi_negCharge_Canvas_->GetName();
	Posi_negCharge_Canvas_->Write();
	Posi_negCharge_Canvas_->Print((canvasname+"_"+name+".eps").c_str());
	Posi_negCharge_Canvas_->Print((canvasname+"_"+name+".C").c_str());
	Posi_negCharge_Canvas_->Close();

	string canvasname2=Posi_posCharge_Canvas_->GetName();
	Posi_posCharge_Canvas_->Write();
	Posi_posCharge_Canvas_->Print((canvasname2+"_"+name+".eps").c_str());
	Posi_posCharge_Canvas_->Print((canvasname2+"_"+name+".C").c_str());
	Posi_posCharge_Canvas_->Close();

	string canvasname3=Posi_allCharge_Canvas_->GetName();
	Posi_allCharge_Canvas_->Write();
	Posi_allCharge_Canvas_->Print((canvasname3+"_"+name+".eps").c_str());
	Posi_allCharge_Canvas_->Print((canvasname3+"_"+name+".C").c_str());
	Posi_allCharge_Canvas_->Close();

	output_rootfile->Write();
	input_rootfile->Close();

	delete output_rootfile, input_rootfile;
//	delete l1,l2,l3,l4;
	delete el1;
	delete Posi_negCharge_Canvas_, Posi_posCharge_Canvas_, Posi_allCharge_Canvas_; 

}

