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
#include "TGraph.h"
#include "TMath.h"
#include "TROOT.h"

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

	TFile * input_rootfile = new TFile(name.c_str(),"READ");
	cout << "Inputfile size = " << input_rootfile->GetSize() << endl;
	//TTree * T = (TTree*)input_rootfile->Get("Tree");
	TTree *T;
	input_rootfile->GetObject("Tree",T);
	cout << "Accessed TTree.." << endl;

	stringstream output_filename;
	output_filename << "analysed_first_peak_" << name;
	TFile * output_rootfile = new TFile(output_filename.str().c_str(),"RECREATE");

	TH1F * Particle_IDs = new TH1F("Particle_IDs","PDG particle IDs of particles with #theta<0.5 existing behind magnet",1000,-40,40);
	Particle_IDs->SetLineColor(4);
	Particle_IDs->GetXaxis()->SetTitle("PDG particle ID");
	Particle_IDs->GetXaxis()->CenterTitle();

	TH1::SetDefaultSumw2();

	TH1F * MomentumHisto_ElePosi = new TH1F("p_ElePosi","Momentum distribution of electrons with #theta<0.5 existing behind magnet",80,0,6.5);
	MomentumHisto_ElePosi->SetLineColor(4);
	MomentumHisto_ElePosi->GetXaxis()->SetTitle("Momentum (GeV)");
	MomentumHisto_ElePosi->GetXaxis()->CenterTitle();

	TH2F * Position_Charge = new TH2F("Posi_Charge","Position of particles with electric charge behind magnet",200,-570,570,200,-570,570);
	Position_Charge->GetXaxis()->SetTitle("x (mm)");
	Position_Charge->GetXaxis()->CenterTitle();
	Position_Charge->GetYaxis()->SetTitle("y (mm)");
	Position_Charge->GetYaxis()->CenterTitle();



	TCanvas * E_Canvas_ElePosi = new TCanvas("E_Canvas_ElePosi_theta_0_5");
	TCanvas * Position_Canvas = new TCanvas("deflected_Position_Canvas_theta_0_5");

	TLegend * E_Legend = new TLegend(0.6,0.63,0.7,0.73);
	E_Legend->SetFillColor(0);

	E_Canvas_ElePosi->Divide(1,2);
	E_Canvas_ElePosi->cd(1);
	gPad->SetLogy();
	T->Draw("Particle_ID >>+ Particle_IDs","Charge!=0 && Theta<0.5 && hasLeftDetector_Status==1");


	E_Canvas_ElePosi->cd(2);
	gPad->SetLogy();
	T->Draw("Momentum >>+ P_ElePosi","(Particle_ID==11 || Particle_ID==-11) && Theta<0.5 && hasLeftDetector_Status==1");
/*	MomentumHisto_Ele->SetMinimum(0.1);
	MomentumHisto_Ele->SetMaximum(100050);
	gPad->Update();
       	TPaveStats *st1_mom = (TPaveStats*)MomentumHisto_Ele->FindObject("stats");
       	double statboxsize_mom = st1_mom->GetY2NDC() - st1_mom->GetY1NDC();
       	st1_mom->SetTextColor(2);
       	st1_mom->SetY1NDC(0.8);
       	st1_mom->SetY2NDC(0.8+statboxsize_mom);	

	T->Draw("Energy >>+ P_Muons","(Particle_ID==13 || Particle_ID==-13) && Theta<0.5 && hasLeftDetector_Status==1","SAMES");
	MomentumHisto_Muons->SetMinimum(0.1);
	MomentumHisto_Muons->SetMaximum(100050);
	gPad->Update();
       	TPaveStats *st2_mom = (TPaveStats*)MomentumHisto_Muons->FindObject("stats");
       	st2_mom->SetTextColor(4);
      	st2_mom->SetY2NDC(st1_mom->GetY1NDC()); //new x start position
 	st2_mom->SetY1NDC(st2_mom->GetY2NDC()-statboxsize_mom); //new x end position

	E_Legend->AddEntry(MomentumHisto_Ele,"e^{-}, e^{+}");
	E_Legend->AddEntry(MomentumHisto_Muons,"#mu^{-}, #mu^{+}");
	E_Legend->Draw();
*/	
	string canvasname1 = E_Canvas_ElePosi->GetName();
	E_Canvas_ElePosi->Write();
	E_Canvas_ElePosi->Print((canvasname1+".eps").c_str());	
	E_Canvas_ElePosi->Print((canvasname1+".C").c_str());	
	E_Canvas_ElePosi->Close();


	Position_Canvas->Divide(1,2);
	Position_Canvas->cd(1);
	gPad->SetLogz();
	gStyle->SetOptStat(11);
	gStyle->SetNumberContours(999);
	T->Draw("Reflectiony:Reflectionx >>+ Posi_Charge","Theta<0.5 && Charge!=0 && hasLeftDetector_Status==1","colz");	
	gPad->Update();
       	TPaveStats *st1 = (TPaveStats*)Position_Charge->FindObject("stats");
       	double statboxsize_x = st1->GetX2NDC() - st1->GetX1NDC();
       	st1->SetTextColor(1);
       	st1->SetX1NDC(0.7);
       	st1->SetX2NDC(0.7+0.7*statboxsize_x);	

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

	TPaveText * CutBox = new TPaveText(.42,.81,0.58,0.86,"brNDC");
	CutBox->SetFillColor(0);
	CutBox->SetTextColor(1);
	CutBox->AddText("Cut on deflection angle: Theta < 0.5 degree");
	CutBox->Draw();

	Position_Canvas->cd(2);
	int globalbin,binx,biny,binz;
	globalbin = Position_Charge->FindBin(0,0,0);
	Position_Charge->GetBinXYZ(globalbin,binx,biny,binz);
	TH1D * ProjectionX_Charge = Position_Charge->ProjectionX("ProjectionX",biny-1,biny+1); 
	ProjectionX_Charge->Draw();

	string canvasname2=Position_Canvas->GetName();
	Position_Canvas->Write();
	Position_Canvas->Print((canvasname2+".eps").c_str());
	Position_Canvas->Print((canvasname2+".C").c_str());
	Position_Canvas->Close();


	output_rootfile->Write();
	input_rootfile->Close();

	delete output_rootfile, input_rootfile;
	delete l1,l2,l3,l4,el1;
	delete E_Canvas_ElePosi;
	delete Position_Canvas;
	delete E_Legend;

}

