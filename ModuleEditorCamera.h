#pragma once
#include "Module.h"
#include "Game/MathGeoLib_Source/Geometry/Frustum.h"

class ModuleEditorCamera : public Module
{
public:
	ModuleEditorCamera();
	~ModuleEditorCamera();

	bool Init();
	bool CleanUp();

	void SetFOV(float valor);
	void SetAspectRatio();
	void SetPlaneDistances();
	float4x4 GetProjectionMatrix();
	float4x4 GetViewMatrix();
	float4x4 GetModelMatrix();

	Frustum frustum;
};

