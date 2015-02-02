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

void DrawingMacro(string outputname,string name1_1,string name1_2,string name2_1,string name2_2); 

int main(int argc,char *argv[]){
	if(argc < 6 || argc > 6){
    		//explain how to use program	
    		cerr << "Type in name of output file AND all root filenames!" << endl;
    		cerr << "e.g. ./DrawHistograms outputfilename.root file1.root file2.root ..." << endl;
    		terminate();
	}	
	string 	filename1_1, filename1_2,
		filename2_1, filename2_2,
		outputfilename;
	outputfilename 	= argv[1];
	filename1_1 	= argv[2];
	filename1_2 	= argv[3];
	filename2_1 	= argv[4];
	filename2_2 	= argv[5];
       
	DrawingMacro(outputfilename,filename1_1,filename1_2,filename2_1,filename2_2);
}

void DrawingMacro(string outputname,string name1_1,string name1_2,string name2_1,string name2_2){
/*
	TFile * input_rootfile1_1 = new TFile(name1_1.c_str(),"READ");
	TFile * input_rootfile1_2 = new TFile(name1_2.c_str(),"READ");
	TTree * T1_1_Lead, T1_1_Concrete;
	TTree * T1_2_Lead, T1_2_Concrete;

	TFile * input_rootfile2_1 = new TFile(name2_1.c_str(),"READ");
	TFile * input_rootfile2_2 = new TFile(name2_2.c_str(),"READ");
	TTree * T2_1_Lead, T2_1_PMMA;
	TTree * T2_2_Lead, T2_2_PMMA;

	input_rootfile1_1->GetObject("Tree_BeforeLead",T1_1_Lead);
	input_rootfile1_1->GetObject("Tree_BeforeConcrete",T1_1_Concrete);
	input_rootfile1_2->GetObject("Tree_BeforeLead",T1_2_Lead);
	input_rootfile1_2->GetObject("Tree_BeforeConcrete",T1_2_Concrete);

	input_rootfile2_1->GetObject("Tree_BeforeLead",T2_1_Lead);
	input_rootfile2_1->GetObject("Tree_BeforePMMA",T2_1_PMMA);
	input_rootfile2_2->GetObject("Tree_BeforeLead",T2_2_Lead);
	input_rootfile2_2->GetObject("Tree_BeforePMMA",T2_2_PMMA);
*/

	TFile * input_rootfile1_1 = new TFile(name1_1.c_str(),"READ");
	TFile * input_rootfile1_2 = new TFile(name1_2.c_str(),"READ");
	TTree * T1_1;
	TTree * T1_2;

	TFile * input_rootfile2_1 = new TFile(name2_1.c_str(),"READ");
	TFile * input_rootfile2_2 = new TFile(name2_2.c_str(),"READ");
	TTree * T2_1;
	TTree * T2_2;

	input_rootfile1_1->GetObject("Tree_MCP",T1_1);
	input_rootfile1_2->GetObject("Tree_MCP",T1_2);

	input_rootfile2_1->GetObject("Tree_MCP",T2_1);
	input_rootfile2_2->GetObject("Tree_MCP",T2_2);


	cout << "Accessed TTrees.." << endl;

	TFile * output_rootfile = new TFile(outputname.c_str(),"RECREATE");

//	TH1::SetDefaultSumw2();

	TH1D * MomentumHisto_5GeV_ElePhot_Concrete = new TH1D("Momentum_5_ElePhot_Concrete","Momentum distribution of particles behind lead block with 20cm thickness and a concrete block/PMMA board. Initial photons with 5.0GeV",80,0,1.6);
	MomentumHisto_5GeV_ElePhot_Concrete->SetLineColor(4);
	MomentumHisto_5GeV_ElePhot_Concrete->GetXaxis()->SetTitle("Momentum (GeV)");
	MomentumHisto_5GeV_ElePhot_Concrete->GetXaxis()->CenterTitle();
	TH1D * MomentumHisto_5GeV_ProtNeut_Concrete = new TH1D("Momentum_5_ProtNeut_Concrete","Momentum distribution of particles behind lead block with 20cm thickness and a concrete block/PMMA board. Initial photons with 5.0GeV",80,0,1.6);
	MomentumHisto_5GeV_ProtNeut_Concrete->SetLineColor(2);
	MomentumHisto_5GeV_ProtNeut_Concrete->GetXaxis()->SetTitle("Momentum (GeV)");
	MomentumHisto_5GeV_ProtNeut_Concrete->GetXaxis()->CenterTitle();
	
	TH1D * MomentumHisto_5GeV_ElePhot_PMMA = new TH1D("Momentum_5_ElePhot_PMMA","Momentum distribution of particles behind lead block with 20cm thickness and a concrete block/PMMA board. Initial photons with 5.0GeV",80,0,1.6);
	MomentumHisto_5GeV_ElePhot_PMMA->SetLineColor(6);
	MomentumHisto_5GeV_ElePhot_PMMA->GetXaxis()->SetTitle("Momentum (GeV)");
	MomentumHisto_5GeV_ElePhot_PMMA->GetXaxis()->CenterTitle();
	TH1D * MomentumHisto_5GeV_ProtNeut_PMMA = new TH1D("Momentum_5_ProtNeut_PMMA","Momentum distribution of particles behind lead block with 20cm thickness and a concrete block/PMMA board. Initial photons with 5.0GeV",80,0,1.6);
	MomentumHisto_5GeV_ProtNeut_PMMA->SetLineColor(3);
	MomentumHisto_5GeV_ProtNeut_PMMA->GetXaxis()->SetTitle("Momentum (GeV)");
	MomentumHisto_5GeV_ProtNeut_PMMA->GetXaxis()->CenterTitle();


	TH1D * Stoppedz_5GeV_ProtNeut_Concrete = new TH1D("Stoppedz_5_ProtNeut_Concrete","End position of particles in a lead block with 20cm thickness and a concrete block/PMMA board. Initial photons with 5.0 GeV",200,22565,22975);
	Stoppedz_5GeV_ProtNeut_Concrete->SetLineColor(2);
	Stoppedz_5GeV_ProtNeut_Concrete->GetXaxis()->SetTitle("z (mm)");
	Stoppedz_5GeV_ProtNeut_Concrete->GetXaxis()->CenterTitle();
	Stoppedz_5GeV_ProtNeut_Concrete->GetYaxis()->SetRangeUser(0.1,100000000);
	TH1D * Stoppedz_5GeV_ElePhot_Concrete = new TH1D("Stoppedz_5_ElePhot_Concrete","End position of particles in a lead block with 20cm thickness and a concrete block/PMMA board. Initial photons with 5.0 GeV",200,22565,22975);
	Stoppedz_5GeV_ElePhot_Concrete->SetLineColor(4);
	Stoppedz_5GeV_ElePhot_Concrete->GetXaxis()->SetTitle("z (mm)");
	Stoppedz_5GeV_ElePhot_Concrete->GetXaxis()->CenterTitle();
	Stoppedz_5GeV_ElePhot_Concrete->GetYaxis()->SetRangeUser(0.1,100000000);

	TH1D * Stoppedz_5GeV_ProtNeut_PMMA = new TH1D("Stoppedz_5_ProtNeut_PMMA","End position of particles in a lead block with 20cm thickness and a concrete block/PMMA board. Initial photons with 5.0 GeV",200,22565,22975);
	Stoppedz_5GeV_ProtNeut_PMMA->SetLineColor(3);
	Stoppedz_5GeV_ProtNeut_PMMA->GetXaxis()->SetTitle("z (mm)");
	Stoppedz_5GeV_ProtNeut_PMMA->GetXaxis()->CenterTitle();
	Stoppedz_5GeV_ProtNeut_PMMA->GetYaxis()->SetRangeUser(0.1,100000000);
	TH1D * Stoppedz_5GeV_ElePhot_PMMA = new TH1D("Stoppedz_5_ElePhot_PMMA","End position of particles in a lead block with 20cm thickness and a concrete block/PMMA board. Initial photons with 5.0 GeV",200,22565,22975);
	Stoppedz_5GeV_ElePhot_PMMA->SetLineColor(6);
	Stoppedz_5GeV_ElePhot_PMMA->GetXaxis()->SetTitle("z (mm)");
	Stoppedz_5GeV_ElePhot_PMMA->GetXaxis()->CenterTitle();
	Stoppedz_5GeV_ElePhot_PMMA->GetYaxis()->SetRangeUser(0.1,100000000);

	TCanvas * ProtNeut_Stopped_Canvas = new TCanvas("5cmPMMA_VS_20cmConcrete_Stopped_Canvas");
	TCanvas * Compare_Canvas = new TCanvas("5cmPMMA_VS_20cmConcrete_EnergyMomentum_Canvas");

	TLegend * legend1 = new TLegend(0.55,0.5,0.75,0.65);
	legend1->SetFillColor(0);
	TLegend * legend2 = new TLegend(0.5,0.63,0.7,0.78);
	legend2->SetFillColor(0);

	ProtNeut_Stopped_Canvas->cd();
	gPad->SetLogy();
	gStyle->SetOptStat(11);
	T1_1->Draw("Reflectionz >>+ Stoppedz_5_ElePhot_Concrete","(Particle_ID==11 || Particle_ID==-11 || Particle_ID==22) && Reflectionx<400 && Reflectionx>-400 && Reflectiony<400 && Reflectiony>-400");	
	T1_2->Draw("Reflectionz >>+ Stoppedz_5_ElePhot_Concrete","(Particle_ID==11 || Particle_ID==-11 || Particle_ID==22) && Reflectionx<400 && Reflectionx>-400 && Reflectiony<400 && Reflectiony>-400","SAME");	
	gPad->Update();
        TPaveStats *st1_pos = (TPaveStats*)Stoppedz_5GeV_ElePhot_Concrete->FindObject("stats");
        double statboxsize_pos = st1_pos->GetY2NDC() - st1_pos->GetY1NDC();
        double statboxsize_pos_x = st1_pos->GetX2NDC() - st1_pos->GetX1NDC();
        st1_pos->SetTextColor(4);
        st1_pos->SetY1NDC(0.85);
        st1_pos->SetY2NDC(0.85+statboxsize_pos);	
        st1_pos->SetX1NDC(0.75);
        st1_pos->SetX2NDC(0.75+statboxsize_pos_x);	

	T1_1->Draw("Reflectionz >>+ Stoppedz_5_ProtNeut_Concrete","(Particle_ID==2212 || Particle_ID==2112) && Reflectionx<400 && Reflectionx>-400 && Reflectiony<400 && Reflectiony>-400","SAMES");	
	T1_2->Draw("Reflectionz >>+ Stoppedz_5_ProtNeut_Concrete","(Particle_ID==2212 || Particle_ID==2112) && Reflectionx<400 && Reflectionx>-400 && Reflectiony<400 && Reflectiony>-400","SAMES");	
	gPad->Update();
        TPaveStats *st2_pos = (TPaveStats*)Stoppedz_5GeV_ProtNeut_Concrete->FindObject("stats");
        st2_pos->SetTextColor(2);
        st2_pos->SetY2NDC(st1_pos->GetY1NDC()); //new x start position
        st2_pos->SetY1NDC(st2_pos->GetY2NDC()-statboxsize_pos); //new x end position
        st2_pos->SetX1NDC(st1_pos->GetX1NDC()); //new x start position
        st2_pos->SetX2NDC(st1_pos->GetX2NDC()); //new x end position

	T2_1->Draw("Reflectionz >>+ Stoppedz_5_ElePhot_PMMA","(Particle_ID==11 || Particle_ID==-11 || Particle_ID==22) && Reflectionx<400 && Reflectionx>-400 && Reflectiony<400 && Reflectiony>-400","SAMES");	
	T2_2->Draw("Reflectionz >>+ Stoppedz_5_ElePhot_PMMA","(Particle_ID==11 || Particle_ID==-11 || Particle_ID==22) && Reflectionx<400 && Reflectionx>-400 && Reflectiony<400 && Reflectiony>-400","SAMES");	
	gPad->Update();
        TPaveStats *st3_pos = (TPaveStats*)Stoppedz_5GeV_ElePhot_PMMA->FindObject("stats");
        st3_pos->SetTextColor(6);
        st3_pos->SetY2NDC(st2_pos->GetY1NDC()); //new x start position
        st3_pos->SetY1NDC(st3_pos->GetY2NDC()-statboxsize_pos); //new x end position
        st3_pos->SetX1NDC(st1_pos->GetX1NDC()); //new x start position
        st3_pos->SetX2NDC(st1_pos->GetX2NDC()); //new x end position

	T2_1->Draw("Reflectionz >>+ Stoppedz_5_ProtNeut_PMMA","(Particle_ID==2212 || Particle_ID==2112) && Reflectionx<400 && Reflectionx>-400 && Reflectiony<400 && Reflectiony>-400","SAMES");	
	T2_2->Draw("Reflectionz >>+ Stoppedz_5_ProtNeut_PMMA","(Particle_ID==2212 || Particle_ID==2112) && Reflectionx<400 && Reflectionx>-400 && Reflectiony<400 && Reflectiony>-400","SAMES");	
	gPad->Update();
        TPaveStats *st4_pos = (TPaveStats*)Stoppedz_5GeV_ProtNeut_PMMA->FindObject("stats");
        st4_pos->SetTextColor(3);
        st4_pos->SetY2NDC(st3_pos->GetY1NDC()); //new x start position
        st4_pos->SetY1NDC(st4_pos->GetY2NDC()-statboxsize_pos); //new x end position
        st4_pos->SetX1NDC(st1_pos->GetX1NDC()); //new x start position
        st4_pos->SetX2NDC(st1_pos->GetX2NDC()); //new x end position

	TText *label2 = new TText();
 	label2-> SetNDC();
 	label2 -> SetTextFont(1);
 	label2 -> SetTextColor(1);
 	label2 -> SetTextSize(0.03);
 	label2 -> SetTextAlign(22);
 	label2 -> SetTextAngle(0);
 	label2 -> DrawText(0.5,0.85, "450,000 initial photons with 5.0 GeV");
	legend2->AddEntry(Stoppedz_5GeV_ElePhot_Concrete,"Concrete: #gamma, e^-, e^+","l");
	legend2->AddEntry(Stoppedz_5GeV_ProtNeut_Concrete,"Concrete: p, n","l");
	legend2->AddEntry(Stoppedz_5GeV_ElePhot_PMMA,"PMMA: #gamma, e^-, e^+","l");
	legend2->AddEntry(Stoppedz_5GeV_ProtNeut_PMMA,"PMMA: p, n","l");
	legend2->Draw();

	string canvasname2=ProtNeut_Stopped_Canvas->GetName();
	ProtNeut_Stopped_Canvas->Write();
	ProtNeut_Stopped_Canvas->Print((canvasname2+".eps").c_str());	
	ProtNeut_Stopped_Canvas->Print((canvasname2+".C").c_str());	
	ProtNeut_Stopped_Canvas->Close();

	Compare_Canvas->cd();
	
	gPad->SetLogy();
	gStyle->SetOptStat(11);
	T1_1->Draw("Momentum >>+ Momentum_5_ElePhot_Concrete","(Particle_ID==22 || Particle_ID==11 || Particle_ID==-11) && Reflectionx<400 && Reflectionx>-400 && Reflectiony<400 && Reflectiony>-400 && Reflectionz>22565 && Reflectionz<22975");
	T1_2->Draw("Momentum >>+ Momentum_5_ElePhot_Concrete","(Particle_ID==22 || Particle_ID==11 || Particle_ID==-11) && Reflectionx<400 && Reflectionx>-400 && Reflectiony<400 && Reflectiony>-400 && Reflectionz>22565 && Reflectionz<22975","SAME");
	gPad->Update();
        TPaveStats *st1 = (TPaveStats*)MomentumHisto_5GeV_ElePhot_Concrete->FindObject("stats");
        double statboxsize = st1->GetY2NDC() - st1->GetY1NDC();
        st1->SetTextColor(4);
        st1->SetY1NDC(0.8);
        st1->SetY2NDC(0.8+statboxsize);	

	T1_1->Draw("Momentum >>+ Momentum_5_ProtNeut_Concrete","(Particle_ID==2212 || Particle_ID==2112) && Reflectionx<400 && Reflectionx>-400 && Reflectiony<400 && Reflectiony>-400 && Reflectionz>22565 && Reflectionz<22975","SAMES");
	T1_2->Draw("Momentum >>+ Momentum_5_ProtNeut_Concrete","(Particle_ID==2212 || Particle_ID==2112) && Reflectionx<400 && Reflectionx>-400 && Reflectiony<400 && Reflectiony>-400 && Reflectionz>22565 && Reflectionz<22975","SAMES");
	gPad->Update();
        TPaveStats *st2 = (TPaveStats*)MomentumHisto_5GeV_ProtNeut_Concrete->FindObject("stats");
        st2->SetTextColor(2);
        st2->SetY2NDC(st1->GetY1NDC()); //new x start position
        st2->SetY1NDC(st2->GetY2NDC()-statboxsize); //new x end position

	T2_1->Draw("Momentum >>+ Momentum_5_ElePhot_PMMA","(Particle_ID==22 || Particle_ID==11 || Particle_ID==-11) && Reflectionx<400 && Reflectionx>-400 && Reflectiony<400 && Reflectiony>-400 && Reflectionz>22565 && Reflectionz<22975","SAMES");
	T2_2->Draw("Momentum >>+ Momentum_5_ElePhot_PMMA","(Particle_ID==22 || Particle_ID==11 || Particle_ID==-11) && Reflectionx<400 && Reflectionx>-400 && Reflectiony<400 && Reflectiony>-400 && Reflectionz>22565 && Reflectionz<22975","SAMES");
	gPad->Update();
        TPaveStats *st3 = (TPaveStats*)MomentumHisto_5GeV_ElePhot_PMMA->FindObject("stats");
        st3->SetTextColor(6);
        st3->SetY2NDC(st2->GetY1NDC()); //new x start position
        st3->SetY1NDC(st3->GetY2NDC()-statboxsize); //new x end position

	T2_1->Draw("Momentum >>+ Momentum_5_ProtNeut_PMMA","(Particle_ID==2212 || Particle_ID==2112) && Reflectionx<400 && Reflectionx>-400 && Reflectiony<400 && Reflectiony>-400 && Reflectionz>22565 && Reflectionz<22975","SAMES");
	T2_2->Draw("Momentum >>+ Momentum_5_ProtNeut_PMMA","(Particle_ID==2212 || Particle_ID==2112) && Reflectionx<400 && Reflectionx>-400 && Reflectiony<400 && Reflectiony>-400 && Reflectionz>22565 && Reflectionz<22975","SAMES");
	gPad->Update();
        TPaveStats *st4 = (TPaveStats*)MomentumHisto_5GeV_ProtNeut_PMMA->FindObject("stats");
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
 	label1 -> DrawText(0.5,0.85, "450,000 initial photons with 5.0 GeV");
	legend1->AddEntry(MomentumHisto_5GeV_ElePhot_Concrete,"Concrete: #gamma, e^-, e^+","l");
	legend1->AddEntry(MomentumHisto_5GeV_ProtNeut_Concrete,"Concrete: p, n","l");
	legend1->AddEntry(MomentumHisto_5GeV_ElePhot_PMMA,"PMMA: #gamma, e^-, e^+","l");
	legend1->AddEntry(MomentumHisto_5GeV_ProtNeut_PMMA,"PMMA: p, n","l");
	legend1->Draw();

	
	string canvasname=Compare_Canvas->GetName();
	Compare_Canvas->Write();
	Compare_Canvas->Print((canvasname+".eps").c_str());	
	Compare_Canvas->Print((canvasname+".C").c_str());	
	Compare_Canvas->Close();


	output_rootfile->Write();
	input_rootfile1_1->Close();
	input_rootfile1_2->Close();
	input_rootfile2_1->Close();
	input_rootfile2_2->Close();
}
