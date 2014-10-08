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



	TH1F * ThetaHisto_05Tesla_1GeV = new TH1F("Theta_1GeV","Angular distribution of particles with charge |e| and an energy of 1GeV in a magnetic field with B = 0.5T",150,0,5);
	ThetaHisto_05Tesla_1GeV->SetLineColor(4);
	ThetaHisto_05Tesla_1GeV->GetXaxis()->SetTitle("#theta (degree)");
	ThetaHisto_05Tesla_1GeV->GetXaxis()->CenterTitle();
	TH1F * ThetaHisto_05Tesla_3GeV = new TH1F("Theta_3GeV","Angular distribution of particles with charge |e| and an energy of 3GeV in a magnetic field with B = 0.5T",150,0,5);
	ThetaHisto_05Tesla_3GeV->SetLineColor(4);
	ThetaHisto_05Tesla_3GeV->GetXaxis()->SetTitle("#theta (degree)");
	ThetaHisto_05Tesla_3GeV->GetXaxis()->CenterTitle();
	TH1F * ThetaHisto_05Tesla_6GeV = new TH1F("Theta_6GeV","Angular distribution of particles with charge |e| and an energy of 6GeV in a magnetic field with B = 0.5T",150,0,5);
	ThetaHisto_05Tesla_6GeV->SetLineColor(4);
	ThetaHisto_05Tesla_6GeV->GetXaxis()->SetTitle("#theta (degree)");
	ThetaHisto_05Tesla_6GeV->GetXaxis()->CenterTitle();

	
	TCanvas * ThetaEnergy_Canvas = new TCanvas("ThetaEnergy_Canvas","Deflection_angle dependent on energy");


	ThetaEnergy_Canvas->Divide(2,2);
	ThetaEnergy_Canvas->cd(1);
	T1->Draw("Theta >>+ Theta_1GeV","Energy>0.9 && Energy<1.1");
	gPad->Update();
        TPaveStats *st1 = (TPaveStats*)ThetaHisto_05Tesla_1GeV->FindObject("stats");
        double statboxsize = 0.7*(st1->GetX2NDC() - st1->GetX1NDC());
        st1->SetTextColor(1);
        st1->SetTextSize(0.03);
        st1->SetX1NDC(0.85);
        st1->SetX2NDC(0.85+statboxsize);	

	TLine * l1 = new TLine(4.313,0,4.313,ThetaHisto_05Tesla_1GeV->GetBinContent(ThetaHisto_05Tesla_1GeV->GetMaximumBin())+10000);
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
 	label1 -> DrawText(0.795,0.875, "4.313 degree");

	ThetaEnergy_Canvas->cd(2);
	T2->Draw("Theta >>+ Theta_3GeV","Energy>2.9 && Energy<3.1");
	gPad->Update();
        TPaveStats *st2 = (TPaveStats*)ThetaHisto_05Tesla_3GeV->FindObject("stats");
        st2->SetTextColor(1);
        st2->SetTextSize(0.03);
        st2->SetX2NDC(0.85); //new x start position
        st2->SetX1NDC(0.85+statboxsize); //new x end position

	TLine * l2 = new TLine(1.433,0,1.433,ThetaHisto_05Tesla_3GeV->GetBinContent(ThetaHisto_05Tesla_3GeV->GetMaximumBin())+10000);
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
 	label2 -> DrawText(0.335,0.875, "1.433 degree");

	ThetaEnergy_Canvas->cd(3);
	T3->Draw("Theta >>+ Theta_6GeV","Energy>5.9 && Energy<6.1");
	gPad->Update();
        TPaveStats *st3 = (TPaveStats*)ThetaHisto_05Tesla_6GeV->FindObject("stats");
        st3->SetTextColor(1);
        st3->SetTextSize(0.03);
        st3->SetX1NDC(0.85);
        st3->SetX2NDC(0.85+statboxsize);	

	TLine * l3 = new TLine(0.716,0,0.716,ThetaHisto_05Tesla_6GeV->GetBinContent(ThetaHisto_05Tesla_6GeV->GetMaximumBin())+10000);
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
 	label3 -> DrawText(0.22,0.875, "0.716 degree");

	string canvasname3=ThetaEnergy_Canvas->GetName();
	ThetaEnergy_Canvas->Write();
	ThetaEnergy_Canvas->Print((canvasname3+".eps").c_str());	
	ThetaEnergy_Canvas->Print((canvasname3+".C").c_str());	
	ThetaEnergy_Canvas->Close();



	output_rootfile->Write();
	input_rootfile1->Close();
	input_rootfile2->Close();
	input_rootfile3->Close();
}
