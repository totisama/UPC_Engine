#include "ModuleTexture.h"
#include "GL/glew.h"
#include "DirectXTex/DirectXTex.h"
#include <string>


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
	if (setMode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

GLuint ModuleTexture::LoadTexture(const char* fileName)
{
	std::string textPath = "./../Assets/Textures/" + std::string(fileName);
	std::string narrowString(textPath);
	std::wstring wideString = std::wstring(narrowString.begin(), narrowString.end());
	const wchar_t* path = wideString.c_str();

	DirectX::ScratchImage* image = new DirectX::ScratchImage();

	HRESULT hResult;
	hResult = DirectX::LoadFromDDSFile(path, DirectX::DDS_FLAGS_NONE, nullptr, *image);

	if (FAILED(hResult))
	{
		hResult = DirectX::LoadFromTGAFile(path, DirectX::TGA_FLAGS_NONE, nullptr, *image);

		if (FAILED(hResult))
		{
			hResult = DirectX::LoadFromWICFile(path, DirectX::WIC_FLAGS_NONE, nullptr, *image);

			if (FAILED(hResult))
			{
				ENGINE_LOG("Error loading the texture");

				return false;
			}
		}
	}

	DirectX::ScratchImage* resultImage = new DirectX::ScratchImage();

	hResult = DirectX::FlipRotate(image->GetImages(), image->GetImageCount(), image->GetMetadata(), DirectX::TEX_FR_FLIP_VERTICAL, *resultImage);

	if (FAILED(hResult))
	{
		ENGINE_LOG("Error fliping the image");

		return false;
	}
	
	GLuint texture;

	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, resultImage->GetMetadata().width, resultImage->GetMetadata().height, 0, GL_RGBA, GL_UNSIGNED_BYTE, resultImage->GetPixels());
	glGenerateMipmap(GL_TEXTURE_2D);

	return texture;
}