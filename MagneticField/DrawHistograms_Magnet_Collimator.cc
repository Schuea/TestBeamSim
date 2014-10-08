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

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

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
	output_filename << "histograms_" << name;
	TFile * output_rootfile = new TFile(output_filename.str().c_str(),"RECREATE");

	TH1F * EnergyHisto_Photons = new TH1F("E_Photons","Energy distribution of photons existing behind converter",100,0,7);
	EnergyHisto_Photons->SetLineColor(4);
	EnergyHisto_Photons->GetXaxis()->SetTitle("Energy/GeV");
	EnergyHisto_Photons->GetXaxis()->CenterTitle();
	TH1F * EnergyHisto_Electrons = new TH1F("E_Electrons","Energy distribution of electrons existing behind converter",50,0,6.5);
	EnergyHisto_Electrons->SetLineColor(4);
	EnergyHisto_Electrons->GetXaxis()->SetTitle("Energy/GeV");
	EnergyHisto_Electrons->GetXaxis()->CenterTitle();
	TH1F * EnergyHisto_Positrons = new TH1F("E_Positrons","Energy distribution of positrons existing behind converter",50,0,6.5);
	EnergyHisto_Positrons->SetLineColor(2);
	EnergyHisto_Positrons->GetXaxis()->SetTitle("Energy/GeV");
	EnergyHisto_Positrons->GetXaxis()->CenterTitle();
	TH1F * EnergyHisto_Muons = new TH1F("E_Muons","Energy distribution of muons existing behind converter",30,0,2);
	EnergyHisto_Muons->SetLineColor(4);
	EnergyHisto_Muons->GetXaxis()->SetTitle("Energy/GeV");
	EnergyHisto_Muons->GetXaxis()->CenterTitle();
	TH1F * EnergyHisto_MuonsPlus = new TH1F("E_MuonsPlus","Energy distribution of positive muons existing behind converter",30,0,2);
	EnergyHisto_MuonsPlus->SetLineColor(2);
	EnergyHisto_MuonsPlus->GetXaxis()->SetTitle("Energy/GeV");
	EnergyHisto_MuonsPlus->GetXaxis()->CenterTitle();
	TH1F * EnergyHisto_PionsMinus = new TH1F("E_PionsMinus","Energy distribution of negative pions existing behind converter",30,0,5);
	EnergyHisto_PionsMinus->SetLineColor(4);
	EnergyHisto_PionsMinus->GetXaxis()->SetTitle("Energy/GeV");
	EnergyHisto_PionsMinus->GetXaxis()->CenterTitle();
	TH1F * EnergyHisto_PionsPlus = new TH1F("E_PionsPlus","Energy distribution of positive pions existing behind converter",30,0,5);
	EnergyHisto_PionsPlus->SetLineColor(2);
	EnergyHisto_PionsPlus->GetXaxis()->SetTitle("Energy/GeV");
	EnergyHisto_PionsPlus->GetXaxis()->CenterTitle();
	TH1F * EnergyHisto_Protons = new TH1F("E_Protons","Energy distribution of protons existing behind converter",30,0,1);
	EnergyHisto_Protons->SetLineColor(4);
	EnergyHisto_Protons->GetXaxis()->SetTitle("Energy/GeV");
	EnergyHisto_Protons->GetXaxis()->CenterTitle();
	TH1F * EnergyHisto_Neutrons = new TH1F("E_Neutrons","Energy distribution of neutrons existing behind converter",30,0,1);
	EnergyHisto_Neutrons->SetLineColor(2);
	EnergyHisto_Neutrons->GetXaxis()->SetTitle("Energy/GeV");
	EnergyHisto_Neutrons->GetXaxis()->CenterTitle();

	TH2F * Position_Electrons = new TH2F("Posi_Electrons","",,-20,20,,-20,20);

	TH1F * EnergyHisto_w_Parents = new TH1F("E_wParent","Energy distribution for electrons with parents",110,0,6.5);
	EnergyHisto_w_Parents->SetLineColor(1);
	EnergyHisto_w_Parents->GetXaxis()->SetTitle("Energy/GeV");
	EnergyHisto_w_Parents->GetXaxis()->CenterTitle();
	TH1F * EnergyHisto_ParentElectron = new TH1F("E_ParentElectron","Energy distribution for electrons with electrons as parent",80,0,6.5);
	EnergyHisto_ParentElectron->SetLineColor(2);
	EnergyHisto_ParentElectron->GetXaxis()->SetTitle("Energy/GeV");
	EnergyHisto_ParentElectron->GetXaxis()->CenterTitle();
	TH1F * EnergyHisto_ParentPositron = new TH1F("E_ParentPositron","Energy distribution for electrons with positrons as parent",80,0,6.5);
	EnergyHisto_ParentPositron->SetLineColor(3);
	EnergyHisto_ParentPositron->GetXaxis()->SetTitle("Energy/GeV");
	EnergyHisto_ParentPositron->GetXaxis()->CenterTitle();
	TH1F * EnergyHisto_ParentPhoton = new TH1F("E_ParentPhoton","Energy distribution for electrons with photons as parent",110,0,6.5);
	EnergyHisto_ParentPhoton->SetLineColor(4);
	EnergyHisto_ParentPhoton->GetXaxis()->SetTitle("Energy/GeV");
	EnergyHisto_ParentPhoton->GetXaxis()->CenterTitle();


	float angle = 0;
	float angleinterval = 1;
	float max_energy = 6.5;
	vector< TH1F* > AngleHisto_Electrons_;
	vector< TCanvas* > Angle_Canvas_;
	vector< TPaveText* > AngleCutBox_;

	for(int i = 1; i < 13; ++i){
		if (angle <= 2){
			angleinterval = 1;
			max_energy = 6.5;
		}

                else if (angle > 2 && angle <= 5){
			angleinterval = 1;
			max_energy = 6.5;
		}
		else if (angle > 5 && angle <= 10){
			angleinterval = 2;
			max_energy = 2;
		}
		else if (angle > 10 && angle <=30){
			angleinterval = 5;
			max_energy = 2;
		}
		else {
			break;
		}

		stringstream histoname,histotitle,drawtext,cuttext,pavetext,savetext, savetext2;
                histoname << "AngleHisto_Electrons_" << i-1;
		histotitle << "Energy distribution of electrons with photons as parents, for an angle between " << angle  << " and " << angle + angleinterval << " between electron and z-axis";
		drawtext << "Energy >>+ " << histoname.str();
		cuttext << "Particle_ID==11 && Theta >= "<< angle <<" && Theta < " << angle + angleinterval << " && Parent_ID==22";
		pavetext << angle << " degree < Angle < " << angle + angleinterval << " degree";
		savetext << histoname.str() << "_" << angle << "degree_" << angle+angleinterval <<"degree.eps";
		savetext2 << histoname.str() << "_" << angle << "degree_" << angle+angleinterval <<"degree.C";

		TH1F* temphisto = new TH1F(histoname.str().c_str(), histotitle.str().c_str(),150,0,max_energy);
                AngleHisto_Electrons_.push_back(temphisto);
               
	  	TCanvas *tempcanvas = new TCanvas();
		Angle_Canvas_.push_back(tempcanvas);
		Angle_Canvas_.at(i-1)->cd();
		gPad->SetLogy();
	
		AngleHisto_Electrons_.at(i-1)->GetXaxis()->SetTitle("Energy/GeV");
		AngleHisto_Electrons_.at(i-1)->GetXaxis()->CenterTitle();
	
		T->Draw(drawtext.str().c_str(), cuttext.str().c_str());

		TPaveText* tempbox = new TPaveText(.5,.7,.7,.8,"brNDC");
		AngleCutBox_.push_back(tempbox);
		AngleCutBox_.at(i-1)->AddText(pavetext.str().c_str());
		AngleCutBox_.at(i-1)->Draw();

		Angle_Canvas_.at(i-1)->Write();
		Angle_Canvas_.at(i-1)->Print(savetext.str().c_str());
		Angle_Canvas_.at(i-1)->Print(savetext2.str().c_str());
		Angle_Canvas_.at(i-1)->Close();
	
		angle += angleinterval;
        }


	TCanvas * E_Canvas_Electrons = new TCanvas("E_Canvas_Electrons");	
	TCanvas * E_Canvas_PhotonsMuonsKaons = new TCanvas("E_Canvas_PhotonsMuonsKaons");	
	TCanvas * CompareSimStatus_Canvas = new TCanvas("Compare_Canvas");

	TLegend * E_Legend = new TLegend(0.5,0.5,0.8,0.7);
	E_Legend->SetFillColor(0);
	TLegend * ComparisonLegend = new TLegend(0.5,0.55,0.8,0.75);
	ComparisonLegend->SetFillColor(0);
	TLegend * ComparisonLegend2 = new TLegend(0.7,0.55,0.8,0.7);
	ComparisonLegend2->SetFillColor(0);
	TLegend * ComparisonLegend3 = new TLegend(0.7,0.55,0.8,0.7);
	ComparisonLegend3->SetFillColor(0);
	TLegend * ComparisonLegend4 = new TLegend(0.7,0.51,0.8,0.7);
	ComparisonLegend4->SetFillColor(0);
	TLegend * ComparisonLegend5 = new TLegend(0.5,0.55,0.8,0.75);
	ComparisonLegend5->SetFillColor(0);

	E_Canvas_Electrons->cd();
	TPad *subpad1, *subpad2;

  	subpad1 = new TPad("subpad1"," ",0,0.3,1,1);
  	subpad1->SetBottomMargin(0.);
  	subpad1->Draw();
  	subpad2 = new TPad("subpad2"," ",0,0,1,0.3);
  	subpad2->SetBottomMargin(0.30);
  	subpad2->SetTopMargin(0.);
  	subpad2->Draw();

  	subpad1->cd();
	gPad->SetLogy();
	EnergyHisto_Electrons->SetStats(kFALSE);
	EnergyHisto_Positrons->SetStats(kFALSE);
	T->Draw("Energy >>+ E_Electrons","Particle_ID==11");
