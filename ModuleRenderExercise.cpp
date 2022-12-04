#include "ModuleRenderExercise.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleRender.h"
#include "ModuleEditorCamera.h"
#include "ModuleWindow.h"
#include "ModuleDebugDraw.h"
#include <GL/glew.h>
#include "SDL.h"

ModuleRenderExercise::ModuleRenderExercise()
{
}

// Destructor
ModuleRenderExercise::~ModuleRenderExercise()
{
}

// Called before render is available
bool ModuleRenderExercise::Init()
{	
	const char* vertexShaderSource = App->program->LoadShaderSource("./../Assets/Shaders/vertexShader.glsl");
	const char* fragmentShaderSource = App->program->LoadShaderSource("./../Assets/Shaders/fragmentShader.glsl");

	unsigned int vertexShader = App->program->CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
	unsigned int fragmentShader = App->program->CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
	App->renderer->defaultProgram = App->program->CreateProgram(vertexShader, fragmentShader);

	modelBH->LoadModel("./../Assets/Models/BakerHouse.fbx");

	return true;
}

update_status ModuleRenderExercise::PreUpdate()
{
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRenderExercise::Update()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	modelBH->DrawModel();

	int w = SCREEN_WIDTH;
	int h = SCREEN_HEIGHT;

	SDL_GetWindowSize(App->window->window, &w, &h);

	App->debugDraw->Draw(App->editorCamera->GetViewMatrix(), App->editorCamera->GetProjectionMatrix(), w, h);
	
	return UPDATE_CONTINUE;
}

update_status ModuleRenderExercise::PostUpdate()
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderExercise::CleanUp()
{
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(App->renderer->defaultProgram);

	return true;
}
