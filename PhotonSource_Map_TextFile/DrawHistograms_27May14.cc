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
#include "TPaveStats.h"

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
	EnergyHisto_Photons->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_Photons->GetXaxis()->CenterTitle();
	TH1F * EnergyHisto_Electrons = new TH1F("E_Electrons","Energy distribution of electrons/positrons existing behind converter",50,0,6.5);
	EnergyHisto_Electrons->SetLineColor(4);
	EnergyHisto_Electrons->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_Electrons->GetXaxis()->CenterTitle();
	TH1F * EnergyHisto_Positrons = new TH1F("E_Positrons","Energy distribution of electrons/positrons existing behind converter",50,0,6.5);
	EnergyHisto_Positrons->SetLineColor(2);
	EnergyHisto_Positrons->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_Positrons->GetXaxis()->CenterTitle();
	TH1F * EnergyHisto_Muons = new TH1F("E_Muons","Energy distribution of muons existing behind converter",30,0,2);
	EnergyHisto_Muons->SetLineColor(4);
	EnergyHisto_Muons->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_Muons->GetXaxis()->CenterTitle();
	TH1F * EnergyHisto_MuonsPlus = new TH1F("E_MuonsPlus","Energy distribution of positive muons existing behind converter",30,0,2);
	EnergyHisto_MuonsPlus->SetLineColor(2);
	EnergyHisto_MuonsPlus->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_MuonsPlus->GetXaxis()->CenterTitle();
	TH1F * EnergyHisto_Pions = new TH1F("E_Pions","Energy distribution of pions existing behind converter",30,0,5);
	EnergyHisto_Pions->SetLineColor(4);
	EnergyHisto_Pions->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_Pions->GetXaxis()->CenterTitle();
	TH1F * EnergyHisto_PionsPlus = new TH1F("E_PionsPlus","Energy distribution of positive pions existing behind converter",30,0,5);
	EnergyHisto_PionsPlus->SetLineColor(2);
	EnergyHisto_PionsPlus->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_PionsPlus->GetXaxis()->CenterTitle();
	TH1F * EnergyHisto_Neutrons = new TH1F("E_Neutrons","Energy distribution of neutrons existing behind converter",30,0,5);
	EnergyHisto_Neutrons->SetLineColor(4);
	EnergyHisto_Neutrons->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_Neutrons->GetXaxis()->CenterTitle();
	TH1F * EnergyHisto_Protons = new TH1F("E_Protons","Energy distribution of protons existing behind converter",30,0,5);
	EnergyHisto_Protons->SetLineColor(2);
	EnergyHisto_Protons->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_Protons->GetXaxis()->CenterTitle();


	TH1F * EnergyHisto_AllStatuses = new TH1F("E_All","Energy distribution for all simulation statuses",110,0,1);
	EnergyHisto_AllStatuses->SetLineColor(1);
	EnergyHisto_AllStatuses->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_AllStatuses->GetXaxis()->CenterTitle();
	TH1F * EnergyHisto_createdInContinuousProcess = new TH1F("E_createdInContinuousProcess","Energy distribution for electrons created in continuous process",70,0,1);
	EnergyHisto_createdInContinuousProcess->SetLineColor(2);
	EnergyHisto_createdInContinuousProcess->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_createdInContinuousProcess->GetXaxis()->CenterTitle();
	TH1F * EnergyHisto_Stopped = new TH1F("E_Stopped","Energy distribution for electrons stopped in tracker",110,0,1);
	EnergyHisto_Stopped->SetLineColor(4);
	EnergyHisto_Stopped->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_Stopped->GetXaxis()->CenterTitle();
	TH1F * EnergyHisto_hasLeftDetector = new TH1F("E_hasLeftDetector","Energy distribution for electrons that left detector",110,0,1);
	EnergyHisto_hasLeftDetector->SetLineColor(7);
	EnergyHisto_hasLeftDetector->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_hasLeftDetector->GetXaxis()->CenterTitle();


	TH1F * EnergyHisto_w_Parents = new TH1F("E_wParent","Energy distribution for electrons with parents",110,0,6.5);
	EnergyHisto_w_Parents->SetLineColor(1);
	EnergyHisto_w_Parents->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_w_Parents->GetXaxis()->CenterTitle();
	TH1F * EnergyHisto_ParentElectron = new TH1F("E_ParentElectron","Energy distribution for electrons with electrons as parent",80,0,6.5);
	EnergyHisto_ParentElectron->SetLineColor(2);
	EnergyHisto_ParentElectron->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_ParentElectron->GetXaxis()->CenterTitle();
	TH1F * EnergyHisto_ParentPositron = new TH1F("E_ParentPositron","Energy distribution for electrons with positrons as parent",80,0,6.5);
	EnergyHisto_ParentPositron->SetLineColor(3);
	EnergyHisto_ParentPositron->GetXaxis()->SetTitle("Energy (GeV)");
	EnergyHisto_ParentPositron->GetXaxis()->CenterTitle();
	TH1F * EnergyHisto_ParentPhoton = new TH1F("E_ParentPhoton","Energy distribution for electrons with photons as parent",110,0,6.5);
	EnergyHisto_ParentPhoton->SetLineColor(4);
	EnergyHisto_ParentPhoton->GetXaxis()->SetTitle("Energy (GeV)");
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
		savetext << histoname.str() << "_" << angle << "degree_" << angle+angleinterval <<"degree_Pipe.eps";
		savetext2 << histoname.str() << "_" << angle << "degree_" << angle+angleinterval <<"degree_Pipe.C";

		TH1F* temphisto = new TH1F(histoname.str().c_str(), histotitle.str().c_str(),150,0,max_energy);
                AngleHisto_Electrons_.push_back(temphisto);
               
	  	TCanvas *tempcanvas = new TCanvas();
		Angle_Canvas_.push_back(tempcanvas);
		Angle_Canvas_.at(i-1)->cd();
		gPad->SetLogy();
	
		AngleHisto_Electrons_.at(i-1)->GetXaxis()->SetTitle("Energy (GeV)");
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

	TLegend * E_Legend = new TLegend(0.5,0.6,0.7,0.7);
	E_Legend->SetFillColor(0);
	TLegend * ComparisonLegend = new TLegend(0.5,0.55,0.7,0.75);
	ComparisonLegend->SetFillColor(0);
	TLegend * ComparisonLegend2 = new TLegend(0.7,0.55,0.8,0.7);
	ComparisonLegend2->SetFillColor(0);
	TLegend * ComparisonLegend3 = new TLegend(0.7,0.55,0.8,0.7);
	ComparisonLegend3->SetFillColor(0);
	TLegend * ComparisonLegend4 = new TLegend(0.7,0.51,0.8,0.7);
	ComparisonLegend4->SetFillColor(0);
	TLegend * ComparisonLegend5 = new TLegend(0.5,0.55,0.7,0.75);
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
	gStyle->SetOptStat(11);
