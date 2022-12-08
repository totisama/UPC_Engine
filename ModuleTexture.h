#pragma once
#include "Module.h"
#include "Globals.h"
#include "DirectXTex/DirectXTex.h"
#include "GL/glew.h"
#include <string>

using namespace std;

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

	GLuint LoadTexture(const char* fileName, string modelPath);
	void SetWireframeMode(bool setMode);
	HRESULT TestTexturePath(string path, DirectX::ScratchImage& image);
};

