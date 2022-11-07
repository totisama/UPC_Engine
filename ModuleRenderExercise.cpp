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

	/*vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);*/

	/*fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);*/

	/*shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);*/

	VBO = App->program->CreateTriangleVBO();

	/*float vertices[] = {
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f
	};
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);*/

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
	glClear(GL_COLOR_BUFFER_BIT);

	App->program->RenderVBO(VBO, shaderProgram);

	/*glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);*/

	return UPDATE_CONTINUE;
}

update_status ModuleRenderExercise::PostUpdate()
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderExercise::CleanUp()
{
	//glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	return true;
}
