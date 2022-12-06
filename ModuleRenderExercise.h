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

	void SetNewModel(const char* path);
	void pushAssimpLog(const char* log);

	Model *GetCurrentModel();
	vector <const char*> getAssimpLogs();

private:
	Model* model = nullptr;

	vector<const char*> assimpLogs;
};

