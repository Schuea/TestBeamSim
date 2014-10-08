#include "TFile.h"
#include "TBrowser.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1.h"
#include "TF1.h"
#include "TCutG.h"
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
#include <utility>

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

std::pair<double,double> GetLowerAndUpperBounds(TH1D *temphisto, double factor){
	double lowrange(-9999), highrange(9999);
	double maxx = temphisto->GetMaximum();

	cout << "maxx = " << maxx << endl;
	//for(int i = 1; i < temphisto->GetNbinsX()-1; ++i){
	//	if(temphisto->GetBinContent(i) > maxx/factor){
	//		lowrange = temphisto->GetBinCenter(i);
	//		break;
	//	}
	//}
	//if(lowrange == -9999){
	//	cerr << "Did not find low range" << endl;
	//}
	//for(int i = temphisto->GetNbinsX()-1; i >= 1; --i){
	//	if(temphisto->GetBinContent(i) > maxx/factor){
	//		highrange = temphisto->GetBinCenter(i);
	//		break;
	//	}
	//}
	//if(highrange == -9999){
	//	cerr << "Did not find high range" << endl;
	//}
	//Phill's new idea goes here:
	int lowbin = temphisto->FindFirstBinAbove(maxx/factor);
	int highbin = temphisto->FindLastBinAbove(maxx/factor);
	int maxbin = temphisto->GetMaximumBin();
	if(highbin - maxbin > maxbin - lowbin){
		highbin = maxbin + (maxbin - lowbin);
	}
	else{
		lowbin = maxbin - (highbin - maxbin);
	}
	lowrange = temphisto->GetBinCenter(lowbin);
	highrange = temphisto->GetBinCenter(highbin);
	std::pair<double,double> ranges(lowrange,highrange);
	return ranges;
}

