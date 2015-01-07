{
	gSystem->Load("libGeom");
	gSystem->Load("libGdml");
	TGeoManager::Import("Pipe_LeadShielding_Converter_Magnet_Collimator_WALL_TBCollimator.gdml");
//	TGeoManager::Import("Test_only_collimator_WALL_TBcollimator_KINK.gdml");
//	TGeoManager::Import("Test_origin_of_rotation.gdml");
//	TGeoManager::Import("Test_beampipeconnector_KINK.gdml");
	gGeoManager->GetTopVolume()->Draw();
//	gGeoManager->GetTopVolume()->Draw("ogl");
}
