#include "ModuleTexture.h"
#include "GL/glew.h"
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
	HRESULT hr;
	DirectX::ScratchImage* image = new DirectX::ScratchImage();
	hr = DirectX::LoadFromWICFile(L"./../Assets/baboon.png", DirectX::WIC_FLAGS_NONE, nullptr, *image);

	if (FAILED(hr))
	{
		ENGINE_LOG("Error loading the image");

		return false;
	}

	HRESULT flipResult;
	resultImage = new DirectX::ScratchImage();

	flipResult = DirectX::FlipRotate(image->GetImages(), image->GetImageCount(), image->GetMetadata(), DirectX::TEX_FR_FLIP_VERTICAL, *resultImage);
	
	if (FAILED(flipResult))
	{
		ENGINE_LOG("Error fliping the image");

		return false;
	}

	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, resultImage->GetMetadata().width, resultImage->GetMetadata().height, 0, GL_RGBA, GL_UNSIGNED_BYTE, resultImage->GetPixels());
	glGenerateMipmap(GL_TEXTURE_2D);

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

void ModuleTexture::SetWireframeMode(bool setMode)
{
	//Set wireframe mode
	if (setMode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}
