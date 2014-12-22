{
	gSystem->Load("libGeom");
	gSystem->Load("libGdml");
	TGeoManager::Import("Pipe_LeadShielding.gdml");
	gGeoManager->GetTopVolume()->Draw("ogl");
}