//	EnergyHisto_Electrons->SetStats(kFALSE);
//	EnergyHisto_Positrons->SetStats(kFALSE);
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
        st2->SetTextColor(2);
        st2->SetY2NDC(st1->GetY1NDC()); //new x start position
        st2->SetY1NDC(st2->GetY2NDC()-statboxsize); //new x end position

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
    	E_Diff->SetMinimum(0.5);    
	E_Diff->SetMaximum(1.5);

	string canvasname = E_Canvas_Electrons->GetName();
	E_Canvas_Electrons->Write();
	E_Canvas_Electrons->Print((canvasname+"_Pipe.eps").c_str());	
	E_Canvas_Electrons->Print((canvasname+"_Pipe.C").c_str());	
	E_Canvas_Electrons->Close();

	E_Canvas_PhotonsMuonsKaons->Divide(2,2);
	E_Canvas_PhotonsMuonsKaons->cd(1);
	gStyle->SetOptStat(11);
	gPad->SetLogy();
	T->Draw("Energy >>+ E_Photons","Particle_ID==22");
	gPad->Update();
        TPaveStats *Photons_st1 = (TPaveStats*)EnergyHisto_Photons->FindObject("stats");
        double Photons_statboxsize = Photons_st1->GetY2NDC() - Photons_st1->GetY1NDC();
        Photons_st1->SetTextColor(4);
        Photons_st1->SetY1NDC(0.8);
        Photons_st1->SetY2NDC(0.8+Photons_statboxsize);	

	E_Canvas_PhotonsMuonsKaons->cd(2);
	gPad->SetLogy();
	gStyle->SetOptStat(11);
