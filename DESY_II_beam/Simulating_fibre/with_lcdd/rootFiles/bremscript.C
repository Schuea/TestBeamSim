{
	TH1F *histo = new TH1F("# bremsstrahlung photons","Average number of bremsstrahlung photons per 10^{7} electrons;Number of photons produced;Count",25,100,320);
	histo->GetXaxis()->CenterTitle();
	histo->GetYaxis()->CenterTitle();
	histo->SetLineColor(4);
	for(int i = 1; i <= 1000; ++i){
		cout << i << endl;
		stringstream filename;
		filename << "Fibre_" << i << ".root";
		TFile *file = TFile::Open(filename.str().c_str());
		TTree *tree;
		file->GetObject("Tree_MCP",tree);
		histo->Fill(tree->GetEntries("Particle_ID==22 && hasLeftDetector_Status==1"));
		file->Close();
	}
	TCanvas *canvas = new TCanvas();
	histo->Draw();
	histo->Fit("gaus");
	gStyle->SetOptFit(110);
	canvas->Print("brem.eps","eps");
}
