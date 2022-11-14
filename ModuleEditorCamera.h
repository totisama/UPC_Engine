#pragma once
#include "Module.h"

class ModuleEditorCamera : public Module
{
public:
	ModuleEditorCamera();
	~ModuleEditorCamera();

	bool Init();
	bool CleanUp();

	void SetFOV();
	void SetAspectRatio();
	void SetPlaneDistances();
	void GetProjectionMatrix();
	void GetViewMatrix();
};

