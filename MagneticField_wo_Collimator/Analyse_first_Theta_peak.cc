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
	output_filename << "analysed_" << name;
	TFile * output_rootfile = new TFile(output_filename.str().c_str(),"RECREATE");

	TH1F * Particle_IDs = new TH1F("Particle_IDs","PDG particle IDs of particles with #theta<0.5 existing behind magnet",10000,-50,50);
	Particle_IDs->GetXaxis()->SetTitle("PDG particle ID");
	Particle_IDs->GetXaxis()->CenterTitle();

	TH1::SetDefaultSumw2();

	TH1F * MomentumHisto_Ele = new TH1F("P_Ele","Momentum distribution of particles existing behind magnet",80,0,6.5);
	MomentumHisto_Ele->SetLineColor(2);
	MomentumHisto_Ele->GetXaxis()->SetTitle("Momentum (GeV)");
	MomentumHisto_Ele->GetXaxis()->CenterTitle();
	TH1F * MomentumHisto_Muons = new TH1F("P_Muons","Momentum distribution of particles existing behind magnet",80,0,6.5);
	MomentumHisto_Muons->SetLineColor(4);
	MomentumHisto_Muons->GetXaxis()->SetTitle("Momentum (GeV)");
	MomentumHisto_Muons->GetXaxis()->CenterTitle();

	TH2F * Position_Charge = new TH2F("Posi_Charge","Position of particles with electric charge behind magnet",200,-570,570,200,-570,570);
	Position_Charge->GetXaxis()->SetTitle("x (mm)");
	Position_Charge->GetXaxis()->CenterTitle();
	Position_Charge->GetYaxis()->SetTitle("y (mm)");
	Position_Charge->GetYaxis()->CenterTitle();

//------For drawing position of particle vertices:
	TH2F * Vertexx_Charge = new TH2F("Vertexx_Charge","x- over z-position of vertices for particles with electric charge",5000,0,24499,300,-5,5);
	Vertexx_Charge->SetLineColor(1);
	Vertexx_Charge->GetXaxis()->SetTitle("z (mm)");
	Vertexx_Charge->GetXaxis()->CenterTitle();
	Vertexx_Charge->GetYaxis()->SetTitle("x (mm)");
	Vertexx_Charge->GetYaxis()->CenterTitle();
	TH2F * Vertexy_Charge = new TH2F("Vertexy_Charge","y- over z-position of vertices for particles with electric charge",5000,0,24499,300,-5,5);
	Vertexy_Charge->SetLineColor(1);
	Vertexy_Charge->GetXaxis()->SetTitle("z (mm)");
	Vertexy_Charge->GetXaxis()->CenterTitle();
	Vertexy_Charge->GetYaxis()->SetTitle("y (mm)");
	Vertexy_Charge->GetYaxis()->CenterTitle();

	TH1F * Vertexz_Charge = new TH1F("Vertexz_Charge","z-position of vertices for particles with electric charge",5000,0,24499);
	Vertexz_Charge->SetLineColor(1);
	Vertexz_Charge->GetXaxis()->SetTitle("z (mm)");
	Vertexz_Charge->GetXaxis()->CenterTitle();

	TCanvas * Vertex_Canvas = new TCanvas("Vertex_Canvas_theta_0_5");
	string cuttext;
	cuttext = "Theta<0.5 && Charge!=0 && hasLeftDetector_Status==1";

	Vertex_Canvas->Divide(1,3);

	Vertex_Canvas->cd(1);
	DrawHistos_Vertex_xyz(Vertex_Canvas,Vertexz_Charge, cuttext, T);
	
	Vertex_Canvas->cd(2);
	DrawHistos_Vertex_xyz(0,Vertexx_Charge, cuttext, T);

	Vertex_Canvas->cd(3);
	DrawHistos_Vertex_xyz(1,Vertexy_Charge, cuttext, T);

	string canvasname3=Vertex_Canvas->GetName();
	Vertex_Canvas->Write();
	Vertex_Canvas->Print((canvasname3+".eps").c_str());
	Vertex_Canvas->Print((canvasname3+".C").c_str());
	Vertex_Canvas->Close();

