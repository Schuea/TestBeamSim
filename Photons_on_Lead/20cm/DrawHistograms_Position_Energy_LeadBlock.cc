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

void DrawingMacro(string outputname,string name1, string name2,string name3); 

int main(int argc,char *argv[]){
	if(argc < 5 || argc > 5){
    		//explain how to use program	
    		cerr << "Type in name of output file AND all root filenames!" << endl;
    		cerr << "e.g. ./DrawHistograms outputfilename.root file1.root file2.root ..." << endl;
    		terminate();
	}	
	string filename1, filename2,filename3, outputfilename;
	outputfilename = argv[1];
	filename1 = argv[2];
	filename2 = argv[3];
	filename3 = argv[4];
       
	DrawingMacro(outputfilename,filename1,filename2,filename3);
}

void DrawingMacro(string outputname,string name1, string name2,string name3){

	TFile * input_rootfile1 = new TFile(name1.c_str(),"READ");
	TFile * input_rootfile2 = new TFile(name2.c_str(),"READ");
	TFile * input_rootfile3 = new TFile(name3.c_str(),"READ");
	TTree * T1;
	input_rootfile1->GetObject("Tree",T1);
	TTree * T2; 
	input_rootfile2->GetObject("Tree",T2);
	TTree * T3;
	input_rootfile3->GetObject("Tree",T3);
	cout << "Accessed TTrees.." << endl;

	TFile * output_rootfile = new TFile(outputname.c_str(),"RECREATE");

//	TH1::SetDefaultSumw2();

	TH2F * Position_05GeV_Photons = new TH2F("Posi_05_Photons","Position of 0.5GeV photons behind lead block with 20cm thickness",200,-20,20,200,-20,20);
	Position_05GeV_Photons->GetXaxis()->SetTitle("x (mm)");
	Position_05GeV_Photons->GetXaxis()->CenterTitle();
	Position_05GeV_Photons->GetYaxis()->SetTitle("y (mm)");
	Position_05GeV_Photons->GetYaxis()->CenterTitle();

	TH2F * Position_5GeV_Photons = new TH2F("Posi_5_Photons","Position of 5GeV photons behind lead block with 20cm thickness",200,-20,20,200,-20,20);
	Position_5GeV_Photons->GetXaxis()->SetTitle("x (mm)");
	Position_5GeV_Photons->GetXaxis()->CenterTitle();
	Position_5GeV_Photons->GetYaxis()->SetTitle("y (mm)");
	Position_5GeV_Photons->GetYaxis()->CenterTitle();

	TH2F * Position_6_3GeV_Photons = new TH2F("Posi_6_3_Photons","Position of 6.3GeV photons behind lead block with 20cm thickness",200,-20,20,200,-20,20);
	Position_6_3GeV_Photons->GetXaxis()->SetTitle("x (mm)");
	Position_6_3GeV_Photons->GetXaxis()->CenterTitle();
	Position_6_3GeV_Photons->GetYaxis()->SetTitle("y (mm)");
	Position_6_3GeV_Photons->GetYaxis()->CenterTitle();


//	TH1::SetDefaultSumw2();

	TH1F * EnergyHisto_05GeV_ElePhot = new TH1F("Energy_05_ElePhot","Energy distribution of particles behind lead block with 20cm thickness. Initial photons with 0.5GeV",100,0,2);
	EnergyHisto_05GeV_ElePhot->SetLineColor(4);
	EnergyHisto_05GeV_ElePhot->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_05GeV_ElePhot->GetXaxis()->CenterTitle();
	TH1F * EnergyHisto_05GeV_ProtNeut = new TH1F("Energy_05_ProtNeut","Energy distribution of particles behind lead block with 20cm thickness. Initial photons with 0.5GeV",100,0,2);
	EnergyHisto_05GeV_ProtNeut->SetLineColor(2);
	EnergyHisto_05GeV_ProtNeut->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_05GeV_ProtNeut->GetXaxis()->CenterTitle();

	TH1F * EnergyHisto_5GeV_ElePhot = new TH1F("Energy_5_ElePhot","Energy distribution of particles behind lead block with 20cm thickness. Initial photons with 5.0GeV",100,0,2);
	EnergyHisto_5GeV_ElePhot->SetLineColor(4);
	EnergyHisto_5GeV_ElePhot->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_5GeV_ElePhot->GetXaxis()->CenterTitle();
	TH1F * EnergyHisto_5GeV_ProtNeut = new TH1F("Energy_5_ProtNeut","Energy distribution of particles behind lead block with 20cm thickness. Initial photons with 5.0GeV",100,0,2);
	EnergyHisto_5GeV_ProtNeut->SetLineColor(2);
	EnergyHisto_5GeV_ProtNeut->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_5GeV_ProtNeut->GetXaxis()->CenterTitle();

	TH1F * EnergyHisto_6_3GeV_ElePhot = new TH1F("Energy_6_3_ElePhot","Energy distribution of particles behind lead block with 20cm thickness. Initial photons with 6.3GeV",100,0,2);
	EnergyHisto_6_3GeV_ElePhot->SetLineColor(4);
	EnergyHisto_6_3GeV_ElePhot->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_6_3GeV_ElePhot->GetXaxis()->CenterTitle();
	TH1F * EnergyHisto_6_3GeV_ProtNeut = new TH1F("Energy_6_3_ProtNeut","Energy distribution of particles behind lead block with 20cm thickness. Initial photons with 6.3GeV",100,0,2);
	EnergyHisto_6_3GeV_ProtNeut->SetLineColor(2);
	EnergyHisto_6_3GeV_ProtNeut->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_6_3GeV_ProtNeut->GetXaxis()->CenterTitle();

	TH1F * MomentumHisto_05GeV_ElePhot = new TH1F("Momentum_05_ElePhot","Momentum distribution of particles behind lead block with 20cm thickness. Initial photons with 0.5GeV",100,0,2);
	MomentumHisto_05GeV_ElePhot->SetLineColor(4);
	MomentumHisto_05GeV_ElePhot->GetXaxis()->SetTitle("Momentum (GeV)");
	MomentumHisto_05GeV_ElePhot->GetXaxis()->CenterTitle();
	TH1F * MomentumHisto_05GeV_ProtNeut = new TH1F("Momentum_05_ProtNeut","Momentum distribution of particles behind lead block with 20cm thickness. Initial photons with 0.5GeV",100,0,2);
	MomentumHisto_05GeV_ProtNeut->SetLineColor(2);
	MomentumHisto_05GeV_ProtNeut->GetXaxis()->SetTitle("Momentum (GeV)");
	MomentumHisto_05GeV_ProtNeut->GetXaxis()->CenterTitle();

	TH1F * MomentumHisto_5GeV_ElePhot = new TH1F("Momentum_5_ElePhot","Momentum distribution of particles behind lead block with 20cm thickness. Initial photons with 5.0GeV",100,0,2);
	MomentumHisto_5GeV_ElePhot->SetLineColor(4);
	MomentumHisto_5GeV_ElePhot->GetXaxis()->SetTitle("Momentum (GeV)");
	MomentumHisto_5GeV_ElePhot->GetXaxis()->CenterTitle();
	TH1F * MomentumHisto_5GeV_ProtNeut = new TH1F("Momentum_5_ProtNeut","Momentum distribution of particles behind lead block with 20cm thickness. Initial photons with 5.0GeV",100,0,2);
	MomentumHisto_5GeV_ProtNeut->SetLineColor(2);
	MomentumHisto_5GeV_ProtNeut->GetXaxis()->SetTitle("Momentum (GeV)");
	MomentumHisto_5GeV_ProtNeut->GetXaxis()->CenterTitle();

	TH1F * MomentumHisto_6_3GeV_ElePhot = new TH1F("Momentum_6_3_ElePhot","Momentum distribution of particles behind lead block with 20cm thickness. Initial photons with 6.3GeV",100,0,2);
	MomentumHisto_6_3GeV_ElePhot->SetLineColor(4);
	MomentumHisto_6_3GeV_ElePhot->GetXaxis()->SetTitle("Momentum (GeV)");
	MomentumHisto_6_3GeV_ElePhot->GetXaxis()->CenterTitle();
	TH1F * MomentumHisto_6_3GeV_ProtNeut = new TH1F("Momentum_6_3_ProtNeut","Momentum distribution of particles behind lead block with 20cm thickness. Initial photons with 6.3GeV",100,0,2);
	MomentumHisto_6_3GeV_ProtNeut->SetLineColor(2);
	MomentumHisto_6_3GeV_ProtNeut->GetXaxis()->SetTitle("Momentum (GeV)");
	MomentumHisto_6_3GeV_ProtNeut->GetXaxis()->CenterTitle();

	
	TCanvas * Energy_Canvas = new TCanvas("20cm_Energy_Canvas");
	TCanvas * Position_Canvas = new TCanvas("20cm_Position_Canvas");
	TCanvas * Momentum_Canvas = new TCanvas("20cm_Momentum_Canvas");

	TLegend * legend1 = new TLegend(0.6,0.5,0.8,0.65);
	legend1->SetFillColor(0);
	TLegend * legend2 = new TLegend(0.6,0.5,0.8,0.65);
	legend2->SetFillColor(0);
	TLegend * legend3 = new TLegend(0.6,0.5,0.8,0.65);
	legend3->SetFillColor(0);

	TLegend * legend1_mom = new TLegend(0.6,0.5,0.8,0.65);
	legend1_mom->SetFillColor(0);
	TLegend * legend2_mom = new TLegend(0.6,0.5,0.8,0.65);
	legend2_mom->SetFillColor(0);
	TLegend * legend3_mom = new TLegend(0.6,0.5,0.8,0.65);
	legend3_mom->SetFillColor(0);


	Position_Canvas->Divide(2,2);
	Position_Canvas->cd(1);
	gPad->SetLogz();
	gStyle->SetOptStat(11);
	gStyle->SetNumberContours(999);
	T1->Draw("Reflectiony:Reflectionx >>+ Posi_05_Photons","hasLeftDetector_Status==1","colz");

	Position_Canvas->cd(2);
	gPad->SetLogz();
	gStyle->SetOptStat(11);
	gStyle->SetNumberContours(999);
	T2->Draw("Reflectiony:Reflectionx >>+ Posi_5_Photons","hasLeftDetector_Status==1","colz");

	Position_Canvas->cd(3);
	gPad->SetLogz();
	gStyle->SetOptStat(11);
	gStyle->SetNumberContours(999);
	T3->Draw("Reflectiony:Reflectionx >>+ Posi_6_3_Photons","hasLeftDetector_Status==1","colz");

	string canvasname1=Position_Canvas->GetName();
	Position_Canvas->Write();
	Position_Canvas->Print((canvasname1+".eps").c_str());	
	Position_Canvas->Print((canvasname1+".C").c_str());	
	Position_Canvas->Close();

	Energy_Canvas->Divide(2,2);
	Energy_Canvas->cd(1);
	gPad->SetLogy();
	gStyle->SetOptStat(11);
	T1->Draw("Energy >>+ Energy_05_ElePhot","hasLeftDetector_Status==1 && (Particle_ID==22 || Particle_ID==11 || Particle_ID==-11)");
	gPad->Update();
        TPaveStats *st1 = (TPaveStats*)EnergyHisto_05GeV_ElePhot->FindObject("stats");
        double statboxsize = st1->GetY2NDC() - st1->GetY1NDC();
        st1->SetTextColor(4);
        st1->SetY1NDC(0.8);
        st1->SetY2NDC(0.8+statboxsize);	

	T1->Draw("Energy >>+ Energy_05_ProtNeut","hasLeftDetector_Status==1 && (Particle_ID==2212 || Particle_ID==2112)","SAMES");
	gPad->Update();
        TPaveStats *st2 = (TPaveStats*)EnergyHisto_05GeV_ProtNeut->FindObject("stats");
        st2->SetTextColor(2);
        st2->SetY2NDC(st1->GetY1NDC()); //new x start position
        st2->SetY1NDC(st2->GetY2NDC()-statboxsize); //new x end position

	TText *label1 = new TText();
 	label1-> SetNDC();
 	label1 -> SetTextFont(1);
 	label1 -> SetTextColor(1);
 	label1 -> SetTextSize(0.03);
 	label1 -> SetTextAlign(22);
 	label1 -> SetTextAngle(0);
 	label1 -> DrawText(0.5,0.85, "100,000 initial photons with 0.5GeV");
	legend1->AddEntry(EnergyHisto_05GeV_ElePhot,"#gamma, e^-, e^+","l");
	legend1->AddEntry(EnergyHisto_05GeV_ProtNeut,"p, n","l");
	legend1->Draw();

	Energy_Canvas->cd(2);
	gPad->SetLogy();
	gStyle->SetOptStat(11);
	T2->Draw("Energy >>+ Energy_5_ElePhot","hasLeftDetector_Status==1 && (Particle_ID==22 || Particle_ID==11 || Particle_ID==-11)");
	gPad->Update();
        TPaveStats *st3 = (TPaveStats*)EnergyHisto_5GeV_ElePhot->FindObject("stats");
        double statboxsize2 = st3->GetY2NDC() - st3->GetY1NDC();
        st3->SetTextColor(4);
        st3->SetY1NDC(0.8);
        st3->SetY2NDC(0.8+statboxsize2);	

	T2->Draw("Energy >>+ Energy_5_ProtNeut","hasLeftDetector_Status==1 && (Particle_ID==2212 || Particle_ID==2112)","SAMES");
	gPad->Update();
        TPaveStats *st4 = (TPaveStats*)EnergyHisto_5GeV_ProtNeut->FindObject("stats");
        st4->SetTextColor(2);
        st4->SetY2NDC(st3->GetY1NDC()); //new x start position
        st4->SetY1NDC(st4->GetY2NDC()-statboxsize2); //new x end position

	TText *label2 = new TText();
 	label2-> SetNDC();
 	label2 -> SetTextFont(1);
 	label2 -> SetTextColor(1);
 	label2 -> SetTextSize(0.03);
 	label2 -> SetTextAlign(22);
 	label2 -> SetTextAngle(0);
 	label2 -> DrawText(0.5,0.85, "100,000 initial photons with 5GeV");
	legend2->AddEntry(EnergyHisto_5GeV_ElePhot,"#gamma, e^-, e^+","l");
	legend2->AddEntry(EnergyHisto_5GeV_ProtNeut,"p, n","l");
	legend2->Draw();

	Energy_Canvas->cd(3);
	gPad->SetLogy();
	gStyle->SetOptStat(11);
	T3->Draw("Energy >>+ Energy_6_3_ElePhot","hasLeftDetector_Status==1 && (Particle_ID==22 || Particle_ID==11 || Particle_ID==-11)");
	gPad->Update();
        TPaveStats *st5 = (TPaveStats*)EnergyHisto_6_3GeV_ElePhot->FindObject("stats");
        double statboxsize3 = st5->GetY2NDC() - st5->GetY1NDC();
        st5->SetTextColor(4);
        st5->SetY1NDC(0.8);
        st5->SetY2NDC(0.8+statboxsize3);	

	T3->Draw("Energy >>+ Energy_6_3_ProtNeut","hasLeftDetector_Status==1 && (Particle_ID==2212 || Particle_ID==2112)","SAMES");
	gPad->Update();
        TPaveStats *st6 = (TPaveStats*)EnergyHisto_6_3GeV_ProtNeut->FindObject("stats");
        st6->SetTextColor(2);
        st6->SetY2NDC(st5->GetY1NDC()); //new x start position
        st6->SetY1NDC(st6->GetY2NDC()-statboxsize3); //new x end position

	TText *label3 = new TText();
 	label3-> SetNDC();
 	label3 -> SetTextFont(1);
 	label3 -> SetTextColor(1);
 	label3 -> SetTextSize(0.03);
 	label3 -> SetTextAlign(22);
 	label3 -> SetTextAngle(0);
 	label3 -> DrawText(0.5,0.85, "100,000 initial photons with 6.3GeV");
	legend3->AddEntry(EnergyHisto_6_3GeV_ElePhot,"#gamma, e^-, e^+","l");
	legend3->AddEntry(EnergyHisto_6_3GeV_ProtNeut,"p, n","l");
	legend3->Draw();

	string canvasname2=Energy_Canvas->GetName();
	Energy_Canvas->Write();
	Energy_Canvas->Print((canvasname2+".eps").c_str());	
	Energy_Canvas->Print((canvasname2+".C").c_str());	
	Energy_Canvas->Close();

	Momentum_Canvas->Divide(2,2);
	Momentum_Canvas->cd(1);
	gPad->SetLogy();
	gStyle->SetOptStat(11);
	T1->Draw("Momentum >>+ Momentum_05_ElePhot","hasLeftDetector_Status==1 && (Particle_ID==22 || Particle_ID==11 || Particle_ID==-11)");
	gPad->Update();
        TPaveStats *st1_mom = (TPaveStats*)MomentumHisto_05GeV_ElePhot->FindObject("stats");
        double statboxsize_mom = st1_mom->GetY2NDC() - st1_mom->GetY1NDC();
        st1_mom->SetTextColor(4);
        st1_mom->SetY1NDC(0.8);
        st1_mom->SetY2NDC(0.8+statboxsize);	

	T1->Draw("Momentum >>+ Momentum_05_ProtNeut","hasLeftDetector_Status==1 && (Particle_ID==2212 || Particle_ID==2112)","SAMES");
	gPad->Update();
        TPaveStats *st2_mom = (TPaveStats*)MomentumHisto_05GeV_ProtNeut->FindObject("stats");
        st2_mom->SetTextColor(2);
        st2_mom->SetY2NDC(st1_mom->GetY1NDC()); //new x start position
        st2_mom->SetY1NDC(st2_mom->GetY2NDC()-statboxsize_mom); //new x end position

	TText *label1_mom = new TText();
 	label1_mom-> SetNDC();
 	label1_mom -> SetTextFont(1);
 	label1_mom -> SetTextColor(1);
 	label1_mom -> SetTextSize(0.03);
 	label1_mom -> SetTextAlign(22);
 	label1_mom -> SetTextAngle(0);
 	label1_mom -> DrawText(0.5,0.85, "100,000 initial photons with 0.5GeV");
	legend1_mom->AddEntry(MomentumHisto_05GeV_ElePhot,"#gamma, e^-, e^+","l");
	legend1_mom->AddEntry(MomentumHisto_05GeV_ProtNeut,"p, n","l");
	legend1_mom->Draw();

	Momentum_Canvas->cd(2);
	gPad->SetLogy();
	gStyle->SetOptStat(11);
	T2->Draw("Momentum >>+ Momentum_5_ElePhot","hasLeftDetector_Status==1 && (Particle_ID==22 || Particle_ID==11 || Particle_ID==-11)");
	gPad->Update();
        TPaveStats *st3_mom = (TPaveStats*)MomentumHisto_5GeV_ElePhot->FindObject("stats");
        double statboxsize2_mom = st3_mom->GetY2NDC() - st3_mom->GetY1NDC();
        st3_mom->SetTextColor(4);
        st3_mom->SetY1NDC(0.8);
        st3_mom->SetY2NDC(0.8+statboxsize2);	

	T2->Draw("Momentum >>+ Momentum_5_ProtNeut","hasLeftDetector_Status==1 && (Particle_ID==2212 || Particle_ID==2112)","SAMES");
	gPad->Update();
        TPaveStats *st4_mom = (TPaveStats*)MomentumHisto_5GeV_ProtNeut->FindObject("stats");
        st4_mom->SetTextColor(2);
        st4_mom->SetY2NDC(st3_mom->GetY1NDC()); //new x start position
        st4_mom->SetY1NDC(st4_mom->GetY2NDC()-statboxsize2_mom); //new x end position

	TText *label2_mom = new TText();
 	label2_mom-> SetNDC();
 	label2_mom -> SetTextFont(1);
 	label2_mom -> SetTextColor(1);
 	label2_mom -> SetTextSize(0.03);
 	label2_mom -> SetTextAlign(22);
 	label2_mom -> SetTextAngle(0);
 	label2_mom -> DrawText(0.5,0.85, "100,000 initial photons with 5GeV");
	legend2_mom->AddEntry(MomentumHisto_5GeV_ElePhot,"#gamma, e^-, e^+","l");
	legend2_mom->AddEntry(MomentumHisto_5GeV_ProtNeut,"p, n","l");
	legend2_mom->Draw();

	Momentum_Canvas->cd(3);
	gPad->SetLogy();
	gStyle->SetOptStat(11);
	T3->Draw("Momentum >>+ Momentum_6_3_ElePhot","hasLeftDetector_Status==1 && (Particle_ID==22 || Particle_ID==11 || Particle_ID==-11)");
	gPad->Update();
        TPaveStats *st5_mom = (TPaveStats*)MomentumHisto_6_3GeV_ElePhot->FindObject("stats");
        double statboxsize3_mom = st5_mom->GetY2NDC() - st5_mom->GetY1NDC();
        st5_mom->SetTextColor(4);
        st5_mom->SetY1NDC(0.8);
        st5_mom->SetY2NDC(0.8+statboxsize3);	

	T3->Draw("Momentum >>+ Momentum_6_3_ProtNeut","hasLeftDetector_Status==1 && (Particle_ID==2212 || Particle_ID==2112)","SAMES");
	gPad->Update();
        TPaveStats *st6_mom = (TPaveStats*)MomentumHisto_6_3GeV_ProtNeut->FindObject("stats");
        st6_mom->SetTextColor(2);
        st6_mom->SetY2NDC(st5_mom->GetY1NDC()); //new x start position
        st6_mom->SetY1NDC(st6_mom->GetY2NDC()-statboxsize3); //new x end position

	TText *label3_mom = new TText();
 	label3_mom-> SetNDC();
 	label3_mom -> SetTextFont(1);
 	label3_mom -> SetTextColor(1);
 	label3_mom -> SetTextSize(0.03);
 	label3_mom -> SetTextAlign(22);
 	label3_mom -> SetTextAngle(0);
 	label3_mom -> DrawText(0.5,0.85, "100,000 initial photons with 6.3GeV");
	legend3_mom->AddEntry(MomentumHisto_6_3GeV_ElePhot,"#gamma, e^-, e^+","l");
	legend3_mom->AddEntry(MomentumHisto_6_3GeV_ProtNeut,"p, n","l");
	legend3_mom->Draw();

	string canvasname3=Momentum_Canvas->GetName();
	Momentum_Canvas->Write();
	Momentum_Canvas->Print((canvasname3+".eps").c_str());	
	Momentum_Canvas->Print((canvasname3+".C").c_str());	
	Momentum_Canvas->Close();


	output_rootfile->Write();
	input_rootfile1->Close();
	input_rootfile2->Close();
	input_rootfile3->Close();
}
