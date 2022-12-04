#pragma once
#include "Module.h"
#include "Globals.h"
#include "Model.h"

class ModuleRenderExercise : public Module
{
public:
	ModuleRenderExercise();
	~ModuleRenderExercise();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	
private:

	unsigned int VBO;
	Model* modelBH = new Model();
};