//-----------------------------------------------

	float energy = 1.0;
	float energyinterval = 1.0;
	vector< TH1F* > AngleHisto_ElePosi_;
	vector< TCanvas* > Angle_Canvas_;
	vector< TPaveText* > AngleCutBox_;

	for(int i = 1; i < 8; ++i){
                if (energy > 5.0 && energy < 7.0){
			energyinterval = 0.3;
		}

		stringstream histoname1;
		stringstream histotitle1;
		stringstream drawtext1;
		stringstream cuttext1;
		stringstream histotitle,pavetext;
		stringstream savetext1,savetext2;
                histoname1 << "AngleHisto_ElePosi_" << i;
		histotitle1 << "Angular distribution of e^{-} and e^{+}, for an energy between " << energy -0.1 << " and " << energy + 0.1;
		drawtext1 << "cos(TMath::DegToRad()*Theta) >>+ " << histoname1.str();
		cuttext1 << "(Particle_ID==11 || Particle_ID==-11) && Energy > "<< energy-0.1 <<" && Energy < " << energy+0.1 << " && hasLeftDetector_Status==1";
		pavetext << energy-0.1 << " GeV < Energy < " << energy+0.1 << " GeV";
		savetext1 << "cos_theta_Magnet_" << energy << "GeV.eps";
		savetext2 << "cos_theta_Magnet_" << energy << "GeV.C";

		TH1F* temphisto1 = new TH1F(histoname1.str().c_str(), histotitle1.str().c_str(),100,0.995,1);
                AngleHisto_ElePosi_.push_back(temphisto1);
//cout << __LINE__<<endl;              
	  	AngleHisto_ElePosi_.at(i-1)->SetLineColor(2);
		AngleHisto_ElePosi_.at(i-1)->GetXaxis()->SetTitle("cos(#theta)");
		AngleHisto_ElePosi_.at(i-1)->GetXaxis()->CenterTitle();
	
		TCanvas *tempcanvas = new TCanvas();
		Angle_Canvas_.push_back(tempcanvas);
		Angle_Canvas_.at(i-1)->cd();
		gStyle->SetOptStat(11);
		gPad->SetLogy();
	
		T->Draw(drawtext1.str().c_str(), cuttext1.str().c_str());
		gPad->Update();
        	TPaveStats *st1 = (TPaveStats*)AngleHisto_ElePosi_.at(i-1)->FindObject("stats");
        	double statboxsize = st1->GetY2NDC() - st1->GetY1NDC();
        	st1->SetTextColor(2);
        	st1->SetY1NDC(0.8);
        	st1->SetY2NDC(0.8+statboxsize);	

		TPaveText* tempbox = new TPaveText(.4,.81,.6,.88,"brNDC");
		AngleCutBox_.push_back(tempbox);
		AngleCutBox_.at(i-1)->AddText(pavetext.str().c_str());
		AngleCutBox_.at(i-1)->Draw();

		AngleCutBox_.at(i-1)->Draw();
	
		Angle_Canvas_.at(i-1)->Write();
		Angle_Canvas_.at(i-1)->Print(savetext1.str().c_str());
		Angle_Canvas_.at(i-1)->Print(savetext2.str().c_str());
		Angle_Canvas_.at(i-1)->Close();
	
		energy += energyinterval;
		delete temphisto1;
		delete tempcanvas;
		delete tempbox;
        }


	TCanvas * E_Canvas_EleMuons = new TCanvas("E_Canvas_EleMuons_theta_0_5");	
	TCanvas * Position_Canvas = new TCanvas("deflected_Position_Canvas_theta_0_5");

	TLegend * E_Legend = new TLegend(0.6,0.63,0.7,0.73);
	E_Legend->SetFillColor(0);

	E_Canvas_EleMuons->Divide(1,2);
	E_Canvas_EleMuons->cd(1);
	gPad->SetLogy();
	T->Draw("Particle_ID >>+ Particle_IDs","Charge!=0 && Theta<0.5 && hasLeftDetector_Status==1");


	E_Canvas_EleMuons->cd(2);
	gPad->SetLogy();
	T->Draw("Momentum >>+ P_Ele","(Particle_ID==11 || Particle_ID==-11) && Theta<0.5 && hasLeftDetector_Status==1");
	MomentumHisto_Ele->SetMinimum(0.1);
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
	
	string canvasname1 = E_Canvas_EleMuons->GetName();
	E_Canvas_EleMuons->Write();
	E_Canvas_EleMuons->Print((canvasname1+".eps").c_str());	
	E_Canvas_EleMuons->Print((canvasname1+".C").c_str());	
	E_Canvas_EleMuons->Close();


	Position_Canvas->cd();
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

	string canvasname2=Position_Canvas->GetName();
	Position_Canvas->Write();
	Position_Canvas->Print((canvasname2+".eps").c_str());
	Position_Canvas->Print((canvasname2+".C").c_str());
	Position_Canvas->Close();


	output_rootfile->Write();
	input_rootfile->Close();

	delete output_rootfile, input_rootfile;
	delete l1,l2,l3,l4,el1;
	delete E_Canvas_EleMuons;
	delete Position_Canvas;
	delete Vertex_Canvas;
	delete E_Legend;

}

