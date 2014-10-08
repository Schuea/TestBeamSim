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
#include "TText.h"
#include "TPaveStats.h"
#include "TEllipse.h"
#include "TROOT.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

void DrawingMacro(string outputname,string name1); 

int main(int argc,char *argv[]){
	if(argc < 3 || argc > 3){
    		//explain how to use program	
    		cerr << "Type in name of output file AND all root filenames!" << endl;
    		cerr << "e.g. ./DrawHistograms outputfilename.root file1.root file2.root ..." << endl;
    		terminate();
	}	
	string filename1, outputfilename;
	outputfilename = argv[1];
	filename1 = argv[2];
       
	DrawingMacro(outputfilename,filename1);
}

void DrawingMacro(string outputname,string name1){

	TFile * input_rootfile1 = new TFile(name1.c_str(),"READ");
	TTree * T1;
	input_rootfile1->GetObject("Tree",T1);
	cout << "Accessed TTrees.." << endl;

	TFile * output_rootfile = new TFile(outputname.c_str(),"RECREATE");

	TH1::SetDefaultSumw2();

	TH1F * MomentumHisto_Phot = new TH1F("Momentum_Phot","Momentum distribution of particles behind the lead shielding with 20cm thickness and concrete shielding with 20cm thickness.",93,0,6.5);
	MomentumHisto_Phot->SetLineColor(3);
	MomentumHisto_Phot->GetXaxis()->SetTitle("Momentum (GeV)");
	MomentumHisto_Phot->GetXaxis()->CenterTitle();
	TH1F * MomentumHisto_ElePosi = new TH1F("Momentum_ElePosi","Momentum distribution of particles behind the lead shielding with 20cm thickness and concrete shielding with 20cm thickness.",93,0,6.5);
	MomentumHisto_ElePosi->SetLineColor(4);
	MomentumHisto_ElePosi->GetXaxis()->SetTitle("Momentum (GeV)");
	MomentumHisto_ElePosi->GetXaxis()->CenterTitle();
	TH1F * MomentumHisto_ProtNeut = new TH1F("Momentum_ProtNeut","Momentum distribution of particles behind the lead shielding with 20cm thickness and concrete shielding with 20cm thickness.",93,0,6.5);
	MomentumHisto_ProtNeut->SetLineColor(2);
	MomentumHisto_ProtNeut->GetXaxis()->SetTitle("Momentum (GeV)");
	MomentumHisto_ProtNeut->GetXaxis()->CenterTitle();

	TH2F * FluxMap_Phot = new TH2F("FluxMap_Phot","Flux map of photons behind the lead shielding with 20cm thickness and concrete shielding with 20cm thickness.",300,-300,300,300,-300,300);
	FluxMap_Phot->GetXaxis()->SetTitle("x (mm)");
	FluxMap_Phot->GetXaxis()->CenterTitle();
	FluxMap_Phot->GetYaxis()->SetTitle("y (mm)");
	FluxMap_Phot->GetYaxis()->CenterTitle();
	TH2F * FluxMap_ElePosi = new TH2F("FluxMap_ElePosi","Flux map of electrons and positrons behind the lead shielding with 20cm thickness and concrete shielding with 20cm thickness.",300,-300,300,300,-300,300);
	FluxMap_ElePosi->GetXaxis()->SetTitle("x (mm)");
	FluxMap_ElePosi->GetXaxis()->CenterTitle();
	FluxMap_ElePosi->GetYaxis()->SetTitle("y (mm)");
	FluxMap_ElePosi->GetYaxis()->CenterTitle();
	TH2F * FluxMap_ProtNeut = new TH2F("FluxMap_ProtNeut","Flux map of protons and neutrons behind the lead shielding with 20cm thickness and concrete shielding with 20cm thickness.",300,-300,300,300,-300,300);
	FluxMap_ProtNeut->GetXaxis()->SetTitle("x (mm)");
	FluxMap_ProtNeut->GetXaxis()->CenterTitle();
	FluxMap_ProtNeut->GetYaxis()->SetTitle("y (mm)");
	FluxMap_ProtNeut->GetYaxis()->CenterTitle();


	TCanvas * FluxMap_Canvas = new TCanvas("FluxMap_Canvas");
	TCanvas * Compare_Canvas = new TCanvas("Momentum_Canvas");

	TLegend * legend1_mom = new TLegend(0.55,0.25,0.65,0.4);
	legend1_mom->SetFillColor(0);


	FluxMap_Canvas->Divide(2,2);
	FluxMap_Canvas->cd(1);
	gPad->SetLogz();
	gStyle->SetOptStat(11);
	gStyle->SetNumberContours(999);
	T1->Draw("Reflectiony:Reflectionx >>+ FluxMap_ElePosi","hasLeftDetector_Status==1 && (Particle_ID==11 || Particle_ID==-11)","colz");

	TLine * l1 = new TLine(-25,-25,25,-25);
	l1->SetLineWidth(2);
	TLine * l2 = new TLine(-25,-25,-25,25);
	l2->SetLineWidth(2);
	TLine * l3 = new TLine(25,-25,25,25);
	l3->SetLineWidth(2);
	TLine * l4 = new TLine(-25,25,25,25);
	l4->SetLineWidth(2);
	l1->Draw();
	l2->Draw();
	l3->Draw();
	l4->Draw();

	FluxMap_Canvas->cd(2);
	gPad->SetLogz();
	gStyle->SetOptStat(11);
	gStyle->SetNumberContours(999);
	T1->Draw("Reflectiony:Reflectionx >>+ FluxMap_Phot","hasLeftDetector_Status==1 && Particle_ID==22","colz");
	l1->Draw();
	l2->Draw();
	l3->Draw();
	l4->Draw();

	FluxMap_Canvas->cd(3);
	gROOT->ForceStyle();
	gPad->SetLogz();
	gStyle->SetOptStat(11);
	gStyle->SetNumberContours(999);
	T1->Draw("Reflectiony:Reflectionx >>+ FluxMap_ProtNeut","hasLeftDetector_Status==1 && (Particle_ID==2212 || Particle_ID==2112)","colz");
	l1->Draw();
	l2->Draw();
	l3->Draw();
	l4->Draw();

	string canvasname3=FluxMap_Canvas->GetName();
	FluxMap_Canvas->Write();
	FluxMap_Canvas->Print((canvasname3+".eps").c_str());	
	FluxMap_Canvas->Print((canvasname3+".C").c_str());	
	FluxMap_Canvas->Close();


	Compare_Canvas->cd();

	gPad->SetLogy();
	gStyle->SetOptStat(11);
	T1->Draw("Momentum >>+ Momentum_ElePosi","hasLeftDetector_Status==1 && (Particle_ID==11 || Particle_ID==-11)");
	MomentumHisto_ElePosi->SetMinimum(0.1);
	MomentumHisto_ElePosi->SetMaximum(10000500);
	gPad->Update();
        TPaveStats *st1_mom = (TPaveStats*)MomentumHisto_ElePosi->FindObject("stats");
        double statboxsize_mom = st1_mom->GetY2NDC() - st1_mom->GetY1NDC();
        st1_mom->SetTextColor(4);
        st1_mom->SetY1NDC(0.85);
        st1_mom->SetY2NDC(0.85+statboxsize_mom);	

	T1->Draw("Momentum >>+ Momentum_Phot","hasLeftDetector_Status==1 && Particle_ID==22","SAMES");
	MomentumHisto_Phot->SetMinimum(0.1);
	MomentumHisto_Phot->SetMaximum(10000500);
	gPad->Update();
        TPaveStats *st2_mom = (TPaveStats*)MomentumHisto_Phot->FindObject("stats");
        st2_mom->SetTextColor(3);
        st2_mom->SetY2NDC(st1_mom->GetY1NDC()); //new x start position
        st2_mom->SetY1NDC(st2_mom->GetY2NDC()-statboxsize_mom); //new x end position

	T1->Draw("Momentum >>+ Momentum_ProtNeut","hasLeftDetector_Status==1 && (Particle_ID==2212 || Particle_ID==2112)","SAMES");
	MomentumHisto_ProtNeut->SetMinimum(0.1);
	MomentumHisto_ProtNeut->SetMaximum(10000500);
	gPad->Update();
        TPaveStats *st3_mom = (TPaveStats*)MomentumHisto_ProtNeut->FindObject("stats");
        st3_mom->SetTextColor(2);
        st3_mom->SetY2NDC(st2_mom->GetY1NDC()); //new x start position
        st3_mom->SetY1NDC(st3_mom->GetY2NDC()-statboxsize_mom); //new x end position

	TText *label1_mom = new TText();
 	label1_mom-> SetNDC();
 	label1_mom -> SetTextFont(1);
 	label1_mom -> SetTextColor(1);
 	label1_mom -> SetTextSize(0.03);
 	label1_mom -> SetTextAlign(22);
 	label1_mom -> SetTextAngle(0);
 	label1_mom -> DrawText(0.5,0.85, "10^{10} initial electrons with 6.3 GeV hitting the carbon fibre");
	legend1_mom->AddEntry(MomentumHisto_ElePosi,"e^{-}, e^{+}","l");
	legend1_mom->AddEntry(MomentumHisto_Phot,"#gamma","l");
	legend1_mom->AddEntry(MomentumHisto_ProtNeut,"p, n","l");
	legend1_mom->Draw();


	string canvasname=Compare_Canvas->GetName();
	Compare_Canvas->Write();
	Compare_Canvas->Print((canvasname+".eps").c_str());	
	Compare_Canvas->Print((canvasname+".C").c_str());	
	Compare_Canvas->Close();


	output_rootfile->Write();
	input_rootfile1->Close();
	delete input_rootfile1, output_rootfile;
	delete FluxMap_Canvas, Compare_Canvas;
	delete legend1_mom;	
}
