#include "ModuleRenderExercise.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleProgram.h"
#include <GL/glew.h>

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
	const char* vertexShaderSource = App->program->LoadShaderSource("./../shaders/uniformVertexShader.glsl");
	//const char* vertexShaderSource = App->program->LoadShaderSource("./../shaders/helloWorldVertexShader.glsl");
	const char* fragmentShaderSource = App->program->LoadShaderSource("./../shaders/helloWorldFragmentShader.glsl");

	vertexShader = App->program->CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
	fragmentShader = App->program->CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
	shaderProgram = App->program->CreateProgram(vertexShader, fragmentShader);
	VBO = App->program->CreateTriangleVBO();

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
	//glClear(GL_COLOR_BUFFER_BIT);

	App->program->RenderVBO(VBO, shaderProgram);

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
	glDeleteProgram(shaderProgram);

	return true;
}