void DrawHistos_Vertex_xyz(TCanvas* canvas,TH1F* histo_vertex_z, string cuttext, TTree* T){

	stringstream drawtext;
	drawtext << "Vertexz >>+ " << histo_vertex_z->GetName();

	gROOT->ForceStyle();

	gStyle->SetOptStat(11);
	T->Draw(drawtext.str().c_str(),cuttext.c_str());	
	gPad->Update();
       	TPaveStats *st = (TPaveStats*)histo_vertex_z->FindObject("stats");
       	double statboxsize = st->GetX2NDC() - st->GetX1NDC();
       	st->SetTextColor(histo_vertex_z->GetLineColor());
       	st->SetX1NDC(0.7);
       	st->SetX2NDC(0.7+0.7*statboxsize);	

	//to draw a shaded area above and below an histogram range, we create
	//a TGraph object.
	int lower_bin_pipe = histo_vertex_z->FindBin(0,0,0);
	int upper_bin_pipe = histo_vertex_z->FindBin(4000,0,0);
	int n_pipe = 2*(upper_bin_pipe - lower_bin_pipe);
	TGraph * gr_pipe = new TGraph(2*n_pipe);
	for (int i=0; i < n_pipe/2; i++) {
		float xlow = histo_vertex_z->GetBinLowEdge(lower_bin_pipe+i);
		float xup  = histo_vertex_z->GetBinLowEdge(lower_bin_pipe+i+1);
		float y    = histo_vertex_z->GetBinContent(histo_vertex_z->GetMaximumBin());		
		gr_pipe->SetPoint(2*i,  xlow,1.08*y);
		gr_pipe->SetPoint(2*i+1,xup, 1.08*y);
		gr_pipe->SetPoint(2*n_pipe-2*i-1,xlow,0);
		gr_pipe->SetPoint(2*n_pipe-2*i-2,xup, 0);

	}
		
	gr_pipe->SetFillColor(17);
	gr_pipe->SetFillStyle(3002);
	gr_pipe->Draw("LF2,same");	

	TText *label_pipe = new TText();
        label_pipe-> SetNDC();
        label_pipe -> SetTextFont(1);
        label_pipe -> SetTextColor(17);
        label_pipe -> SetTextSize(0.03);
        label_pipe -> SetTextAlign(22);
        label_pipe -> SetTextAngle(90);
        label_pipe -> DrawText(0.17,0.65, "Beam pipe");

	int lower_bin_converter = histo_vertex_z->FindBin(23240,0,0);
	int upper_bin_converter = histo_vertex_z->FindBin(23245,0,0);
	int n_converter = 2*(upper_bin_converter - lower_bin_converter);
	TGraph * gr_converter = new TGraph(2*n_converter);
	for (int i=0; i < n_converter/2; i++) {
		float xlow = histo_vertex_z->GetBinLowEdge(lower_bin_converter+i);
		float xup  = histo_vertex_z->GetBinLowEdge(lower_bin_converter+i+1);
		float y    = histo_vertex_z->GetBinContent(histo_vertex_z->GetMaximumBin());		
		gr_converter->SetPoint(2*i,  xlow,1.08*y);
		gr_converter->SetPoint(2*i+1,xup, 1.08*y);
		gr_converter->SetPoint(2*n_converter-2*i-1,xlow,0);
		gr_converter->SetPoint(2*n_converter-2*i-2,xup, 0);
	}
	gr_converter->SetFillColor(34);
	gr_converter->SetFillStyle(3002);
	gr_converter->Draw("LF2,same");	

	TText *label_converter = new TText();
        label_converter-> SetNDC();
        label_converter -> SetTextFont(1);
        label_converter -> SetTextColor(34);
        label_converter -> SetTextSize(0.03);
        label_converter -> SetTextAlign(22);
        label_converter -> SetTextAngle(90);
        label_converter -> DrawText(0.857,0.65, "Converter plate");

	int lower_bin_pipe2 = histo_vertex_z->FindBin(23265,0,0);
	int upper_bin_pipe2 = histo_vertex_z->FindBin(23435,0,0);
	int n_pipe2 = 2*(upper_bin_pipe2 - lower_bin_pipe2);
	TGraph * gr_pipe2 = new TGraph(2*n_pipe2);
	for (int i=0; i < n_pipe2/2; i++) {
		float xlow = histo_vertex_z->GetBinLowEdge(lower_bin_pipe2+i);
		float xup  = histo_vertex_z->GetBinLowEdge(lower_bin_pipe2+i+1);
		float y    = histo_vertex_z->GetBinContent(histo_vertex_z->GetMaximumBin());		
		gr_pipe2->SetPoint(2*i,  xlow,1.08*y);
		gr_pipe2->SetPoint(2*i+1,xup, 1.08*y);
		gr_pipe2->SetPoint(2*n_pipe2-2*i-1,xlow,0);
		gr_pipe2->SetPoint(2*n_pipe2-2*i-2,xup, 0);
	}
	gr_pipe2->SetFillColor(17);
	gr_pipe2->SetFillStyle(3002);
	gr_pipe2->Draw("LF2,same");	

	TText *label_pipe2 = new TText();
        label_pipe2-> SetNDC();
        label_pipe2 -> SetTextFont(1);
        label_pipe2 -> SetTextColor(17);
        label_pipe2 -> SetTextSize(0.03);
        label_pipe2 -> SetTextAlign(22);
        label_pipe2 -> SetTextAngle(90);
        label_pipe2 -> DrawText(0.861,0.79, "Beam pipe");

	int lower_bin_magnet = histo_vertex_z->FindBin(23435,0,0);
	int upper_bin_magnet = histo_vertex_z->FindBin(24499,0,0);
	int n_magnet = 2*(upper_bin_magnet - lower_bin_magnet);
	TGraph * gr_magnet = new TGraph(2*n_magnet);
	for (int i=0; i < n_magnet/2; i++) {
		float xlow = histo_vertex_z->GetBinLowEdge(lower_bin_magnet+i);
		float xup  = histo_vertex_z->GetBinLowEdge(lower_bin_magnet+i+1);
		float y    = histo_vertex_z->GetBinContent(histo_vertex_z->GetMaximumBin());		
		gr_magnet->SetPoint(2*i,  xlow,1.08*y);
		gr_magnet->SetPoint(2*i+1,xup, 1.08*y);
		gr_magnet->SetPoint(2*n_magnet-2*i-1,xlow,0);
		gr_magnet->SetPoint(2*n_magnet-2*i-2,xup, 0);
	}
	gr_magnet->SetFillColor(kGreen+3);
	gr_magnet->SetFillStyle(3002);
	gr_magnet->Draw("LF2,same");	

	TText *label_magnet = new TText();
        label_magnet-> SetNDC();
        label_magnet -> SetTextFont(1);
        label_magnet -> SetTextColor(kGreen+3);
        label_magnet -> SetTextSize(0.03);
        label_magnet -> SetTextAlign(22);
        label_magnet -> SetTextAngle(90);
        label_magnet -> DrawText(0.88,0.65, "Test beam magnet");

	TPaveText * CutBox = new TPaveText(.42,.81,0.58,0.86,"brNDC");
	CutBox->SetFillColor(0);
	CutBox->SetTextColor(1);
	CutBox->AddText("Cut on deflection angle: Theta < 0.5 degree");
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
       	st->SetX1NDC(0.7);
       	st->SetX2NDC(0.7+0.7*statboxsize);	

	//to draw a shaded area above and below an histogram range, we create
	//a TGraph object.
	
	float ylow = histo_vertex_xy_z->GetYaxis()->GetBinLowEdge(1);
	float yup = histo_vertex_xy_z->GetYaxis()->GetBinLowEdge(histo_vertex_xy_z->GetNbinsY());

	int lower_bin_pipe = histo_vertex_xy_z->FindBin(0.1,0,0);
	int upper_bin_pipe = histo_vertex_xy_z->FindBin(4000,0,0);
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
	
	TText *label_pipe = new TText();
        label_pipe-> SetNDC();
        label_pipe -> SetTextFont(1);
        label_pipe -> SetTextColor(17);
        label_pipe -> SetTextSize(0.03);
        label_pipe -> SetTextAlign(22);
        label_pipe -> SetTextAngle(90);
        label_pipe -> DrawText(0.17,0.65, "Beam pipe");

	int lower_bin_converter = histo_vertex_xy_z->FindBin(23240,0,0);
	int upper_bin_converter = histo_vertex_xy_z->FindBin(23245,0,0);
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

	TText *label_converter = new TText();
        label_converter-> SetNDC();
        label_converter -> SetTextFont(1);
        label_converter -> SetTextColor(34);
        label_converter -> SetTextSize(0.03);
        label_converter -> SetTextAlign(22);
        label_converter -> SetTextAngle(90);
        label_converter -> DrawText(0.857,0.65, "Converter plate");

	int lower_bin_pipe2 = histo_vertex_xy_z->FindBin(23265,0,0);
	int upper_bin_pipe2 = histo_vertex_xy_z->FindBin(23435,0,0);
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

	TText *label_pipe2 = new TText();
        label_pipe2-> SetNDC();
        label_pipe2 -> SetTextFont(1);
        label_pipe2 -> SetTextColor(17);
        label_pipe2 -> SetTextSize(0.03);
        label_pipe2 -> SetTextAlign(22);
        label_pipe2 -> SetTextAngle(90);
        label_pipe2 -> DrawText(0.861,0.79, "Beam pipe");

	int lower_bin_magnet = histo_vertex_xy_z->FindBin(23435,0,0);
	int upper_bin_magnet = histo_vertex_xy_z->FindBin(24499,0,0);
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

	TText *label_magnet = new TText();
        label_magnet-> SetNDC();
        label_magnet -> SetTextFont(1);
        label_magnet -> SetTextColor(kGreen+3);
        label_magnet -> SetTextSize(0.03);
        label_magnet -> SetTextAlign(22);
        label_magnet -> SetTextAngle(90);
        label_magnet -> DrawText(0.88,0.65, "Test beam magnet");

	TPaveText * CutBox = new TPaveText(.42,.81,0.58,0.86,"brNDC");
	CutBox->SetFillColor(0);
	CutBox->SetTextColor(1);
	CutBox->AddText("Cut on deflection angle: Theta < 0.5 degree");
	CutBox->Draw();


}
