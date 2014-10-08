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
void DrawHistos_Vertex_xyz(TCanvas* canvas,TH1F* histo_vertex_z, string cuttext, TTree* T);
void DrawHistos_Vertex_xyz(int dimension, TH2F* histo_vertex_xy_z, string cuttext, TTree* T);

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
	output_filename << "vertex_plots_" << name;
	TFile * output_rootfile = new TFile(output_filename.str().c_str(),"RECREATE");

//------For drawing position of particle vertices:
	TH2F * Vertexx_Charge = new TH2F("Vertexx_Charge","x- over z-position of vertices for particles with electric charge",500/*5500*/,0,24727.5,500/*300*/,-5,5);
	Vertexx_Charge->SetLineColor(1);
	Vertexx_Charge->GetXaxis()->SetTitle("z (mm)");
	Vertexx_Charge->GetXaxis()->CenterTitle();
	Vertexx_Charge->GetYaxis()->SetTitle("x (mm)");
	Vertexx_Charge->GetYaxis()->CenterTitle();
	TH2F * Vertexy_Charge = new TH2F("Vertexy_Charge","y- over z-position of vertices for particles with electric charge",500/*5500*/,0,24727.5,300,-5,5);
	Vertexy_Charge->SetLineColor(1);
	Vertexy_Charge->GetXaxis()->SetTitle("z (mm)");
	Vertexy_Charge->GetXaxis()->CenterTitle();
	Vertexy_Charge->GetYaxis()->SetTitle("y (mm)");
	Vertexy_Charge->GetYaxis()->CenterTitle();

	TH1F * Vertexz_Charge = new TH1F("Vertexz_Charge","z-position of vertices for particles with electric charge",500/*5500*/,0,24727.5);
	Vertexz_Charge->SetLineColor(1);
	Vertexz_Charge->GetXaxis()->SetTitle("z (mm)");
	Vertexz_Charge->GetXaxis()->CenterTitle();

	TCanvas * Vertex_Canvas = new TCanvas("Vertex_Canvas_thetacut");

	string cuttext;
	cuttext = "Theta<0.5 && Charge!=0 && hasLeftDetector_Status==1";

	Vertex_Canvas->Divide(1,3);

	Vertex_Canvas->cd(1);
	DrawHistos_Vertex_xyz(Vertex_Canvas,Vertexz_Charge, cuttext, T);
	
	Vertex_Canvas->cd(2);
	DrawHistos_Vertex_xyz(0,Vertexx_Charge, cuttext, T);

	Vertex_Canvas->cd(3);
	DrawHistos_Vertex_xyz(1,Vertexy_Charge, cuttext, T);

	string canvasname1=Vertex_Canvas->GetName();
	Vertex_Canvas->Write();
	Vertex_Canvas->Print((canvasname1+".eps").c_str());
	Vertex_Canvas->Print((canvasname1+".pdf").c_str());
	Vertex_Canvas->Print((canvasname1+".C").c_str());
	Vertex_Canvas->Close();

//-----------------------------------------------

	output_rootfile->Write();
	input_rootfile->Close();

	delete output_rootfile, input_rootfile;
	delete Vertex_Canvas;

}

