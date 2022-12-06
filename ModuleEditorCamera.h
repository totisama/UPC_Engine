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
	void Rotate(float3 rotation);
	
	float cameraSpeed = 0.001f;
	float cameraRotationSpeed = 0.001f;
	void SetFOV(float valor);
	void SetAspectRatio();
	void SetPlaneDistances();

	float4x4 GetProjectionMatrix();
	float4x4 GetViewMatrix();
	float4x4 GetModelMatrix();

	void ResetCameraPosition();
	void ResetCameraRotation();
	void SetCameraPosition(float3 position);
	float3 GetCameraPos();
	float3 GetCameraUp();

	float GetCameraRotationSpeed();
	void SetCameraRotationSpeed(float value);

	float GetCameraSpeed();
	void SetCameraSpeed(float value);

private:
	Frustum frustum;
};