//	EnergyHisto_Muons->SetStats(kFALSE);
//	EnergyHisto_MuonsPlus->SetStats(kFALSE);
	T->Draw("Energy >>+ E_Muons","Particle_ID==13");
	gPad->Update();
        TPaveStats *Muons_st1 = (TPaveStats*)EnergyHisto_Muons->FindObject("stats");
        double Muons_statboxsize = Muons_st1->GetY2NDC() - Muons_st1->GetY1NDC();
        Muons_st1->SetTextColor(4);
        Muons_st1->SetY1NDC(0.8);
        Muons_st1->SetY2NDC(0.8+Muons_statboxsize);	
	T->Draw("Energy >>+ E_MuonsPlus","Particle_ID==-13","SAMES");
	gPad->Update();
        TPaveStats *Muons_st2 = (TPaveStats*)EnergyHisto_MuonsPlus->FindObject("stats");
        Muons_st2->SetTextColor(2);
        Muons_st2->SetY2NDC(Muons_st1->GetY1NDC()); //new x start position
        Muons_st2->SetY1NDC(Muons_st2->GetY2NDC()-Muons_statboxsize); //new x end position
	ComparisonLegend2->AddEntry(EnergyHisto_Muons,"#mu^{-}");
	ComparisonLegend2->AddEntry(EnergyHisto_MuonsPlus,"#mu^{+}");
	ComparisonLegend2->Draw();

	E_Canvas_PhotonsMuonsKaons->cd(3);
	gPad->SetLogy();
	gStyle->SetOptStat(11);
//	EnergyHisto_Neutrons->SetStats(kFALSE);
//	EnergyHisto_Protons->SetStats(kFALSE);
	T->Draw("Energy >>+ E_Neutrons","Particle_ID==2112");
	gPad->Update();
        TPaveStats *Neutrons_st1 = (TPaveStats*)EnergyHisto_Neutrons->FindObject("stats");
        double Neutrons_statboxsize = Neutrons_st1->GetY2NDC() - Neutrons_st1->GetY1NDC();
        Neutrons_st1->SetTextColor(4);
        Neutrons_st1->SetY1NDC(0.8);
        Neutrons_st1->SetY2NDC(0.8+Neutrons_statboxsize);	
	T->Draw("Energy >>+ E_Protons","Particle_ID==2212","SAMES");
	gPad->Update();
        TPaveStats *Protons_st2 = (TPaveStats*)EnergyHisto_Protons->FindObject("stats");
        Protons_st2->SetTextColor(2);
        Protons_st2->SetY2NDC(Neutrons_st1->GetY1NDC()); //new x start position
        Protons_st2->SetY1NDC(Protons_st2->GetY2NDC()-Neutrons_statboxsize); //new x end position
	ComparisonLegend3->AddEntry(EnergyHisto_Neutrons,"n");
	ComparisonLegend3->AddEntry(EnergyHisto_Protons,"p");
	ComparisonLegend3->Draw();

	E_Canvas_PhotonsMuonsKaons->cd(4);
	gPad->SetLogy();
	gStyle->SetOptStat(11);
