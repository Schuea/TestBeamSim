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

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

void DrawingMacro(string outputname,string name1, string name2); 

int main(int argc,char *argv[]){
	if(argc < 4 || argc > 4){
    		//explain how to use program	
    		cerr << "Type in name of output file AND all root filenames!" << endl;
    		cerr << "e.g. ./DrawHistograms outputfilename.root file1.root file2.root ..." << endl;
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
	TFile * input_rootfile2 = new TFile(name2.c_str(),"READ");
	TTree * T1;
	input_rootfile1->GetObject("Tree",T1);
	TTree * T2; 
	input_rootfile2->GetObject("Tree",T2);
	cout << "Accessed TTrees.." << endl;

	TFile * output_rootfile = new TFile(outputname.c_str(),"RECREATE");

//	TH1::SetDefaultSumw2();

	TH1F * EnergyHisto_5GeV_ElePhot_20cm = new TH1F("Energy_5_ElePhot_20cm","Energy distribution of particles behind lead block with 20cm/40cm thickness. Initial photons with 5.0GeV",100,0,2);
	EnergyHisto_5GeV_ElePhot_20cm->SetLineColor(4);
	EnergyHisto_5GeV_ElePhot_20cm->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_5GeV_ElePhot_20cm->GetXaxis()->CenterTitle();
	TH1F * EnergyHisto_5GeV_ProtNeut_20cm = new TH1F("Energy_5_ProtNeut_20cm","Energy distribution of particles behind lead block with 20cm/40cm thickness. Initial photons with 5.0GeV",100,0,2);
	EnergyHisto_5GeV_ProtNeut_20cm->SetLineColor(2);
	EnergyHisto_5GeV_ProtNeut_20cm->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_5GeV_ProtNeut_20cm->GetXaxis()->CenterTitle();
	TH1F * EnergyHisto_5GeV_ElePhot_40cm = new TH1F("Energy_5_ElePhot_40cm","Energy distribution of particles behind lead block with 20cm/40cm thickness. Initial photons with 5.0GeV",100,0,2);
	EnergyHisto_5GeV_ElePhot_40cm->SetLineColor(6);
	EnergyHisto_5GeV_ElePhot_40cm->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_5GeV_ElePhot_40cm->GetXaxis()->CenterTitle();
	TH1F * EnergyHisto_5GeV_ProtNeut_40cm = new TH1F("Energy_5_ProtNeut_40cm","Energy distribution of particles behind lead block with 20cm/40cm thickness. Initial photons with 5.0GeV",100,0,2);
	EnergyHisto_5GeV_ProtNeut_40cm->SetLineColor(3);
	EnergyHisto_5GeV_ProtNeut_40cm->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_5GeV_ProtNeut_40cm->GetXaxis()->CenterTitle();

	TH1F * MomentumHisto_5GeV_ElePhot_20cm = new TH1F("Momentum_5_ElePhot_20cm","Momentum distribution of particles behind lead block with 20cm/40cm thickness. Initial photons with 5.0GeV",100,0,2);
	MomentumHisto_5GeV_ElePhot_20cm->SetLineColor(4);
	MomentumHisto_5GeV_ElePhot_20cm->GetXaxis()->SetTitle("Momentum (GeV)");
	MomentumHisto_5GeV_ElePhot_20cm->GetXaxis()->CenterTitle();
	TH1F * MomentumHisto_5GeV_ProtNeut_20cm = new TH1F("Momentum_5_ProtNeut_20cm","Momentum distribution of particles behind lead block with 20cm/40cm thickness. Initial photons with 5.0GeV",100,0,2);
	MomentumHisto_5GeV_ProtNeut_20cm->SetLineColor(2);
	MomentumHisto_5GeV_ProtNeut_20cm->GetXaxis()->SetTitle("Momentum (GeV)");
	MomentumHisto_5GeV_ProtNeut_20cm->GetXaxis()->CenterTitle();
	TH1F * MomentumHisto_5GeV_ElePhot_40cm = new TH1F("Momentum_5_ElePhot_40cm","Momentum distribution of particles behind lead block with 20cm/40cm thickness. Initial photons with 5.0GeV",100,0,2);
	MomentumHisto_5GeV_ElePhot_40cm->SetLineColor(6);
	MomentumHisto_5GeV_ElePhot_40cm->GetXaxis()->SetTitle("Momentum (GeV)");
	MomentumHisto_5GeV_ElePhot_40cm->GetXaxis()->CenterTitle();
	TH1F * MomentumHisto_5GeV_ProtNeut_40cm = new TH1F("Momentum_5_ProtNeut_40cm","Momentum distribution of particles behind lead block with 20cm/40cm thickness. Initial photons with 5.0GeV",100,0,2);
	MomentumHisto_5GeV_ProtNeut_40cm->SetLineColor(3);
	MomentumHisto_5GeV_ProtNeut_40cm->GetXaxis()->SetTitle("Momentum (GeV)");
	MomentumHisto_5GeV_ProtNeut_40cm->GetXaxis()->CenterTitle();

	TH1F * MomentumHisto_5GeV_ProtNeut_20cm_to_500MeV = new TH1F("Momentum_5_ProtNeut_20cm_to_500MeV","Momentum distribution of particles behind lead block with 20cm/40cm thickness. Initial photons with 5.0GeV",100,0,0.6);
	MomentumHisto_5GeV_ProtNeut_20cm_to_500MeV->SetLineColor(2);
	MomentumHisto_5GeV_ProtNeut_20cm_to_500MeV->GetXaxis()->SetTitle("Momentum (GeV)");
	MomentumHisto_5GeV_ProtNeut_20cm_to_500MeV->GetXaxis()->CenterTitle();
	TH1F * MomentumHisto_5GeV_ProtNeut_40cm_to_500MeV = new TH1F("Momentum_5_ProtNeut_40cm_to_500MeV","Momentum distribution of particles behind lead block with 20cm/40cm thickness. Initial photons with 5.0GeV",100,0,0.6);
	MomentumHisto_5GeV_ProtNeut_40cm_to_500MeV->SetLineColor(3);
	MomentumHisto_5GeV_ProtNeut_40cm_to_500MeV->GetXaxis()->SetTitle("Momentum (GeV)");
	MomentumHisto_5GeV_ProtNeut_40cm_to_500MeV->GetXaxis()->CenterTitle();


	TH1F * Vertexz_5GeV_ProtNeut_40cm = new TH1F("Vertexz_5_ProtNeut_40cm","Origin of protons and neutrons in a lead block with 40cm thickness. Initial photons with 5.0 GeV",400,45,500);
	Vertexz_5GeV_ProtNeut_40cm->SetLineColor(4);
	Vertexz_5GeV_ProtNeut_40cm->GetXaxis()->SetTitle("z (mm)");;
	Vertexz_5GeV_ProtNeut_40cm->GetXaxis()->CenterTitle();;
	
	TCanvas * ProtNeut_Vertex_Canvas = new TCanvas("40cm_20cm_ProtNeut_Vertex_Canvas");
	TCanvas * Compare_Canvas = new TCanvas("40cm_20cm_Compare_Canvas");

	TLegend * legend1 = new TLegend(0.55,0.5,0.75,0.65);
	legend1->SetFillColor(0);

	TLegend * legend1_mom = new TLegend(0.55,0.5,0.75,0.65);
	legend1_mom->SetFillColor(0);

	ProtNeut_Vertex_Canvas->cd();
	gPad->SetLogy();
	T2->Draw("Vertexz >>+ Vertexz_5_ProtNeut_40cm","Particle_ID==2212 || Particle_ID==2112");	

	string canvasname2=ProtNeut_Vertex_Canvas->GetName();
	ProtNeut_Vertex_Canvas->Write();
	ProtNeut_Vertex_Canvas->Print((canvasname2+".eps").c_str());	
	ProtNeut_Vertex_Canvas->Print((canvasname2+".C").c_str());	
	ProtNeut_Vertex_Canvas->Close();

	Compare_Canvas->Divide(2,2);
	Compare_Canvas->cd(1);
	
	gPad->SetLogy();
	gStyle->SetOptStat(11);
	T1->Draw("Energy >>+ Energy_5_ElePhot_20cm","hasLeftDetector_Status==1 && (Particle_ID==22 || Particle_ID==11 || Particle_ID==-11)");
	gPad->Update();
        TPaveStats *st1 = (TPaveStats*)EnergyHisto_5GeV_ElePhot_20cm->FindObject("stats");
        double statboxsize = st1->GetY2NDC() - st1->GetY1NDC();
        st1->SetTextColor(4);
        st1->SetY1NDC(0.8);
        st1->SetY2NDC(0.8+statboxsize);	

	T1->Draw("Energy >>+ Energy_5_ProtNeut_20cm","hasLeftDetector_Status==1 && (Particle_ID==2212 || Particle_ID==2112)","SAMES");
	gPad->Update();
        TPaveStats *st2 = (TPaveStats*)EnergyHisto_5GeV_ProtNeut_20cm->FindObject("stats");
        st2->SetTextColor(2);
        st2->SetY2NDC(st1->GetY1NDC()); //new x start position
        st2->SetY1NDC(st2->GetY2NDC()-statboxsize); //new x end position

	T2->Draw("Energy >>+ Energy_5_ElePhot_40cm","hasLeftDetector_Status==1 && (Particle_ID==22 || Particle_ID==11 || Particle_ID==-11)","SAMES");
	gPad->Update();
        TPaveStats *st3 = (TPaveStats*)EnergyHisto_5GeV_ElePhot_40cm->FindObject("stats");
        st3->SetTextColor(6);
        st3->SetY2NDC(st2->GetY1NDC());
        st3->SetY1NDC(st3->GetY2NDC()-statboxsize);	

	T2->Draw("Energy >>+ Energy_5_ProtNeut_40cm","hasLeftDetector_Status==1 && (Particle_ID==2212 || Particle_ID==2112)","SAMES");
	gPad->Update();
        TPaveStats *st4 = (TPaveStats*)EnergyHisto_5GeV_ProtNeut_40cm->FindObject("stats");
        st4->SetTextColor(3);
        st4->SetY2NDC(st3->GetY1NDC()); //new x start position
        st4->SetY1NDC(st4->GetY2NDC()-statboxsize); //new x end position

	TText *label1 = new TText();
 	label1-> SetNDC();
 	label1 -> SetTextFont(1);
 	label1 -> SetTextColor(1);
 	label1 -> SetTextSize(0.03);
 	label1 -> SetTextAlign(22);
 	label1 -> SetTextAngle(0);
 	label1 -> DrawText(0.5,0.85, "100,000 initial photons with 5.0 GeV");
	legend1->AddEntry(EnergyHisto_5GeV_ElePhot_20cm,"20cm: #gamma, e^-, e^+","l");
	legend1->AddEntry(EnergyHisto_5GeV_ProtNeut_20cm,"20cm: p, n","l");
	legend1->AddEntry(EnergyHisto_5GeV_ElePhot_40cm,"40cm: #gamma, e^-, e^+","l");
	legend1->AddEntry(EnergyHisto_5GeV_ProtNeut_40cm,"40cm: p, n","l");
	legend1->Draw();

	Compare_Canvas->cd(2);
	gPad->SetLogy();
	gStyle->SetOptStat(11);
	T1->Draw("Momentum >>+ Momentum_5_ElePhot_20cm","hasLeftDetector_Status==1 && (Particle_ID==22 || Particle_ID==11 || Particle_ID==-11)");
	gPad->Update();
        TPaveStats *st1_mom = (TPaveStats*)MomentumHisto_5GeV_ElePhot_20cm->FindObject("stats");
        double statboxsize_mom = st1_mom->GetY2NDC() - st1_mom->GetY1NDC();
        st1_mom->SetTextColor(4);
        st1_mom->SetY1NDC(0.8);
        st1_mom->SetY2NDC(0.8+statboxsize_mom);	

	T1->Draw("Momentum >>+ Momentum_5_ProtNeut_20cm","hasLeftDetector_Status==1 && (Particle_ID==2212 || Particle_ID==2112)","SAMES");
	gPad->Update();
        TPaveStats *st2_mom = (TPaveStats*)MomentumHisto_5GeV_ProtNeut_20cm->FindObject("stats");
        st2_mom->SetTextColor(2);
        st2_mom->SetY2NDC(st1_mom->GetY1NDC()); //new x start position
        st2_mom->SetY1NDC(st2_mom->GetY2NDC()-statboxsize_mom); //new x end position

	T2->Draw("Momentum >>+ Momentum_5_ElePhot_40cm","hasLeftDetector_Status==1 && (Particle_ID==22 || Particle_ID==11 || Particle_ID==-11)","SAMES");
	gPad->Update();
        TPaveStats *st3_mom = (TPaveStats*)MomentumHisto_5GeV_ElePhot_40cm->FindObject("stats");
        st3_mom->SetTextColor(6);
        st3_mom->SetY2NDC(st2_mom->GetY1NDC());
        st3_mom->SetY1NDC(st3->GetY2NDC()-statboxsize_mom);	

	T2->Draw("Momentum >>+ Momentum_5_ProtNeut_40cm","hasLeftDetector_Status==1 && (Particle_ID==2212 || Particle_ID==2112)","SAMES");
	gPad->Update();
        TPaveStats *st4_mom = (TPaveStats*)MomentumHisto_5GeV_ProtNeut_40cm->FindObject("stats");
        st4_mom->SetTextColor(3);
        st4_mom->SetY2NDC(st3_mom->GetY1NDC()); //new x start position
        st4_mom->SetY1NDC(st4_mom->GetY2NDC()-statboxsize_mom); //new x end position

	TText *label1_mom = new TText();
 	label1_mom-> SetNDC();
 	label1_mom -> SetTextFont(1);
 	label1_mom -> SetTextColor(1);
 	label1_mom -> SetTextSize(0.03);
 	label1_mom -> SetTextAlign(22);
 	label1_mom -> SetTextAngle(0);
 	label1_mom -> DrawText(0.5,0.85, "100,000 initial photons with 5.0 GeV");
	legend1_mom->AddEntry(MomentumHisto_5GeV_ElePhot_20cm,"20cm: #gamma, e^-, e^+","l");
	legend1_mom->AddEntry(MomentumHisto_5GeV_ProtNeut_20cm,"20cm: p, n","l");
	legend1_mom->AddEntry(MomentumHisto_5GeV_ElePhot_40cm,"40cm: #gamma, e^-, e^+","l");
	legend1_mom->AddEntry(MomentumHisto_5GeV_ProtNeut_40cm,"40cm: p, n","l");
	legend1_mom->Draw();

	Compare_Canvas->cd(3);
	gPad->SetLogy();
	T1->Draw("Momentum >>+ Momentum_5_ProtNeut_20cm_to_500MeV","hasLeftDetector_Status==1 &&(Particle_ID==2212 || Particle_ID==2112)");	
	gPad->Update();
        TPaveStats *st1_mom2 = (TPaveStats*)MomentumHisto_5GeV_ProtNeut_20cm_to_500MeV->FindObject("stats");
        double statboxsize_mom2 = st1_mom2->GetY2NDC() - st1_mom2->GetY1NDC();
	st1_mom2->SetTextColor(2);
	st1_mom2->SetY1NDC(0.8);
	st1_mom2->SetY2NDC(0.8+statboxsize_mom2);	

	T2->Draw("Momentum >>+ Momentum_5_ProtNeut_40cm_to_500MeV","hasLeftDetector_Status==1 &&(Particle_ID==2212 || Particle_ID==2112)","SAMES");	
	gPad->Update();
	TPaveStats *st2_mom2 = (TPaveStats*)MomentumHisto_5GeV_ProtNeut_40cm_to_500MeV->FindObject("stats");
        st2_mom2->SetTextColor(3);
        st2_mom2->SetY2NDC(st1_mom2->GetY1NDC()); //new x start position
        st2_mom2->SetY1NDC(st2_mom2->GetY2NDC()-statboxsize_mom2); //new x end position

	string canvasname=Compare_Canvas->GetName();
	Compare_Canvas->Write();
	Compare_Canvas->Print((canvasname+".eps").c_str());	
	Compare_Canvas->Print((canvasname+".C").c_str());	
	Compare_Canvas->Close();


	output_rootfile->Write();
	input_rootfile1->Close();
	input_rootfile2->Close();
}
