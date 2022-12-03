#pragma once
#include "Module.h"
#include "Globals.h"
#include "DirectXTex/DirectXTex.h"
#include "GL/glew.h"

class ModuleTexture : public Module
{
public:
	ModuleTexture();
	~ModuleTexture();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	GLuint LoadTexture(const char* fileName);
	void SetWireframeMode(bool setMode);
};