//	EnergyHisto_Pions->SetStats(kFALSE);
//	EnergyHisto_PionsPlus->SetStats(kFALSE);
	T->Draw("Energy >>+ E_Pions","Particle_ID==-211");
	gPad->Update();
        TPaveStats *Pions_st1 = (TPaveStats*)EnergyHisto_Pions->FindObject("stats");
        double Pions_statboxsize = Pions_st1->GetY2NDC() - Pions_st1->GetY1NDC();
        Pions_st1->SetTextColor(4);
        Pions_st1->SetY1NDC(0.8);
        Pions_st1->SetY2NDC(0.8+Pions_statboxsize);	
	T->Draw("Energy >>+ E_PionsPlus","Particle_ID==211","SAMES");
	gPad->Update();
        TPaveStats *Pions_st2 = (TPaveStats*)EnergyHisto_PionsPlus->FindObject("stats");
        Pions_st2->SetTextColor(2);
        Pions_st2->SetY2NDC(Pions_st1->GetY1NDC()); //new x start position
        Pions_st2->SetY1NDC(Pions_st2->GetY2NDC()-Pions_statboxsize); //new x end position
	ComparisonLegend4->AddEntry(EnergyHisto_Pions,"#pi^{-}");
	ComparisonLegend4->AddEntry(EnergyHisto_PionsPlus,"#pi^{+}");
	ComparisonLegend4->Draw();

	string canvasname2 = E_Canvas_PhotonsMuonsKaons->GetName();
	E_Canvas_PhotonsMuonsKaons->Write();
	E_Canvas_PhotonsMuonsKaons->Print((canvasname2+"_Pipe.eps").c_str());	
	E_Canvas_PhotonsMuonsKaons->Print((canvasname2+"_Pipe.C").c_str());	
	E_Canvas_PhotonsMuonsKaons->Close();


	CompareSimStatus_Canvas->Divide(1,2);
	CompareSimStatus_Canvas->cd(1);
	gPad->SetLogy();
	gStyle->SetOptStat(11);
//	EnergyHisto_AllStatuses->SetStats(kFALSE);
//	EnergyHisto_createdInContinuousProcess->SetStats(kFALSE);
//	EnergyHisto_Stopped->SetStats(kFALSE);
//	EnergyHisto_hasLeftDetector->SetStats(kFALSE);
	T->Draw("Energy >>+ E_All","Particle_ID==11");
	gPad->Update();
        TPaveStats *Compare_st1 = (TPaveStats*)EnergyHisto_AllStatuses->FindObject("stats");
        double Compare_statboxsize = Compare_st1->GetY2NDC() - Compare_st1->GetY1NDC();
        Compare_st1->SetTextColor(1);
        Compare_st1->SetY1NDC(0.8);
        Compare_st1->SetY2NDC(0.8+Compare_statboxsize);	

	T->Draw("Energy >>+ E_createdInContinuousProcess","Particle_ID==11 && createdInContinuousProcess_Status==1","SAMES");
	gPad->Update();
        TPaveStats *Compare_st2 = (TPaveStats*)EnergyHisto_createdInContinuousProcess->FindObject("stats");
        Compare_st2->SetTextColor(2);
        Compare_st2->SetY2NDC(Compare_st1->GetY1NDC()); //new x start position
        Compare_st2->SetY1NDC(Compare_st2->GetY2NDC()-Compare_statboxsize); //new x end position

	T->Draw("Energy >>+ E_Stopped","Particle_ID==11 && Stopped_Status==1","SAMES");
	gPad->Update();
        TPaveStats *Compare_st3 = (TPaveStats*)EnergyHisto_Stopped->FindObject("stats");
        Compare_st3->SetTextColor(4);
        Compare_st3->SetY2NDC(Compare_st2->GetY1NDC()); //new x start position
        Compare_st3->SetY1NDC(Compare_st3->GetY2NDC()-Compare_statboxsize); //new x end position

	T->Draw("Energy >>+ E_hasLeftDetector","Particle_ID==11 && hasLeftDetector_Status==1","SAMES");
	gPad->Update();
        TPaveStats *Compare_st4 = (TPaveStats*)EnergyHisto_hasLeftDetector->FindObject("stats");
        Compare_st4->SetTextColor(7);
        Compare_st4->SetY2NDC(Compare_st3->GetY1NDC()); //new x start position
        Compare_st4->SetY1NDC(Compare_st4->GetY2NDC()-Compare_statboxsize); //new x end position

	ComparisonLegend->AddEntry(EnergyHisto_AllStatuses,"All electrons","l");
	ComparisonLegend->AddEntry(EnergyHisto_createdInContinuousProcess,"CreatedInContinuousProcess","l");
	ComparisonLegend->AddEntry(EnergyHisto_Stopped,"Stopped","l");
	ComparisonLegend->AddEntry(EnergyHisto_hasLeftDetector,"HasLeftDetector","l");
	ComparisonLegend->Draw();

	CompareSimStatus_Canvas->cd(2);
	gStyle->SetOptStat(11);
	gPad->SetLogy();
