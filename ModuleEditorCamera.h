#pragma once
#include "Module.h"
#include "Game/MathGeoLib_Source/Geometry/Frustum.h"
#include "Game/MathGeoLib_Source/Math/float3x3.h"

class ModuleEditorCamera : public Module
{
public:
	ModuleEditorCamera();
	~ModuleEditorCamera();

	bool Init();
	bool CleanUp();

	void Translate(float3 position);
	
	float cameraSpeed = 1.0f;
	void SetFOV(float valor);
	void SetAspectRatio();
	void SetPlaneDistances();

	float4x4 GetProjectionMatrix();
	float4x4 GetViewMatrix();
	float4x4 GetModelMatrix();

	float3 GetCameraPosition();
	float GetCameraSpeed();
	void SetCameraSpeed(float value);

private:
	Frustum frustum;
};

