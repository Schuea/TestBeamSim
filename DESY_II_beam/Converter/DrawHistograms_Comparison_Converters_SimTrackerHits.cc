#include "TFile.h"
#include "TBrowser.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1.h"
#include "THStack.h"
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

void DrawingMacro(string outputname,string name_1,string name_2,string name_3,string name_4,string name_5,string name_6,string name_7); 

int main(int argc,char *argv[]){
	if(argc < 9 || argc > 9){
    		//explain how to use program	
    		cerr << "Type in name of output file AND all root filenames!" << endl;
    		cerr << "e.g. ./DrawHistograms outputfilename.root file1.root file2.root ..." << endl;
    		terminate();
	}	
	string 	filename_Copper_1, filename_Copper_2, filename_Copper_3,
	 	filename_Alu_1, filename_Alu_2, filename_Alu_3,
		filename_Copper_wire,
		outputfilename;
	outputfilename 		= argv[1];
	filename_Copper_1 	= argv[2];
	filename_Copper_2 	= argv[3];
	filename_Copper_3 	= argv[4];
	filename_Alu_1 		= argv[5];
	filename_Alu_2 		= argv[6];
	filename_Alu_3 		= argv[7];
	filename_Copper_wire 	= argv[8];
       
	DrawingMacro(outputfilename,	filename_Copper_1,filename_Copper_2,filename_Copper_3,					
					filename_Alu_1,filename_Alu_2,filename_Alu_3,
					filename_Copper_wire);
}

