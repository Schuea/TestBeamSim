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
#include "TMath.h"

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
	TH1::SetDefaultSumw2();

	TFile * input_rootfile = new TFile(name.c_str(),"READ");
	cout << "Inputfile size = " << input_rootfile->GetSize() << endl;
	TTree * T = (TTree*)input_rootfile->Get("Tree");
	cout << "Accessed TTree.." << endl;

	stringstream output_filename;
	output_filename << "theta_enegy_plots_" << name;
	TFile * output_rootfile = new TFile(output_filename.str().c_str(),"RECREATE");

	TH1F * EnergyHisto_ElePosi = new TH1F("E_ElePosi","Energy distribution of electrons and positrons existing behind magnet",80,0,6.5);
	EnergyHisto_ElePosi->SetLineColor(4);
	EnergyHisto_ElePosi->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_ElePosi->GetXaxis()->CenterTitle();

	TH1F * EnergyHisto_ProtNeut = new TH1F("E_ProtNeut","Energy distribution of protonsi, neutrons and photons existing behind magnet",93,0,6.5);
	EnergyHisto_ProtNeut->SetLineColor(4);
	EnergyHisto_ProtNeut->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_ProtNeut->GetXaxis()->CenterTitle();
	TH1F * EnergyHisto_Photons = new TH1F("E_Photons","Energy distribution of protons and photons existing behind magnet",93,0,6.5);
	EnergyHisto_Photons->SetLineColor(2);
	EnergyHisto_Photons->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_Photons->GetXaxis()->CenterTitle();

	TH1F * AngleHisto_Prot = new TH1F("AngleHisto_Prot","Angular distribution of protons and #pi^{+} behind magnet",70,0,180);
	AngleHisto_Prot->SetLineColor(4);
	AngleHisto_Prot->GetXaxis()->SetTitle("#theta (degree)");
	AngleHisto_Prot->GetXaxis()->CenterTitle();
	TH1F * AngleHisto_Pions = new TH1F("AngleHisto_Pions","Angular distribution of protons and #pi^{+} behind magnet",70,0,180);
	AngleHisto_Pions->SetLineColor(6);
	AngleHisto_Pions->GetXaxis()->SetTitle("#theta (degree)");
	AngleHisto_Pions->GetXaxis()->CenterTitle();

	float energy = 1.0;
	float energyinterval = 1.0;
	float max_angle = 6.0;
	float max_angle2 = 6.0;
	vector< TH1F* > AngleHisto_ElePosi_;
	vector< TH1F* > AngleHisto_Muon_;
	vector< TH1F* > AngleHisto_PionCharge_;
	vector< TH1F* > AngleHisto_Pion_;
	vector< TH1F* > AngleHisto_Phot_;
	vector< TH1F* > AngleHisto_Neut_;

	vector< TH1F* > cosTheta_histo_ElePosi_;
	vector< TH1F* > cosTheta_histo_Muon_;
	vector< TH1F* > cosTheta_histo_PionCharge_;
	vector< TH1F* > cosTheta_histo_Pion_;
	vector< TH1F* > cosTheta_histo_Phot_;
	vector< TH1F* > cosTheta_histo_Neut_;

	vector< TCanvas* > Angle_Canvas_;
	vector< TCanvas* > cosTheta_Canvas_;
	vector< TPaveText* > AngleCutBox_;
	vector< TPaveText* > cosTheta_CutBox_;

	for(int i = 1; i < 8; ++i){
		if (energy <= 2.0){
			max_angle = 6.0;
			max_angle2 = 6.0;
		}
                else if (energy > 2.0 && energy <=5.0 ){
			max_angle = 3.0;
			max_angle2 = 3.0;
		}
                else if (energy > 5.0 && energy < 7.0){
			max_angle = 2.0;
			max_angle2 = 0.5;
			energyinterval = 0.3;
		}

		stringstream histoname1_angle,histoname2_angle,histoname3_angle,histoname4_angle,histoname5_angle,histoname6_angle;
		histoname1_angle << "AngleHisto_ElePosi_" << i;
                histoname2_angle << "AngleHisto_Muon_" << i;
                histoname3_angle << "AngleHisto_PionCharge_" << i;
                histoname4_angle << "AngleHisto_Pion_" << i;
                histoname5_angle << "AngleHisto_Phot_" << i;
                histoname6_angle << "AngleHisto_Neut_" << i;
		stringstream histoname1_cosTheta,histoname2_cosTheta,histoname3_cosTheta,histoname4_cosTheta,histoname5_cosTheta,histoname6_cosTheta;
		histoname1_cosTheta << "cosTheta_histo_ElePosi_" << i;
                histoname2_cosTheta << "cosTheta_histo_Muon_" << i;
                histoname3_cosTheta << "cosTheta_histo_PionCharge_" << i;
                histoname4_cosTheta << "cosTheta_histo_Pion_" << i;
                histoname5_cosTheta << "cosTheta_histo_Phot_" << i;
                histoname6_cosTheta << "cosTheta_histo_Neut_" << i;

		stringstream histotitle1_angle,histotitle2_angle;
		histotitle1_angle << "Angular distribution of charged particles for an energy between " << energy -0.1 << " and " << energy + 0.1 << " GeV";
		histotitle2_angle << "Angular distribution of neutral particles for an energy between " << energy -0.1 << " and " << energy + 0.1 << " GeV";
		stringstream histotitle1_cosTheta,histotitle2_cosTheta;
		histotitle1_cosTheta << "cos(#theta)-distribution of charged particles for an energy between " << energy -0.1 << " and " << energy + 0.1 << " GeV";
		histotitle2_cosTheta << "cos(#theta)-distribution of neutral particles for an energy between " << energy -0.1 << " and " << energy + 0.1 << " GeV";

		stringstream drawtext1_angle,drawtext2_angle,drawtext3_angle,drawtext4_angle,drawtext5_angle,drawtext6_angle;
		drawtext1_angle << "Theta >>+ " << histoname1_angle.str();
		drawtext2_angle << "Theta >>+ " << histoname2_angle.str();
		drawtext3_angle << "Theta >>+ " << histoname3_angle.str();
		drawtext4_angle << "Theta >>+ " << histoname4_angle.str();
		drawtext5_angle << "Theta >>+ " << histoname5_angle.str();
		drawtext6_angle << "Theta >>+ " << histoname6_angle.str();
		stringstream drawtext1_cosTheta,drawtext2_cosTheta,drawtext3_cosTheta,drawtext4_cosTheta,drawtext5_cosTheta,drawtext6_cosTheta;
		drawtext1_cosTheta << "cos(TMath::DegToRad()*Theta) >>+ " << histoname1_cosTheta.str();
		drawtext2_cosTheta << "cos(TMath::DegToRad()*Theta) >>+ " << histoname2_cosTheta.str();
		drawtext3_cosTheta << "cos(TMath::DegToRad()*Theta) >>+ " << histoname3_cosTheta.str();
		drawtext4_cosTheta << "cos(TMath::DegToRad()*Theta) >>+ " << histoname4_cosTheta.str();
		drawtext5_cosTheta << "cos(TMath::DegToRad()*Theta) >>+ " << histoname5_cosTheta.str();
		drawtext6_cosTheta << "cos(TMath::DegToRad()*Theta) >>+ " << histoname6_cosTheta.str();
	
		stringstream cuttext1,cuttext2,cuttext3,cuttext4,cuttext5,cuttext6;
		cuttext1 << "(Particle_ID==11 || Particle_ID==-11) && Energy > "<< energy-0.1 <<" && Energy < " << energy+0.1 << " && hasLeftDetector_Status==1";
		cuttext2 << "(Particle_ID==13 || Particle_ID==-13) && Energy > "<< energy-0.1 <<" && Energy < " << energy+0.1 << " && hasLeftDetector_Status==1";
		cuttext3 << "(Particle_ID==211 || Particle_ID==-211) && Energy > "<< energy-0.1 <<" && Energy < " << energy+0.1 << " && hasLeftDetector_Status==1";
		cuttext4 << "Particle_ID==111 && Energy > "<< energy-0.1 <<" && Energy < " << energy+0.1 << " && hasLeftDetector_Status==1";
		cuttext5 << "Particle_ID==22 && Energy > "<< energy-0.1 <<" && Energy < " << energy+0.1 << " && hasLeftDetector_Status==1";
		cuttext6 << "Particle_ID==2112 && Energy > "<< energy-0.1 <<" && Energy < " << energy+0.1 << " && hasLeftDetector_Status==1";
	
		stringstream pavetext;
		pavetext << energy-0.1 << " GeV < Energy < " << energy+0.1 << " GeV";
		stringstream savetext1_angle, savetext2_angle;
		savetext1_angle << "DeflectionAngle_Magnet_" << energy << "GeV.eps";
		savetext2_angle << "DeflectionAngle_Magnet_" << energy << "GeV.C";
		stringstream savetext1_cosTheta, savetext2_cosTheta;
		savetext1_cosTheta << "cosTheta_Magnet_" << energy << "GeV.eps";
		savetext2_cosTheta << "cosTheta_Magnet_" << energy << "GeV.C";

		TH1F* temphisto1_angle = new TH1F(histoname1_angle.str().c_str(), histotitle1_angle.str().c_str(),80,0,max_angle);
		TH1F* temphisto2_angle = new TH1F(histoname2_angle.str().c_str(), histotitle1_angle.str().c_str(),80,0,max_angle);
		TH1F* temphisto3_angle = new TH1F(histoname3_angle.str().c_str(), histotitle1_angle.str().c_str(),80,0,max_angle);
		TH1F* temphisto4_angle = new TH1F(histoname4_angle.str().c_str(), histotitle2_angle.str().c_str(),80,0,max_angle2);
		TH1F* temphisto5_angle = new TH1F(histoname5_angle.str().c_str(), histotitle2_angle.str().c_str(),80,0,max_angle2);
		TH1F* temphisto6_angle = new TH1F(histoname6_angle.str().c_str(), histotitle2_angle.str().c_str(),80,0,max_angle2);
                AngleHisto_ElePosi_.push_back(temphisto1_angle);
	  	AngleHisto_ElePosi_.at(i-1)->SetLineColor(2);
		AngleHisto_ElePosi_.at(i-1)->GetXaxis()->SetTitle("#theta (degree)");
		AngleHisto_ElePosi_.at(i-1)->GetXaxis()->CenterTitle();
       
	        AngleHisto_Muon_.push_back(temphisto2_angle);
		AngleHisto_Muon_.at(i-1)->SetLineColor(3);
		AngleHisto_Muon_.at(i-1)->GetXaxis()->SetTitle("#theta (degree)");
		AngleHisto_Muon_.at(i-1)->GetXaxis()->CenterTitle();
                
		AngleHisto_PionCharge_.push_back(temphisto3_angle);
               	AngleHisto_PionCharge_.at(i-1)->SetLineColor(4);
		AngleHisto_PionCharge_.at(i-1)->GetXaxis()->SetTitle("#theta (degree)");
		AngleHisto_PionCharge_.at(i-1)->GetXaxis()->CenterTitle();

		AngleHisto_Pion_.push_back(temphisto4_angle);
               	AngleHisto_Pion_.at(i-1)->SetLineColor(6);
		AngleHisto_Pion_.at(i-1)->GetXaxis()->SetTitle("#theta (degree)");
		AngleHisto_Pion_.at(i-1)->GetXaxis()->CenterTitle();
              
		AngleHisto_Phot_.push_back(temphisto5_angle);
               	AngleHisto_Phot_.at(i-1)->SetLineColor(7);
		AngleHisto_Phot_.at(i-1)->GetXaxis()->SetTitle("#theta (degree)");
		AngleHisto_Phot_.at(i-1)->GetXaxis()->CenterTitle();

		AngleHisto_Neut_.push_back(temphisto6_angle);
		AngleHisto_Neut_.at(i-1)->SetLineColor(9);
		AngleHisto_Neut_.at(i-1)->GetXaxis()->SetTitle("#theta (degree)");
		AngleHisto_Neut_.at(i-1)->GetXaxis()->CenterTitle();

		TCanvas *tempcanvas_angle = new TCanvas();
		Angle_Canvas_.push_back(tempcanvas_angle);

		Angle_Canvas_.at(i-1)->Divide(1,2);
		Angle_Canvas_.at(i-1)->cd(1);
		gStyle->SetOptStat(11);
		gPad->SetLogy();
	
		T->Draw(drawtext1_angle.str().c_str(), cuttext1.str().c_str());
		gPad->Update();
        	TPaveStats *st1 = (TPaveStats*)AngleHisto_ElePosi_.at(i-1)->FindObject("stats");
        	double statboxsize = st1->GetY2NDC() - st1->GetY1NDC();
        	st1->SetTextColor(2);
        	st1->SetY1NDC(0.8);
        	st1->SetY2NDC(0.8+statboxsize);	
		T->Draw(drawtext2_angle.str().c_str(), cuttext2.str().c_str(),"SAMES");
		gPad->Update();
        	TPaveStats *st2 = (TPaveStats*)AngleHisto_Muon_.at(i-1)->FindObject("stats");
        	st2->SetTextColor(3);
        	st2->SetY1NDC(0.8);
        	st2->SetY2NDC(0.8+statboxsize);	
		T->Draw(drawtext3_angle.str().c_str(), cuttext3.str().c_str(),"SAMES");
		gPad->Update();
        	TPaveStats *st3 = (TPaveStats*)AngleHisto_PionCharge_.at(i-1)->FindObject("stats");
        	st3->SetTextColor(4);
        	st3->SetY1NDC(0.8);
        	st3->SetY2NDC(0.8+statboxsize);	

		TLegend * AngleLegend = new TLegend(0.75,0.4,0.85,0.5);
		AngleLegend->SetFillColor(0);
		AngleLegend->AddEntry(AngleHisto_ElePosi_.at(i-1),"e^{-}, e^{+}");
		AngleLegend->AddEntry(AngleHisto_Muon_.at(i-1),"#mu^{-}, #mu^{+}");
		AngleLegend->AddEntry(AngleHisto_PionCharge_.at(i-1),"#pi^{-}, #pi^{+}");
		AngleLegend->Draw();	

		TPaveText* tempbox = new TPaveText(.4,.81,.6,.88,"brNDC");
		AngleCutBox_.push_back(tempbox);
		AngleCutBox_.at(i-1)->AddText(pavetext.str().c_str());
		AngleCutBox_.at(i-1)->Draw();

		Angle_Canvas_.at(i-1)->cd(2);
		gStyle->SetOptStat(11);
		gPad->SetLogy();

		T->Draw(drawtext4_angle.str().c_str(), cuttext4.str().c_str());
		gPad->Update();
        	TPaveStats *st4 = (TPaveStats*)AngleHisto_Pion_.at(i-1)->FindObject("stats");
        	st4->SetTextColor(6);
         	st4->SetY1NDC(0.8);
        	st4->SetY2NDC(0.8+statboxsize);	
		T->Draw(drawtext5_angle.str().c_str(), cuttext5.str().c_str(),"SAMES");
		gPad->Update();
        	TPaveStats *st5 = (TPaveStats*)AngleHisto_Phot_.at(i-1)->FindObject("stats");
        	st5->SetTextColor(7);
         	st5->SetY1NDC(0.8);
        	st5->SetY2NDC(0.8+statboxsize);	
		T->Draw(drawtext6_angle.str().c_str(), cuttext6.str().c_str(),"SAMES");
		gPad->Update();
        	TPaveStats *st6 = (TPaveStats*)AngleHisto_Neut_.at(i-1)->FindObject("stats");
        	st6->SetTextColor(9);
         	st6->SetY1NDC(0.8);
        	st6->SetY2NDC(0.8+statboxsize);	

		TLegend * AngleLegend2 = new TLegend(0.75,0.4,0.85,0.5);
		AngleLegend2->SetFillColor(0);
		AngleLegend2->AddEntry(AngleHisto_Pion_.at(i-1),"#pi^{0}");
		AngleLegend2->AddEntry(AngleHisto_Phot_.at(i-1),"#gamma");
		AngleLegend2->AddEntry(AngleHisto_Neut_.at(i-1),"n");
		AngleLegend2->Draw();	

		AngleCutBox_.at(i-1)->Draw();

		Angle_Canvas_.at(i-1)->Write();
		Angle_Canvas_.at(i-1)->Print(savetext1_angle.str().c_str());
		Angle_Canvas_.at(i-1)->Print(savetext2_angle.str().c_str());
		Angle_Canvas_.at(i-1)->Close();

                //---cos(theta) distributions:	
		TH1F* temphisto1_cosTheta = new TH1F(histoname1_cosTheta.str().c_str(), histotitle1_cosTheta.str().c_str(),100,0.995,1);
		TH1F* temphisto2_cosTheta = new TH1F(histoname2_cosTheta.str().c_str(), histotitle1_cosTheta.str().c_str(),100,0.995,1);
		TH1F* temphisto3_cosTheta = new TH1F(histoname3_cosTheta.str().c_str(), histotitle1_cosTheta.str().c_str(),100,0.995,1);
		TH1F* temphisto4_cosTheta = new TH1F(histoname4_cosTheta.str().c_str(), histotitle2_cosTheta.str().c_str(),100,0.995,1);
		TH1F* temphisto5_cosTheta = new TH1F(histoname5_cosTheta.str().c_str(), histotitle2_cosTheta.str().c_str(),100,0.995,1);
		TH1F* temphisto6_cosTheta = new TH1F(histoname6_cosTheta.str().c_str(), histotitle2_cosTheta.str().c_str(),100,0.995,1);
                cosTheta_histo_ElePosi_.push_back(temphisto1_cosTheta);
	  	cosTheta_histo_ElePosi_.at(i-1)->SetLineColor(2);
		cosTheta_histo_ElePosi_.at(i-1)->GetXaxis()->SetTitle("cos(#theta)");
		cosTheta_histo_ElePosi_.at(i-1)->GetXaxis()->CenterTitle();
       
	        cosTheta_histo_Muon_.push_back(temphisto2_cosTheta);
		cosTheta_histo_Muon_.at(i-1)->SetLineColor(3);
		cosTheta_histo_Muon_.at(i-1)->GetXaxis()->SetTitle("cos(#theta)");
		cosTheta_histo_Muon_.at(i-1)->GetXaxis()->CenterTitle();
                
		cosTheta_histo_PionCharge_.push_back(temphisto3_cosTheta);
               	cosTheta_histo_PionCharge_.at(i-1)->SetLineColor(4);
		cosTheta_histo_PionCharge_.at(i-1)->GetXaxis()->SetTitle("cos(#theta)");
		cosTheta_histo_PionCharge_.at(i-1)->GetXaxis()->CenterTitle();

		cosTheta_histo_Pion_.push_back(temphisto4_cosTheta);
               	cosTheta_histo_Pion_.at(i-1)->SetLineColor(6);
		cosTheta_histo_Pion_.at(i-1)->GetXaxis()->SetTitle("cos(#theta)");
		cosTheta_histo_Pion_.at(i-1)->GetXaxis()->CenterTitle();
              
		cosTheta_histo_Phot_.push_back(temphisto5_cosTheta);
               	cosTheta_histo_Phot_.at(i-1)->SetLineColor(7);
		cosTheta_histo_Phot_.at(i-1)->GetXaxis()->SetTitle("cos(#theta)");
		cosTheta_histo_Phot_.at(i-1)->GetXaxis()->CenterTitle();

		cosTheta_histo_Neut_.push_back(temphisto6_cosTheta);
		cosTheta_histo_Neut_.at(i-1)->SetLineColor(9);
		cosTheta_histo_Neut_.at(i-1)->GetXaxis()->SetTitle("cos(#theta)");
		cosTheta_histo_Neut_.at(i-1)->GetXaxis()->CenterTitle();

		TCanvas *tempcanvas_cosTheta = new TCanvas();
		cosTheta_Canvas_.push_back(tempcanvas_cosTheta);

		cosTheta_Canvas_.at(i-1)->Divide(1,2);
		cosTheta_Canvas_.at(i-1)->cd(1);
		gStyle->SetOptStat(11);
		gPad->SetLogy();
	
		T->Draw(drawtext1_cosTheta.str().c_str(), cuttext1.str().c_str());
		gPad->Update();
        	TPaveStats *st1_cosTheta = (TPaveStats*)cosTheta_histo_ElePosi_.at(i-1)->FindObject("stats");
        	double statboxsize_cosTheta = st1_cosTheta->GetY2NDC() - st1_cosTheta->GetY1NDC();
        	st1_cosTheta->SetTextColor(2);
        	st1_cosTheta->SetY1NDC(0.8);
        	st1_cosTheta->SetY2NDC(0.8+statboxsize_cosTheta);	
		T->Draw(drawtext2_cosTheta.str().c_str(), cuttext2.str().c_str(),"SAMES");
		gPad->Update();
        	TPaveStats *st2_cosTheta = (TPaveStats*)cosTheta_histo_Muon_.at(i-1)->FindObject("stats");
        	st2_cosTheta->SetTextColor(3);
        	st2_cosTheta->SetY1NDC(0.8);
        	st2_cosTheta->SetY2NDC(0.8+statboxsize_cosTheta);	
		T->Draw(drawtext3_cosTheta.str().c_str(), cuttext3.str().c_str(),"SAMES");
		gPad->Update();
        	TPaveStats *st3_cosTheta = (TPaveStats*)cosTheta_histo_PionCharge_.at(i-1)->FindObject("stats");
        	st3_cosTheta->SetTextColor(4);
        	st3_cosTheta->SetY1NDC(0.8);
        	st3_cosTheta->SetY2NDC(0.8+statboxsize_cosTheta);	

		TLegend * cosTheta_Legend = new TLegend(0.75,0.4,0.85,0.5);
		cosTheta_Legend->SetFillColor(0);
		cosTheta_Legend->AddEntry(cosTheta_histo_ElePosi_.at(i-1),"e^{-}, e^{+}");
		cosTheta_Legend->AddEntry(cosTheta_histo_Muon_.at(i-1),"#mu^{-}, #mu^{+}");
		cosTheta_Legend->AddEntry(cosTheta_histo_PionCharge_.at(i-1),"#pi^{-}, #pi^{+}");
		cosTheta_Legend->Draw();	

		TPaveText* tempbox_cosTheta = new TPaveText(.4,.81,.6,.88,"brNDC");
		cosTheta_CutBox_.push_back(tempbox_cosTheta);
		cosTheta_CutBox_.at(i-1)->AddText(pavetext.str().c_str());
		cosTheta_CutBox_.at(i-1)->Draw();

		cosTheta_Canvas_.at(i-1)->cd(2);
		gStyle->SetOptStat(11);
		gPad->SetLogy();

		T->Draw(drawtext4_cosTheta.str().c_str(), cuttext4.str().c_str());
		gPad->Update();
        	TPaveStats *st4_cosTheta = (TPaveStats*)cosTheta_histo_Pion_.at(i-1)->FindObject("stats");
        	st4_cosTheta->SetTextColor(6);
         	st4_cosTheta->SetY1NDC(0.8);
        	st4_cosTheta->SetY2NDC(0.8+statboxsize_cosTheta);	
		T->Draw(drawtext5_cosTheta.str().c_str(), cuttext5.str().c_str(),"SAMES");
		gPad->Update();
        	TPaveStats *st5_cosTheta = (TPaveStats*)cosTheta_histo_Phot_.at(i-1)->FindObject("stats");
        	st5_cosTheta->SetTextColor(7);
         	st5_cosTheta->SetY1NDC(0.8);
        	st5_cosTheta->SetY2NDC(0.8+statboxsize_cosTheta);	
		T->Draw(drawtext6_cosTheta.str().c_str(), cuttext6.str().c_str(),"SAMES");
		gPad->Update();
        	TPaveStats *st6_cosTheta = (TPaveStats*)cosTheta_histo_Neut_.at(i-1)->FindObject("stats");
        	st6_cosTheta->SetTextColor(9);
         	st6_cosTheta->SetY1NDC(0.8);
        	st6_cosTheta->SetY2NDC(0.8+statboxsize_cosTheta);	

		TLegend * cosTheta_Legend2 = new TLegend(0.75,0.4,0.85,0.5);
		cosTheta_Legend2->SetFillColor(0);
		cosTheta_Legend2->AddEntry(cosTheta_histo_Pion_.at(i-1),"#pi^{0}");
		cosTheta_Legend2->AddEntry(cosTheta_histo_Phot_.at(i-1),"#gamma");
		cosTheta_Legend2->AddEntry(cosTheta_histo_Neut_.at(i-1),"n");
		cosTheta_Legend2->Draw();	

		cosTheta_CutBox_.at(i-1)->Draw();

		cosTheta_Canvas_.at(i-1)->Write();
		cosTheta_Canvas_.at(i-1)->Print(savetext1_cosTheta.str().c_str());
		cosTheta_Canvas_.at(i-1)->Print(savetext2_cosTheta.str().c_str());
		cosTheta_Canvas_.at(i-1)->Close();

		energy += energyinterval;
 		delete temphisto1_angle,temphisto2_angle,temphisto3_angle,temphisto4_angle,temphisto5_angle,temphisto6_angle;
 		delete temphisto1_cosTheta,temphisto2_cosTheta,temphisto3_cosTheta,temphisto4_cosTheta,temphisto5_cosTheta,temphisto6_cosTheta;
		delete tempcanvas_angle,tempcanvas_cosTheta;
		delete tempbox,tempbox_cosTheta;
		delete AngleLegend, AngleLegend2;
		delete cosTheta_Legend, cosTheta_Legend2;      
 }


	TCanvas * E_Canvas_ElePhotProt = new TCanvas("E_Canvas_ElePhotProt");	

	TLegend * E_Legend = new TLegend(0.6,0.63,0.7,0.73);
	E_Legend->SetFillColor(0);
	TLegend * ComparisonLegend = new TLegend(0.6,0.6,0.7,0.7);
	ComparisonLegend->SetFillColor(0);

	E_Canvas_ElePhotProt->Divide(2,2);
	E_Canvas_ElePhotProt->cd(1);
	gPad->SetLogy();
	T->Draw("Energy >>+ E_ElePosi","(Particle_ID==11 || Particle_ID==-11) && hasLeftDetector_Status==1 && Reflectionx<33 && Reflectionx>-33 && Reflectiony<33 && Reflectiony>-33");

	E_Canvas_ElePhotProt->cd(2);
	gPad->SetLogy();
	T->Draw("Energy >>+ E_Photons","Particle_ID==22 && hasLeftDetector_Status==1 && Reflectionx<33 && Reflectionx>-33 && Reflectiony<33 && Reflectiony>-33");
	EnergyHisto_Photons->SetMinimum(0.1);
	EnergyHisto_Photons->SetMaximum(10000500);
	gPad->Update();
       	TPaveStats *st1_PhotProt = (TPaveStats*)EnergyHisto_Photons->FindObject("stats");
       	double statboxsize_PhotProt = st1_PhotProt->GetY2NDC() - st1_PhotProt->GetY1NDC();
       	st1_PhotProt->SetTextColor(2);
       	st1_PhotProt->SetY1NDC(0.8);
       	st1_PhotProt->SetY2NDC(0.8+statboxsize_PhotProt);	

	T->Draw("Energy >>+ E_ProtNeut","(Particle_ID==2212 || Particle_ID==2112) && hasLeftDetector_Status==1 && Reflectionx<33 && Reflectionx>-33 && Reflectiony<33 && Reflectiony>-33","SAMES");
	EnergyHisto_ProtNeut->SetMinimum(0.1);
	EnergyHisto_ProtNeut->SetMaximum(10000500);
	gPad->Update();
       	TPaveStats *st2_PhotProt = (TPaveStats*)EnergyHisto_ProtNeut->FindObject("stats");
       	st2_PhotProt->SetTextColor(4);
      	st2_PhotProt->SetY2NDC(st1_PhotProt->GetY1NDC()); //new x start position
 	st2_PhotProt->SetY1NDC(st2_PhotProt->GetY2NDC()-statboxsize_PhotProt); //new x end position

	E_Legend->AddEntry(EnergyHisto_Photons,"#gamma");
	E_Legend->AddEntry(EnergyHisto_Protons,"p");
	E_Legend->Draw();
	
	E_Canvas_ElePhotProt->cd(3);
	gPad->SetLogy();
	T->Draw("Theta >>+ AngleHisto_Prot","Particle_ID==2212");
	gPad->Update();
       	TPaveStats *st1_ProtPions = (TPaveStats*)AngleHisto_Prot->FindObject("stats");
       	double statboxsize_ProtPions = st1_ProtPions->GetY2NDC() - st1_ProtPions->GetY1NDC();
       	st1_ProtPions->SetTextColor(4);
       	st1_ProtPions->SetY1NDC(0.8);
       	st1_ProtPions->SetY2NDC(0.8+statboxsize_ProtPions);	

	T->Draw("Theta >>+ AngleHisto_Pions","Particle_ID==211","SAMES");
	gPad->Update();
       	TPaveStats *st2_ProtPions = (TPaveStats*)AngleHisto_Pions->FindObject("stats");
       	st2_ProtPions->SetTextColor(6);
      	st2_ProtPions->SetY2NDC(st1_ProtPions->GetY1NDC()); //new x start position
 	st2_ProtPions->SetY1NDC(st2_ProtPions->GetY2NDC()-statboxsize_ProtPions); //new x end position

	ComparisonLegend->AddEntry(AngleHisto_Prot,"p");
	ComparisonLegend->AddEntry(AngleHisto_Pions,"#pi^{+}");
	ComparisonLegend->Draw();

	string canvasname1 = E_Canvas_ElePhotProt->GetName();
	E_Canvas_ElePhotProt->Write();
	E_Canvas_ElePhotProt->Print((canvasname1+".eps").c_str());	
	E_Canvas_ElePhotProt->Print((canvasname1+".C").c_str());	
	E_Canvas_ElePhotProt->Close();

	
	output_rootfile->Write();
	input_rootfile->Close();
}