void DrawingMacro(string name){
	TH1::SetDefaultSumw2();

	TFile * input_rootfile = new TFile(name.c_str(),"READ");
	cout << "Inputfile size = " << input_rootfile->GetSize() << endl;
	TTree * T = (TTree*)input_rootfile->Get("Tree");
	cout << "Accessed TTree.." << endl;

	stringstream output_filename;
	output_filename << "position_plots_" << name;
	TFile * output_rootfile = new TFile(output_filename.str().c_str(),"RECREATE");

	TH2F * Position_negCharge = new TH2F("Posi_negCharge","Position behind magnet of particles with negative charge",200,-600,600,200,-600,600);
	Position_negCharge->GetXaxis()->SetTitle("x (mm)");
	Position_negCharge->GetXaxis()->CenterTitle();
	Position_negCharge->GetYaxis()->SetTitle("y (mm)");
	Position_negCharge->GetYaxis()->CenterTitle();
	TH2F * Position_posCharge = new TH2F("Posi_posCharge","Position behind magnet of particles with positive charge",200,-600,600,200,-600,600);
	Position_posCharge->GetXaxis()->SetTitle("x (mm)");
	Position_posCharge->GetXaxis()->CenterTitle();
	Position_posCharge->GetYaxis()->SetTitle("y (mm)");
	Position_posCharge->GetYaxis()->CenterTitle();
	TH2F * Position_noCharge = new TH2F("Posi_noCharge","Position behind magnet of particles without electric charge",300,-600,600,300,-600,600);
	Position_noCharge->GetXaxis()->SetTitle("x (mm)");
	Position_noCharge->GetXaxis()->CenterTitle();
	Position_noCharge->GetYaxis()->SetTitle("y (mm)");
	Position_noCharge->GetYaxis()->CenterTitle();

	int no_bins = 700;
	float energy = 1.0;
	float energyinterval = 1.0;
	vector< TH2F* > Position_negCharge_;
	vector< TH2F* > Position_posCharge_;
	vector< TH2F* > Position_noCharge_;
	vector< TH1D* > Projectionx_negCharge_;
	vector< TH1D* > Projectionx_posCharge_;
	vector< TH1D* > Projectionx_noCharge_;
		
	TCanvas* Posi_negCharge_Canvas_ = new TCanvas("Position_negCharge_Canvas");
	TCanvas* Posi_posCharge_Canvas_ = new TCanvas("Position_posCharge_Canvas");
	TCanvas* Posi_noCharge_Canvas_ = new TCanvas("Position_noCharge_Canvas");
	TCanvas* Proj_negCharge_Canvas_ = new TCanvas("ProjectionX_negCharge_Canvas");
	TCanvas* Proj_posCharge_Canvas_ = new TCanvas("ProjectionX_posCharge_Canvas");
	TCanvas* Proj_noCharge_Canvas_ = new TCanvas("ProjectionX_noCharge_Canvas");
		
	gROOT->ForceStyle();

	Posi_negCharge_Canvas_->Divide(2,4);
	Posi_negCharge_Canvas_->cd(1);
	gPad->SetLogz();
	gStyle->SetOptStat(11);
	gStyle->SetNumberContours(999);
	T->Draw("Reflectiony:Reflectionx >>+ Posi_negCharge","Charge<0 && hasLeftDetector_Status==1","colz");	

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

	TEllipse * el1 = new TEllipse(0,0,60,60);
	el1->SetLineWidth(2.5);
	el1->SetFillStyle(0000);
	el1->Draw();

	Posi_posCharge_Canvas_->Divide(2,4);
	Posi_posCharge_Canvas_->cd(1);
	gPad->SetLogz();
	gStyle->SetOptStat(11);
	gStyle->SetNumberContours(999);
	T->Draw("Reflectiony:Reflectionx >>+ Posi_posCharge","Charge>0 && hasLeftDetector_Status==1","colz");	
	l1->Draw();
	l2->Draw();
	l3->Draw();
	l4->Draw();
	el1->Draw();

	Posi_noCharge_Canvas_->Divide(2,4);
	Posi_noCharge_Canvas_->cd(1);
	gPad->SetLogz();
	gStyle->SetOptStat(11);
	gStyle->SetNumberContours(999);
	T->Draw("Reflectiony:Reflectionx >>+ Posi_noCharge","Charge==0 && hasLeftDetector_Status==1","colz");	
	l1->Draw();
	l2->Draw();
	l3->Draw();
	l4->Draw();
	el1->Draw();

	Proj_negCharge_Canvas_->Divide(2,4);
	Proj_negCharge_Canvas_->cd(1);

	TCutG *roicut = new TCutG("roicut",4);
	roicut->SetVarX("Reflectionx");
	roicut->SetVarY("Reflectiony");
	roicut->SetPoint(0,0,20);
	roicut->SetPoint(1,600,20);
	roicut->SetPoint(2,600,-20);
	roicut->SetPoint(3,0,-20);
	roicut->SetPoint(4,0,20);
	TH1D * ProjectionX_negCharge = Position_negCharge->ProjectionX("ProjectionX for particles with negative charge behind magnet",0,-1,"e,[roicut]"); 
	ProjectionX_negCharge->Draw();

	Proj_posCharge_Canvas_->Divide(2,4);
	Proj_posCharge_Canvas_->cd(1);

	TCutG *roicut_pos = new TCutG("roicut_pos",4);
	roicut_pos->SetVarX("Reflectionx");
	roicut_pos->SetVarY("Reflectiony");
	roicut_pos->SetPoint(0,0,20);
	roicut_pos->SetPoint(1,-600,20);
	roicut_pos->SetPoint(2,-600,-20);
	roicut_pos->SetPoint(3,0,-20);
	roicut_pos->SetPoint(4,0,20);
	TH1D * ProjectionX_posCharge = Position_posCharge->ProjectionX("ProjectionX for particles with posative charge behind magnet",0,-1,"e,[roicut_pos]"); 
	ProjectionX_posCharge->Draw();

	Proj_noCharge_Canvas_->cd();
	//TH2F * Position_noCharge_2 = new TH2F("Posi_noCharge_2","Position behind magnet of particles without electric charge",500,-30,30,500,-30,30);
	TH2F * Position_noCharge_2 = new TH2F("Posi_noCharge_2","Position behind magnet of particles without electric charge",100,-0.2,0.2,500,-30,30);
	T->Draw("Reflectiony:Reflectionx >>+ Posi_noCharge_2","Charge==0 && hasLeftDetector_Status==1","goff");	
	TH1D * ProjectionX_noCharge = Position_noCharge_2->ProjectionX("ProjectionX for particles with noative charge behind magnet",0,-1,"e"); 
	
	TF1 *gausfit_first_no = new TF1("gausfit_first_no","gaus",-1,1);
	ProjectionX_noCharge->Fit(gausfit_first_no,"R");
	ProjectionX_noCharge->GetXaxis()->SetRangeUser(-0.03,0.03);
	ProjectionX_noCharge->GetYaxis()->SetRangeUser(0,6000000);
	ProjectionX_noCharge->Draw();

	for(int i = 1; i < 8; ++i){
	//		if (energy >= 3.0 && energy <= 5.0){
	//			no_bins = 200;
	//		}
			if (energy > 5.0 && energy < 7.0){
				energyinterval = 0.3;
				no_bins = 1000;
			}

			stringstream histoname_Posi_negCharge,histoname_Posi_posCharge,histoname_Posi_noCharge; 
			histoname_Posi_negCharge << "Posi_negCharge" << i;
			histoname_Posi_posCharge << "Posi_posCharge" << i;
			histoname_Posi_noCharge << "Posi_noCharge" << i;

			stringstream histotitle_Posi_negCharge,histotitle_Posi_posCharge,histotitle_Posi_noCharge; 
			stringstream histotitle_Proj_negCharge,histotitle_Proj_posCharge,histotitle_Proj_noCharge; 
			histotitle_Posi_negCharge << "Position behind magnet of particles with negative charge for an energy between " << energy -0.1 << " and " << energy + 0.1 << " GeV";
			histotitle_Posi_posCharge << "Position behind magnet of particles with positive charge for an energy between " << energy -0.1 << " and " << energy + 0.1 << " GeV";
			histotitle_Posi_noCharge << "Position behind magnet of particles without charge for an energy between " << energy -0.1 << " and " << energy + 0.1 << " GeV";
			histotitle_Proj_negCharge << "ProjectionX of position behind magnet of particles with negative charge for an energy between " << energy -0.1 << " and " << energy + 0.1 << " GeV";
			histotitle_Proj_posCharge << "ProjectionX of position behind magnet of particles with positive charge for an energy between " << energy -0.1 << " and " << energy + 0.1 << " GeV";
			histotitle_Proj_noCharge << "ProjectionX of position behind magnet of particles without charge for an energy between " << energy -0.1 << " and " << energy + 0.1 << " GeV";

			TH2F * temphisto_Posi_negCharge = new TH2F(histoname_Posi_negCharge.str().c_str(),histotitle_Posi_negCharge.str().c_str(),no_bins,-600,600,no_bins,-600,600);
			TH2F * temphisto_Posi_posCharge = new TH2F(histoname_Posi_posCharge.str().c_str(),histotitle_Posi_posCharge.str().c_str(),no_bins,-600,600,no_bins,-600,600);
			TH2F * temphisto_Posi_noCharge = new TH2F(histoname_Posi_noCharge.str().c_str(),histotitle_Posi_noCharge.str().c_str(),no_bins,-600,600,no_bins,-600,600);

			stringstream drawtext_Posi_negCharge,drawtext_Posi_posCharge,drawtext_Posi_noCharge; 
			drawtext_Posi_negCharge << "Reflectiony:Reflectionx >>+ " << histoname_Posi_negCharge.str();
			drawtext_Posi_posCharge << "Reflectiony:Reflectionx >>+ " << histoname_Posi_posCharge.str();
			drawtext_Posi_noCharge << "Reflectiony:Reflectionx >>+ " << histoname_Posi_noCharge.str();

			stringstream cuttext_negCharge,cuttext_posCharge,cuttext_noCharge; 
			cuttext_negCharge << "Charge < 0 && hasLeftDetector_Status==1 && Energy > " << energy-0.1 << " && Energy < " << energy+0.1;
			cuttext_posCharge << "Charge > 0 && hasLeftDetector_Status==1 && Energy > " << energy-0.1 << " && Energy < " << energy+0.1;
			cuttext_noCharge << "Charge == 0 && hasLeftDetector_Status==1 && Energy > " << energy-0.1 << " && Energy < " << energy+0.1;

			Position_negCharge_.push_back(temphisto_Posi_negCharge);
			Position_negCharge_.at(i-1)->GetXaxis()->SetTitle("x (mm)");
			Position_negCharge_.at(i-1)->GetXaxis()->CenterTitle();
			Position_negCharge_.at(i-1)->GetYaxis()->SetTitle("y (mm)");
			Position_negCharge_.at(i-1)->GetYaxis()->CenterTitle();

			Position_posCharge_.push_back(temphisto_Posi_posCharge);
			Position_posCharge_.at(i-1)->GetXaxis()->SetTitle("x (mm)");
			Position_posCharge_.at(i-1)->GetXaxis()->CenterTitle();
			Position_posCharge_.at(i-1)->GetYaxis()->SetTitle("y (mm)");
			Position_posCharge_.at(i-1)->GetYaxis()->CenterTitle();

			Position_noCharge_.push_back(temphisto_Posi_noCharge);
			Position_noCharge_.at(i-1)->GetXaxis()->SetTitle("x (mm)");
			Position_noCharge_.at(i-1)->GetXaxis()->CenterTitle();
			Position_noCharge_.at(i-1)->GetYaxis()->SetTitle("y (mm)");
			Position_noCharge_.at(i-1)->GetYaxis()->CenterTitle();

			Posi_negCharge_Canvas_->cd(i+1);
			gPad->SetLogz();
			gStyle->SetOptStat(11);
			gStyle->SetNumberContours(999);
			//gROOT->ForceStyle();
			T->Draw(drawtext_Posi_negCharge.str().c_str(),cuttext_negCharge.str().c_str(),"colz");	
			l1->Draw();
			l2->Draw();
			l3->Draw();
			l4->Draw();
			el1->Draw();

			Posi_posCharge_Canvas_->cd(i+1);
			gPad->SetLogz();
			gStyle->SetOptStat(11);
			gStyle->SetNumberContours(999);
			T->Draw(drawtext_Posi_posCharge.str().c_str(),cuttext_posCharge.str().c_str(),"colz");	
			l1->Draw();
			l2->Draw();
			l3->Draw();
			l4->Draw();
			el1->Draw();

			Posi_noCharge_Canvas_->cd(i+1);
			gPad->SetLogz();
			gStyle->SetOptStat(11);
			gStyle->SetNumberContours(999);
			T->Draw(drawtext_Posi_noCharge.str().c_str(),cuttext_noCharge.str().c_str(),"colz");	
			l1->Draw();
			l2->Draw();
			l3->Draw();
			l4->Draw();
			el1->Draw();


			Proj_negCharge_Canvas_->cd(i+1);

			TH1D * temphisto_Proj_negCharge = Position_negCharge_.at(i-1)->ProjectionX(histotitle_Proj_negCharge.str().c_str(),0,-1,"e,[roicut]"); 
			Projectionx_negCharge_.push_back(temphisto_Proj_negCharge);
			Projectionx_negCharge_.at(i-1)->GetXaxis()->SetTitle("x (mm)");
			Projectionx_negCharge_.at(i-1)->GetXaxis()->CenterTitle();

			pair<double,double> range_neg = GetLowerAndUpperBounds(Projectionx_negCharge_.at(i-1),9.0);
			TF1 *gausfit_neg = new TF1("gausfit_neg","gaus",range_neg.first,range_neg.second);
			Projectionx_negCharge_.at(i-1)->Fit(gausfit_neg,"R");
				
			Projectionx_negCharge_.at(i-1)->GetXaxis()->SetRangeUser(range_neg.first-100,range_neg.second+50);
			Projectionx_negCharge_.at(i-1)->Draw();
		
			Proj_posCharge_Canvas_->cd(i+1);

			TH1D * temphisto_Proj_posCharge = Position_posCharge_.at(i-1)->ProjectionX(histotitle_Proj_posCharge.str().c_str(),0,-1,"e,[roicut_pos]"); 
			Projectionx_posCharge_.push_back(temphisto_Proj_posCharge);
			Projectionx_posCharge_.at(i-1)->GetXaxis()->SetTitle("x (mm)");
			Projectionx_posCharge_.at(i-1)->GetXaxis()->CenterTitle();
	
			pair<double,double> range_pos = GetLowerAndUpperBounds(Projectionx_posCharge_.at(i-1),10.0);
			TF1 *gausfit_pos = new TF1("gausfit_pos","gaus",range_pos.first,range_pos.second);
			Projectionx_posCharge_.at(i-1)->Fit(gausfit_pos,"R");
			
			Projectionx_posCharge_.at(i-1)->GetXaxis()->SetRangeUser(range_pos.first-50,range_pos.second+100);
			Projectionx_posCharge_.at(i-1)->Draw();
						
/*
			Proj_noCharge_Canvas_->cd(i+1);

			TH1D * temphisto_Proj_noCharge = Position_noCharge_.at(i-1)->ProjectionX(histotitle_Proj_noCharge.str().c_str(),0,-1,"e"); 
			Projectionx_noCharge_.push_back(temphisto_Proj_noCharge);
			Projectionx_noCharge_.at(i-1)->GetXaxis()->SetTitle("x (mm)");
			Projectionx_noCharge_.at(i-1)->GetXaxis()->CenterTitle();
		
//			pair<double,double> range_no = GetLowerAndUpperBounds(Projectionx_noCharge_.at(i-1), 2000.0);
			TF1 *gausfit_no = new TF1("gausfit_no","gaus",-0.5,0.5);
			Projectionx_noCharge_.at(i-1)->Fit(gausfit_no,"R");
			Projectionx_noCharge_.at(i-1)->GetXaxis()->SetRangeUser(-1,1);
			Projectionx_noCharge_.at(i-1)->Draw();
*/
			energy += energyinterval;
			//delete temphisto_Posi_negCharge;
			//delete temphisto_Posi_posCharge;
			//delete temphisto_Posi_noCharge;	
			//delete temphisto_Proj_negCharge;
			//delete temphisto_Proj_posCharge;
			//delete temphisto_Proj_noCharge;	
	}	

	string canvasname=Posi_negCharge_Canvas_->GetName();
	Posi_negCharge_Canvas_->Write();
	Posi_negCharge_Canvas_->Print((canvasname+".eps").c_str());
	Posi_negCharge_Canvas_->Print((canvasname+".C").c_str());
	Posi_negCharge_Canvas_->Close();

	string canvasname2=Posi_posCharge_Canvas_->GetName();
	Posi_posCharge_Canvas_->Write();
	Posi_posCharge_Canvas_->Print((canvasname2+".eps").c_str());
	Posi_posCharge_Canvas_->Print((canvasname2+".C").c_str());
	Posi_posCharge_Canvas_->Close();

	string canvasname3=Posi_noCharge_Canvas_->GetName();
	Posi_noCharge_Canvas_->Write();
	Posi_noCharge_Canvas_->Print((canvasname3+".eps").c_str());
	Posi_noCharge_Canvas_->Print((canvasname3+".C").c_str());
	Posi_noCharge_Canvas_->Close();

	string canvasname4=Proj_negCharge_Canvas_->GetName();
	Proj_negCharge_Canvas_->Write();
	Proj_negCharge_Canvas_->Print((canvasname4+".eps").c_str());
	Proj_negCharge_Canvas_->Print((canvasname4+".C").c_str());
	Proj_negCharge_Canvas_->Close();

	string canvasname5=Proj_posCharge_Canvas_->GetName();
	Proj_posCharge_Canvas_->Write();
	Proj_posCharge_Canvas_->Print((canvasname5+".eps").c_str());
	Proj_posCharge_Canvas_->Print((canvasname5+".C").c_str());
	Proj_posCharge_Canvas_->Close();

	string canvasname6=Proj_noCharge_Canvas_->GetName();
	Proj_noCharge_Canvas_->Write();
	Proj_noCharge_Canvas_->Print((canvasname6+".eps").c_str());
	Proj_noCharge_Canvas_->Print((canvasname6+".C").c_str());
	Proj_noCharge_Canvas_->Close();

	output_rootfile->Write();
	input_rootfile->Close();

	delete output_rootfile, input_rootfile;
	delete l1,l2,l3,l4,el1;
	delete Posi_negCharge_Canvas_, Posi_posCharge_Canvas_, Posi_noCharge_Canvas_; 
	delete Proj_negCharge_Canvas_, Proj_posCharge_Canvas_, Proj_noCharge_Canvas_; 

}