void DrawingMacro(string outputname,string name_1,string name_2,string name_3,string name_4,string name_5,string name_6,string name_7){

	TFile * input_rootfile_Copper_1 	= new TFile(name_1.c_str(),"READ");
	TFile * input_rootfile_Copper_2 	= new TFile(name_2.c_str(),"READ");
	TFile * input_rootfile_Copper_3 	= new TFile(name_3.c_str(),"READ");
	TFile * input_rootfile_Alu_1 		= new TFile(name_4.c_str(),"READ");
	TFile * input_rootfile_Alu_2 		= new TFile(name_5.c_str(),"READ");
	TFile * input_rootfile_Alu_3 		= new TFile(name_6.c_str(),"READ");
	TFile * input_rootfile_Copper_wire 	= new TFile(name_7.c_str(),"READ");
	
	TTree * T_Copper_1_Plane1;
	TTree * T_Copper_1_Plane2;
	TTree * T_Copper_2_Plane1;
	TTree * T_Copper_2_Plane2;
	TTree * T_Copper_3_Plane1;
	TTree * T_Copper_3_Plane2;
	TTree * T_Alu_1_Plane1;
	TTree * T_Alu_1_Plane2;
	TTree * T_Alu_2_Plane1;
	TTree * T_Alu_2_Plane2;
	TTree * T_Alu_3_Plane1;
	TTree * T_Alu_3_Plane2;
	TTree * T_Copper_wire_Plane1;
	TTree * T_Copper_wire_Plane2;

	input_rootfile_Copper_1		->GetObject("Tree_TrackerPlane1",T_Copper_1_Plane1);
	input_rootfile_Copper_1		->GetObject("Tree_TrackerPlane2",T_Copper_1_Plane2);
	input_rootfile_Copper_2		->GetObject("Tree_TrackerPlane1",T_Copper_2_Plane1);
	input_rootfile_Copper_2		->GetObject("Tree_TrackerPlane2",T_Copper_2_Plane2);
	input_rootfile_Copper_3		->GetObject("Tree_TrackerPlane1",T_Copper_3_Plane1);
	input_rootfile_Copper_3		->GetObject("Tree_TrackerPlane2",T_Copper_3_Plane2);
	input_rootfile_Alu_1		->GetObject("Tree_TrackerPlane1",T_Alu_1_Plane1);
	input_rootfile_Alu_1		->GetObject("Tree_TrackerPlane2",T_Alu_1_Plane2);
	input_rootfile_Alu_2		->GetObject("Tree_TrackerPlane1",T_Alu_2_Plane1);
	input_rootfile_Alu_2		->GetObject("Tree_TrackerPlane2",T_Alu_2_Plane2);
	input_rootfile_Alu_3		->GetObject("Tree_TrackerPlane1",T_Alu_3_Plane1);
	input_rootfile_Alu_3		->GetObject("Tree_TrackerPlane2",T_Alu_3_Plane2);
	input_rootfile_Copper_wire	->GetObject("Tree_TrackerPlane1",T_Copper_wire_Plane1);
	input_rootfile_Copper_wire	->GetObject("Tree_TrackerPlane2",T_Copper_wire_Plane2);

	cout << "Accessed TTrees.." << endl;

	TFile * output_rootfile = new TFile(outputname.c_str(),"RECREATE");

//	TH1::SetDefaultSumw2();

	TH1F * EnergyHisto_Copper_5mm = new TH1F("Energy_Copper_5mm","Energy distribution of electrons/positrons behind the converter plate.",50,0,7);
	EnergyHisto_Copper_5mm->SetLineColor(kGreen+1);
	EnergyHisto_Copper_5mm->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_Copper_5mm->GetXaxis()->CenterTitle();

	TH1F * EnergyHisto_Copper_4mm = new TH1F("Energy_Copper_4mm","Energy distribution of electrons/positrons behind the converter plate.",50,0,7);
	EnergyHisto_Copper_4mm->SetLineColor(kOrange);
	EnergyHisto_Copper_4mm->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_Copper_4mm->GetXaxis()->CenterTitle();

	TH1F * EnergyHisto_Copper_3mm = new TH1F("Energy_Copper_3mm","Energy distribution of electrons/positrons behind the converter plate.",50,0,7);
	EnergyHisto_Copper_3mm->SetLineColor(kOrange+7);
	EnergyHisto_Copper_3mm->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_Copper_3mm->GetXaxis()->CenterTitle();

	TH1F * EnergyHisto_Alu_3mm = new TH1F("Energy_Alu_3mm","Energy distribution of electrons/positrons behind the converter plate.",50,0,7);
	EnergyHisto_Alu_3mm->SetLineColor(kRed+1);
	EnergyHisto_Alu_3mm->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_Alu_3mm->GetXaxis()->CenterTitle();

	TH1F * EnergyHisto_Alu_2mm = new TH1F("Energy_Alu_2mm","Energy distribution of electrons/positrons behind the converter plate.",50,0,7);
	EnergyHisto_Alu_2mm->SetLineColor(kMagenta+3);
	EnergyHisto_Alu_2mm->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_Alu_2mm->GetXaxis()->CenterTitle();

	TH1F * EnergyHisto_Alu_1mm = new TH1F("Energy_Alu_1mm","Energy distribution of electrons/positrons behind the converter plate.",50,0,7);
	EnergyHisto_Alu_1mm->SetLineColor(kBlue);
	EnergyHisto_Alu_1mm->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_Alu_1mm->GetXaxis()->CenterTitle();

	TH1F * EnergyHisto_Copper_wire = new TH1F("Energy_Copper_wire","Energy distribution of electrons/positrons behind the converter plate.",50,0,7);
	EnergyHisto_Copper_wire->SetLineColor(kCyan+2);
	EnergyHisto_Copper_wire->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_Copper_wire->GetXaxis()->CenterTitle();


	TLegend * legend1 = new TLegend(0.55,0.5,0.75,0.65);
	legend1->SetFillColor(0);

	TCanvas * Compare_Canvas = new TCanvas("Compare_Energy_Canvas");

	Compare_Canvas->cd();
	
	gPad->SetLogy();
	gStyle->SetOptStat(11);
	T_Copper_1_Plane2->Draw("HitEnergy >>+ Energy_Copper_5mm","(HitParticle_ID==11 || HitParticle_ID==-11)");
	gPad->Update();
        TPaveStats *st1 = (TPaveStats*)EnergyHisto_Copper_5mm->FindObject("stats");
        double statboxsize = st1->GetY2NDC() - st1->GetY1NDC();
        st1->SetTextColor(kGreen+1);
        st1->SetY1NDC(0.86);
        st1->SetY2NDC(0.86+statboxsize);	

	T_Copper_2_Plane2->Draw("HitEnergy >>+ Energy_Copper_4mm","(HitParticle_ID==11 || HitParticle_ID==-11)","SAMES");
	gPad->Update();
        TPaveStats *st2 = (TPaveStats*)EnergyHisto_Copper_4mm->FindObject("stats");
        st2->SetTextColor(kOrange);
        st2->SetY2NDC(st1->GetY1NDC()); //new x start position
        st2->SetY1NDC(st2->GetY2NDC()-statboxsize); //new x end position

	T_Copper_3_Plane2->Draw("HitEnergy >>+ Energy_Copper_3mm","(HitParticle_ID==11 || HitParticle_ID==-11)","SAMES");
	gPad->Update();
        TPaveStats *st3 = (TPaveStats*)EnergyHisto_Copper_3mm->FindObject("stats");
        st3->SetTextColor(kOrange+7);
        st3->SetY2NDC(st2->GetY1NDC()); //new x start position
        st3->SetY1NDC(st3->GetY2NDC()-statboxsize); //new x end position

	T_Alu_1_Plane2->Draw("HitEnergy >>+ Energy_Alu_3mm","(HitParticle_ID==11 || HitParticle_ID==-11)","SAMES");
	gPad->Update();
        TPaveStats *st4 = (TPaveStats*)EnergyHisto_Alu_3mm->FindObject("stats");
        st4->SetTextColor(kRed+1);
        st4->SetY2NDC(st3->GetY1NDC()); //new x start position
        st4->SetY1NDC(st4->GetY2NDC()-statboxsize); //new x end position

	T_Alu_2_Plane2->Draw("HitEnergy >>+ Energy_Alu_2mm","(HitParticle_ID==11 || HitParticle_ID==-11)","SAMES");
	gPad->Update();
        TPaveStats *st5 = (TPaveStats*)EnergyHisto_Alu_2mm->FindObject("stats");
        st5->SetTextColor(kMagenta+3);
        st5->SetY2NDC(st4->GetY1NDC()); //new x start position
        st5->SetY1NDC(st5->GetY2NDC()-statboxsize); //new x end position

	T_Alu_3_Plane2->Draw("HitEnergy >>+ Energy_Alu_1mm","(HitParticle_ID==11 || HitParticle_ID==-11)","SAMES");
	gPad->Update();
        TPaveStats *st6 = (TPaveStats*)EnergyHisto_Alu_1mm->FindObject("stats");
        st6->SetTextColor(kBlue);
        st6->SetY2NDC(st5->GetY1NDC()); //new x start position
        st6->SetY1NDC(st6->GetY2NDC()-statboxsize); //new x end position

	T_Copper_wire_Plane2->Draw("HitEnergy >>+ Energy_Copper_wire","(HitParticle_ID==11 || HitParticle_ID==-11)","SAMES");
	gPad->Update();
        TPaveStats *st7 = (TPaveStats*)EnergyHisto_Copper_wire->FindObject("stats");
        st7->SetTextColor(kCyan+2);
        st7->SetY2NDC(st6->GetY1NDC()); //new x start position
        st7->SetY1NDC(st7->GetY2NDC()-statboxsize); //new x end position

	TText *label1 = new TText();
 	label1-> SetNDC();
 	label1 -> SetTextFont(1);
 	label1 -> SetTextColor(1);
 	label1 -> SetTextSize(0.03);
 	label1 -> SetTextAlign(22);
 	label1 -> SetTextAngle(0);
	legend1->AddEntry(EnergyHisto_Copper_5mm,"Copper_5mm","l");
	legend1->AddEntry(EnergyHisto_Copper_4mm,"Copper_4mm","l");
	legend1->AddEntry(EnergyHisto_Copper_3mm,"Copper_3mm","l");
	legend1->AddEntry(EnergyHisto_Alu_3mm,"Alu_3mm","l");
	legend1->AddEntry(EnergyHisto_Alu_2mm,"Alu_2mm","l");
	legend1->AddEntry(EnergyHisto_Alu_1mm,"Alu_1mm","l");
	legend1->AddEntry(EnergyHisto_Copper_wire,"Copper_wire 1mm","l");
	legend1->Draw();

	
	string canvasname=Compare_Canvas->GetName();
	Compare_Canvas->Write();
	Compare_Canvas->Print((canvasname+".eps").c_str());	
	Compare_Canvas->Print((canvasname+".C").c_str());	

	THStack *stack = new THStack("teststack","Energy distribution of electrons and positrons behind the converter plate;Energy (GeV);Count");
	EnergyHisto_Copper_wire->SetFillColor(kCyan+2);
	EnergyHisto_Alu_1mm->SetFillColor(kBlue);
	EnergyHisto_Alu_2mm->SetFillColor(kMagenta+3);
	EnergyHisto_Alu_3mm->SetFillColor(kRed+1);
	EnergyHisto_Copper_3mm->SetFillColor(kOrange+7);
	EnergyHisto_Copper_4mm->SetFillColor(kOrange);
	EnergyHisto_Copper_5mm->SetFillColor(kGreen+1);
	stack->Add(EnergyHisto_Copper_wire);
	stack->Add(EnergyHisto_Alu_1mm);
	stack->Add(EnergyHisto_Alu_2mm);
	stack->Add(EnergyHisto_Alu_3mm);
	stack->Add(EnergyHisto_Copper_3mm);
	stack->Add(EnergyHisto_Copper_4mm);
	stack->Add(EnergyHisto_Copper_5mm);
	stack->SetMinimum(2.0);
	stack->SetMaximum(100000);
	stack->Draw();
	Compare_Canvas->Print("stack.eps","eps");


	Compare_Canvas->Close();


	output_rootfile->Write();
	input_rootfile_Copper_1->Close();
	input_rootfile_Copper_2->Close();
	input_rootfile_Copper_3->Close();
	input_rootfile_Alu_1->Close();
	input_rootfile_Alu_2->Close();
	input_rootfile_Alu_3->Close();
	input_rootfile_Copper_wire->Close();
}
