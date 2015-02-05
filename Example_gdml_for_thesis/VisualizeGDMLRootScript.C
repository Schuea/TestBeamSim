{
	gSystem->Load("libGeom");
	gSystem->Load("libGdml");
	TGeoManager::Import("Example_geometry.gdml");
	gGeoManager->GetTopVolume()->Draw();
}
