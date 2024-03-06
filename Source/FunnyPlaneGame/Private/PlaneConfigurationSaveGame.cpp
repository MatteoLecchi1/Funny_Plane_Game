// Fill out your copyright notice in the Description page of Project Settings.

#include "PlaneConfigurationSaveGame.h"

FSavedPlane* UPlaneConfigurationSaveGame::GetCurrentPlane()
{
	if (SavedPlanes.Contains(CurrentPlaneKey))
	{
		return &(SavedPlanes[CurrentPlaneKey]);
	}

	ensure(false);
	return nullptr;
}