//	EnergyHisto_Electrons->Scale(1.0/(EnergyHisto_Electrons->Integral()));
	gPad->Update();
 	TPaveStats *st1 = (TPaveStats*)EnergyHisto_Electrons->FindObject("stats");
  	double statboxsize = st1->GetY2NDC() - st1->GetY1NDC();
 	st1->SetTextColor(4);
  	st1->SetY1NDC(0.8);
  	st1->SetY2NDC(0.8+statboxsize);	

	T->Draw("Energy >>+ E_Positrons","Particle_ID==-11","SAMES");
//	EnergyHisto_Positrons->Scale(1.0/(EnergyHisto_Positrons->Integral()));
	gPad->Update();
  	TPaveStats *st2 = (TPaveStats*)EnergyHisto_Positrons->FindObject("stats");
  	st2->SetY2NDC(st1->GetY1NDC()); //new x start position
  	st2->SetY1NDC(st2->GetY2NDC()-statboxsize); //new x end position
  	st2->SetTextColor(2);
	E_Legend->AddEntry(EnergyHisto_Electrons,"All electrons","l");	
	E_Legend->AddEntry(EnergyHisto_Positrons,"All positrons","l");	
	E_Legend->Draw();

	subpad2->cd();
  	TH1F* E_Diff;

	string E_DiffName;
    	E_DiffName =  "EnergyDistribution_Difference";

      	E_Diff = new TH1F(E_DiffName.c_str(),E_DiffName.c_str(), EnergyHisto_Electrons->GetNbinsX(), EnergyHisto_Electrons->GetXaxis()->GetXmin(), EnergyHisto_Electrons->GetXaxis()->GetXmax());
    	E_Diff->Divide(EnergyHisto_Positrons,EnergyHisto_Electrons,1,1,"b");
    	E_Diff->SetLineColor(EnergyHisto_Positrons->GetLineColor());
    	E_Diff->SetLineWidth(1);
    	E_Diff->SetMarkerStyle(EnergyHisto_Electrons->GetMarkerStyle());
    	E_Diff->SetMarkerColor(EnergyHisto_Electrons->GetMarkerColor());
    	E_Diff->SetMarkerSize(.8);
    	E_Diff->Draw("EP");	

	TLine *line1 = new TLine(EnergyHisto_Electrons->GetBinLowEdge(1),1,EnergyHisto_Electrons->GetBinLowEdge(EnergyHisto_Electrons->GetNbinsX()+1),1);
  	line1->SetLineWidth(2);
  	line1->SetLineColor(kBlack);
  	line1->Draw();
  	E_Diff->GetXaxis()->SetTitle(EnergyHisto_Electrons->GetXaxis()->GetTitle());
  	E_Diff->GetYaxis()->SetTitle("Ratio");
  	E_Diff->GetXaxis()->SetTitleSize(0.17);
  	E_Diff->GetYaxis()->SetTitleSize(0.15);
  	E_Diff->GetXaxis()->SetTitleOffset(0.80);
  	E_Diff->GetYaxis()->SetTitleOffset(0.3);
  	E_Diff->SetLabelSize(0.13,"X");
  	E_Diff->SetLabelSize(0.07,"Y");
  	E_Diff->SetTitle("");
  	E_Diff->GetXaxis()->SetLabelOffset(0.010);
  	E_Diff->GetYaxis()->SetLabelOffset(0.015);
  	E_Diff->SetStats(kFALSE);
  	E_Diff->SetDirectory(0);
    	E_Diff->SetMinimum(0.75);    
	E_Diff->SetMaximum(2.1);

	string canvasname = E_Canvas_Electrons->GetName();
	E_Canvas_Electrons->Write();
	E_Canvas_Electrons->Print((canvasname+".eps").c_str());	
	E_Canvas_Electrons->Print((canvasname+".C").c_str());	
	E_Canvas_Electrons->Close();

	E_Canvas_PhotonsMuonsKaons->Divide(2,2);
	E_Canvas_PhotonsMuonsKaons->cd(1);
	gPad->SetLogy();
	T->Draw("Energy >>+ E_Photons","Particle_ID==22");
	E_Canvas_PhotonsMuonsKaons->cd(2);
	gPad->SetLogy();
	EnergyHisto_Muons->SetStats(kFALSE);
	EnergyHisto_MuonsPlus->SetStats(kFALSE);
	T->Draw("Energy >>+ E_Muons","Particle_ID==13");
	T->Draw("Energy >>+ E_MuonsPlus","Particle_ID==-13","same");
	ComparisonLegend2->AddEntry(EnergyHisto_Muons,"#mu^{-}");
	ComparisonLegend2->AddEntry(EnergyHisto_MuonsPlus,"#mu^{+}");
	ComparisonLegend2->Draw();
	E_Canvas_PhotonsMuonsKaons->cd(3);
	gPad->SetLogy();
	EnergyHisto_Kaons->SetStats(kFALSE);
	EnergyHisto_KaonsPlus->SetStats(kFALSE);
	T->Draw("Energy >>+ E_KaonsPlus","Particle_ID==321");
	T->Draw("Energy >>+ E_Kaons","Particle_ID==-321","same");
	ComparisonLegend3->AddEntry(EnergyHisto_Kaons,"#Kappa^{-}");
	ComparisonLegend3->AddEntry(EnergyHisto_KaonsPlus,"#Kappa^{+}");
	ComparisonLegend3->Draw();
	E_Canvas_PhotonsMuonsKaons->cd(4);
	gPad->SetLogy();
	EnergyHisto_Pions->SetStats(kFALSE);
	EnergyHisto_PionsPlus->SetStats(kFALSE);
	T->Draw("Energy >>+ E_Pions","Particle_ID==-211");
	T->Draw("Energy >>+ E_PionsPlus","Particle_ID==211","same");
	ComparisonLegend4->AddEntry(EnergyHisto_Pions,"#pi^{-}");
	ComparisonLegend4->AddEntry(EnergyHisto_PionsPlus,"#pi^{+}");
	ComparisonLegend4->Draw();

	string canvasname2 = E_Canvas_PhotonsMuonsKaons->GetName();
	E_Canvas_PhotonsMuonsKaons->Write();
	E_Canvas_PhotonsMuonsKaons->Print((canvasname2+".eps").c_str());	
	E_Canvas_PhotonsMuonsKaons->Print((canvasname2+".C").c_str());	
	E_Canvas_PhotonsMuonsKaons->Close();


	CompareSimStatus_Canvas->cd();
	gPad->SetLogy();
	EnergyHisto_w_Parents->SetStats(kFALSE);
	EnergyHisto_ParentElectron->SetStats(kFALSE);
	EnergyHisto_ParentPositron->SetStats(kFALSE);
	EnergyHisto_ParentPhoton->SetStats(kFALSE);
	T->Draw("Energy >>+ E_wParent","Particle_ID==11 && NumberOfParents==1");
	T->Draw("Energy >>+ E_ParentElectron","Particle_ID==11 && Parent_ID==11","Same");
	T->Draw("Energy >>+ E_ParentPositron","Particle_ID==11 && Parent_ID==-11","Same");
	T->Draw("Energy >>+ E_ParentPhoton","Particle_ID==11 && Parent_ID==22 "/*&& Angle_initialfinal<10"*/,"Same");
	ComparisonLegend5->AddEntry(EnergyHisto_w_Parents,"all particles with parents","l");
	ComparisonLegend5->AddEntry(EnergyHisto_ParentElectron,"with electrons as parents","l");
	ComparisonLegend5->AddEntry(EnergyHisto_ParentPositron,"with positrons as parents","l");
	ComparisonLegend5->AddEntry(EnergyHisto_ParentPhoton,"with photons as parents","l");
	ComparisonLegend5->Draw();
/*
	TPaveText * CutBox = new TPaveText(.3,.78,0.7,0.87,"brNDC");
	CutBox->SetFillColor(0);
	CutBox->SetTextColor(1);

	CutBox->AddText("Cut on angle between inital photon and particle: Angle < 10");
	CutBox->Draw();
*/
	string canvasname3=CompareSimStatus_Canvas->GetName();
	CompareSimStatus_Canvas->Write();
	CompareSimStatus_Canvas->Print((canvasname3+".eps").c_str());
	CompareSimStatus_Canvas->Print((canvasname3+".C").c_str());
	CompareSimStatus_Canvas->Close();

	output_rootfile->Write();
	input_rootfile->Close();
}
