#pragma once
#include "Module.h"
#include "Globals.h"
#include "Game/MathGeoLib_Source/Geometry/Frustum.h"

typedef unsigned __int8 Uint8;

class ModuleInput : public Module
{
public:
	
	ModuleInput();
	~ModuleInput();

	bool Init();
	update_status Update();
	bool CleanUp();

private:
	const Uint8 *keyboard = NULL;
	float cameraRotationSpeed;
};