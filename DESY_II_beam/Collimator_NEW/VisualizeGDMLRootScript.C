{
	gSystem->Load("libGeom");
	gSystem->Load("libGdml");
//	TGeoManager::Import("Pipe_LeadShielding_Converter_Magnet.gdml");
	TGeoManager::Import("Test_only_collimator_KINK.gdml");
	gGeoManager->GetTopVolume()->Draw();
//	gGeoManager->GetTopVolume()->Draw("ogl");
}