void DrawHistos_Vertex_xyz(TCanvas* canvas,TH1F* histo_vertex_z, string cuttext, TTree* T){

	stringstream drawtext;
	drawtext << "Vertexz >>+ " << histo_vertex_z->GetName();

	gROOT->ForceStyle();
	gPad->SetLogy();
	gStyle->SetOptStat(11);
	T->Draw(drawtext.str().c_str(),cuttext.c_str());	
	gPad->Update();
       	TPaveStats *st = (TPaveStats*)histo_vertex_z->FindObject("stats");
       	double statboxsize = st->GetX2NDC() - st->GetX1NDC();
       	st->SetTextColor(histo_vertex_z->GetLineColor());
       	st->SetX1NDC(0.67);
       	st->SetX2NDC(0.67+0.7*statboxsize);	

	//to draw a shaded area above and below an histogram range, we create
	//a TGraph object.
	int lower_bin_pipe = histo_vertex_z->FindBin(0,0,0);
	int upper_bin_pipe = histo_vertex_z->FindBin(4005,0,0);
	int n_pipe = 2*(upper_bin_pipe - lower_bin_pipe);
	TGraph * gr_pipe = new TGraph(2*n_pipe);
	for (int i=0; i < n_pipe/2; i++) {
		float xlow = histo_vertex_z->GetBinLowEdge(lower_bin_pipe+i);
		float xup  = histo_vertex_z->GetBinLowEdge(lower_bin_pipe+i+1);
		float y    = histo_vertex_z->GetBinContent(histo_vertex_z->GetMaximumBin());		
		gr_pipe->SetPoint(2*i,  xlow,4*y);
		gr_pipe->SetPoint(2*i+1,xup, 4*y);
		gr_pipe->SetPoint(2*n_pipe-2*i-1,xlow,0.001);
		gr_pipe->SetPoint(2*n_pipe-2*i-2,xup, 0.001);

	}
		
	gr_pipe->SetFillColor(17);
	gr_pipe->SetFillStyle(3002);
	gr_pipe->Draw("LF2,same");	
/*
	TText *label_pipe = new TText();
        label_pipe-> SetNDC();
        label_pipe -> SetTextFont(1);
        label_pipe -> SetTextColor(17);
        label_pipe -> SetTextSize(0.03);
        label_pipe -> SetTextAlign(22);
        label_pipe -> SetTextAngle(90);
        label_pipe -> DrawText(0.17,0.7, "Beam pipe");
*/
	//---------------
	int lower_bin_lead_shielding = histo_vertex_z->FindBin(22570,0,0);
	int upper_bin_lead_shielding = histo_vertex_z->FindBin(22770,0,0);
	int n_lead_shielding = 2*(upper_bin_lead_shielding - lower_bin_lead_shielding);
	TGraph * gr_lead_shielding = new TGraph(2*n_lead_shielding);
	for (int i=0; i < n_lead_shielding/2; i++) {
		float xlow = histo_vertex_z->GetBinLowEdge(lower_bin_lead_shielding+i);
		float xup  = histo_vertex_z->GetBinLowEdge(lower_bin_lead_shielding+i+1);
		float y    = histo_vertex_z->GetBinContent(histo_vertex_z->GetMaximumBin());		
		gr_lead_shielding->SetPoint(2*i,  xlow,4*y);
		gr_lead_shielding->SetPoint(2*i+1,xup, 4*y);
		gr_lead_shielding->SetPoint(2*n_lead_shielding-2*i-1,xlow,0.001);
		gr_lead_shielding->SetPoint(2*n_lead_shielding-2*i-2,xup, 0.001);
	}
	gr_lead_shielding->SetFillColor(5);
	gr_lead_shielding->SetFillStyle(3002);
	gr_lead_shielding->Draw("LF2,same");	
/*
	TText *label_lead_shielding = new TText();
        label_lead_shielding-> SetNDC();
        label_lead_shielding -> SetTextFont(1);
        label_lead_shielding -> SetTextColor(41);
        label_lead_shielding -> SetTextSize(0.03);
        label_lead_shielding -> SetTextAlign(22);
        label_lead_shielding -> SetTextAngle(90);
        label_lead_shielding -> DrawText(0.848,0.37, "Lead shielding");
*/
	//---------------
	int lower_bin_concrete_shielding = histo_vertex_z->FindBin(22770,0,0);
	int upper_bin_concrete_shielding = histo_vertex_z->FindBin(22970,0,0);
	int n_concrete_shielding = 2*(upper_bin_concrete_shielding - lower_bin_concrete_shielding);
	TGraph * gr_concrete_shielding = new TGraph(2*n_concrete_shielding);
	for (int i=0; i < n_concrete_shielding/2; i++) {
		float xlow = histo_vertex_z->GetBinLowEdge(lower_bin_concrete_shielding+i);
		float xup  = histo_vertex_z->GetBinLowEdge(lower_bin_concrete_shielding+i+1);
		float y    = histo_vertex_z->GetBinContent(histo_vertex_z->GetMaximumBin());		
		gr_concrete_shielding->SetPoint(2*i,  xlow,4*y);
		gr_concrete_shielding->SetPoint(2*i+1,xup, 4*y);
		gr_concrete_shielding->SetPoint(2*n_concrete_shielding-2*i-1,xlow,0.001);
		gr_concrete_shielding->SetPoint(2*n_concrete_shielding-2*i-2,xup, 0.001);
	}
	gr_concrete_shielding->SetFillColor(46);
	gr_concrete_shielding->SetFillStyle(3002);
	gr_concrete_shielding->Draw("LF2,same");	
/*
	TText *label_concrete_shielding = new TText();
        label_concrete_shielding-> SetNDC();
        label_concrete_shielding -> SetTextFont(1);
        label_concrete_shielding -> SetTextColor(46);
        label_concrete_shielding -> SetTextSize(0.03);
        label_concrete_shielding -> SetTextAlign(22);
        label_concrete_shielding -> SetTextAngle(90);
        label_concrete_shielding -> DrawText(0.853,0.7, "Heavy concrete shielding");
*/
	//---------------
	int lower_bin_converter = histo_vertex_z->FindBin(23055,0,0);
	int upper_bin_converter = histo_vertex_z->FindBin(23085,0,0);
	int n_converter = 2*(upper_bin_converter - lower_bin_converter);
	TGraph * gr_converter = new TGraph(2*n_converter);
	for (int i=0; i < n_converter/2; i++) {
		float xlow = histo_vertex_z->GetBinLowEdge(lower_bin_converter+i);
		float xup  = histo_vertex_z->GetBinLowEdge(lower_bin_converter+i+1);
		float y    = histo_vertex_z->GetBinContent(histo_vertex_z->GetMaximumBin());		
		gr_converter->SetPoint(2*i,  xlow,4*y);
		gr_converter->SetPoint(2*i+1,xup, 4*y);
		gr_converter->SetPoint(2*n_converter-2*i-1,xlow,0.001);
		gr_converter->SetPoint(2*n_converter-2*i-2,xup, 0.001);
	}
	gr_converter->SetFillColor(34);
	gr_converter->SetFillStyle(3002);
	gr_converter->Draw("LF2,same");	
/*
	TText *label_converter = new TText();
        label_converter-> SetNDC();
        label_converter -> SetTextFont(1);
        label_converter -> SetTextColor(34);
        label_converter -> SetTextSize(0.03);
        label_converter -> SetTextAlign(22);
        label_converter -> SetTextAngle(90);
        label_converter -> DrawText(0.857,0.37, "Converter plate");
*/
	//----------------
	int lower_bin_pipe2 = histo_vertex_z->FindBin(23400.0,0,0);
	int upper_bin_pipe2 = histo_vertex_z->FindBin(23676.25,0,0);
	int n_pipe2 = 2*(upper_bin_pipe2 - lower_bin_pipe2);
	TGraph * gr_pipe2 = new TGraph(2*n_pipe2);
	for (int i=0; i < n_pipe2/2; i++) {
		float xlow = histo_vertex_z->GetBinLowEdge(lower_bin_pipe2+i);
		float xup  = histo_vertex_z->GetBinLowEdge(lower_bin_pipe2+i+1);
		float y    = histo_vertex_z->GetBinContent(histo_vertex_z->GetMaximumBin());		
		gr_pipe2->SetPoint(2*i,  xlow,4*y);
		gr_pipe2->SetPoint(2*i+1,xup, 4*y);
		gr_pipe2->SetPoint(2*n_pipe2-2*i-1,xlow,0.001);
		gr_pipe2->SetPoint(2*n_pipe2-2*i-2,xup, 0.001);
	}
	gr_pipe2->SetFillColor(17);
	gr_pipe2->SetFillStyle(3002);
	gr_pipe2->Draw("LF2,same");	
/*
	TText *label_pipe2 = new TText();
        label_pipe2-> SetNDC();
        label_pipe2 -> SetTextFont(1);
        label_pipe2 -> SetTextColor(17);
        label_pipe2 -> SetTextSize(0.03);
        label_pipe2 -> SetTextAlign(22);
        label_pipe2 -> SetTextAngle(90);
        label_pipe2 -> DrawText(0.861,0.7, "Beam pipe");
*/
	//----------------
	int lower_bin_magnet = histo_vertex_z->FindBin(23672.5,0,0);
	int upper_bin_magnet = histo_vertex_z->FindBin(24727.5,0,0);
	int n_magnet = 2*(upper_bin_magnet - lower_bin_magnet);
	TGraph * gr_magnet = new TGraph(2*n_magnet);
	for (int i=0; i < n_magnet/2; i++) {
		float xlow = histo_vertex_z->GetBinLowEdge(lower_bin_magnet+i);
		float xup  = histo_vertex_z->GetBinLowEdge(lower_bin_magnet+i+1);
		float y    = histo_vertex_z->GetBinContent(histo_vertex_z->GetMaximumBin());		
		gr_magnet->SetPoint(2*i,  xlow,4*y);
		gr_magnet->SetPoint(2*i+1,xup, 4*y);
		gr_magnet->SetPoint(2*n_magnet-2*i-1,xlow,0.001);
		gr_magnet->SetPoint(2*n_magnet-2*i-2,xup, 0.001);
	}
	gr_magnet->SetFillColor(kGreen+3);
	gr_magnet->SetFillStyle(3002);
	gr_magnet->Draw("LF2,same");	
/*
	TText *label_magnet = new TText();
        label_magnet-> SetNDC();
        label_magnet -> SetTextFont(1);
        label_magnet -> SetTextColor(kGreen+3);
        label_magnet -> SetTextSize(0.03);
        label_magnet -> SetTextAlign(22);
        label_magnet -> SetTextAngle(90);
        label_magnet -> DrawText(0.88,0.7, "Test beam magnet");
*/
	TPaveText * CutBox = new TPaveText(.42,.81,0.58,0.86,"brNDC");
	CutBox->SetFillColor(0);
	CutBox->SetTextColor(1);
	CutBox->AddText(cuttext.c_str());
	CutBox->Draw();
	
	canvas->Update();
}

