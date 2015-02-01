{
	gSystem->Load("libGeom");
	gSystem->Load("libGdml");
//	TGeoManager::Import("Pipe_LeadShielding.gdml");
	TGeoManager::Import("Test_only_shielding.gdml");
	gGeoManager->GetTopVolume()->Draw();
}
