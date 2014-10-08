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

void DrawingMacro(string outputname,string name1, string name2,string name3, string name4,string name5, string name6); 

int main(int argc,char *argv[]){
	if(argc < 8 || argc > 8){
    		//explain how to use program	
    		cerr << "Type in name of output file AND all root filenames, first w/o pipe, second with pipe!" << endl;
    		cerr << "e.g. ./DrawHistograms outputfilename.root file1.root file2.root ..." << endl;
    		terminate();
	}	
	string filename1, filename2,filename3,filename4,filename5,filename6, outputfilename;
	outputfilename = argv[1];
	filename1 = argv[2];
	filename2 = argv[3];
	filename3 = argv[4];
	filename4 = argv[5];
	filename5 = argv[6];
	filename6 = argv[7];
       
	DrawingMacro(outputfilename,filename1,filename2,filename3,filename4,filename5,filename6);
}

void DrawingMacro(string outputname,string name1, string name2,string name3, string name4,string name5, string name6){

	TFile * input_rootfile1 = new TFile(name1.c_str(),"READ");
	TFile * input_rootfile2 = new TFile(name2.c_str(),"READ");
	TFile * input_rootfile3 = new TFile(name3.c_str(),"READ");
	TFile * input_rootfile4 = new TFile(name4.c_str(),"READ");
	TFile * input_rootfile5 = new TFile(name5.c_str(),"READ");
	TFile * input_rootfile6 = new TFile(name6.c_str(),"READ");
	TTree * T1;
	input_rootfile1->GetObject("Tree",T1);
	TTree * T2; 
	input_rootfile2->GetObject("Tree",T2);
	TTree * T3;
	input_rootfile3->GetObject("Tree",T3);
	TTree * T4; 
	input_rootfile4->GetObject("Tree",T4);
	TTree * T5;
	input_rootfile5->GetObject("Tree",T5);
	TTree * T6; 
	input_rootfile6->GetObject("Tree",T6);
	cout << "Accessed TTrees.." << endl;

	TFile * output_rootfile = new TFile(outputname.c_str(),"RECREATE");

//	TH1::SetDefaultSumw2();

	TH1F * Reflectionx_02Tesla_Electrons = new TH1F("Reflx_02_Electrons","x-position of electrons in magnetic field with B = 0.2T",200,0,45);
	Reflectionx_02Tesla_Electrons->SetLineColor(4);
	Reflectionx_02Tesla_Electrons->GetXaxis()->SetTitle("x (mm)");
	Reflectionx_02Tesla_Electrons->GetXaxis()->CenterTitle();
	TH1F * Reflectionx_02Tesla_Positrons = new TH1F("Reflx_02_Positrons","x-position of positrons in magnetic field with B = 0.2T",200,-45,0);
	Reflectionx_02Tesla_Positrons->SetLineColor(4);
	Reflectionx_02Tesla_Positrons->GetXaxis()->SetTitle("x (mm)");
	Reflectionx_02Tesla_Positrons->GetXaxis()->CenterTitle();

	TH1F * Reflectionx_05Tesla_Electrons = new TH1F("Reflx_05_Electrons","x-position of electrons in magnetic field with B = 0.5T",200,0,45);
	Reflectionx_05Tesla_Electrons->SetLineColor(4);
	Reflectionx_05Tesla_Electrons->GetXaxis()->SetTitle("x (mm)");
	Reflectionx_05Tesla_Electrons->GetXaxis()->CenterTitle();
	TH1F * Reflectionx_05Tesla_Positrons = new TH1F("Reflx_05_Positrons","x-position of positrons in magnetic field with B = 0.5T",200,-45,0);
	Reflectionx_05Tesla_Positrons->SetLineColor(4);
	Reflectionx_05Tesla_Positrons->GetXaxis()->SetTitle("x (mm)");
	Reflectionx_05Tesla_Positrons->GetXaxis()->CenterTitle();
	
	TH1F * Reflectionx_5Tesla_Electrons = new TH1F("Reflx_5_Electrons","x-position of electrons in magnetic field with B = 5T",400,0,200);
	Reflectionx_5Tesla_Electrons->SetLineColor(4);
	Reflectionx_5Tesla_Electrons->GetXaxis()->SetTitle("x (mm)");
	Reflectionx_5Tesla_Electrons->GetXaxis()->CenterTitle();
	TH1F * Reflectiony_5Tesla_Electrons = new TH1F("Refly_5_Electrons","y-position of electrons in magnetic field with B = 5T",200,-50,50);
	Reflectiony_5Tesla_Electrons->SetLineColor(4);
	Reflectiony_5Tesla_Electrons->GetXaxis()->SetTitle("y (mm)");
	Reflectiony_5Tesla_Electrons->GetXaxis()->CenterTitle();
	TH1F * Reflectionz_5Tesla_Electrons = new TH1F("Reflz_5_Electrons","z-position of electrons in magnetic field with B = 5T",200,905,1005);
	Reflectionz_5Tesla_Electrons->SetLineColor(4);
	Reflectionz_5Tesla_Electrons->GetXaxis()->SetTitle("z (mm)");
	Reflectionz_5Tesla_Electrons->GetXaxis()->CenterTitle();

	TH1F * Reflectionx_5Tesla_Positrons = new TH1F("Reflx_5_Positrons","x-position of positrons in magnetic field with B = 5T",400,-200,0);
	Reflectionx_5Tesla_Positrons->SetLineColor(4);
	Reflectionx_5Tesla_Positrons->GetXaxis()->SetTitle("x (mm)");
	Reflectionx_5Tesla_Positrons->GetXaxis()->CenterTitle();


	TH1F * ThetaHisto_02Tesla = new TH1F("Theta_02","Angular distribution of particles with charge |e| in magnetic field with B = 0.2T",150,0,5);
	ThetaHisto_02Tesla->SetLineColor(4);
	ThetaHisto_02Tesla->GetXaxis()->SetTitle("#theta (degree)");
	ThetaHisto_02Tesla->GetXaxis()->CenterTitle();
	TH1F * ThetaHisto_05Tesla = new TH1F("Theta_05","Angular distribution of particles with charge |e| in magnetic field with B = 0.5T",150,0,5);
	ThetaHisto_05Tesla->SetLineColor(4);
	ThetaHisto_05Tesla->GetXaxis()->SetTitle("#theta (degree)");
	ThetaHisto_05Tesla->GetXaxis()->CenterTitle();
	TH1F * ThetaHisto_5Tesla = new TH1F("Theta_5","Angular distribution of particles with charge |e| in magnetic field with B = 5T",200,0,18);
	ThetaHisto_5Tesla->SetLineColor(4);
	ThetaHisto_5Tesla->GetXaxis()->SetTitle("#theta (degree)");
	ThetaHisto_5Tesla->GetXaxis()->CenterTitle();

	
	TCanvas * Theta_Canvas = new TCanvas("Theta_Canvas","Deflection_angle");
	TCanvas * Reflectionx_Canvas = new TCanvas("Reflectionx_Canvas");
	TCanvas * Reflectionxyz_Canvas = new TCanvas("Reflectionxyz_Canvas");

	Reflectionxyz_Canvas->Divide(2,2);
	Reflectionxyz_Canvas->cd(1);
	T5->Draw("Reflectionx >>+ Reflx_5_Electrons","Energy>3.9 && Energy<4.1");
	gPad->Update();
	TPaveStats *st1 = (TPaveStats*)Reflectionx_5Tesla_Electrons->FindObject("stats");
  	double statboxsize = st1->GetX2NDC() - st1->GetX1NDC();
 	st1->SetTextColor(1);
  	st1->SetX1NDC(0.65);
  	st1->SetX2NDC(0.65+statboxsize);	

	Reflectionxyz_Canvas->cd(2);
	T5->Draw("Reflectiony >>+ Refly_5_Electrons","Energy>3.9 && Energy<4.1");
	gPad->Update();
	TPaveStats *st2 = (TPaveStats*)Reflectiony_5Tesla_Electrons->FindObject("stats");
  	double statboxsize2 = st2->GetX2NDC() - st2->GetX1NDC();
 	st2->SetTextColor(1);
  	st2->SetX1NDC(0.65);
  	st2->SetX2NDC(0.65+statboxsize2);	

	Reflectionxyz_Canvas->cd(3);
	T5->Draw("Reflectionz >>+ Reflz_5_Electrons","Energy>3.9 && Energy<4.1");
	gPad->Update();
	TPaveStats *st3 = (TPaveStats*)Reflectionz_5Tesla_Electrons->FindObject("stats");
  	double statboxsize3 = st3->GetX2NDC() - st3->GetX1NDC();
 	st3->SetTextColor(1);
  	st3->SetX1NDC(0.65);
  	st3->SetX2NDC(0.65+statboxsize3);	

	string canvasname1=Reflectionxyz_Canvas->GetName();
	Reflectionxyz_Canvas->Write();
	Reflectionxyz_Canvas->Print((canvasname1+".eps").c_str());	
	Reflectionxyz_Canvas->Print((canvasname1+".C").c_str());	
	Reflectionxyz_Canvas->Close();

	Reflectionx_Canvas->Divide(3,2);
	Reflectionx_Canvas->cd(1);
	T1->Draw("Reflectionx >>+ Reflx_02_Electrons","Energy>3.9 && Energy<4.1");

	Reflectionx_Canvas->cd(4);
	T2->Draw("Reflectionx >>+ Reflx_02_Positrons","Energy>3.9 && Energy<4.1");

	Reflectionx_Canvas->cd(2);
	T3->Draw("Reflectionx >>+ Reflx_05_Electrons","Energy>3.9 && Energy<4.1");

	Reflectionx_Canvas->cd(5);
	T4->Draw("Reflectionx >>+ Reflx_05_Positrons","Energy>3.9 && Energy<4.1");

	Reflectionx_Canvas->cd(3);
	T5->Draw("Reflectionx >>+ Reflx_5_Electrons","Energy>3.9 && Energy<4.1");
	gPad->Update();
	TPaveStats *st4 = (TPaveStats*)Reflectionx_5Tesla_Electrons->FindObject("stats");
  	double statboxsize4 = st4->GetX2NDC() - st4->GetX1NDC();
 	st4->SetTextColor(1);
	st4->SetX1NDC(0.65);
  	st4->SetX2NDC(0.65+statboxsize4);	

	Reflectionx_Canvas->cd(6);
	T6->Draw("Reflectionx >>+ Reflx_5_Positrons","Energy>3.9 && Energy<4.1");
	gPad->Update();
	TPaveStats *st5 = (TPaveStats*)Reflectionx_5Tesla_Positrons->FindObject("stats");
  	double statboxsize5 = st5->GetX2NDC() - st5->GetX1NDC();
 	st5->SetTextColor(1);
  	st5->SetX1NDC(0.65);
  	st5->SetX2NDC(0.65+statboxsize5);	

	string canvasname2=Reflectionx_Canvas->GetName();
	Reflectionx_Canvas->Write();
	Reflectionx_Canvas->Print((canvasname2+".eps").c_str());	
	Reflectionx_Canvas->Print((canvasname2+".C").c_str());	
	Reflectionx_Canvas->Close();

	Theta_Canvas->Divide(2,2);
	Theta_Canvas->cd(1);
	T1->Draw("Theta >>+ Theta_02","Energy>3.9 && Energy<4.1");
	TLine * l1 = new TLine(0.43,0,0.43,ThetaHisto_02Tesla->GetBinContent(ThetaHisto_02Tesla->GetMaximumBin())+10000);
	l1->SetLineWidth(1.5);
	l1->SetLineColor(2);
	l1->Draw();
	TText *label1 = new TText();
 	label1-> SetNDC();
 	label1 -> SetTextFont(1);
 	label1 -> SetTextColor(2);
 	label1 -> SetTextSize(0.03);
 	label1 -> SetTextAlign(22);
 	label1 -> SetTextAngle(0);
 	label1 -> DrawText(0.175,0.875, "0.430 degree");

	Theta_Canvas->cd(2);
	T3->Draw("Theta >>+ Theta_05","Energy>3.9 && Energy<4.1");
	TLine * l2 = new TLine(1.075,0,1.075,ThetaHisto_05Tesla->GetBinContent(ThetaHisto_05Tesla->GetMaximumBin())+10000);
	l2->SetLineWidth(1.5);
	l2->SetLineColor(2);
	l2->Draw();
	TText *label2 = new TText();
 	label2-> SetNDC();
 	label2 -> SetTextFont(1);
 	label2 -> SetTextColor(2);
 	label2 -> SetTextSize(0.03);
 	label2 -> SetTextAlign(22);
 	label2 -> SetTextAngle(0);
 	label2 -> DrawText(0.28,0.875, "1.075 degree");

	Theta_Canvas->cd(3);
	T5->Draw("Theta >>+ Theta_5","Energy>3.9 && Energy<4.1");
	TLine * l3 = new TLine(11.012,0,11.012,ThetaHisto_5Tesla->GetBinContent(ThetaHisto_5Tesla->GetMaximumBin())+10000);
	l3->SetLineWidth(1.5);
	l3->SetLineColor(2);
	l3->Draw();
	TText *label3 = new TText();
 	label3-> SetNDC();
 	label3 -> SetTextFont(1);
 	label3 -> SetTextColor(2);
 	label3 -> SetTextSize(0.03);
 	label3 -> SetTextAlign(22);
 	label3 -> SetTextAngle(0);
 	label3 -> DrawText(0.59,0.875, "11.012 degree");

	string canvasname3=Theta_Canvas->GetName();
	Theta_Canvas->Write();
	Theta_Canvas->Print((canvasname3+".eps").c_str());	
	Theta_Canvas->Print((canvasname3+".C").c_str());	
	Theta_Canvas->Close();



	output_rootfile->Write();
	input_rootfile1->Close();
	input_rootfile2->Close();
	input_rootfile3->Close();
	input_rootfile4->Close();
	input_rootfile5->Close();
	input_rootfile6->Close();
}