void DrawHistos_Vertex_xyz(int dimension, TH2F* histo_vertex_xy_z, string cuttext, TTree* T){

	stringstream drawtext;
	if(dimension==0) drawtext << "Vertexx:Vertexz >>+ " << histo_vertex_xy_z->GetName();
	else if(dimension==1) drawtext << "Vertexy:Vertexz >>+ " << histo_vertex_xy_z->GetName();
	else {
		cerr << "You can only plot the x- or y-position over z!" << endl;
    		cerr << "Type e.g. dimension = 0 for the x-position" << endl;
    		terminate();
	}	

	gStyle->SetOptStat(11);
	T->Draw(drawtext.str().c_str(),cuttext.c_str());	
	gPad->Update();
       	TPaveStats *st = (TPaveStats*)histo_vertex_xy_z->FindObject("stats");
       	double statboxsize = st->GetX2NDC() - st->GetX1NDC();
       	st->SetTextColor(histo_vertex_xy_z->GetLineColor());
       	st->SetX1NDC(0.67);
       	st->SetX2NDC(0.67+0.7*statboxsize);	

	//to draw a shaded area above and below an histogram range, we create
	//a TGraph object.
	
	float ylow = histo_vertex_xy_z->GetYaxis()->GetBinLowEdge(1);
	float yup = histo_vertex_xy_z->GetYaxis()->GetBinLowEdge(histo_vertex_xy_z->GetNbinsY());

	int lower_bin_pipe = histo_vertex_xy_z->FindBin(0.1,0,0);
	int upper_bin_pipe = histo_vertex_xy_z->FindBin(4005,0,0);
	int n_pipe = 2*(upper_bin_pipe - lower_bin_pipe);
	TGraph * gr_pipe = new TGraph(2*n_pipe);
	for (int i=0; i < n_pipe/2; i++) {
		int binx,biny,binz;
		histo_vertex_xy_z->GetBinXYZ(lower_bin_pipe,binx,biny,binz);
	
		float xlow = histo_vertex_xy_z->GetXaxis()->GetBinLowEdge(binx+i);
		float xup  = histo_vertex_xy_z->GetXaxis()->GetBinLowEdge(binx+i+1);
		gr_pipe->SetPoint(2*i,  xlow,yup);
		gr_pipe->SetPoint(2*i+1,xup, yup);
		gr_pipe->SetPoint(2*n_pipe-2*i-1,xlow,ylow);
		gr_pipe->SetPoint(2*n_pipe-2*i-2,xup, ylow);
	}
	gr_pipe->SetFillColor(17);
	gr_pipe->SetFillStyle(3002);
	gr_pipe->Draw("LF2,same");	
/*	
	TText *label_pipe = new TText();
        label_pipe-> SetNDC();
        label_pipe -> SetTextFont(1);
        label_pipe -> SetTextColor(17);
        label_pipe -> SetTextSize(0.04);
        label_pipe -> SetTextAlign(22);
        label_pipe -> SetTextAngle(90);
        label_pipe -> DrawText(0.16,0.7, "Beam pipe");
*/
	//---------------
	int lower_bin_lead_shielding = histo_vertex_xy_z->FindBin(22570,0,0);
	int upper_bin_lead_shielding = histo_vertex_xy_z->FindBin(22770,0,0);
	int n_lead_shielding = 2*(upper_bin_lead_shielding - lower_bin_lead_shielding);
	TGraph * gr_lead_shielding = new TGraph(2*n_lead_shielding);
	for (int i=0; i < n_lead_shielding/2; i++) {
		int binx,biny,binz;
		histo_vertex_xy_z->GetBinXYZ(lower_bin_lead_shielding,binx,biny,binz);
	
		float xlow = histo_vertex_xy_z->GetXaxis()->GetBinLowEdge(binx+i);
		float xup  = histo_vertex_xy_z->GetXaxis()->GetBinLowEdge(binx+i+1);
		gr_lead_shielding->SetPoint(2*i,  xlow,yup);
		gr_lead_shielding->SetPoint(2*i+1,xup, yup);
		gr_lead_shielding->SetPoint(2*n_lead_shielding-2*i-1,xlow,ylow);
		gr_lead_shielding->SetPoint(2*n_lead_shielding-2*i-2,xup, ylow);
	}
	gr_lead_shielding->SetFillColor(5);
	gr_lead_shielding->SetFillStyle(3002);
	gr_lead_shielding->Draw("LF2,same");	
/*
	TText *label_lead_shielding = new TText();
        label_lead_shielding-> SetNDC();
        label_lead_shielding -> SetTextFont(1);
        label_lead_shielding -> SetTextColor(41);
        label_lead_shielding -> SetTextSize(0.04);
        label_lead_shielding -> SetTextAlign(22);
        label_lead_shielding -> SetTextAngle(90);
        label_lead_shielding -> DrawText(0.83,0.3, "Lead shielding");
*/
	//---------------
	int lower_bin_concrete_shielding = histo_vertex_xy_z->FindBin(22770,0,0);
	int upper_bin_concrete_shielding = histo_vertex_xy_z->FindBin(22970,0,0);
	int n_concrete_shielding = 2*(upper_bin_concrete_shielding - lower_bin_concrete_shielding);
	TGraph * gr_concrete_shielding = new TGraph(2*n_concrete_shielding);
	for (int i=0; i < n_concrete_shielding/2; i++) {
		int binx,biny,binz;
		histo_vertex_xy_z->GetBinXYZ(lower_bin_concrete_shielding,binx,biny,binz);
	
		float xlow = histo_vertex_xy_z->GetXaxis()->GetBinLowEdge(binx+i);
		float xup  = histo_vertex_xy_z->GetXaxis()->GetBinLowEdge(binx+i+1);
		gr_concrete_shielding->SetPoint(2*i,  xlow,yup);
		gr_concrete_shielding->SetPoint(2*i+1,xup, yup);
		gr_concrete_shielding->SetPoint(2*n_concrete_shielding-2*i-1,xlow,ylow);
		gr_concrete_shielding->SetPoint(2*n_concrete_shielding-2*i-2,xup, ylow);
	}
	gr_concrete_shielding->SetFillColor(46);
	gr_concrete_shielding->SetFillStyle(3002);
	gr_concrete_shielding->Draw("LF2,same");	
/*
	TText *label_concrete_shielding = new TText();
        label_concrete_shielding-> SetNDC();
        label_concrete_shielding -> SetTextFont(1);
        label_concrete_shielding -> SetTextColor(46);
        label_concrete_shielding -> SetTextSize(0.04);
        label_concrete_shielding -> SetTextAlign(22);
        label_concrete_shielding -> SetTextAngle(90);
        label_concrete_shielding -> DrawText(0.84,0.7, "Heavy concrete shielding");
*/
	//---------------

	int lower_bin_converter = histo_vertex_xy_z->FindBin(23055,0,0);
	int upper_bin_converter = histo_vertex_xy_z->FindBin(23085,0,0);
	int n_converter = 2*(upper_bin_converter - lower_bin_converter);
	TGraph * gr_converter = new TGraph(2*n_converter);
	for (int i=0; i < n_converter/2; i++) {
		int binx,biny,binz;
		histo_vertex_xy_z->GetBinXYZ(lower_bin_converter,binx,biny,binz);
	
		float xlow = histo_vertex_xy_z->GetXaxis()->GetBinLowEdge(binx+i);
		float xup  = histo_vertex_xy_z->GetXaxis()->GetBinLowEdge(binx+i+1);
		gr_converter->SetPoint(2*i,  xlow,yup);
		gr_converter->SetPoint(2*i+1,xup, yup);
		gr_converter->SetPoint(2*n_converter-2*i-1,xlow,ylow);
		gr_converter->SetPoint(2*n_converter-2*i-2,xup, ylow);
	}
	gr_converter->SetFillColor(34);
	gr_converter->SetFillStyle(3002);
	gr_converter->Draw("LF2,same");	
/*
	TText *label_converter = new TText();
        label_converter-> SetNDC();
        label_converter -> SetTextFont(1);
        label_converter -> SetTextColor(34);
        label_converter -> SetTextSize(0.04);
        label_converter -> SetTextAlign(22);
        label_converter -> SetTextAngle(90);
        label_converter -> DrawText(0.848,0.3, "Converter plate");
*/
	//-----------------
	int lower_bin_pipe2 = histo_vertex_xy_z->FindBin(23400,0,0);
	int upper_bin_pipe2 = histo_vertex_xy_z->FindBin(23676.25,0,0);
	int n_pipe2 = 2*(upper_bin_pipe2 - lower_bin_pipe2);
	TGraph * gr_pipe2 = new TGraph(2*n_pipe2);
	for (int i=0; i < n_pipe2/2; i++) {
		int binx,biny,binz;
		histo_vertex_xy_z->GetBinXYZ(lower_bin_pipe2,binx,biny,binz);
	
		float xlow = histo_vertex_xy_z->GetXaxis()->GetBinLowEdge(binx+i);
		float xup  = histo_vertex_xy_z->GetXaxis()->GetBinLowEdge(binx+i+1);
		gr_pipe2->SetPoint(2*i,  xlow,yup);
		gr_pipe2->SetPoint(2*i+1,xup, yup);
		gr_pipe2->SetPoint(2*n_pipe2-2*i-1,xlow,ylow);
		gr_pipe2->SetPoint(2*n_pipe2-2*i-2,xup, ylow);
	}
	gr_pipe2->SetFillColor(17);
	gr_pipe2->SetFillStyle(3002);
	gr_pipe2->Draw("LF2,same");	
/*
	TText *label_pipe2 = new TText();
        label_pipe2-> SetNDC();
        label_pipe2 -> SetTextFont(1);
        label_pipe2 -> SetTextColor(17);
        label_pipe2 -> SetTextSize(0.04);
        label_pipe2 -> SetTextAlign(22);
        label_pipe2 -> SetTextAngle(90);
        label_pipe2 -> DrawText(0.861,0.7, "Beam pipe");
*/
	//---------------------
	int lower_bin_magnet = histo_vertex_xy_z->FindBin(23672.5,0,0);
	int upper_bin_magnet = histo_vertex_xy_z->FindBin(24727.5,0,0);
	int n_magnet = 2*(upper_bin_magnet - lower_bin_magnet);
	TGraph * gr_magnet = new TGraph(2*n_magnet);
	for (int i=0; i < n_magnet/2; i++) {
		int binx,biny,binz;
		histo_vertex_xy_z->GetBinXYZ(lower_bin_magnet,binx,biny,binz);
	
		float xlow = histo_vertex_xy_z->GetXaxis()->GetBinLowEdge(binx+i);
		float xup  = histo_vertex_xy_z->GetXaxis()->GetBinLowEdge(binx+i+1);
		gr_magnet->SetPoint(2*i,  xlow,yup);
		gr_magnet->SetPoint(2*i+1,xup, yup);
		gr_magnet->SetPoint(2*n_magnet-2*i-1,xlow,ylow);
		gr_magnet->SetPoint(2*n_magnet-2*i-2,xup, ylow);
	}
	gr_magnet->SetFillColor(kGreen+3);
	gr_magnet->SetFillStyle(3002);
	gr_magnet->Draw("LF2,same");	
/*
	TText *label_magnet = new TText();
        label_magnet-> SetNDC();
        label_magnet -> SetTextFont(1);
        label_magnet -> SetTextColor(kGreen+3);
        label_magnet -> SetTextSize(0.04);
        label_magnet -> SetTextAlign(22);
        label_magnet -> SetTextAngle(90);
        label_magnet -> DrawText(0.884,0.7, "Test beam magnet");
*/
	TPaveText * CutBox = new TPaveText(.42,.81,0.58,0.86,"brNDC");
	CutBox->SetFillColor(0);
	CutBox->SetTextColor(1);
	CutBox->AddText(cuttext.c_str());
	CutBox->Draw();


}
