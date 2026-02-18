#pragma once
#include "Datatypes/Camera.h"
#include "Datatypes/LoadedAreaManager.h"
#include "Datatypes/LocalPlayer.h"

class Game final
{
public:
	static LocalPlayer GetLocalPlayer();
	static std::vector<TesObjectCell> GetLoadedCells();
	static Camera GetPlayerCamera();

private:
	static LoadedAreaManager GetLoadedAreaManager();	
	virtual void Dummy() = 0;
};