//	EnergyHisto_w_Parents->SetStats(kFALSE);
//	EnergyHisto_ParentElectron->SetStats(kFALSE);
//	EnergyHisto_ParentPositron->SetStats(kFALSE);
//	EnergyHisto_ParentPhoton->SetStats(kFALSE);
	T->Draw("Energy >>+ E_wParent","Particle_ID==11 && NumberOfParents==1");
	gPad->Update();
        TPaveStats *Parents_st1 = (TPaveStats*)EnergyHisto_w_Parents->FindObject("stats");
        Compare_st4->SetTextColor(7);
        double Parents_statboxsize = Parents_st1->GetY2NDC() - Parents_st1->GetY1NDC();
        Parents_st1->SetTextColor(1);
        Parents_st1->SetY1NDC(0.8);
        Parents_st1->SetY2NDC(0.8+Parents_statboxsize);	

	T->Draw("Energy >>+ E_ParentElectron","Particle_ID==11 && Parent_ID==11","SAMES");
	gPad->Update();
        TPaveStats *Parents_st2 = (TPaveStats*)EnergyHisto_ParentElectron->FindObject("stats");
        Compare_st4->SetTextColor(7);
        Parents_st2->SetTextColor(2);
        Parents_st2->SetY2NDC(Parents_st1->GetY1NDC()); //new x start position
        Parents_st2->SetY1NDC(Parents_st2->GetY2NDC()-Parents_statboxsize); //new x end position

	T->Draw("Energy >>+ E_ParentPositron","Particle_ID==11 && Parent_ID==-11","SAMES");
	gPad->Update();
        TPaveStats *Parents_st3 = (TPaveStats*)EnergyHisto_ParentPositron->FindObject("stats");
        Compare_st4->SetTextColor(7);
        Parents_st3->SetTextColor(3);
        Parents_st3->SetY2NDC(Parents_st2->GetY1NDC()); //new x start position
        Parents_st3->SetY1NDC(Parents_st3->GetY2NDC()-Parents_statboxsize); //new x end position

	T->Draw("Energy >>+ E_ParentPhoton","Particle_ID==11 && Parent_ID==22 && Angle_initialfinal<10","SAMES");
	gPad->Update();
        TPaveStats *Parents_st4 = (TPaveStats*)EnergyHisto_ParentPhoton->FindObject("stats");
        Compare_st4->SetTextColor(7);
        Parents_st4->SetTextColor(4);
        Parents_st4->SetY2NDC(Parents_st3->GetY1NDC()); //new x start position
        Parents_st4->SetY1NDC(Parents_st4->GetY2NDC()-Parents_statboxsize); //new x end position

	ComparisonLegend5->AddEntry(EnergyHisto_w_Parents,"all particles with parents","l");
	ComparisonLegend5->AddEntry(EnergyHisto_ParentElectron,"with electrons as parents","l");
	ComparisonLegend5->AddEntry(EnergyHisto_ParentPositron,"with positrons as parents","l");
	ComparisonLegend5->AddEntry(EnergyHisto_ParentPhoton,"with photons as parents","l");
	ComparisonLegend5->Draw();

	TPaveText * CutBox = new TPaveText(.3,.78,0.7,0.87,"brNDC");
	CutBox->SetFillColor(0);
	CutBox->SetTextColor(1);

	CutBox->AddText("Cut on angle between inital photon and particle: Angle < 10");
	CutBox->Draw();

	string canvasname3=CompareSimStatus_Canvas->GetName();
	CompareSimStatus_Canvas->Write();
	CompareSimStatus_Canvas->Print((canvasname3+"_Pipe.eps").c_str());
	CompareSimStatus_Canvas->Print((canvasname3+"_Pipe.C").c_str());
	CompareSimStatus_Canvas->Close();

	output_rootfile->Write();
	input_rootfile->Close();
}
