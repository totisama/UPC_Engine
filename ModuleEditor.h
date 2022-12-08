#pragma once
#include "Module.h"
#include "Globals.h"
#include <vector>

class ModuleEditor : public Module
{
public:

	ModuleEditor();
	~ModuleEditor();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	update_status continueValue = UPDATE_CONTINUE;

private:
	void ShowWindow();

	std::vector<float> fps;
};