#include "ModuleTexture.h"
#include <GL/glew.h>
#include "DirectXTex/DirectXTex.h"

ModuleTexture::ModuleTexture()
{
}

// Destructor
ModuleTexture::~ModuleTexture()
{
}

// Called before render is available
bool ModuleTexture::Init()
{
	float buffer_data[] = {
		-1.0f, -1.0f, 0.0f, // v0 pos
		 1.0f, -1.0f, 0.0f, // v1 pos
		 0.0f, 1.0f, 0.0f, // v2 pos
		 0.0f, 0.0f, // v0 texcoord
		 1.0f, 0.0f, // v1 texcoord
		 0.5f, 1.0f // v2 texcoord
	};

	//DirectX::LoadFromDDSFile();

	/*glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)sizeof(float) * 3 * 3);
	glActiveTexture(GL_TEXTURE0);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, texture_object);*/

	return true;
}

update_status ModuleTexture::PreUpdate()
{
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleTexture::Update()
{
	
	return UPDATE_CONTINUE;
}

update_status ModuleTexture::PostUpdate()
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleTexture::CleanUp()
{
	return true;
}
